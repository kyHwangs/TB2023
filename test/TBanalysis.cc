#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>

#include "TBevt.h"
#include "TButility.h"

#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>

// compile rule
// g++ -I<path_to_include_dir> -L<path_to_lib_dir> <path_to_libdrcTB.so> `root-config --cflags --libs` TBanalysis.cc -o TBanalysis
// example
// g++ -I/u/user/kyhwang/scratch/WorkSpace/DRC/Aug2022TB/dev/dual-readout_TB/install/include -L/u/user/kyhwang/scratch/WorkSpace/DRC/Aug2022TB/dev/dual-readout_TB/install/lib64 /u/user/kyhwang/scratch/WorkSpace/DRC/Aug2022TB/dev/dual-readout_TB/install/lib64/libdrcTB.so `root-config --cflags --libs` TBanalysis.cc -o TBanalysis

float peakADC(std::vector<short> waveform){
  float ped = 0;
  for ( int i = 1; i < 101; i++ )
    ped += (float)waveform.at(i)/100.;

  
  return ped - (float)*std::min_element(waveform.begin()+1, waveform.end()-23);
}

int main(int argc, char** argv) {

  TString baseDir = "/pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/ntuple/waveform/";
  TString runNum = argv[1];
  TString filename = baseDir + "ntuple_Run_" + runNum + "_Wave.root";

  TH1D* muPeak = new TH1D("", ";;", 1024, 0., 4096.);
  TH1D* psPeak = new TH1D("", ";;", 1024, 0., 4096.);


  TFile* afile = new TFile(filename);
  TTree* atree = (TTree*)afile->Get("events");

  TBevt<TBwaveform>* anevt = new TBevt<TBwaveform>();
  atree->SetBranchAddress("TBevt",&anevt);

  TButility utility = TButility();
  utility.loading("mapping_data_MCPPMT_positiveSignal_v3.csv");
  utility.loadped("sample_ped.csv");

  TBcid pscid = utility.getcid(TBdetector::detid::preshower);
  TBcid mucid = utility.getcid(TBdetector::detid::muon);

  unsigned channelsize = 32;

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

  int maxEvents = atree->GetEntries();

  // for (unsigned ievt = 0; ievt < atree->GetEntries(); ievt++) {
  for (unsigned ievt = 0; ievt < maxEvents; ievt++) {

    if ( ievt > 0 && ievt % 8 == 0 ) {
      std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; //delete
      float percent_done = 1. * ievt / (float)maxEvents;
      std::chrono::duration time_left = time_taken * ( 1 / percent_done - 1 );
      std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
      std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
      std::cout << "\r\033[F"<< " " << ievt << " events  "<< minutes_left.count() << ":";
      printf("%02d left   %.1f%%            \n",int(seconds_left.count()),percent_done*100);
    }

    atree->GetEntry(ievt);

    //example: id (0:nothing, 1:hadron, 2:mu, 3:el)
    TBwaveform adata_ps = anevt->data(pscid);
    TBwaveform adata_mu = anevt->data(mucid);
    // std::cout << "pid:" << utility.pid(adata_ps.adc(), adata_mu.adc()) << std::endl;
    // std::cout << ievt << " | ps peak : " << peakADC(adata_ps.waveform()) << " | mc peak : " << peakADC(adata_mu.waveform()) << std::endl;

    muPeak->Fill(peakADC(adata_mu.waveform()));
    psPeak->Fill(peakADC(adata_ps.waveform()));

    // double adc = 0.;

    // for (int nmid = 0; nmid < anevt->size(); nmid++) {
    //   for (int ch = 0; ch < channelsize; ch++) {
    //     TBcid cid = TBcid(nmid+1,ch+1); // mid 1 - 15, ch 1 - 32
    //     TBdetector det = utility.find(cid);

    //     // example: sum adc of all scint SiPMs
    //     if (!det.isNull() && det.isModule() && det.isSiPM() && !det.isCeren()) {
    //       TBwaveform adata = anevt->data(cid);
    //       adc += adata.adc();
    //     }
    //   } // channel loop
    // } // mid loop

    // std::cout << adc << std::endl;
  } // event loop

  TCanvas* c = new TCanvas("","");

  c->cd(); muPeak->Draw("Hist"); c->SaveAs("ADC_muPeak.png");
  c->cd(); psPeak->Draw("Hist"); c->SaveAs("ADC_psPeak.png");

  return 0;
}
