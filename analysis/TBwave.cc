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
#include "TBauxfactory.h"
#include "TBdwc.h"

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
  TBload<TBwaveform>* loader = new TBload<TBwaveform>(runNum);

  int maxEvents = std::stoi(argv[2]); 

  std::string pedPath = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/pedestal/mean/Run" + std::to_string(runNum) + "_pedestalHist_mean.root";

  TButility utility = TButility();
  utility.loading("dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/mapping/mapping_Aug2022TB.root");
  utility.loadped(pedPath);

  AuxFactory<TBwaveform>* auxFac = new AuxFactory<TBwaveform>(runNum, 2.5);
  DWCset<TBwaveform>* dwcSet = new DWCset<TBwaveform>(runNum, dwc_calib);

  auxFac->print();
  dwcSet->print();

  TBcid pscid = utility.getcid(TBdetector::detid::preshower);
  TBcid mucid = utility.getcid(TBdetector::detid::muon);
  TBcid tccid = utility.getcid(TBdetector::detid::tail);

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

  std::vector<TBcid> M1TC;
  // M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, true));
  // M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, true));
  // M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, true));

  std::vector<TBcid> M1TS;
  // M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, false));
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, false));
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

  TBhist* moduleM1T1C = new TBhist("moduleM1T1C", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* moduleM1T2C = new TBhist("moduleM1T2C", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* moduleM1T3C = new TBhist("moduleM1T3C", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* moduleM1T4C = new TBhist("moduleM1T4C", "int. ADC", "events", 1100, -5000., 50000.);

  TBhist* moduleM1T1S = new TBhist("moduleM1T1S", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* moduleM1T2S = new TBhist("moduleM1T2S", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* moduleM1T3S = new TBhist("moduleM1T3S", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* moduleM1T4S = new TBhist("moduleM1T4S", "int. ADC", "events", 1100, -5000., 50000.);

  TBhist* moduleM1TC = new TBhist("moduleM1TC", "int. ADC", "events", 2100, -5000., 100000.);
  TBhist* moduleM1TS = new TBhist("moduleM1TS", "int. ADC", "events", 2100, -5000., 100000.);


  TH2D* dwc1pos = new TH2D("dwc1pos", "dwc1pos;mm;mm;events", 480, -120, 120, 480, -120, 120);
  TH2D* dwc2pos = new TH2D("dwc2pos", "dwc2pos;mm;mm;events", 480, -120, 120, 480, -120, 120);
  TH2D* dwc1vs2x = new TH2D("dwc1vs2x", "dwc1vs2x;mm;mm", 480, -120, 120, 480, -120, 120);
  TH2D* dwc1vx2y = new TH2D("dwc1vx2y", "dwc1vx2y;mm;mm", 480, -120, 120, 480, -120, 120);

  TBhist* dwc1_X = new TBhist("dwc1_X", "mm", "events", 480, -120., 120.);
  TBhist* dwc1_Y = new TBhist("dwc1_Y", "mm", "events", 480, -120., 120.);
  TBhist* dwc2_X = new TBhist("dwc2_X", "mm", "events", 480, -120., 120.);
  TBhist* dwc2_Y = new TBhist("dwc2_Y", "mm", "events", 480, -120., 120.);

  TBhist* dwc_X_diff = new TBhist("dwc_X_diff", "mm", "events", 240, 0., 120.);
  TBhist* dwc_Y_diff = new TBhist("dwc_Y_diff", "mm", "events", 240, 0., 120.);

  TH2D* moduleM1T2C_2D = new TH2D("moduleM1T2C_2D", "", 150, 0., 150., 5096, -1000., 4096.);
  TH2D* moduleM1T2S_2D = new TH2D("moduleM1T2S_2D", "", 150, 0., 150., 5096, -1000., 4096.);
  TH2D* preShower_2D = new TH2D("preShower_2D", "", 150, 0., 150., 5096, -1000., 4096.);
  TH2D* muonCounter_2D = new TH2D("muonCounter_2D", "", 150, 0., 150., 5096, -1000., 4096.);
  TH2D* tailCapture_2D = new TH2D("tailCapture_2D", "", 150, 0., 150., 5096, -1000., 4096.);

  TH2D* moduleM1T2C_Full = new TH2D("moduleM1T2C_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);
  TH2D* moduleM1T2S_Full = new TH2D("moduleM1T2S_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);
  TH2D* preShower_Full = new TH2D("preShower_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);
  TH2D* muonCounter_Full = new TH2D("muonCounter_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);
  TH2D* tailCaptu_Fullre = new TH2D("tailCapture_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);

  TH1D* psAvgTime = new TH1D("psAvgTime", "", 1024, 0., 1024.); psAvgTime->Sumw2();
  TH1D* muAvgTime = new TH1D("muAvgTime", "", 1024, 0., 1024.); muAvgTime->Sumw2();
  TH1D* tcAvgTime = new TH1D("tcAvgTime", "", 1024, 0., 1024.); tcAvgTime->Sumw2();
  TH1D* m1t2CAvgTime = new TH1D("m1t2CAvgTime", "", 1024, 0., 1024.); m1t2CAvgTime->Sumw2();
  TH1D* m1t2SAvgTime = new TH1D("m1t2SAvgTime", "", 1024, 0., 1024.); m1t2SAvgTime->Sumw2();

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

  if ( maxEvents == -1 )
    maxEvents = loader->getEntries();

  if ( maxEvents > loader->getEntries() )
    throw std::runtime_error("Events number is larger than entries !");

  std::chrono::time_point time_begin = std::chrono::system_clock::now(); //delete
  printf("\n");

  float selEvent = 0;
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

    TBwaveform adata_ps = anEvt->data(pscid);
    TBwaveform adata_mu = anEvt->data(mucid);
    TBwaveform adata_tc = anEvt->data(tccid);

    std::vector<TBwaveform> setDWC1;
    for ( int i = 0; i < dwc1.size(); i++ )
      setDWC1.push_back(anEvt->data(dwc1.at(i)));

    std::vector<TBwaveform> setDWC2;
    for ( int i = 0; i < dwc2.size(); i++ )
      setDWC2.push_back(anEvt->data(dwc2.at(i)));

    // std::vector<TBwaveform> M2TC_data;
    // std::vector<TBwaveform> M2TS_data;
    // for ( int i = 0; i < 8; i++ ) {
    //   M2TC_data.push_back(anEvt->data(M2TC.at(i)));
    //   M2TS_data.push_back(anEvt->data(M2TS.at(i)));
    // }

    std::vector<TBwaveform> M1TC_data;
    std::vector<TBwaveform> M1TS_data;
    for ( int i = 0; i < M1TC.size(); i++ ) {
      M1TC_data.push_back(anEvt->data(M1TC.at(i)));
      M1TS_data.push_back(anEvt->data(M1TS.at(i)));
    }

    dwcSet->SetDWC(setDWC1, setDWC2);
    auto dwc_pos = dwcSet->GetPosition();

    float psADC = 0;
    float muADC = 0;
    float tcADC = 0;
    float moduleM1T2Cadc = 0;
    float moduleM1T2Sadc = 0;

    auto psWave_origin = adata_ps.waveform();
    auto muWave_origin = adata_mu.waveform();
    auto tcWave_origin = adata_tc.waveform();
    auto moduleM1T2Cwave_origin = M1TC_data.at(0).waveform();
    auto moduleM1T2Swave_origin = M1TS_data.at(0).waveform();

    auto psWave_full = adata_ps.pedcorrectedWaveform(utility.retrievePed(pscid));
    auto muWave_full = adata_mu.pedcorrectedWaveform(utility.retrievePed(mucid));
    auto tcWave_full = adata_tc.pedcorrectedWaveform(utility.retrievePed(tccid));
    auto moduleM1T2Cwave_full = M1TC_data.at(0).pedcorrectedWaveform(utility.retrievePed(M1TC.at(0)));
    auto moduleM1T2Swave_full = M1TS_data.at(0).pedcorrectedWaveform(utility.retrievePed(M1TS.at(0)));

    auto psWave = getLeadEdgeRegion(psWave_full, 200, 500);
    auto muWave = getPeakRegion(muWave_full, 800, 23);
    auto tcWave = getLeadEdgeRegion(tcWave_full, 200, 500);
    auto moduleM1T2Cwave = getPeakRegion(moduleM1T2Cwave_full, 50, 780);
    auto moduleM1T2Swave = getPeakRegion(moduleM1T2Swave_full, 50, 780);

    for ( int i = 20; i < 140; i++ ) {
      psADC += psWave.at(i);
      muADC += muWave.at(i+9);
      tcADC += tcWave.at(i);
      moduleM1T2Cadc += moduleM1T2Cwave.at(i);
      moduleM1T2Sadc += moduleM1T2Swave.at(i);
    }

    // auxFac->SetAUXadc(psADC, muADC);
    if ( !dwcSet->inAlign(2) ) continue;
    // if ( !dwcSet->inVeto() ) continue;
    // if ( !(auxFac->GetPID() == 11) ) continue;

    selEvent++;

    for ( int i = 0; i < 150; i++ ) {
      moduleM1T2C_2D->Fill(i, moduleM1T2Cwave.at(i));
      moduleM1T2S_2D->Fill(i, moduleM1T2Swave.at(i));
      preShower_2D->Fill(i, psWave.at(i));
      muonCounter_2D->Fill(i, muWave.at(i));
      tailCapture_2D->Fill(i, tcWave.at(i));
    }

    for ( int i = 0; i < 1024; i++ ) {
      moduleM1T2C_Full->Fill(i, moduleM1T2Cwave_full.at(i));
      moduleM1T2S_Full->Fill(i, moduleM1T2Swave_full.at(i));
      preShower_Full->Fill(i, psWave_full.at(i));
      muonCounter_Full->Fill(i, muWave_full.at(i));
      tailCaptu_Fullre->Fill(i, tcWave_full.at(i));

      psAvgTime->Fill(i, psWave_origin.at(i));
      muAvgTime->Fill(i, muWave_origin.at(i));
      tcAvgTime->Fill(i, tcWave_origin.at(i));
      m1t2CAvgTime->Fill(i, moduleM1T2Cwave_origin.at(i));
      m1t2SAvgTime->Fill(i, moduleM1T2Swave_origin.at(i));
    }

    preShower->Fill(psADC);
    muonCounter->Fill(muADC);
    tailCapture->Fill(tcADC);
    moduleM1T2C->Fill(moduleM1T2Cadc);
    moduleM1T2S->Fill(moduleM1T2Sadc);

    dwc1pos->Fill(dwc_pos.at(0), dwc_pos.at(1));
    dwc2pos->Fill(dwc_pos.at(2), dwc_pos.at(3));
    dwc1vs2x->Fill(dwc_pos.at(0), dwc_pos.at(2));
    dwc1vx2y->Fill(dwc_pos.at(1), dwc_pos.at(3));

    dwc1_X->Fill(dwc_pos.at(0));
    dwc1_Y->Fill(dwc_pos.at(1));
    dwc2_X->Fill(dwc_pos.at(2));
    dwc2_Y->Fill(dwc_pos.at(3));

    dwc_X_diff->Fill(std::abs(dwc_pos.at(0) - dwc_pos.at(2)));
    dwc_Y_diff->Fill(std::abs(dwc_pos.at(1) - dwc_pos.at(3)));

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
  }
  loader->close();

  psAvgTime->Scale(1./selEvent);
  muAvgTime->Scale(1./selEvent);
  tcAvgTime->Scale(1./selEvent);
  m1t2CAvgTime->Scale(1./selEvent);
  m1t2SAvgTime->Scale(1./selEvent);

  for ( int i = 0; i < 1024; i++ ) {
    psAvgTime->SetBinError(i+1, 1e-10);
    muAvgTime->SetBinError(i+1, 1e-10);
    tcAvgTime->SetBinError(i+1, 1e-10);
    m1t2CAvgTime->SetBinError(i+1, 1e-10);
    m1t2SAvgTime->SetBinError(i+1, 1e-10);
  }

  std::cout << selEvent << std::endl;

  TFile* rootOutput = new TFile((TString)("/d0/scratch/kyhwang/WorkSpace/DRC/Aug2022TB/dev_230201/dual-readout_TB/analysis/230210/Run" + std::to_string(runNum) + "_RunInfo.root"), "RECREATE");
  rootOutput->cd();
  
  preShower->getHist()->Write();
  muonCounter->getHist()->Write();
  tailCapture->getHist()->Write();
  moduleM1T2C->getHist()->Write();
  moduleM1T2S->getHist()->Write();
  
  moduleM1T2C_2D->Write();
  moduleM1T2S_2D->Write();
  preShower_2D->Write();
  muonCounter_2D->Write();
  tailCapture_2D->Write();

  moduleM1T2C_Full->Write();
  moduleM1T2S_Full->Write();
  preShower_Full->Write();
  muonCounter_Full->Write();
  tailCaptu_Fullre->Write();

  dwc1pos->Write();
  dwc2pos->Write();
  dwc1vs2x->Write();
  dwc1vx2y->Write();
  dwc1_X->getHist()->Write();
  dwc1_Y->getHist()->Write();
  dwc2_X->getHist()->Write();
  dwc2_Y->getHist()->Write();  
  dwc_X_diff->getHist()->Write();
  dwc_Y_diff->getHist()->Write();

  psAvgTime->Write();
  muAvgTime->Write();
  tcAvgTime->Write();
  m1t2CAvgTime->Write();
  m1t2SAvgTime->Write();

  rootOutput->Close();

  return 0;
}
