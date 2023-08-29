#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "TBevt.h"
#include "TButility.h"

#include "functions.cc"

#include "TStyle.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"

int main(int argc, char** argv) {
    gStyle->SetOptFit(1);

    int runNum = std::stoi(argv[1]);
    int maxEntry = std::stoi(argv[2]);

    // Load mapping, pedestal info is not available for now, will be updated
    auto map = getModuleConfigMap();
    std::vector<int> MCP_sum_C_MID_and_Ch = map.at("mid-Ceren"); // MCPPMT summation C ch.
    std::vector<int> MCP_sum_S_MID_and_Ch = map.at("mid-Scint"); // MCPPMT summation S ch.

    // Map returns MID and Ch. in form of std::vector<int> { MID, Ch. }
    TBcid MCP_sum_C = TBcid(MCP_sum_C_MID_and_Ch.at(0), MCP_sum_C_MID_and_Ch.at(1));
    TBcid MCP_sum_S = TBcid(MCP_sum_S_MID_and_Ch.at(0), MCP_sum_S_MID_and_Ch.at(1));

    // Preparing histograms
    TH1F* MCP_sum_C_AvgTimeStruc = new TH1F("MCP_sum_C_AvgTimeStruc", "MCP_sum_C_AvgTimeStruc;bin;ADC", 1000, 0, 1000);
    TH1F* MCP_sum_C_PeakADC = new TH1F("MCP_sum_C_PeakADC", "MCP_sum_C_PeakADC;PeakADC;Evt", 1024, 0, 4096);
    TH1F* MCP_sum_C_IntADC = new TH1F("MCP_sum_C_IntADC", "MCP_sum_C_IntADC;IntADC;Evt", 1000, -5000, 200000);

    TH1F* MCP_sum_S_AvgTimeStruc = new TH1F("MCP_sum_S_AvgTimeStruc", "MCP_sum_S_AvgTimeStruc;bin;ADC", 1000, 0, 1000);
    TH1F* MCP_sum_S_PeakADC = new TH1F("MCP_sum_S_PeakADC", "MCP_sum_S_PeakADC;PeakADC;Evt", 1024, 0, 4096);
    TH1F* MCP_sum_S_IntADC = new TH1F("MCP_sum_S_IntADC", "MCP_sum_S_IntADC;IntADC;Evt", 1000, -5000, 200000);
    
    // Load Ntuple using TChain
    TChain* evtChain = new TChain("event");
    for (int fn = 0; fn < 50; fn++) {
        std::string fileName = "ntuple_Run_" + std::to_string(runNum) + "_Wave_" + std::to_string(fn) + ".root";
        std::string filePath = "/u/user/swkim/SE_UserHome/2023_DRC_TB_ntuple/Run_"  + std::to_string(runNum) + "/Run_" + std::to_string(runNum) + "_Wave/"+ fileName;
        if ( !access(filePath.c_str(), F_OK) ){
            std::cout << fn << " Ntuple file added to TChain : " << filePath << std::endl;
            evtChain->Add(filePath.c_str());
        }
    }
    TBevt<TBwaveform>* anEvt = new TBevt<TBwaveform>(); // Will use waveform data
    evtChain->SetBranchAddress("TBevt", &anEvt);

    // Check total # of events, limit # of events to process if maxEntry is specified
    int totalEntry = evtChain->GetEntries();
    std::cout << "Total entries : " << totalEntry << std::endl;
    if ( (maxEntry > 0) && (maxEntry < totalEntry) ) {
        totalEntry = maxEntry;
        std::cout << "Will process maximum " << std::to_string(totalEntry) << " events" << std::endl;
    }

    // Container to hold waveform subtracted from pedestal (evt-by-evt)
    std::vector<float> MCP_sum_C_pedCorrectedWave;
    std::vector<float> MCP_sum_S_pedCorrectedWave;
    // Evt Loop start
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
        printProgress(iEvt + 1, totalEntry); // Print progress bar
        evtChain->GetEntry(iEvt); // Get event

        TBwaveform MCP_sum_C_Data = anEvt->GetData(MCP_sum_C); // Get data from event
        TBwaveform MCP_sum_S_Data = anEvt->GetData(MCP_sum_S); // Get data from event

        std::vector<short> MCP_sum_C_waveform = MCP_sum_C_Data.waveform(); // From data, get waveform
        std::vector<short> MCP_sum_S_waveform = MCP_sum_S_Data.waveform(); // From data, get waveform

        float MCP_sum_C_ped = std::accumulate( MCP_sum_C_waveform.begin() + 1, MCP_sum_C_waveform.begin() + 101, 0.) / 100.; // Use first 100 bins to get prompt pedestal, will be updated
        float MCP_sum_S_ped = std::accumulate( MCP_sum_S_waveform.begin() + 1, MCP_sum_S_waveform.begin() + 101, 0.) / 100.; // Use first 100 bins to get prompt pedestal, will be updated

        MCP_sum_C_pedCorrectedWave.clear();
        MCP_sum_S_pedCorrectedWave.clear();
        for (int bin = 0; bin < 1000; bin++) {
            int waveformBin = bin + 1;
            MCP_sum_C_AvgTimeStruc->Fill(bin, MCP_sum_C_waveform[waveformBin]); // Fill avg time struc histogram (will be scaled later)
            MCP_sum_S_AvgTimeStruc->Fill(bin, MCP_sum_S_waveform[waveformBin]); // Fill avg time struc histogram (will be scaled later)

            MCP_sum_C_pedCorrectedWave.push_back(MCP_sum_C_ped - MCP_sum_C_waveform[waveformBin]); // Fill (ped - waveform) in container
            MCP_sum_S_pedCorrectedWave.push_back(MCP_sum_S_ped - MCP_sum_S_waveform[waveformBin]); // Fill (ped - waveform) in container
        }

        // Peak search Start bin : 150 End bin : 300
        // Find peak ADC and fill histograms 
        MCP_sum_C_PeakADC->Fill(*std::max_element(MCP_sum_C_pedCorrectedWave.begin() + 150, MCP_sum_C_pedCorrectedWave.begin() + 300));
        MCP_sum_S_PeakADC->Fill(*std::max_element(MCP_sum_S_pedCorrectedWave.begin() + 150, MCP_sum_S_pedCorrectedWave.begin() + 300));

        // Int. search Start bin : 150 End bin : 300
        // Find Int. ADC and fill histograms 
        MCP_sum_C_IntADC->Fill(std::accumulate(MCP_sum_C_pedCorrectedWave.begin() + 150, MCP_sum_C_pedCorrectedWave.begin() + 300, 0.));
        MCP_sum_S_IntADC->Fill(std::accumulate(MCP_sum_S_pedCorrectedWave.begin() + 150, MCP_sum_S_pedCorrectedWave.begin() + 300, 0.));
    }

    // Save the outputs
    std::string outFile = "./Sample_analysis_Run_" + std::to_string(runNum) + ".root";
    TFile* outputRoot = new TFile(outFile.c_str(), "RECREATE");
    outputRoot->cd();

    MCP_sum_C_AvgTimeStruc->Scale( (float)(1. / (float) totalEntry));
    MCP_sum_C_AvgTimeStruc->GetYaxis()->SetRangeUser(-100, 4096);
    MCP_sum_C_AvgTimeStruc->Write();
    MCP_sum_C_PeakADC->Write();
    MCP_sum_C_IntADC->Write();

    MCP_sum_S_AvgTimeStruc->SetLineColor(kRed);
    MCP_sum_S_AvgTimeStruc->Scale( (float)(1. / (float) totalEntry));
    MCP_sum_S_AvgTimeStruc->GetYaxis()->SetRangeUser(-100, 4096);
    MCP_sum_S_AvgTimeStruc->Write();
    MCP_sum_S_PeakADC->SetLineColor(kRed);
    MCP_sum_S_PeakADC->Write();
    MCP_sum_S_IntADC->SetLineColor(kRed);
    MCP_sum_S_IntADC->Write();

    outputRoot->Close();
}
