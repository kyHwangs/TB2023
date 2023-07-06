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
    if (MID != 12) MIDs.push_back(12);

    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), maxEntry, 1, "/Users/yhep/scratch/YUdaq", MIDs);
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TCanvas* c = new TCanvas("c", "c", 800, 800);
    c->cd();
    c->Divide(1,2);
    c->Modified();
    c->Update();

    TH1F* single_time = new TH1F((TString) (plotName + "_timing"), (TString)(plotName+"_timing"), 1000, 0., 200.);
    single_time->GetXaxis()->SetTitle("ns");
    single_time->GetYaxis()->SetTitle("Evt");

    TH1F* diff_time = new TH1F((TString) (plotName + "_diff_timing"), (TString)(plotName+"_diff_timing"), 1000, 0., 200.);
    diff_time->GetXaxis()->SetTitle("ns");
    diff_time->GetYaxis()->SetTitle("Evt");

    int module_start_bin = 100;
    int module_end_bin   = 350;
    if ( (plotName == "T1") || (plotName == "T2") || (plotName == "C1")  || (plotName == "C2")  || (plotName == "T1N") || (plotName == "T2N") || (plotName == "Coin") ) {
        module_start_bin = 400;
        module_end_bin   = 700;
    }
    int trig_start_bin   = 400;
    int trig_end_bin     = 700;

    int rising_time_bin  = 70;

    std::vector<short> single_waveform;
    std::vector<short> trig_waveform;
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
        // 12-26 : Coin
        trig_waveform   = anEvent.GetData(TBcid(12, 18)).waveform();

        int module_thrs_bin = 0;
        float module_ped = getPed(single_waveform);
        float module_min = getMin(single_waveform);
        int   module_idx = getMinIdx(single_waveform);
        if (module_idx < module_start_bin) continue;
        if (module_idx > module_end_bin) continue;
        float module_peakADC = module_ped - module_min;
        for(int bin = module_idx - rising_time_bin; bin < module_idx; bin++) {
            if ( (module_ped - single_waveform.at(bin)) >= ( module_peakADC * fraction ) ) {
                module_thrs_bin = bin;
                break;
            }
        }

        int trig_thrs_bin = 0;
        float trig_ped = getPed(trig_waveform);
        float trig_min = getMin(trig_waveform);
        int   trig_idx = getMinIdx(trig_waveform);
        if (trig_idx < trig_start_bin) continue;
        if (trig_idx > trig_end_bin) continue;
        float trig_peakADC = trig_ped - trig_min;
        for(int bin = trig_idx - rising_time_bin; bin < trig_idx; bin++) {
            if ( (trig_ped - trig_waveform.at(bin)) >= ( trig_peakADC * fraction ) ) {
                trig_thrs_bin = bin;
                break;
            }
        }

        single_time->Fill(module_thrs_bin * 0.2);
        if (plotName != "Coin") diff_time->Fill( (trig_thrs_bin - module_thrs_bin) * 0.2);
        else diff_time->Fill( (module_thrs_bin - trig_thrs_bin ) * 0.2);
    }

    c->cd(1);
    single_time->Draw("Hist");
    c->Modified();
    c->Update();

    c->cd(2);
    diff_time->Draw("Hist");
    c->Modified();
    c->Update();

    // c->SaveAs((TString)("./Timing/Run_" + runNum + "_" + plotName + ".png"));
    
    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
