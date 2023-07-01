#include "drawFunction.h"

enum plotting_mode{
    kAuto,
    kEnter,
    kPress1toPass
};

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    std::string runNum   = argv[1];
    std::string plotName = argv[2];
    int start_evt        = std::stoi(argv[3]);
    int max_evt          = std::stoi(argv[4]);
    plotting_mode plotMode = kEnter;

    char meta[64];
    short adc[32736];

    auto map = getModuleConfigMap();
    if (map.find(plotName) == map.end()) {
        std::cout << "Plot Name error : " << plotName << " not found" << std::endl;
        return 0;
    }

    std::vector<int> MIDandCh = map.at(plotName);
    int MID = MIDandCh.at(0);
    int Ch  = MIDandCh.at(1);

    int total_entry = 0;
    std::vector<int> entry_per_file;
    std::vector<std::string> fileName_vector;
    for (int idx = 0; idx < 1000; idx++) {
        std::string fileName = getFileName(runNum, MID, idx);
        if (!access(fileName.c_str(), F_OK)) {
            int entry = getEntry(fileName);
            entry_per_file.emplace_back(entry);
            fileName_vector.emplace_back(fileName);
            total_entry += entry;
        }
        else {
            break;
        }
    }

    std::cout << "Total # of file : " << entry_per_file.size() <<  " total # of entry : " << total_entry << std::endl;
    if (total_entry < start_evt) start_evt = 0;
    if (total_entry < start_evt + max_evt) max_evt = total_entry - start_evt;

    TCanvas* c = new TCanvas("c", "c", 800, 600);
    TH1F* plot = new TH1F((TString) (plotName + "_waveform"), (TString)(plotName+"_waveform"), 1024, 0., 1024.);
    plot->SetStats(0);
    plot->SetMaximum(4096.);
    plot->SetMinimum(0.);
    plot->GetXaxis()->SetTitle("bin");
    plot->GetYaxis()->SetTitle("ADC");

    int event_passed = 0;
    for (int fileIdx = 0; fileIdx < fileName_vector.size(); )  {
        FILE* dat_file = fopen(fileName_vector.at(fileIdx).c_str(), "rb");

        for(int iEvt = 0; iEvt < entry_per_file.at(fileIdx); iEvt++) {
            plot->Reset();
            fread(meta, 1, 64, dat_file);
            fread(adc, 2, 32736, dat_file);
            event_passed++;
            if (start_evt > event_passed) continue;
            if (event_passed == start_evt + max_evt) return 0;
            plot->SetTitle( (TString) (plotName + "_evt_" + std::to_string(iEvt+1)) );
            for(int idx = 0; idx < 1024; idx++) {
                plot->Fill(idx, adc[idx * 32 + Ch]);
            }
            c->cd();
            plot->Draw("Hist");
            c->Update();

            switch(plotMode) {
                case kAuto :
                    gSystem->Sleep(600);
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

        fclose(dat_file);
    }

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
