#include "drawFunction.h"

#include "TBevt.h"
#include "TBread.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    std::string runNum   = argv[1];
    std::string plotName = argv[2];
    float fraction = std::stof(argv[3]);
    int maxEntry  = std::stoi(argv[4]);

    auto map = getModuleConfigMap();
    if (map.find(plotName) == map.end()) {
        std::cout << "Plot Name error : " << plotName << " not found" << std::endl;
        return 0;
    }

    std::vector<int> MIDandCh = map.at(plotName);
    int MID = MIDandCh.at(0);
    int Ch  = MIDandCh.at(1);

    std::vector<int> MIDs = {MID};
    if (MID != 8) MIDs.push_back(8);
    if (MID != 12) MIDs.push_back(12);

    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), maxEntry, -1, "/Users/yhep/scratch/YUdaq", MIDs);
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TCanvas* c = new TCanvas("c", "c", 800, 1200);
    c->cd();
    c->Divide(1,3);
    c->Modified();
    c->Update();

    TH1F* single_time = new TH1F((TString) (plotName + "_timing"), (TString)(plotName+"_timing"), 1000, 0., 200.);
    single_time->GetXaxis()->SetTitle("ns");
    single_time->GetYaxis()->SetTitle("Evt");

    TH1F* trig_time = new TH1F((TString) (plotName + "_trig_timing"), (TString)(plotName+"_trig_timing"), 1000, 0., 200.);
    trig_time->GetXaxis()->SetTitle("ns");
    trig_time->GetYaxis()->SetTitle("Evt");

    TH1F* diff_time = new TH1F((TString) (plotName + "_diff_timing"), (TString)(plotName+"_diff_timing"), 1000, 0., 200.);
    diff_time->GetXaxis()->SetTitle("ns");
    diff_time->GetYaxis()->SetTitle("Evt");

    std::vector<short> single_waveform;
    std::vector<short> trig_waveform;
    std::vector<short> c1_waveform;
    std::vector<short> c2_waveform;

    for(int iEvt = 0; iEvt < readerWave.GetMaxEvent(); iEvt++) {
        printProgress(iEvt+1, maxEntry);

        single_waveform.clear();
        trig_waveform.clear();

        auto anEvent = readerWave.GetAnEvent();
        single_waveform = anEvent.GetData(TBcid(MID, Ch)).waveform();
        // 12-2  : T1
        // 12-8  : C1
        // 12-10 : T2
        // 12-16 : C2 
        // 12-18 : T1N
        // 12-24 : T2N
        // 8-15 : Coin
        trig_waveform   = anEvent.GetData(TBcid(8, 15)).waveform();
        c1_waveform = anEvent.GetData(TBcid(12, 8)).waveform();
        c2_waveform = anEvent.GetData(TBcid(12, 16)).waveform();

        float c1_peakADC = GetPeak(c1_waveform, 200, 700);
        float c2_peakADC = GetPeak(c2_waveform, 200, 700);

        if ( (c1_peakADC < 300) || (c2_peakADC < 150) ) continue;

        int module_thrs_bin = 0;
        float module_ped = std::accumulate( single_waveform.begin() + 1, single_waveform.begin() + 101, 0.) / 100.;
        // 100 bin ~ 350 bin
        float module_min = *(std::min_element(single_waveform.begin() + 100, single_waveform.end() - 673));

        int   module_idx = std::distance( single_waveform.begin(), std::min_element(single_waveform.begin() + 100, single_waveform.end() - 673) );
        float module_peakADC = module_ped - module_min;

        for(int bin = module_idx - 100; bin < module_idx; bin++) {
            if ( (module_ped - single_waveform.at(bin)) >= ( module_peakADC * fraction ) ) {
                module_thrs_bin = bin;
                break;
            }
        }

        int trig_thrs_bin = 0;
        float tirg_ped = std::accumulate( trig_waveform.begin() + 1, trig_waveform.begin() + 101, 0.) / 100.;
        // 650 bin ~ 850 bin for coin
        // 550 ~ 800 bin for T2N
        // float tirg_min = *(std::min_element(trig_waveform.begin() + 650, trig_waveform.end() - 173));
        // int   tirg_idx = std::distance( trig_waveform.begin(), std::min_element(trig_waveform.begin() + 650, trig_waveform.end() - 173) );
        float tirg_min = *(std::min_element(trig_waveform.begin() + 550, trig_waveform.end() - 223));
        int   tirg_idx = std::distance( trig_waveform.begin(), std::min_element(trig_waveform.begin() + 550, trig_waveform.end() - 223) );
        float tirg_peakADC = tirg_ped - tirg_min;

        for(int bin = tirg_idx - 100; bin < tirg_idx; bin++) {
            if ( (tirg_ped - trig_waveform.at(bin)) >= ( tirg_peakADC * fraction ) ) {
                trig_thrs_bin = bin;
                break;
            }
        }


        single_time->Fill(module_thrs_bin * 0.2);
        trig_time->Fill(trig_thrs_bin * 0.2);
        // if (plotName != "Coin") diff_time->Fill( (trig_thrs_bin - module_thrs_bin) * 0.2);
        // else diff_time->Fill( (module_thrs_bin - trig_thrs_bin ) * 0.2);
        diff_time->Fill( (trig_thrs_bin - module_thrs_bin ) * 0.2);
    }

    c->cd(1);
    single_time->Draw("Hist");
    c->Modified();
    c->Update();

    c->cd(2);
    trig_time->Draw("Hist");
    c->Modified();
    c->Update();

    c->cd(3);
    diff_time->Draw("Hist");
    c->Modified();
    c->Update();

    // c->SaveAs((TString)("./Timing/Run_" + runNum + "_" + plotName + ".png"));
    
    // TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    // rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
