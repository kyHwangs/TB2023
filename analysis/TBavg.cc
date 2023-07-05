#include "TLegend.h"

#include "drawFunction.h"

#include "TBread.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    std::string runNum   = argv[1];
    std::vector<std::string> channel_names;
    for(int plot_args = 4; plot_args < argc; plot_args++ ) {
        channel_names.push_back(argv[plot_args]);
    }
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
    for(int idx = 0; idx < channel_names.size(); idx++) {
        std::vector<int> MIDandCh = map.at(channel_names.at(idx));
        MIDs.push_back(MIDandCh.at(0));
        unique_MIDs.push_back(MIDandCh.at(0));
        Chs.push_back(MIDandCh.at(1));
        plots.push_back(new TH1F( (TString)channel_names.at(idx), (TString)channel_names.at(idx), 1000, 0., 1000.) );
    }
    unique_MIDs.erase( std::unique( unique_MIDs.begin(), unique_MIDs.end() ), unique_MIDs.end() );
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), start_evt + max_evt, -1, "/Users/yhep/scratch/YUdaq", unique_MIDs);
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TCanvas* c = new TCanvas("c", "c", 800, 600);
    c->cd();

    for(int iEvt = 0; iEvt < start_evt + max_evt; iEvt++) {
        if (iEvt < start_evt) continue;
        if (iEvt > start_evt + max_evt) gApplication->Terminate();

        auto anEvent = readerWave.GetAnEvent();
        for(int idx = 0; idx < plots.size(); idx++) {
            TBcid cid = TBcid(MIDs.at(idx), Chs.at(idx));
            auto single_waveform = anEvent.GetData(cid).waveform();
            plots.at(idx)->Reset();
            plots.at(idx)->SetTitle( (TString)(channel_names.at(idx) + "_Evt_" + std::to_string(iEvt+1)) );
            plots.at(idx)->SetMaximum(4096.);
            plots.at(idx)->SetMinimum(0.);
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
        c->BuildLegend();
        c->Update();
    }

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
