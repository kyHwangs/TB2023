#include <iostream>
#include <vector>
#include <stdexcept>

#include "TCanvas.h"
#include "TPad.h"
#include "TPaveStats.h"

#include "TBplot.h"
#include "TPaletteAxis.h"

TBplotbase::TBplotbase(int ww, int wh, TString canvasname, TBplotbase::kind plotkind)
: plotkind_(plotkind), pads_(0), savename_(0), canvasname_(canvasname) {
  c_ = new TCanvas((TString)(canvasname_+std::to_string(plotkind_)), (TString)(canvasname_+std::to_string(plotkind_)), ww, wh);

  init();
}

TBplotbase::TBplotbase(int ww, int wh, const std::string& canvasname, const std::string& plotkind)
: canvasname_(canvasname), savename_("") {
  if (plotkind=="hitmap")
    plotkind_ = kind::hitmap;
  else if (plotkind=="distribution")
    plotkind_ = kind::distribution;
  else if (plotkind=="waveform")
    plotkind_ = kind::waveform;
  else if (plotkind=="sipmHitMap")
    plotkind_ = kind::sipmHitMap;
  else if (plotkind=="dwc")
    plotkind_ = kind::dwc;
  else if (plotkind=="auxiliary")
    plotkind_ = kind::auxiliary;
  else
    throw std::runtime_error("TBplotbase - please check TBplotbase::kind!");

  c_ = new TCanvas((TString)(canvasname_+std::to_string(plotkind_)), (TString)(canvasname_+std::to_string(plotkind_)), ww, wh);

  init();
}

void TBplotbase::init() {
  for (int i = 0; i < xlow.at(plotkind_).size(); i++) {
    c_->cd();
    TPad* tmpPad = new TPad((TString)(canvasname_+std::to_string(plotkind_)+""+std::to_string(i)),
                            (TString)(canvasname_+std::to_string(plotkind_)+""+std::to_string(i)),
                            xlow.at(plotkind_).at(i),
                            ylow.at(plotkind_).at(i),
                            xup.at(plotkind_).at(i),
                            yup.at(plotkind_).at(i));

    pads_.push_back(tmpPad);
    padSet(pads_.at(i), 0.1);
  }
}

TBplot::TBplot(int ww, int wh, double distMax, int distBin, TString plotname, TBplotbase::kind plotkind)
: TBplotbase(ww, wh, plotname, plotkind), distMax_(distMax), distBin_(distBin), plots1D_(0), plots2D_(0), plotname_(plotname) {
  init_plots();
}

TBplot::TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind, std::vector<TH1D*> plot1D)
: TBplotbase(ww, wh, plotname, plotkind), plotname_(plotname), plots1D_(plot1D), plots2D_(0) {}

TBplot::TBplot(int ww, int wh, TString plotname, TBplotbase::kind plotkind, std::vector<TH2D*> plot2D)
: TBplotbase(ww, wh, plotname, plotkind), plotname_(plotname), plots1D_(0), plots2D_(plot2D) {}

TBplot::TBplot(int ww, int wh, const std::string& plotname, const std::string& plotkind)
: TBplotbase(ww, wh, plotname, plotkind), plotname_(plotname), plots1D_(0), plots2D_(0) {}

