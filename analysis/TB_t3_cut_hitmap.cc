#include "TLegend.h"
#include "TLatex.h"
#include "TColor.h"
#include "TH2.h"

#include "drawFunction.h"

#include "TBread.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    gStyle->SetOptStat(0);

    std::string runNum = argv[1];
    int max_evt        = std::stoi(argv[2]);
    int start_bin      = std::stoi(argv[3]);
    int end_bin        = std::stoi(argv[4]);
    float c1_cut       = std::stof(argv[5]);
    float c2_cut       = std::stof(argv[6]);
    float t3_cut       = std::stof(argv[7]);
    int logic          = std::stoi(argv[8]);

    auto map = getModuleConfigMap();
    TH2F* hit_C = new TH2F( (TString)("hit_C_Run_" + runNum), (TString)("hit_C_Run_" + runNum), 5, 0., 5., 10, 0., 10.);
    TH2F* hit_S = new TH2F( (TString)("hit_S_Run_" + runNum), (TString)("hit_S_Run_" + runNum), 5, 0., 5., 10, 0., 10.);

    // std::vector<int> unique_MIDs;
    // std::map<int, int> keyMap;
    // if (keyMap.find(1) == keyMap.end()){
    //     keyMap.insert(std::make_pair(1, 1));
    //     unique_MIDs.push_back(1);
    // }
    // if (keyMap.find(2) == keyMap.end()){
    //     keyMap.insert(std::make_pair(2, 1));
    //     unique_MIDs.push_back(2);
    // }
    // if (keyMap.find(3) == keyMap.end()){
    //     keyMap.insert(std::make_pair(3, 1));
    //     unique_MIDs.push_back(3);
    // }
    // if (keyMap.find(4) == keyMap.end()){
    //     keyMap.insert(std::make_pair(4, 1));
    //     unique_MIDs.push_back(4);
    // }
    // if (keyMap.find(12) == keyMap.end()){
    //     keyMap.insert(std::make_pair(12, 1));
    //     unique_MIDs.push_back(12);
    // }
    // if (keyMap.find(8) == keyMap.end()){
    //     keyMap.insert(std::make_pair(8, 1));
    //     unique_MIDs.push_back(8);
    // }
    std::vector<int> MIDs = {1, 2, 3, 4, 8, 12};
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), max_evt, -1, "/Users/yhep/scratch/YUdaq", MIDs);
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    int event_passed = 0;
    std::vector<float> mcppmt_c_IntADC(50, 0.);
    std::vector<float> mcppmt_s_IntADC(50, 0.);
    for(int iEvt = 0; iEvt < readerWave.GetMaxEvent(); iEvt++) {
        printProgress(iEvt, readerWave.GetMaxEvent());

        auto anEvent = readerWave.GetAnEvent();

        auto c1_waveform = anEvent.GetData(TBcid(12, 8)).waveform();
        auto c2_waveform = anEvent.GetData(TBcid(12, 16)).waveform();
        auto t3_waveform = anEvent.GetData(TBcid(8, 19)).waveform();

        float c1_peakADC = GetPeak(c1_waveform, 350, 700);
        float c2_peakADC = GetPeak(c2_waveform, 200, 500);
        float t3_peakADC = GetPeak(t3_waveform, 300, 500); // T3 cut : 20 mV = 81.92ADC ~ 80ADC

        if (logic == 11) {
            if (!( (c1_peakADC > c1_cut) && (c2_peakADC > c2_cut) ) ) continue;
        }
        if (logic == 10) {
            if (!( (c1_peakADC > c1_cut) && (c2_peakADC < c2_cut) ) ) continue;
        }
        if (logic == 1) {
            if (!( (c1_peakADC < c1_cut) && (c2_peakADC > c2_cut) ) ) continue;
        }
        if (logic == 0) {
            if (!( (c1_peakADC < c1_cut) && (c2_peakADC < c2_cut) ) ) continue;
        }
        if (t3_peakADC < t3_cut) continue; // T3 cut : 20 mV = 81.92ADC ~ 80ADC
        event_passed++;

        for (int idx = 0; idx < mcppmt_c_names.size(); idx++) {
            TBcid cid = TBcid( map.at(mcppmt_c_names.at(idx)).at(0), map.at(mcppmt_c_names.at(idx)).at(1) );
            int row = IdxToRow(idx);
            int col = IdxToCol(idx);
            auto mcppmt_c_waveform = anEvent.GetData(cid).waveform();
            float IntADC = GetInt(mcppmt_c_waveform, 100, 300); // MCPPMT : 100~300 bin for int range
            mcppmt_c_IntADC.at(idx) = ( mcppmt_c_IntADC.at(idx) + (IntADC / (float)readerWave.GetMaxEvent()) );
        }
        for (int idx = 0; idx < mcppmt_s_names.size(); idx++) {
            TBcid cid = TBcid( map.at(mcppmt_s_names.at(idx)).at(0), map.at(mcppmt_s_names.at(idx)).at(1) );
            int row = IdxToRow(idx);
            int col = IdxToCol(idx);
            auto mcppmt_s_waveform = anEvent.GetData(cid).waveform();
            float IntADC = GetInt(mcppmt_s_waveform, 100, 300); // MCPPMT : 100~300 bin for int range
            mcppmt_s_IntADC.at(idx) = ( mcppmt_s_IntADC.at(idx) + (IntADC / (float)readerWave.GetMaxEvent()) );
        }
    }

    std::cout << "Total event = " << readerWave.GetMaxEvent() << std::endl;
    std::cout << "Event passed = " << event_passed << std::endl;
    std::cout << "Passed / Total = " << 100. * (float) event_passed / (float) readerWave.GetMaxEvent() << " %" << std::endl;

    for(int idx = 0; idx < mcppmt_s_names.size(); idx++) {
        hit_C->SetBinContent( IdxToRow(idx), IdxToCol(idx), mcppmt_c_IntADC.at(idx) );
        hit_S->SetBinContent( IdxToRow(idx), IdxToCol(idx), mcppmt_s_IntADC.at(idx) );
    }

    for(int row = 1; row <= 5; row++) {
        hit_C->GetXaxis()->SetBinLabel(row, std::to_string(row).c_str());
        hit_S->GetXaxis()->SetBinLabel(row, std::to_string(row).c_str());
    }
    for(int col = 1; col <= 10; col++) {
        hit_C->GetYaxis()->SetBinLabel(col, std::to_string(col).c_str());
        hit_S->GetYaxis()->SetBinLabel(col, std::to_string(col).c_str());
    }

    TCanvas* hit = new TCanvas("hit", "hit", 1200, 600);
    hit->cd();
    hit->Divide(2,1);
    hit->cd(1);
    Double_t ceren_diff = hit_C->GetMaximum() - hit_C->GetMinimum();
    Double_t ceren_level[] = { hit_C->GetMinimum(),
                               hit_C->GetMinimum() + (ceren_diff * 0.1),
                               hit_C->GetMinimum() + (ceren_diff * 0.2),
                               hit_C->GetMinimum() + (ceren_diff * 0.3),
                               hit_C->GetMinimum() + (ceren_diff * 0.4),
                               hit_C->GetMinimum() + (ceren_diff * 0.5),
                               hit_C->GetMinimum() + (ceren_diff * 0.6),
                               hit_C->GetMinimum() + (ceren_diff * 0.7),
                               hit_C->GetMinimum() + (ceren_diff * 0.8),
                               hit_C->GetMinimum() + (ceren_diff * 0.9),
                               hit_C->GetMaximum()}; // Hitmap Ceren and Scint both color map displayed in same ratio
    hit_C->SetContour( ( sizeof(ceren_level) / sizeof(Double_t) ), ceren_level);
    hit_C->Draw("colz & text");
    hit->Update();

    hit->cd(2);
    Double_t scint_diff = hit_S->GetMaximum() - hit_S->GetMinimum();
    Double_t scint_level[] = { hit_S->GetMinimum(),
                               hit_S->GetMinimum() + (scint_diff * 0.1),
                               hit_S->GetMinimum() + (scint_diff * 0.2),
                               hit_S->GetMinimum() + (scint_diff * 0.3),
                               hit_S->GetMinimum() + (scint_diff * 0.4),
                               hit_S->GetMinimum() + (scint_diff * 0.5),
                               hit_S->GetMinimum() + (scint_diff * 0.6),
                               hit_S->GetMinimum() + (scint_diff * 0.7),
                               hit_S->GetMinimum() + (scint_diff * 0.8),
                               hit_S->GetMinimum() + (scint_diff * 0.9),
                               hit_S->GetMaximum()}; // Hitmap Ceren and Scint both color map displayed in same ratio
    hit_S->SetContour( ( sizeof(scint_level) / sizeof(Double_t) ), scint_level);
    hit_S->Draw("colz & text");
    hit->Update();

    app.Run();

    return 0;
}
