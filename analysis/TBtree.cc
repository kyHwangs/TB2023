#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <vector>
#include <numeric>
#include <functional>

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
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, true));
  M1TC.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, true));

  std::vector<TBcid> M1TS;
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 1, false));
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 2, false));
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 3, false));
  M1TS.push_back(utility.getcid(TBdetector::detid::PMT, 1, 4, false));

  TBhist* preShower = new TBhist("preshower", "int. ADC", "events", 820, -5000., 200000.);
  TBhist* muonCounter = new TBhist("muoncounter", "int. ADC", "events", 1100, -5000., 50000.);
  TBhist* tailCatcher = new TBhist("tailcatcher", "int. ADC", "events", 1100, -5000., 50000.);

  std::vector<TBhist*> moduleM1TC;
  moduleM1TC.reserve(4);
  moduleM1TC.emplace_back(new TBhist("moduleM1T1C", "int. ADC", "events", 1100, -5000., 50000.));
  moduleM1TC.emplace_back(new TBhist("moduleM1T2C", "int. ADC", "events", 1100, -5000., 50000.));
  moduleM1TC.emplace_back(new TBhist("moduleM1T3C", "int. ADC", "events", 1100, -5000., 50000.));
  moduleM1TC.emplace_back(new TBhist("moduleM1T4C", "int. ADC", "events", 1100, -5000., 50000.));

  std::vector<TBhist*> moduleM1TS;
  moduleM1TS.reserve(4);
  moduleM1TS.emplace_back(new TBhist("moduleM1T1S", "int. ADC", "events", 1100, -5000., 50000.));
  moduleM1TS.emplace_back(new TBhist("moduleM1T2S", "int. ADC", "events", 1100, -5000., 50000.));
  moduleM1TS.emplace_back(new TBhist("moduleM1T3S", "int. ADC", "events", 1100, -5000., 50000.));
  moduleM1TS.emplace_back(new TBhist("moduleM1T4S", "int. ADC", "events", 1100, -5000., 50000.));

  TH2D* dwc1pos = new TH2D("dwc1pos", "dwc1pos;mm;mm;events", 720, -120, 120, 720, -120, 120);
  TH2D* dwc2pos = new TH2D("dwc2pos", "dwc2pos;mm;mm;events", 720, -120, 120, 720, -120, 120);
  TH2D* dwc1vs2x = new TH2D("dwc1vs2x", "dwc1vs2x;mm;mm", 720, -120, 120, 720, -120, 120);
  TH2D* dwc1vx2y = new TH2D("dwc1vx2y", "dwc1vx2y;mm;mm", 720, -120, 120, 720, -120, 120);

  TBhist* dwc1_X = new TBhist("dwc1_X", "mm", "events", 720, -120., 120.);
  TBhist* dwc1_Y = new TBhist("dwc1_Y", "mm", "events", 720, -120., 120.);
  TBhist* dwc2_X = new TBhist("dwc2_X", "mm", "events", 720, -120., 120.);
  TBhist* dwc2_Y = new TBhist("dwc2_Y", "mm", "events", 720, -120., 120.);

  TBhist* dwc_X_diff = new TBhist("dwc_X_diff", "mm", "events", 720, -120., 120.);
  TBhist* dwc_Y_diff = new TBhist("dwc_Y_diff", "mm", "events", 720, -120., 120.);

  TH2D* preShower_Full = new TH2D("preShower_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);
  TH2D* muonCounter_Full = new TH2D("muonCounter_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);
  TH2D* tailCatcher_Full = new TH2D("tailCatcher_Full", "", 1024, 0., 1024., 5096, -1000., 4096.);

  TH1D* psAvgTime = new TH1D("psAvgTime", "", 1024, 0., 1024.); psAvgTime->Sumw2();
  TH1D* muAvgTime = new TH1D("muAvgTime", "", 1024, 0., 1024.); muAvgTime->Sumw2();
  TH1D* tcAvgTime = new TH1D("tcAvgTime", "", 1024, 0., 1024.); tcAvgTime->Sumw2();

  std::vector<TH1D*> moduleM1TCavgTimeStructure;
  moduleM1TCavgTimeStructure.reserve(4);
  moduleM1TCavgTimeStructure.emplace_back(new TH1D("m1t1CAvgTime", "M1T1 C avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TCavgTimeStructure.at(0)->Sumw2();
  moduleM1TCavgTimeStructure.emplace_back(new TH1D("m1t2CAvgTime", "M1T2 C avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TCavgTimeStructure.at(1)->Sumw2();
  moduleM1TCavgTimeStructure.emplace_back(new TH1D("m1t3CAvgTime", "M1T3 C avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TCavgTimeStructure.at(2)->Sumw2();
  moduleM1TCavgTimeStructure.emplace_back(new TH1D("m1t4CAvgTime", "M1T4 C avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TCavgTimeStructure.at(3)->Sumw2();

  std::vector<TH1D*> moduleM1TSavgTimeStructure;
  moduleM1TSavgTimeStructure.reserve(4);
  moduleM1TSavgTimeStructure.emplace_back(new TH1D("m1t1SAvgTime", "M1T1 S avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TSavgTimeStructure.at(0)->Sumw2();
  moduleM1TSavgTimeStructure.emplace_back(new TH1D("m1t2SAvgTime", "M1T2 S avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TSavgTimeStructure.at(1)->Sumw2();
  moduleM1TSavgTimeStructure.emplace_back(new TH1D("m1t3SAvgTime", "M1T3 S avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TSavgTimeStructure.at(2)->Sumw2();
  moduleM1TSavgTimeStructure.emplace_back(new TH1D("m1t4SAvgTime", "M1T4 S avg. Time Structure;bin [0.4ns/bin];ADC", 1024, 0., 1024.)); moduleM1TSavgTimeStructure.at(3)->Sumw2();

  TFile* treeFile = new TFile((TString)("./TreeRun" + std::to_string(runNum) + "_RunInfo.root"), "RECREATE");

  TTree* aTree = new TTree("data", "data");

  float _dwc_1_x, _dwc_1_y, _dwc_2_x, _dwc_2_y;
  float _muADC, _psADC;
  float _m1t1c, _m1t2c, _m1t3c, _m1t4c;
  float _m1t1s, _m1t2s, _m1t3s, _m1t4s;

  aTree->Branch("dwc_1_x", &_dwc_1_x);
  aTree->Branch("dwc_1_y", &_dwc_1_y);
  aTree->Branch("dwc_2_x", &_dwc_2_x);
  aTree->Branch("dwc_2_y", &_dwc_2_y);

  aTree->Branch("muADC", &_muADC);
  aTree->Branch("psADC", &_psADC);

  aTree->Branch("m1t1c", &_m1t1c);
  aTree->Branch("m1t2c", &_m1t2c);
  aTree->Branch("m1t3c", &_m1t3c);
  aTree->Branch("m1t4c", &_m1t4c);
  
  aTree->Branch("m1t1s", &_m1t1s);
  aTree->Branch("m1t2s", &_m1t2s);
  aTree->Branch("m1t3s", &_m1t3s);
  aTree->Branch("m1t4s", &_m1t4s);

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
    float moduleM1T1Cadc = 0;
    float moduleM1T1Sadc = 0;
    float moduleM1T2Cadc = 0;
    float moduleM1T2Sadc = 0;
    float moduleM1T3Cadc = 0;
    float moduleM1T3Sadc = 0;
    float moduleM1T4Cadc = 0;
    float moduleM1T4Sadc = 0;

    auto psWave_origin = adata_ps.waveform();
    auto muWave_origin = adata_mu.waveform();
    auto tcWave_origin = adata_tc.waveform();

    auto moduleM1T1Cwave_origin = M1TC_data.at(0).waveform();
    auto moduleM1T1Swave_origin = M1TS_data.at(0).waveform();

    auto moduleM1T2Cwave_origin = M1TC_data.at(1).waveform();
    auto moduleM1T2Swave_origin = M1TS_data.at(1).waveform();

    auto moduleM1T3Cwave_origin = M1TC_data.at(2).waveform();
    auto moduleM1T3Swave_origin = M1TS_data.at(2).waveform();

    auto moduleM1T4Cwave_origin = M1TC_data.at(3).waveform();
    auto moduleM1T4Swave_origin = M1TS_data.at(3).waveform();

    auto psWave_ped = adata_ps.pedcorrectedWaveform(utility.retrievePed(pscid));
    auto muWave_ped = adata_mu.pedcorrectedWaveform(utility.retrievePed(mucid));
    auto tcWave_ped = adata_tc.pedcorrectedWaveform(utility.retrievePed(tccid));

    auto moduleM1T1Cwave_ped = M1TC_data.at(0).pedcorrectedWaveform(utility.retrievePed(M1TC.at(0)));
    auto moduleM1T1Swave_ped = M1TS_data.at(0).pedcorrectedWaveform(utility.retrievePed(M1TS.at(0)));

    auto moduleM1T2Cwave_ped = M1TC_data.at(1).pedcorrectedWaveform(utility.retrievePed(M1TC.at(1)));
    auto moduleM1T2Swave_ped = M1TS_data.at(1).pedcorrectedWaveform(utility.retrievePed(M1TS.at(1)));

    auto moduleM1T3Cwave_ped = M1TC_data.at(2).pedcorrectedWaveform(utility.retrievePed(M1TC.at(2)));
    auto moduleM1T3Swave_ped = M1TS_data.at(2).pedcorrectedWaveform(utility.retrievePed(M1TS.at(2)));
    
    auto moduleM1T4Cwave_ped = M1TC_data.at(3).pedcorrectedWaveform(utility.retrievePed(M1TC.at(3)));
    auto moduleM1T4Swave_ped = M1TS_data.at(3).pedcorrectedWaveform(utility.retrievePed(M1TS.at(3)));

    psADC = std::accumulate(psWave_ped.begin() + 220, psWave_ped.begin() + 220 + 170, 0.);
    muADC = std::accumulate(muWave_ped.begin() + 840, muWave_ped.begin() + 840 + 150, 0.);
    tcADC = std::accumulate(tcWave_ped.begin() + 280, tcWave_ped.begin() + 280 + 150, 0.);

    moduleM1T1Cadc = std::accumulate(moduleM1T1Cwave_ped.begin() + 120, moduleM1T1Cwave_ped.begin() + 120 + 100, 0.);
    moduleM1T1Sadc = std::accumulate(moduleM1T1Swave_ped.begin() + 120, moduleM1T1Swave_ped.begin() + 120 + 110, 0.);

    moduleM1T2Cadc = std::accumulate(moduleM1T2Cwave_ped.begin() + 120, moduleM1T2Cwave_ped.begin() + 120 + 100, 0.);
    moduleM1T2Sadc = std::accumulate(moduleM1T2Swave_ped.begin() + 120, moduleM1T2Swave_ped.begin() + 120 + 110, 0.);

    moduleM1T3Cadc = std::accumulate(moduleM1T3Cwave_ped.begin() + 120, moduleM1T3Cwave_ped.begin() + 120 + 100, 0.);
    moduleM1T3Sadc = std::accumulate(moduleM1T3Swave_ped.begin() + 120, moduleM1T3Swave_ped.begin() + 120 + 110, 0.);

    moduleM1T4Cadc = std::accumulate(moduleM1T4Cwave_ped.begin() + 120, moduleM1T4Cwave_ped.begin() + 120 + 100, 0.);
    moduleM1T4Sadc = std::accumulate(moduleM1T4Swave_ped.begin() + 120, moduleM1T4Swave_ped.begin() + 120 + 110, 0.);

    _dwc_1_x = dwc_pos.at(0);
    _dwc_1_y = dwc_pos.at(1);
    _dwc_2_x = dwc_pos.at(2);
    _dwc_2_y = dwc_pos.at(3);

    _muADC = muADC;
    _psADC = psADC;

    _m1t1c = moduleM1T1Cadc;
    _m1t2c = moduleM1T2Cadc;
    _m1t3c = moduleM1T3Cadc;
    _m1t4c = moduleM1T4Cadc; 

    _m1t1s = moduleM1T1Sadc;
    _m1t2s = moduleM1T2Sadc;
    _m1t3s = moduleM1T3Sadc;
    _m1t4s = moduleM1T4Sadc;

    aTree->Fill();

    if ( !dwcSet->inAlign(1.5) ) continue;
    if ( !( psADC > 36000. ) ) continue;
    if ( !( muADC < 2400. ) ) continue;

    selEvent++;

    for ( int i = 0; i < 1024; i++ ) {
      preShower_Full->Fill(i, psWave_origin.at(i));
      muonCounter_Full->Fill(i, muWave_origin.at(i));
      tailCatcher_Full->Fill(i, tcWave_origin.at(i));

      psAvgTime->Fill(i, psWave_origin.at(i));
      muAvgTime->Fill(i, muWave_origin.at(i));
      tcAvgTime->Fill(i, tcWave_origin.at(i));

      moduleM1TCavgTimeStructure.at(0)->Fill(i, moduleM1T1Cwave_origin.at(i));
      moduleM1TSavgTimeStructure.at(0)->Fill(i, moduleM1T1Swave_origin.at(i));
      moduleM1TCavgTimeStructure.at(1)->Fill(i, moduleM1T2Cwave_origin.at(i));
      moduleM1TSavgTimeStructure.at(1)->Fill(i, moduleM1T2Swave_origin.at(i));
      moduleM1TCavgTimeStructure.at(2)->Fill(i, moduleM1T3Cwave_origin.at(i));
      moduleM1TSavgTimeStructure.at(2)->Fill(i, moduleM1T3Swave_origin.at(i));
      moduleM1TCavgTimeStructure.at(3)->Fill(i, moduleM1T4Cwave_origin.at(i));
      moduleM1TSavgTimeStructure.at(3)->Fill(i, moduleM1T4Swave_origin.at(i));
    }

    preShower->Fill(psADC);
    muonCounter->Fill(muADC);
    tailCatcher->Fill(tcADC);

    moduleM1TC.at(0)->Fill(moduleM1T1Cadc);
    moduleM1TS.at(0)->Fill(moduleM1T1Sadc);
    moduleM1TC.at(1)->Fill(moduleM1T2Cadc);
    moduleM1TS.at(1)->Fill(moduleM1T2Sadc);
    moduleM1TC.at(2)->Fill(moduleM1T3Cadc);
    moduleM1TS.at(2)->Fill(moduleM1T3Sadc);
    moduleM1TC.at(3)->Fill(moduleM1T4Cadc);
    moduleM1TS.at(3)->Fill(moduleM1T4Sadc);

    dwc1pos->Fill(dwc_pos.at(0), dwc_pos.at(1));
    dwc2pos->Fill(dwc_pos.at(2), dwc_pos.at(3));
    dwc1vs2x->Fill(dwc_pos.at(0), dwc_pos.at(2));
    dwc1vx2y->Fill(dwc_pos.at(1), dwc_pos.at(3));

    dwc1_X->Fill(dwc_pos.at(0));
    dwc1_Y->Fill(dwc_pos.at(1));
    dwc2_X->Fill(dwc_pos.at(2));
    dwc2_Y->Fill(dwc_pos.at(3));

    dwc_X_diff->Fill(dwc_pos.at(0) - dwc_pos.at(2));
    dwc_Y_diff->Fill(dwc_pos.at(1) - dwc_pos.at(3));
  }
  loader->close();

  treeFile->cd();
  aTree->Write();
  treeFile->Close();

  psAvgTime->Scale(1./selEvent);
  muAvgTime->Scale(1./selEvent);
  tcAvgTime->Scale(1./selEvent);

  for ( int i = 0; i < 4; i++ ) {
    moduleM1TCavgTimeStructure.at(i)->Scale(1./selEvent);
    moduleM1TSavgTimeStructure.at(i)->Scale(1./selEvent);
  }

  for ( int i = 0; i < 1024; i++ ) {
    psAvgTime->SetBinError(i+1, 1e-10);
    muAvgTime->SetBinError(i+1, 1e-10);
    tcAvgTime->SetBinError(i+1, 1e-10);
    
    for ( int j = 0; j < 4; j++ ) {
      moduleM1TCavgTimeStructure.at(j)->SetBinError(i+1, 1e-10);
      moduleM1TSavgTimeStructure.at(j)->SetBinError(i+1, 1e-10);
    }
  }

  TFile* rootOutput = new TFile((TString)("./IntRun" + std::to_string(runNum) + "_RunInfo.root"), "RECREATE");
  rootOutput->cd();
  
  preShower->getHist()->Write();
  muonCounter->getHist()->Write();
  tailCatcher->getHist()->Write();

  moduleM1TC.at(0)->getHist()->Write();
  moduleM1TC.at(1)->getHist()->Write();
  moduleM1TC.at(2)->getHist()->Write();
  moduleM1TC.at(3)->getHist()->Write();

  moduleM1TS.at(0)->getHist()->Write();
  moduleM1TS.at(1)->getHist()->Write();
  moduleM1TS.at(2)->getHist()->Write();
  moduleM1TS.at(3)->getHist()->Write();
  
  preShower_Full->Write();
  muonCounter_Full->Write();
  tailCatcher_Full->Write();

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

  moduleM1TCavgTimeStructure.at(0)->Write();
  moduleM1TCavgTimeStructure.at(1)->Write();
  moduleM1TCavgTimeStructure.at(2)->Write();
  moduleM1TCavgTimeStructure.at(3)->Write();

  moduleM1TSavgTimeStructure.at(0)->Write();
  moduleM1TSavgTimeStructure.at(1)->Write();
  moduleM1TSavgTimeStructure.at(2)->Write();
  moduleM1TSavgTimeStructure.at(3)->Write();


  rootOutput->Close();

  return 0;
}