void TBplot::init_plots() {
  if ( plotkind_ == TBplotbase::kind::sipmHitMap ) {
    for( int i = 0; i < xlow.at(plotkind_).size(); i++ ) {
      TH2D* tmpHist = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 20, 0, 20, 20, 0, 20);
      tmpHist->SetStats(0);
      tmpHist->SetTitle("");
      tmpHist->GetXaxis()->SetTickLength(0.);
      tmpHist->GetYaxis()->SetTickLength(0.);
      tmpHist->GetZaxis()->SetRangeUser(0., distMax_);
      tmpHist->Sumw2();
      plots2D_.push_back(tmpHist);
    }
  } else if ( plotkind_ == TBplotbase::kind::hitmap ) {
    TH2D* colPal = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(0)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(0)), 1, 0, 1, 10000, 0, 1);
    colPal->SetStats(0);
    colPal->SetTitle("");
    colPal->GetXaxis()->SetTickLength(0.);
    colPal->GetYaxis()->SetTickLength(0.);
    colPal->GetZaxis()->SetRangeUser(0., distMax_);
    colPal->Sumw2();

    for (int i = 1; i <= 10000; i++)
      colPal->SetBinContent(1, i, (distMax_/9999.)*(i-1));

    plots2D_.push_back(colPal);

    for( int i = 1; i < xlow.at(plotkind_).size(); i++ ) {
      TH2D* tmpHist;
      if( i != 9 && i != 22 ) {
        tmpHist = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 1, 0, 1, 1, 0, 1);
        tmpHist->SetStats(0); tmpHist->SetTitle("");
        tmpHist->GetXaxis()->SetTickLength(0.);
        tmpHist->GetYaxis()->SetTickLength(0.);
        tmpHist->GetZaxis()->SetRangeUser(0., distMax_);
        tmpHist->Sumw2();
      } else {
        tmpHist = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 20, 0, 20, 20, 0, 20);
        tmpHist->SetStats(0);
        tmpHist->SetTitle("");
        tmpHist->GetXaxis()->SetTickLength(0.);
        tmpHist->GetYaxis()->SetTickLength(0.);
        tmpHist->GetZaxis()->SetRangeUser(0., distMax_/20.);
        tmpHist->Sumw2();
      }

      plots2D_.push_back(tmpHist);
    }
  } else if ( plotkind_ == TBplotbase::kind::distribution ) {
    for( int i = 0; i < 2*xlow.at(plotkind_).size(); i++ ) {
      TH1D* tmpHist;

      if( i != 8 && i != 21 ) {
        tmpHist = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), distBin_, -500, distMax_);
      } else {
        tmpHist = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), distBin_, -500, distMax_*4);
      }

      tmpHist->SetStats(1);
      tmpHist->SetLineWidth(1);
      tmpHist->SetTitle("");
      tmpHist->Sumw2();

      if( i < xlow.at(plotkind_).size() )
        tmpHist->SetLineColor(kBlue);
      else
        tmpHist->SetLineColor(kRed);

      plots1D_.push_back(tmpHist);
    }
  } else if ( plotkind_ == TBplotbase::kind::waveform ) {
    for( int i = 0; i < 2*xlow.at(plotkind_).size(); i++ ) {
      TH1D* tmpHist = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), (TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(i)), 1000, 0, 1000);
      tmpHist->SetTitle("");
      tmpHist->SetLineWidth(1);
      tmpHist->GetYaxis()->SetRangeUser(0., 4100.);
      tmpHist->Sumw2();
      if( i < xlow.at(plotkind_).size() )
        tmpHist->SetLineColor(kBlue);
      else
        tmpHist->SetLineColor(kRed);

      plots1D_.push_back(tmpHist);
    }
  } else if ( plotkind_ == TBplotbase::kind::dwc ) {
    TH2D* tmpHist1 = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(0)), (TString)(plotname_+"_DWC1_Upstream view;X [mm];Y [mm]"), 100, -50., 50., 100, -50, 50);
    tmpHist1->SetStats(1);
    plots2D_.push_back(tmpHist1);
    TH2D* tmpHist2 = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(1)), (TString)(plotname_+"_DWC2_Upstream view;X [mm];Y [mm]"), 100, -50., 50., 100, -50, 50);
    tmpHist2->SetStats(1);
    plots2D_.push_back(tmpHist2);
    TH2D* tmpHist3 = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(2)), (TString)(plotname_+"_DWC1_vs_DWC2_X;DWC1 X [mm]; DWC2 X [mm]"), 100, -50., 50., 100, -50, 50);
    tmpHist3->SetStats(1);
    plots2D_.push_back(tmpHist3);
    TH2D* tmpHist4 = new TH2D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(3)), (TString)(plotname_+"_DWC1_vs_DWC2_Y;DWC1 Y [mm]; DWC2 Y [mm]"), 100, -50., 50., 100, -50, 50);
    tmpHist4->SetStats(1);
    plots2D_.push_back(tmpHist4);

  } else if ( plotkind_ == TBplotbase::kind::auxiliary ) {
    TH1D* tmpHist1 = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(0)), "Pre Shower Detector; Integrated ADC; # entries", distBin_, -500, distMax_);
    tmpHist1->SetStats(1);
    TH1D* tmpHist2 = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(1)), "Tail Catcher; Integrated ADC; # entries", distBin_, -500, distMax_);
    tmpHist2->SetStats(1);
    TH1D* tmpHist3 = new TH1D((TString)(plotname_+std::to_string(plotkind_)+""+std::to_string(2)), "Muon Counter; Integrated ADC; # entries", distBin_, -500, distMax_);
    tmpHist3->SetStats(1);
    plots1D_.push_back(tmpHist1);
    plots1D_.push_back(tmpHist2);
    plots1D_.push_back(tmpHist3);

  } else {
    throw std::runtime_error("Available plot : Hitmap, ADC distribution, Waveform");
  }
}

