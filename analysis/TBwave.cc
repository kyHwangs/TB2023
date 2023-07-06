#include "TLegend.h"

#include "drawFunction.h"

#include "TBread.h"

enum plotting_mode{
    kAuto,
    kEnter,
    kPress1toPass
};

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    std::string runNum   = argv[1];
    int start_evt        = std::stoi(argv[2]);
    int max_evt          = std::stoi(argv[3]);
    std::vector<std::string> channel_names;
    for(int plot_args = 4; plot_args < argc; plot_args++ ) {
        channel_names.push_back(argv[plot_args]);
    }
    plotting_mode plotMode = kEnter;
    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);

    auto map = getModuleConfigMap();
    for(int idx = 0; idx < channel_names.size(); idx++) {
        if (map.find(channel_names.at(idx)) == map.end()) {
            std::cout << "Plot Name error : " << channel_names.at(idx) << " not found" << std::endl;
            return 0;
        }
    }

    std::vector<int> MIDs;
    std::vector<int> unique_MIDs;
    std::vector<int> Chs;
    std::vector<TH1F*> plots;

    std::map<int, int> keyMap;

    for(int idx = 0; idx < channel_names.size(); idx++) {
        std::vector<int> MIDandCh = map.at(channel_names.at(idx));
        MIDs.push_back(MIDandCh.at(0));
        Chs.push_back(MIDandCh.at(1));

        if (keyMap.find(MIDandCh.at(0)) == keyMap.end()){
            keyMap.insert(std::make_pair(MIDandCh.at(0), 1));
            unique_MIDs.push_back(MIDandCh.at(0));
        }
    
        plots.push_back(new TH1F( (TString)channel_names.at(idx), (TString)channel_names.at(idx), 1000, 0., 1000.) );
    }
    // unique_MIDs.erase( std::unique( unique_MIDs.begin(), unique_MIDs.end() ), unique_MIDs.end() );
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), start_evt + max_evt, -1, "/Users/yhep/scratch/YUdaq", unique_MIDs);
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TCanvas* c = new TCanvas("c", "c", 800, 600);
    c->cd();

    for(int iEvt = 0; iEvt < start_evt + max_evt; iEvt++) {
        auto anEvent = readerWave.GetAnEvent();

        if (iEvt < start_evt) continue;
        if (iEvt > start_evt + max_evt) gApplication->Terminate();

        for(int idx = 0; idx < plots.size(); idx++) {
            TBcid cid = TBcid(MIDs.at(idx), Chs.at(idx));
            auto single_waveform = anEvent.GetData(cid).waveform();
            plots.at(idx)->Reset();
            plots.at(idx)->SetTitle( (TString)(channel_names.at(idx) + "_Evt_" + std::to_string(iEvt+1)) );
            plots.at(idx)->SetMaximum(4096.);
            plots.at(idx)->SetMinimum(3000.);
            plots.at(idx)->GetXaxis()->SetTitle("bin");
            plots.at(idx)->GetYaxis()->SetTitle("ADC");
            plots.at(idx)->SetLineWidth(2);

            for(int bin = 0; bin < single_waveform.size()-23; bin++) {
                plots.at(idx)->Fill(bin, single_waveform.at(bin+1));
            }
            c->cd();
            if (idx == 0) plots.at(idx)->Draw("Hist & PLC");
            else plots.at(idx)->Draw("Hist & PLC & sames");
            c->Update();
        }
        c->BuildLegend(0.7, 0.2, 0.9, 0.4);
        c->Update();
        switch(plotMode) {
            case kAuto :
                gSystem->Sleep(500);
                break;

            case kEnter :
                std::cout << "Press Enter to continue" << std::endl;
                std::cin.get();
                break;

            case kPress1toPass :
                std::cout << "Press 1 to continue, 0 to quit" << std::endl;
                int input = -1;
                while (input == -1) {
                    std::cin >> input;
                    if (! ( (input == 1) || (input == 0) ) ) {
                        std::cout << "Please enter 1 or 0" << std::endl;
                        input = -1;
                    }
                    if (input == 1) {
                        std::cout << "Keep plotting..." << std::endl;
                        break;
                    }
                    if (input == 0) {
                        std::cout << "Exiting..." << std::endl;
                        gApplication->Terminate();
                    }
                }
                break;
        }   
    }

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
