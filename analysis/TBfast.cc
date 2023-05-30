#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

#include "TBevt.h"
#include "TButility.h"
#include "TBload.h"
#include "TBhist.h"

#include "functions.cc"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>



int main(int argc, char** argv) {

  gStyle->SetOptFit(1);

  int runNum = std::stoi(argv[1]);
  TBload<TBfastmode>* loader = new TBload<TBfastmode>(runNum);

  int maxEvents = std::stoi(argv[2]); 

  std::string pedPath = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/pedestal/mean/Run" + std::to_string(runNum) + "_pedestalHist_mean.root";

  TButility utility = TButility();
  utility.loading("dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/mapping/mapping_Aug2022TB.root");
  utility.loadped(pedPath);

  AuxFactory<TBfastmode>* auxFac = new AuxFactory<TBfastmode>(runNum, 2.5);
  DWCset<TBfastmode>* dwcSet = new DWCset<TBfastmode>(runNum, dwc_calib);

  TBcid pscid = utility.getcid(TBdetector::detid::preshower);
  TBcid mucid = utility.getcid(TBdetector::detid::muon);
  TBcid tccid = utility.getcid(TBdetector::detid::tail);

  pscid.print();
  mucid.print();
  tccid.print();

  // TBcid pscid = TBcid(1,25);
  // TBcid mucid = TBcid(1,27);
  // TBcid tccid = TBcid(1,26);

  std::vector<TBcid> dwc1;
  dwc1.push_back(TBcid(1,17));
  dwc1.push_back(TBcid(1,19));
  dwc1.push_back(TBcid(1,21));
  dwc1.push_back(TBcid(1,23));

  std::vector<TBcid> dwc2;
  dwc2.push_back(TBcid(1,25));
  dwc2.push_back(TBcid(1,27));
  dwc2.push_back(TBcid(1,29));
  dwc2.push_back(TBcid(1,31));

  for ( int i = 0; i < dwc1.size(); i++ )
    dwc1.at(i).print();

  for ( int i = 0; i < dwc2.size(); i++ )
    dwc2.at(i).print();


  std::vector<TBcid> M1TC;
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, true));

  for ( int i = 0; i < M1TC.size(); i++ )
    M1TC.at(i).print();

  // std::vector<TBcid> M1TS;
  // M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, false));
  // M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, false));
  // M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, false));
  // M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, false));

  // std::vector<TBcid> M2TC;
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, true));
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, true));
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, true));
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, true));
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, true));
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, true));
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, true));
  // M2TC.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, true));

  // std::vector<TBcid> M2TS;
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 1, false));
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 2, false));
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 3, false));
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 4, false));
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 6, false));
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 7, false));
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 8, false));
  // M2TS.push_back(utility.getcid(TBdetector::detid::PMT, 2, 9, false));

  TBhist* preShower = new TBhist("preshower", "int. ADC", "events", 820, -5000., 200000.);
  TBhist* muonCounter = new TBhist("muoncounter", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* tailCapture = new TBhist("tailcapture", "int. ADC", "events", 1100, -5000., 50000.);

  // TBhist* module_M1T1C = new TBhist("M1T1C", "int. ADC", "events", 1100, -5000., 50000.); module_M1T1C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M1T2C = new TBhist("M1T2C", "int. ADC", "events", 1100, -5000., 50000.); module_M1T2C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M1T3C = new TBhist("M1T3C", "int. ADC", "events", 1100, -5000., 50000.); module_M1T3C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M1T4C = new TBhist("M1T4C", "int. ADC", "events", 1100, -5000., 50000.); module_M1T4C->getHist()->SetLineColor(kBlue);
  
  // TBhist* module_M1T1S = new TBhist("M1T1S", "int. ADC", "events", 1100, -5000., 50000.); module_M1T1S->getHist()->SetLineColor(kRed);
  // TBhist* module_M1T2S = new TBhist("M1T2S", "int. ADC", "events", 1100, -5000., 50000.); module_M1T2S->getHist()->SetLineColor(kRed);
  // TBhist* module_M1T3S = new TBhist("M1T3S", "int. ADC", "events", 1100, -5000., 50000.); module_M1T3S->getHist()->SetLineColor(kRed);
  // TBhist* module_M1T4S = new TBhist("M1T4S", "int. ADC", "events", 1100, -5000., 50000.); module_M1T4S->getHist()->SetLineColor(kRed);
  
  // TBhist* module_M2T1C = new TBhist("M2T1C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T1C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M2T2C = new TBhist("M2T2C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T2C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M2T3C = new TBhist("M2T3C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T3C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M2T4C = new TBhist("M2T4C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T4C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M2T6C = new TBhist("M2T6C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T6C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M2T7C = new TBhist("M2T7C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T7C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M2T8C = new TBhist("M2T8C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T8C->getHist()->SetLineColor(kBlue);
  // TBhist* module_M2T9C = new TBhist("M2T9C", "int. ADC", "events", 1100, -5000., 50000.); module_M2T9C->getHist()->SetLineColor(kBlue);
  
  // TBhist* module_M2T1S = new TBhist("M2T1S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T1S->getHist()->SetLineColor(kRed);
  // TBhist* module_M2T2S = new TBhist("M2T2S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T2S->getHist()->SetLineColor(kRed);
  // TBhist* module_M2T3S = new TBhist("M2T3S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T3S->getHist()->SetLineColor(kRed);
  // TBhist* module_M2T4S = new TBhist("M2T4S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T4S->getHist()->SetLineColor(kRed);
  // TBhist* module_M2T6S = new TBhist("M2T6S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T6S->getHist()->SetLineColor(kRed);
  // TBhist* module_M2T7S = new TBhist("M2T7S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T7S->getHist()->SetLineColor(kRed);
  // TBhist* module_M2T8S = new TBhist("M2T8S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T8S->getHist()->SetLineColor(kRed);
  // TBhist* module_M2T9S = new TBhist("M2T9S", "int. ADC", "events", 1100, -5000., 50000.); module_M2T9S->getHist()->SetLineColor(kRed);

  TH2D* dwc1pos = new TH2D("dwc1pos", "dwc1pos;mm;mm;events", 480, -120, 120, 480, -120, 120);
  TH2D* dwc2pos = new TH2D("dwc2pos", "dwc2pos;mm;mm;events", 480, -120, 120, 480, -120, 120);
  TH2D* dwc1vs2x = new TH2D("dwc1vs2x", "dwc1vs2x;mm;mm", 480, -120, 120, 480, -120, 120);
  TH2D* dwc1vx2y = new TH2D("dwc1vx2y", "dwc1vx2y;mm;mm", 480, -120, 120, 480, -120, 120);

  TBhist* dwc1_X = new TBhist("dwc1_X", "mm", "events", 480, -120., 120.);
  TBhist* dwc1_Y = new TBhist("dwc1_Y", "mm", "events", 480, -120., 120.);
  TBhist* dwc2_X = new TBhist("dwc2_X", "mm", "events", 480, -120., 120.);
  TBhist* dwc2_Y = new TBhist("dwc2_Y", "mm", "events", 480, -120., 120.);

  TBhist* dwc_center_diff = new TBhist("dwc_center_offset", "mm", "events", 240, 0., 120.);
  TBhist* dwc1_centerPos = new TBhist("dwc1_centerPos", "mm", "events", 240, 0., 120.);
  TBhist* dwc2_centerPos = new TBhist("dwc2_centerPos", "mm", "events", 240, 0., 120.);
  TBhist* dwc_center_diff_angle = new TBhist("dec_cernter_angle_offset", "deg", "events", 150, 0., 1.5);

  TBhist* dwc_X_diff = new TBhist("dwc_X_diff", "mm", "events", 480, -120., 120.);
  TBhist* dwc_Y_diff = new TBhist("dwc_Y_diff", "mm", "events", 480, -120., 120.);
  TBhist* dwc_X_diff_abs = new TBhist("dwc_X_diff_abs", "mm", "events", 120, 0., 120.);
  TBhist* dwc_Y_diff_abs = new TBhist("dwc_Y_diff_abs", "mm", "events", 120, 0., 120.);

 

  if ( maxEvents == -1 )
    maxEvents = loader->getEntries();

  if ( maxEvents > loader->getEntries() )
    throw std::runtime_error("Events number is larger than entries !");

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

  for ( int ievt = 0; ievt < maxEvents; ievt++ ) {

    if ( ievt > 0 && ievt % 8 == 0 ) {
      std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; //delete
      float percent_done = 1. * ievt / (float)maxEvents;
      std::chrono::duration time_left = time_taken * ( 1 / percent_done - 1 );
      std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
      std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
      std::cout << "\r\033[F"<< " " << ievt << " events  "<< minutes_left.count() << ":";
      printf("%02d left   %.1f%%            \n",int(seconds_left.count()),percent_done*100);
    }

    loader->loading();
    auto anEvt = loader->getEvent();

    TBfastmode adata_ps = anEvt->data(pscid);
    TBfastmode adata_mu = anEvt->data(mucid);
    TBfastmode adata_tc = anEvt->data(tccid);

    std::vector<TBfastmode> DWC1_fast;
    for ( int i = 0; i < dwc1.size(); i++ )
      DWC1_fast.push_back(anEvt->data(dwc1.at(i)));

    std::vector<TBfastmode> DWC2_fast;
    for ( int i = 0; i < dwc2.size(); i++ )
      DWC2_fast.push_back(anEvt->data(dwc2.at(i)));

    // std::vector<TBfastmode> M2TC_data;
    // std::vector<TBfastmode> M2TS_data;
    // for ( int i = 0; i < 8; i++ ) {
    //   M2TC_data.push_back(anEvt->data(M2TC.at(i)));
    //   M2TS_data.push_back(anEvt->data(M2TS.at(i)));
    // }

    // std::vector<TBfastmode> M1TC_data;
    // std::vector<TBfastmode> M1TS_data;
    // for ( int i = 0; i < M1TC.size(); i++ ) {
    //   M1TC_data.push_back(anEvt->data(M1TC.at(i)));
    //   M1TS_data.push_back(anEvt->data(M1TS.at(i)));
    // }

    std::vector<int> dwc_time;

    for ( int i = 0; i < 4; i++ )
      dwc_time.push_back(DWC1_fast.at(i).timing());

    for ( int i = 0; i < 4; i++ )
      dwc_time.push_back(DWC2_fast.at(i).timing());

    auto dwc_pos = getDWCposition(dwc_time);
    auto centerPos = getPositionFromDWC(dwc_pos);
    float lecDiff = getLengthDiff(dwc_pos);
    float angleDiff = 180./const_pi() * std::atan( lecDiff / 1675. );

    float muADC = adata_mu.adc();
    float psADC = adata_ps.adc();
    float tcADC = adata_tc.adc();


    dwc_X_diff->Fill(dwc_pos.at(0) - dwc_pos.at(2));
    dwc_Y_diff->Fill(dwc_pos.at(1) - dwc_pos.at(3));

    dwc_X_diff_abs->Fill(std::abs(dwc_pos.at(0) - dwc_pos.at(2)));
    dwc_Y_diff_abs->Fill(std::abs(dwc_pos.at(1) - dwc_pos.at(3)));

    dwc_center_diff->Fill(lecDiff);
    dwc_center_diff_angle->Fill(angleDiff);
    dwc1_centerPos->Fill(centerPos.at(0));
    dwc2_centerPos->Fill(centerPos.at(1));

    dwc1pos->Fill(dwc_pos.at(0), dwc_pos.at(1));
    dwc2pos->Fill(dwc_pos.at(2), dwc_pos.at(3));
    dwc1vs2x->Fill(dwc_pos.at(0), dwc_pos.at(2));
    dwc1vx2y->Fill(dwc_pos.at(1), dwc_pos.at(3));

    dwc1_X->Fill(dwc_pos.at(0));
    dwc1_Y->Fill(dwc_pos.at(1));
    dwc2_X->Fill(dwc_pos.at(2));
    dwc2_Y->Fill(dwc_pos.at(3));

    // module_M1T1C->Fill(M1TC_data.at(0).adc());
    // module_M1T2C->Fill(M1TC_data.at(1).adc());
    // module_M1T3C->Fill(M1TC_data.at(2).adc());
    // module_M1T4C->Fill(M1TC_data.at(3).adc());

    // module_M1T1S->Fill(M1TS_data.at(0).adc());
    // module_M1T2S->Fill(M1TS_data.at(1).adc());
    // module_M1T3S->Fill(M1TS_data.at(2).adc());
    // module_M1T4S->Fill(M1TS_data.at(3).adc());

    // module_M2T1C->Fill(M2TC_data.at(0).adc());
    // module_M2T2C->Fill(M2TC_data.at(1).adc());
    // module_M2T3C->Fill(M2TC_data.at(2).adc());
    // module_M2T4C->Fill(M2TC_data.at(3).adc());
    // module_M2T6C->Fill(M2TC_data.at(4).adc());
    // module_M2T7C->Fill(M2TC_data.at(5).adc());
    // module_M2T8C->Fill(M2TC_data.at(6).adc());
    // module_M2T9C->Fill(M2TC_data.at(7).adc());

    // module_M2T1S->Fill(M2TS_data.at(0).adc());
    // module_M2T2S->Fill(M2TS_data.at(1).adc());
    // module_M2T3S->Fill(M2TS_data.at(2).adc());
    // module_M2T4S->Fill(M2TS_data.at(3).adc());
    // module_M2T6S->Fill(M2TS_data.at(4).adc());
    // module_M2T7S->Fill(M2TS_data.at(5).adc());
    // module_M2T8S->Fill(M2TS_data.at(6).adc());
    // module_M2T9S->Fill(M2TS_data.at(7).adc());

    preShower->Fill(psADC);
    muonCounter->Fill(muADC);
    tailCapture->Fill(tcADC);

  }
  loader->close();

  TFile* rootOutput = new TFile((TString)("/u/user/kyhwang/scratch/WorkSpace/DRC/Aug2022TB/dev_230125/dual-readout_TB/analysis/230127/RunInfo/Fast/Run" + std::to_string(runNum) + "_FastRunInfo.root"), "RECREATE");
  rootOutput->cd();
  
  preShower->getHist()->Write();
  muonCounter->getHist()->Write();
  tailCapture->getHist()->Write();

  dwc_X_diff->getHist()->Write();
  dwc_Y_diff->getHist()->Write();
  dwc_X_diff_abs->getHist()->Write();
  dwc_Y_diff_abs->getHist()->Write();
  dwc_center_diff->getHist()->Write();
  dwc_center_diff_angle->getHist()->Write();
  dwc1_centerPos->getHist()->Write();
  dwc2_centerPos->getHist()->Write();
  dwc1pos->Write();
  dwc2pos->Write();
  dwc1vs2x->Write();
  dwc1vx2y->Write();
  dwc1_X->getHist()->Write();
  dwc1_Y->getHist()->Write();
  dwc2_X->getHist()->Write();
  dwc2_Y->getHist()->Write();  

  rootOutput->Close();

  return 0;
}
