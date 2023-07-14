#include "TH1.h"

#include "TBdetector.h"
#include "TBmid.h"
#include "TBread.h"

#include "drawFunction.h"

int main(int argc, char* argv[]) {

    int runNum    = std::stoi(argv[1]);
    int start_bin = std::stoi(argv[2]);
    int end_bin   = std::stoi(argv[3]);
	int maxE      = std::stoi(argv[4]);

    // Calib const version : 230707 morning (before run 4227)
    // float mid_S_calib = 0.00004164; TBcid mid_S_cid = TBcid(8, 26);
    // float mid_C_calib = 0.00007905; TBcid mid_C_cid = TBcid(9, 26);
    float mid_S_calib = 0.00018320; TBcid mid_S_cid = TBcid(8, 26); // Last set
    float mid_C_calib = 0.00031072; TBcid mid_C_cid = TBcid(9, 26); // Last set

    float W1_S_calib  = 0.00000420; TBcid W1_S_cid = TBcid(8, 6);
    float W1_C_calib  = 0.00000580; TBcid W1_C_cid = TBcid(9, 6);

    float W2_S_calib  = 0.00000391; TBcid W2_S_cid = TBcid(8, 8);
    float W2_C_calib  = 0.00000851; TBcid W2_C_cid = TBcid(9, 8);

    float W3_S_calib  = 0.00000413; TBcid W3_S_cid = TBcid(8, 4);
    float W3_C_calib  = 0.00000724; TBcid W3_C_cid = TBcid(9, 4);
    
    float HW_S_calib  = 0.00003930; TBcid HW_S_cid = TBcid(8, 2);
    float HW_C_calib  = 0.00006498; TBcid HW_C_cid = TBcid(9, 2);

    float H1_S_calib  = 0.00003428; TBcid H1_S_cid = TBcid(8, 14);
    float H1_C_calib  = 0.00005739; TBcid H1_C_cid = TBcid(9, 14);

    float H2_S_calib  = 0.00003600; TBcid H2_S_cid = TBcid(8, 12);
    float H2_C_calib  = 0.00005601; TBcid H2_C_cid = TBcid(9, 12);

    float H3_S_calib  = 0.00003461; TBcid H3_S_cid = TBcid(8, 20);
    float H3_C_calib  = 0.00005983; TBcid H3_C_cid = TBcid(9, 20);
    
    float L1_S_calib  = 0.00004191; TBcid L1_S_cid = TBcid(8, 16);
    float L1_C_calib  = 0.00006760; TBcid L1_C_cid = TBcid(9, 16);

    float L2_S_calib  = 0.00003089; TBcid L2_S_cid = TBcid(8, 10);
    float L2_C_calib  = 0.00039626; TBcid L2_C_cid = TBcid(9, 10);

    float L3_S_calib  = 0.00004460; TBcid L3_S_cid = TBcid(8, 22);
    float L3_C_calib  = 0.00007100; TBcid L3_C_cid = TBcid(9, 22);

    float L4_S_calib  = 0.00006871; TBcid L4_S_cid = TBcid(8, 18);
    float L4_C_calib  = 0.00013515; TBcid L4_C_cid = TBcid(9, 18);

    std::vector<float> S_calib = {  mid_S_calib,
                                    W1_S_calib,
                                    W2_S_calib,
                                    W3_S_calib,
                                    HW_S_calib,
                                    H1_S_calib,
                                    H2_S_calib,
                                    H3_S_calib,
                                    L1_S_calib,
                                    L2_S_calib,
                                    L3_S_calib,
                                    L4_S_calib};

    std::vector<float> C_calib = {  mid_C_calib,
                                    W1_C_calib,
                                    W2_C_calib,
                                    W3_C_calib,
                                    HW_C_calib,
                                    H1_C_calib,
                                    H2_C_calib,
                                    H3_C_calib,
                                    L1_C_calib,
                                    L2_C_calib,
                                    L3_C_calib,
                                    L4_C_calib};

    std::vector<TBcid> S_cid = {    mid_S_cid,
                                    W1_S_cid,
                                    W2_S_cid,
                                    W3_S_cid,
                                    HW_S_cid,
                                    H1_S_cid,
                                    H2_S_cid,
                                    H3_S_cid,
                                    L1_S_cid,
                                    L2_S_cid,
                                    L3_S_cid,
                                    L4_S_cid};

    std::vector<TBcid> C_cid = {    mid_C_cid,
                                    W1_C_cid,
                                    W2_C_cid,
                                    W3_C_cid,
                                    HW_C_cid,
                                    H1_C_cid,
                                    H2_C_cid,
                                    H3_C_cid,
                                    L1_C_cid,
                                    L2_C_cid,
                                    L3_C_cid,
                                    L4_C_cid};
                        

    TH1F* ceren_plot     = new TH1F("Ceren", "Ceren", 100, 0, 10);
    TH1F* scint_plot     = new TH1F("Scint", "Scint", 100, 0, 10);
    TH1F* sum_plot       = new TH1F("Sum", "Sum", 200, 0, 20);
    TH1F* ceren_plot_cut = new TH1F("Ceren_cut", "Ceren_cut", 100, 0, 10);
    TH1F* scint_plot_cut = new TH1F("Scint_cut", "Scint_cut", 100, 0, 10);
    TH1F* sum_plot_cut   = new TH1F("Sum_cut", "Sum_cut", 200, 0, 20);

    TH1F* c1_plot     = new TH1F("Ceren counter 1", "Ceren counter 1", 1024, 0., 4096.);
    TH1F* c2_plot     = new TH1F("Ceren counter 2", "Ceren counter 2", 1024, 0., 4096.);
    TH1F* c1_plot_cut = new TH1F("Ceren counter 1 cut", "Ceren counter 1 cut", 1024, 0., 4096.);
    TH1F* c2_plot_cut = new TH1F("Ceren counter 2 cut", "Ceren counter 2 cut", 1024, 0., 4096.);

    std::vector<int> MIDs = {8, 9, 12};
    //Run 4183 : 30k event at MCPPMT center after new position set & calib const.
    // TBread<TBwaveform> readerWave = TBread<TBwaveform>(runNum, maxE, -1, "/afs/cern.ch/user/s/sungwon/cernbox/public/TB2023_data", MIDs);
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(runNum, maxE, -1, "/Users/yhep/scratch/YUdaq", MIDs);


    float total_E_S = 0;
    float total_E_C = 0;
    float total_E_Sum = 0;

    float C_1_thr = 50.;
    float C_2_thr = 50.;
    
    for (int iEvt = 0; iEvt < readerWave.GetMaxEvent(); iEvt++) {
        auto event = readerWave.GetAnEvent();
        printProgress(iEvt+1, readerWave.GetMaxEvent());
        
        total_E_S = 0;
        total_E_C = 0;
        total_E_Sum = 0;

        for(int idx = 0; idx < C_cid.size(); idx++) {

            TBcid Sch_cid = S_cid.at(idx);
            TBcid Cch_cid = C_cid.at(idx);
            
            float S_calib_const = S_calib.at(idx);
            float C_calib_const = C_calib.at(idx);
            
            auto S_waveform = event.GetData(Sch_cid).waveform();
            auto C_waveform = event.GetData(Cch_cid).waveform();

            float S_intADC = GetInt(S_waveform, start_bin, end_bin);
            float C_intADC = GetInt(C_waveform, start_bin, end_bin);

            float S_calibE = S_intADC * S_calib_const;
            float C_calibE = C_intADC * C_calib_const;

            total_E_S += S_calibE; 
            total_E_C += C_calibE; 
            total_E_Sum += (S_calibE + C_calibE);
        }

        scint_plot->Fill(total_E_S);
        ceren_plot->Fill(total_E_C);
        sum_plot->Fill(total_E_Sum);

        auto C1_waveform = event.GetData( TBcid(12,  8) ).waveform();
        auto C2_waveform = event.GetData( TBcid(12, 16) ).waveform();

        float C1_peak = GetPeak(C1_waveform, 100, 700);
        float C2_peak = GetPeak(C2_waveform, 100, 700);

        c1_plot->Fill(C1_peak);
        c2_plot->Fill(C2_peak);

        if ( (C1_peak < C_1_thr) || (C2_peak < C_2_thr) ) continue;

        scint_plot_cut->Fill(total_E_S);
        ceren_plot_cut->Fill(total_E_C);
        sum_plot_cut->Fill(total_E_Sum);

        c1_plot_cut->Fill(C1_peak);
        c2_plot_cut->Fill(C2_peak);
    }

    gSystem->mkdir("./E_calib", true);
    TFile* f = TFile::Open( (TString) ("./E_calib/Run_" + std::to_string(runNum) + ".root"), "RECREATE");
    f->cd();

    scint_plot->SetLineColor(kRed);
    scint_plot->SetLineWidth(2);
    scint_plot->Draw("Hist");
    scint_plot->Write();

    ceren_plot->SetLineColor(kBlue);
    ceren_plot->SetLineWidth(2);
    ceren_plot->Draw("Hist");
    ceren_plot->Write();

    sum_plot->SetLineColor(kBlack);
    sum_plot->SetLineWidth(2);
    sum_plot->Draw("Hist");
    sum_plot->Write();

    c1_plot->Draw("Hist");
    c1_plot->SetLineWidth(2);
    c1_plot->Write();

    c2_plot->Draw("Hist");
    c2_plot->SetLineWidth(2);
    c2_plot->Write();

    scint_plot_cut->SetLineColor(kRed);
    scint_plot_cut->SetLineWidth(2);
    scint_plot_cut->Draw("Hist");
    scint_plot_cut->Write();

    ceren_plot_cut->SetLineColor(kBlue);
    ceren_plot_cut->SetLineWidth(2);
    ceren_plot_cut->Draw("Hist");
    ceren_plot_cut->Write();

    sum_plot_cut->SetLineColor(kBlack);
    sum_plot_cut->SetLineWidth(2);
    sum_plot_cut->Draw("Hist");
    sum_plot_cut->Write();

    c1_plot_cut->Draw("Hist");
    c1_plot_cut->SetLineWidth(2);
    c1_plot_cut->Write();

    c2_plot_cut->Draw("Hist");
    c2_plot_cut->SetLineWidth(2);
    c2_plot_cut->Write();

    f->Close();

    return 0;
}