void TBplot::openFile(const std::string& name) {
  dqmFile_ = TFile::Open(name.c_str(),"READ");
}

void TBplot::closeFile() {
  dqmFile_->Close();
}

void TBplot::loadTH(const std::string& name, int num) {
  std::string classname = dqmFile_->Get((TString)(name+std::to_string(0)))->ClassName();
  if (classname.compare("TH1D")==0) {
    for(int i = 0; i < num; i++)
      plots1D_.push_back((TH1D*)dqmFile_->Get((TString)(name+std::to_string(i))));
  } else if (classname.compare("TH2D")==0) {
    for (int i = 0; i < num; i++)
      plots2D_.push_back((TH2D*)dqmFile_->Get((TString)(name+std::to_string(i))));
  } else {
    throw std::runtime_error("Object Class "+classname+" not found");
  }
}

void TBplot::loadTH1D(const std::string& name, int num) {
  for(int i = 0; i < num; i++)
    plots1D_.push_back((TH1D*)dqmFile_->Get((TString)(name+std::to_string(i))));
}

void TBplot::loadTH2D(const std::string& name, int num) {
  for(int i = 0; i < num; i++)
    plots2D_.push_back((TH2D*)dqmFile_->Get((TString)(name+std::to_string(i))));
}

void TBplot::fillWaveform( TBdetector detid, std::vector<short> awave ) {
  if ( plotkind_ != TBplotbase::kind::waveform )
    throw std::runtime_error("TBplot::fillWaveform is only available with waveform plot");

  int Nmodule  = detid.module();
  int Ntower   = detid.tower();
  bool isCeren = detid.isCeren();
  int plate    = detid.plate();
  int column   = detid.column();

  if ( isCeren ) {
    if ( Nmodule == 1 ) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(0)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(1)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(2)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(3)->Fill(i+0.5, awave.at(i+1));
    } else if ( Nmodule == 2) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(4)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(5)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(6)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(7)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 6 ) for(int i = 0; i < 1000; i++) plots1D_.at(9)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 7 ) for(int i = 0; i < 1000; i++) plots1D_.at(10)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 8 ) for(int i = 0; i < 1000; i++) plots1D_.at(11)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 9 ) for(int i = 0; i < 1000; i++) plots1D_.at(12)->Fill(i+0.5, awave.at(i+1));
    }

  } else {
    if ( Nmodule == 1 ) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(13)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(14)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(15)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(16)->Fill(i+0.5, awave.at(i+1));
    } else if ( Nmodule == 2) {
      if ( Ntower == 1 ) for(int i = 0; i < 1000; i++) plots1D_.at(17)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 2 ) for(int i = 0; i < 1000; i++) plots1D_.at(18)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 3 ) for(int i = 0; i < 1000; i++) plots1D_.at(19)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 4 ) for(int i = 0; i < 1000; i++) plots1D_.at(20)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 6 ) for(int i = 0; i < 1000; i++) plots1D_.at(22)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 7 ) for(int i = 0; i < 1000; i++) plots1D_.at(23)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 8 ) for(int i = 0; i < 1000; i++) plots1D_.at(24)->Fill(i+0.5, awave.at(i+1));
      if ( Ntower == 9 ) for(int i = 0; i < 1000; i++) plots1D_.at(25)->Fill(i+0.5, awave.at(i+1));
    }
  }
}

