#include "drawFunction.h"

int main(int argc, char *argv[])
{
  TApplication app("app", &argc, argv);

  std::string runNum = argv[1];
  std::string aCase = argv[2];

  std::string filename = "./ROOT/DQM_Run" + runNum + "_" + aCase + ".root";

  // allowed ["AvgTimeStruc", "IntADC", "PeakADC", "fast_IntADC", "fast_timing"]
  if ( (aCase == "fast_IntADC") || (aCase == "fast_timing") ) filename = "./ROOT/DQM_fast_Run" + runNum + ".root";
  if (aCase == "AvgTimeStruc") gStyle->SetOptStat("n");
  aCase = aCase + "_";

  // auto mModuleConfig = getModuleConfigMap();
  // auto mPosition = getPositionMap();

  std::vector<TString> vPlotName;
  vPlotName.push_back("L2");
  vPlotName.push_back("HW");
  vPlotName.push_back("L1");
  vPlotName.push_back("H2");
  vPlotName.push_back("");
  vPlotName.push_back("H1");
  vPlotName.push_back("L4");
  vPlotName.push_back("H3");
  vPlotName.push_back("L3");

  std::vector<TString> vPlotNameMCP;
  vPlotNameMCP.push_back("C1");
  vPlotNameMCP.push_back("W1");
  vPlotNameMCP.push_back("C2");
  vPlotNameMCP.push_back("W3");
  vPlotNameMCP.push_back("mid");
  vPlotNameMCP.push_back("W2");

  std::vector<TH1F*> pCerenGeneric;
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(0) + "-Ceren"), true, vPlotName.at(0) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(1) + "-Ceren"), true, vPlotName.at(1) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(2) + "-Ceren"), true, vPlotName.at(2) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(3) + "-Ceren"), true, vPlotName.at(3) + "-Ceren"));
  pCerenGeneric.push_back(new TH1F("", "", 1, 0., 1.));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(5) + "-Ceren"), true, vPlotName.at(5) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(6) + "-Ceren"), true, vPlotName.at(6) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(7) + "-Ceren"), true, vPlotName.at(7) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(8) + "-Ceren"), true, vPlotName.at(8) + "-Ceren"));

  std::vector<TH1F*> pScintGeneric;
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(0) + "-Scint"), false, vPlotName.at(0) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(1) + "-Scint"), false, vPlotName.at(1) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(2) + "-Scint"), false, vPlotName.at(2) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(3) + "-Scint"), false, vPlotName.at(3) + "-Scint"));
  pScintGeneric.push_back(new TH1F("", "", 1, 0., 1.));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(5) + "-Scint"), false, vPlotName.at(5) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(6) + "-Scint"), false, vPlotName.at(6) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(7) + "-Scint"), false, vPlotName.at(7) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + vPlotName.at(8) + "-Scint"), false, vPlotName.at(8) + "-Scint"));

  std::vector<TH1F*> pCerenMCPPMT;
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(0)), true, vPlotNameMCP.at(0)));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(1) + "-Ceren"), true, vPlotNameMCP.at(1) + "-Ceren"));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(2)), true, vPlotNameMCP.at(2)));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(3) + "-Ceren"), true, vPlotNameMCP.at(3) + "-Ceren"));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(4) + "-Ceren"), true, vPlotNameMCP.at(4) + "-Ceren"));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(5) + "-Ceren"), true, vPlotNameMCP.at(5) + "-Ceren"));

  std::vector<TH1F*> pScintMCPPMT;
  pScintMCPPMT.push_back(new TH1F("", "", 1, 0., 1.));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(1) + "-Scint"), false, vPlotNameMCP.at(1) + "-Scint"));
  pScintMCPPMT.push_back(new TH1F("", "", 1, 0., 1.));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(3) + "-Scint"), false, vPlotNameMCP.at(3) + "-Scint"));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(4) + "-Scint"), false, vPlotNameMCP.at(4) + "-Scint"));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + vPlotNameMCP.at(5) + "-Scint"), false, vPlotNameMCP.at(5) + "-Scint"));

  TCanvas *c_generic = new TCanvas("c_generic", "c_generic", 1000, 1000);

  c_generic->cd();
  c_generic->Divide(3, 3);
  c_generic->Update();
  c_generic->Draw();

  for (int i = 0; i < 9; i++) {
    if (i == 4)
      continue;
    
    if (pScintGeneric.at(i)->GetMaximum() > pCerenGeneric.at(i)->GetMaximum()) {
      c_generic->cd(i + 1);

      pScintGeneric.at(i)->Draw("Hist");
      c_generic->Modified();
      c_generic->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* S_stat = (TPaveStats*)pScintGeneric.at(i)->FindObject("stats");
        S_stat->SetTextColor(kRed);
        S_stat->SetX1NDC(0.65);
        S_stat->SetX2NDC(0.95);
        S_stat->SetY1NDC(0.8);
        S_stat->SetY2NDC(1.0);
        S_stat->SaveStyle();
      }

      pCerenGeneric.at(i)->Draw("Hist & sames");
      c_generic->Modified();
      c_generic->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* C_stat = (TPaveStats*)pCerenGeneric.at(i)->FindObject("stats");
        C_stat->SetTextColor(kBlue);
        C_stat->SetX1NDC(0.65);
        C_stat->SetX2NDC(0.95);
        C_stat->SetY1NDC(0.6);
        C_stat->SetY2NDC(0.8);
        C_stat->SaveStyle();
      }

      c_generic->Modified();
      c_generic->Update();
    } else {
      c_generic->cd(i + 1);

      pCerenGeneric.at(i)->Draw("Hist");
      c_generic->Modified();
      c_generic->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* C_stat = (TPaveStats*)pCerenGeneric.at(i)->FindObject("stats");
        C_stat->SetTextColor(kBlue);
        C_stat->SetX1NDC(0.65);
        C_stat->SetX2NDC(0.95);
        C_stat->SetY1NDC(0.6);
        C_stat->SetY2NDC(0.8);
        C_stat->SaveStyle();
      }

      pScintGeneric.at(i)->Draw("Hist & sames");
      c_generic->Modified();
      c_generic->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* S_stat = (TPaveStats*)pScintGeneric.at(i)->FindObject("stats");
        S_stat->SetTextColor(kRed);
        S_stat->SetX1NDC(0.65);
        S_stat->SetX2NDC(0.95);
        S_stat->SetY1NDC(0.8);
        S_stat->SetY2NDC(1.0);
        S_stat->SaveStyle();
      }

      c_generic->Modified();
      c_generic->Update();
    }
  }

  TCanvas *c_mcppmt = new TCanvas("c_mcppmt", "c_mcppmt", 1000, 660);
  c_mcppmt->cd();
  c_mcppmt->Divide(3, 2);
  c_mcppmt->Update();
  c_mcppmt->Draw();

  for (int i = 0; i < 6; i++) {
    if (i == 0 || i == 2) {
      c_mcppmt->cd(i + 1);

      pCerenMCPPMT.at(i)->Draw("Hist");
      c_mcppmt->Modified();
      c_mcppmt->Update();
      
      if (aCase != "AvgTimeStruc_") {
        TPaveStats* C_stat = (TPaveStats*)pCerenMCPPMT.at(i)->FindObject("stats");
        C_stat->SetTextColor(kBlue);
        C_stat->SetX1NDC(0.65);
        C_stat->SetX2NDC(0.95);
        C_stat->SetY1NDC(0.6);
        C_stat->SetY2NDC(0.8);
        C_stat->SaveStyle();
      }
      continue;
    }

    if (pScintMCPPMT.at(i)->GetMaximum() > pCerenMCPPMT.at(i)->GetMaximum()) {
      c_mcppmt->cd(i + 1);

      pScintMCPPMT.at(i)->Draw("Hist");
      c_mcppmt->Modified();
      c_mcppmt->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* S_stat = (TPaveStats*)pScintMCPPMT.at(i)->FindObject("stats");
        S_stat->SetTextColor(kRed);
        S_stat->SetX1NDC(0.65);
        S_stat->SetX2NDC(0.95);
        S_stat->SetY1NDC(0.8);
        S_stat->SetY2NDC(1.0);
        S_stat->SaveStyle();
      }

      pCerenMCPPMT.at(i)->Draw("Hist & sames");
      c_mcppmt->Modified();
      c_mcppmt->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* C_stat = (TPaveStats*)pCerenMCPPMT.at(i)->FindObject("stats");
        C_stat->SetTextColor(kBlue);
        C_stat->SetX1NDC(0.65);
        C_stat->SetX2NDC(0.95);
        C_stat->SetY1NDC(0.6);
        C_stat->SetY2NDC(0.8);
        C_stat->SaveStyle();
      }

      c_mcppmt->Modified();
      c_mcppmt->Update();
    } else {
      c_mcppmt->cd(i + 1);

      pCerenMCPPMT.at(i)->Draw("Hist");
      c_mcppmt->Modified();
      c_mcppmt->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* C_stat = (TPaveStats*)pCerenMCPPMT.at(i)->FindObject("stats");
        C_stat->SetTextColor(kBlue);
        C_stat->SetX1NDC(0.65);
        C_stat->SetX2NDC(0.95);
        C_stat->SetY1NDC(0.6);
        C_stat->SetY2NDC(0.8);
        C_stat->SaveStyle();
      }

      pScintMCPPMT.at(i)->Draw("Hist & sames");
      c_mcppmt->Modified();
      c_mcppmt->Update();

      if (aCase != "AvgTimeStruc_") {
        TPaveStats* S_stat = (TPaveStats*)pScintMCPPMT.at(i)->FindObject("stats");
        S_stat->SetTextColor(kRed);
        S_stat->SetX1NDC(0.65);
        S_stat->SetX2NDC(0.95);
        S_stat->SetY1NDC(0.8);
        S_stat->SetY2NDC(1.0);
        S_stat->SaveStyle();
      }

      c_mcppmt->Modified();
      c_mcppmt->Update();
    }
  }

  // TRootCanvas *rc_generic = (TRootCanvas *)c_generic->GetCanvasImp();
  // rc_generic->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

  // TRootCanvas *rc_mcppmt = (TRootCanvas *)c_mcppmt->GetCanvasImp();
  // rc_mcppmt->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

  app.Run();

  return 0;
}