void TBplot::fillADC( TBdetector detid, float adc ) {
  if ( plotkind_ == TBplotbase::kind::waveform )
    throw std::runtime_error("TBplot::fillADC is only available with ADC distribution and Hitmap plot");

  int Nmodule  = detid.module();
  int Ntower   = detid.tower();
  bool isCeren = detid.isCeren();
  int plate    = detid.plate() - 20;
  int column   = detid.column() - 20;

  if ( Ntower == 255 )
    return;

  if ( plotkind_ == TBplotbase::kind::hitmap ) {
    if ( isCeren ) {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots2D_.at(1)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(2)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(3)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(4)->Fill(1 - 0.5, 1 - 0.5, adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots2D_.at(5)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(6)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(7)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(8)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 5 ) plots2D_.at(9)->Fill(column - 0.5, plate - 0.5, adc); // SiPM
        if ( Ntower == 6 ) plots2D_.at(10)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 7 ) plots2D_.at(11)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 8 ) plots2D_.at(12)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 9 ) plots2D_.at(13)->Fill(1 - 0.5, 1 - 0.5, adc);
      }

    } else {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots2D_.at(14)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(15)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(16)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(17)->Fill(1 - 0.5, 1 - 0.5, adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots2D_.at(18)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 2 ) plots2D_.at(19)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 3 ) plots2D_.at(20)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 4 ) plots2D_.at(21)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 5 ) plots2D_.at(22)->Fill(column- 0.5, plate - 0.5, adc); // SiPM
        if ( Ntower == 6 ) plots2D_.at(23)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 7 ) plots2D_.at(24)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 8 ) plots2D_.at(25)->Fill(1 - 0.5, 1 - 0.5, adc);
        if ( Ntower == 9 ) plots2D_.at(26)->Fill(1 - 0.5, 1 - 0.5, adc);
      }
    }
  } else if ( plotkind_ == TBplotbase::kind::sipmHitMap ) {
    if ( isCeren ) {
      if ( Nmodule == 2 && Ntower == 5)
        plots2D_.at(0)->Fill(column - 0.5, plate - 0.5, adc); // SiPM
    } else {
      if ( Nmodule == 2 && Ntower == 5)
        plots2D_.at(1)->Fill(column - 0.5, plate - 0.5, adc); // SiPM
    }
  } else if ( plotkind_ == TBplotbase::kind::distribution ) {
    if ( isCeren ) {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots1D_.at(0)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(1)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(2)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(3)->Fill(adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots1D_.at(4)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(5)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(6)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(7)->Fill(adc);
        if ( Ntower == 6 ) plots1D_.at(9)->Fill(adc);
        if ( Ntower == 7 ) plots1D_.at(10)->Fill(adc);
        if ( Ntower == 8 ) plots1D_.at(11)->Fill(adc);
        if ( Ntower == 9 ) plots1D_.at(12)->Fill(adc);
      }
    } else {
      if ( Nmodule == 1 ) {
        if ( Ntower == 1 ) plots1D_.at(13)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(14)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(15)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(16)->Fill(adc);
      } else if ( Nmodule == 2) {
        if ( Ntower == 1 ) plots1D_.at(17)->Fill(adc);
        if ( Ntower == 2 ) plots1D_.at(18)->Fill(adc);
        if ( Ntower == 3 ) plots1D_.at(19)->Fill(adc);
        if ( Ntower == 4 ) plots1D_.at(20)->Fill(adc);
        if ( Ntower == 6 ) plots1D_.at(22)->Fill(adc);
        if ( Ntower == 7 ) plots1D_.at(23)->Fill(adc);
        if ( Ntower == 8 ) plots1D_.at(24)->Fill(adc);
        if ( Ntower == 9 ) plots1D_.at(25)->Fill(adc);
      }
    }
  }
}

void TBplot::fillDWC(std::vector<int> dwcTime) {
  // New calibration based on R - L, U - D
  // dwcTime vector is mapped as DWC1(R, L, U, D) -> (0, 1, 2, 3) and DWC2(R, L, U, D) -> (4, 5, 6, 7).
  // Time difference of R - L = dwcTime.at(0) - dwcTime.at(1), but should be flipped by multiplying -1
  // Time difference of U - D = dwcTime.at(2) - dwcTime.at(3)
  float dwc1horizontalSlope = -0.1740806676;
  float dwc1horizontalOffset = -0.1680572999;
  float dwc1VerticalSlope = -0.17424779576;
  float dwc1VerticalOffset = -0.053701300;

  float dwc2horizontalSlope = -0.17257273;
  float dwc2horizontalOffset = -0.579927452;
  float dwc2VerticalSlope = -0.1741203164;
  float dwc2VerticalOffset = -0.278179655;

  float dwc1X = -((float)(dwcTime.at(0) - dwcTime.at(1))*25./1000. * dwc1horizontalSlope + dwc1horizontalOffset);
  float dwc1Y = (float)(dwcTime.at(2) - dwcTime.at(3))*25./1000. * dwc1VerticalSlope + dwc1VerticalOffset;
  float dwc2X = -((float)(dwcTime.at(4) - dwcTime.at(5))*25./1000. * dwc2horizontalSlope + dwc2horizontalOffset);
  float dwc2Y = (float)(dwcTime.at(6) - dwcTime.at(7))*25./1000. * dwc2VerticalSlope + dwc2VerticalOffset;

  plots2D_.at(0)->Fill(dwc1X, dwc1Y);
  plots2D_.at(1)->Fill(dwc2X, dwc2Y);
  plots2D_.at(2)->Fill(dwc1X, dwc2X);
  plots2D_.at(3)->Fill(dwc1Y, dwc2Y);
}

void TBplot::fillAux(TBdetector detid, float adc) {

  if(detid.det() == TBdetector::detid::preshower) plots1D_.at(0)->Fill(adc);
  if(detid.det() == TBdetector::detid::tail) plots1D_.at(1)->Fill(adc);
  if(detid.det() == TBdetector::detid::muon) plots1D_.at(2)->Fill(adc);

}

void TBplot::print() {
  for (int i = 0; i < plots2D_.size(); i++) {
    std::cout << plots2D_.at(i)->GetNbinsX() << " " << plots2D_.at(i)->GetNbinsY() << std::endl;
    for (int j = 1; j <= plots2D_.at(i)->GetNbinsX(); j++) {
      for (int t = 1; t <= plots2D_.at(i)->GetNbinsY(); t++)
        std::cout << "plots2D_.at(" << i << ").[" << j << "][" << t << "] : " << plots2D_.at(i)->GetBinContent(j,t) << std::endl;
    }
  }
}

void TBplot::Reset() {
  if ( plotkind_ == TBplotbase::kind::sipmHitMap || plotkind_ == TBplotbase::kind::hitmap ) {
    for(int i = 0; i < plots2D_.size(); i++)
      plots2D_.at(i)->Reset();
  } else if ( plotkind_ == TBplotbase::kind::distribution || plotkind_ == TBplotbase::kind::waveform ) {
    for(int i = 0; i < plots1D_.size(); i++)
      plots1D_.at(i)->Reset();
  }
}

void TBplot::Draw() {
  if ( plotkind_ == TBplotbase::kind::sipmHitMap || plotkind_ == TBplotbase::kind::dwc ) {
    for (int i = 0; i < xlow.at(plotkind_).size(); i++) {
      c_->cd();
      pads_.at(i)->cd();
      plots2D_.at(i)->Draw("col");
    }
  } else if ( plotkind_ == TBplotbase::kind::hitmap ) {
    c_->cd();
    pads_.at(0)->cd();
    plots2D_.at(0)->Draw("col");

    for (int i = 1; i < xlow.at(plotkind_).size(); i++) {
      c_->cd();
      pads_.at(i)->cd();
      plots2D_.at(i)->Draw("col");
    }
  } else if ( plotkind_ == TBplotbase::kind::distribution || plotkind_ == TBplotbase::kind::waveform ) {
    for (int i = 0; i < xlow.at(plotkind_).size(); i++) {

      // Scintillation
      c_->cd();
      pads_.at(i)->cd();
      plots1D_.at(i+xlow.at(plotkind_).size())->Draw("Hist");
      c_->Modified();
      c_->Update();

      TPaveStats* S_stat = (TPaveStats*)plots1D_.at(i+xlow.at(plotkind_).size())->FindObject("stats");
      S_stat->SetName("Scintillation");
      S_stat->SetTextColor(kRed);
      S_stat->SetX1NDC(0.65);
      S_stat->SetX2NDC(0.95);
      S_stat->SetY1NDC(0.8);
      S_stat->SetY2NDC(1.0);
      S_stat->SaveStyle();

      // Cerenkov
     plots1D_.at(i)->Draw("Hist&sames");
      c_->Modified();
      c_->Update();

      TPaveStats* C_stat = (TPaveStats*)plots1D_.at(i)->FindObject("stats");
      C_stat->SetName("Cerenkov");
      C_stat->SetTextColor(kBlue);
      C_stat->SetX1NDC(0.65);
      C_stat->SetX2NDC(0.95);
      C_stat->SetY1NDC(0.6);
      C_stat->SetY2NDC(0.8);
      C_stat->SaveStyle();

      c_->cd();
      pads_.at(i)->cd();
      pads_.at(i)->Modified();
      pads_.at(i)->Update();

      c_->cd();
      pads_.at(i)->cd();
      pads_.at(i)->Modified();
      pads_.at(i)->Update();
    }
  } else if ( plotkind_ == TBplotbase::kind::auxiliary ) {
    for (int i = 0; i < xlow.at(plotkind_).size(); i++) {
      c_->cd();
      pads_.at(i)->cd();
      plots1D_.at(i)->Draw("Hist");
  	  c_->Update();
    }
  }
}
