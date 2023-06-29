#include "drawFunction.h"

int main(int argc, char *argv[])
{
  TApplication app("app", &argc, argv);

  std::string runNum = argv[1];
  std::string filename = "DQM_Run" + runNum + ".root";

  std::string aCase = argv[2]; // allowed ["AvgTimeStruc", "IntADC", "PeakADC", "fast_IntADC", "fast_timing"]
  aCase = aCase + "_";

  if ( (aCase == "fast_IntADC_") || (aCase == "fast_timing_") ) {
    filename = "DQM_fast_Run" + runNum + ".root";
  }

  bool hasStat = true;
  if (aCase == "AvgTimeStruc_")
    hasStat = false;

  auto mModuleConfig = getModuleConfigMap();
  auto mPosition = getPositionMap();

  std::vector<TString> vPlotName;
  vPlotName.push_back("LEGO-L2");
  vPlotName.push_back("SFHS-HW");
  vPlotName.push_back("LEGO-L1");
  vPlotName.push_back("SFHS-H2");
  vPlotName.push_back("");
  vPlotName.push_back("SFHS-H1");
  vPlotName.push_back("LEGO-L4");
  vPlotName.push_back("SFHS-H3");
  vPlotName.push_back("LEGO-L3");

  std::vector<TString> vPlotNameMCP;
  vPlotNameMCP.push_back("");
  vPlotNameMCP.push_back("MCPPMT_gen-W1");
  vPlotNameMCP.push_back("");
  vPlotNameMCP.push_back("MCPPMT_gen-W3");
  vPlotNameMCP.push_back("MCPPMT_gen-mid");
  vPlotNameMCP.push_back("MCPPMT_gen-W2");

  std::vector<TString> vNewPlotName;
  vNewPlotName.push_back("L2");
  vNewPlotName.push_back("HW");
  vNewPlotName.push_back("L1");
  vNewPlotName.push_back("H2");
  vNewPlotName.push_back("");
  vNewPlotName.push_back("H1");
  vNewPlotName.push_back("L4");
  vNewPlotName.push_back("H3");
  vNewPlotName.push_back("L3");

  std::vector<TString> vNewPlotNameMCP;
  vNewPlotNameMCP.push_back("");
  vNewPlotNameMCP.push_back("W1");
  vNewPlotNameMCP.push_back("");
  vNewPlotNameMCP.push_back("W3");
  vNewPlotNameMCP.push_back("mid");
  vNewPlotNameMCP.push_back("W2");

  std::vector<TH1F*> pCerenGeneric;
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(0) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(0) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(0) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(1) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(1) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(1) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(2) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(2) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(2) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(3) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(3) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(3) + "-Ceren"));
  pCerenGeneric.push_back(new TH1F("", "", 1, 0., 1.));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(5) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(5) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(5) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(6) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(6) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(6) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(7) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(7) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(7) + "-Ceren"));
  pCerenGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(8) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(8) + "-Ceren").Data()).at(1)), true, vNewPlotName.at(8) + "-Ceren"));

  std::vector<TH1F*> pScintGeneric;
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(0) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(0) + "-Scint").Data()).at(1)), false, vNewPlotName.at(0) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(1) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(1) + "-Scint").Data()).at(1)), false, vNewPlotName.at(1) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(2) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(2) + "-Scint").Data()).at(1)), false, vNewPlotName.at(2) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(3) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(3) + "-Scint").Data()).at(1)), false, vNewPlotName.at(3) + "-Scint"));
  pScintGeneric.push_back(new TH1F("", "", 1, 0., 1.));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(5) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(5) + "-Scint").Data()).at(1)), false, vNewPlotName.at(5) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(6) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(6) + "-Scint").Data()).at(1)), false, vNewPlotName.at(6) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(7) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(7) + "-Scint").Data()).at(1)), false, vNewPlotName.at(7) + "-Scint"));
  pScintGeneric.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotName.at(8) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotName.at(8) + "-Scint").Data()).at(1)), false, vNewPlotName.at(8) + "-Scint"));

  std::vector<TH1F*> pCerenMCPPMT;
  pCerenMCPPMT.push_back(new TH1F("", "", 1, 0., 1.));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(1) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(1) + "-Ceren").Data()).at(1)), true, vNewPlotNameMCP.at(1) + "-Ceren"));
  pCerenMCPPMT.push_back(new TH1F("", "", 1, 0., 1.));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(3) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(3) + "-Ceren").Data()).at(1)), true, vNewPlotNameMCP.at(3) + "-Ceren"));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(4) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(4) + "-Ceren").Data()).at(1)), true, vNewPlotNameMCP.at(4) + "-Ceren"));
  pCerenMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(5) + "-Ceren").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(5) + "-Ceren").Data()).at(1)), true, vNewPlotNameMCP.at(5) + "-Ceren"));

  std::vector<TH1F*> pScintMCPPMT;
  pScintMCPPMT.push_back(new TH1F("", "", 1, 0., 1.));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(1) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(1) + "-Scint").Data()).at(1)), false, vNewPlotNameMCP.at(1) + "-Scint"));
  pScintMCPPMT.push_back(new TH1F("", "", 1, 0., 1.));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(3) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(3) + "-Scint").Data()).at(1)), false, vNewPlotNameMCP.at(3) + "-Scint"));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(4) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(4) + "-Scint").Data()).at(1)), false, vNewPlotNameMCP.at(4) + "-Scint"));
  pScintMCPPMT.push_back(GetHist(filename, (TString)(aCase + "Mid" +  mModuleConfig.at((vPlotNameMCP.at(5) + "-Scint").Data()).at(0) + "Ch" + mModuleConfig.at((vPlotNameMCP.at(5) + "-Scint").Data()).at(1)), false, vNewPlotNameMCP.at(5) + "-Scint"));



  // if (!hasStat)
  // {
  //   for (int i = 0; i < 9; i++)
  //   {
  //     pCerenGeneric.at(i)->SetStats(0);
  //     pScintGeneric.at(i)->SetStats(0);
  //   }

  //   for (int i = 0; i < 6; i++) {
  //     pCerenMCPPMT.at(i)->SetStats(0);
  //     pScintMCPPMT.at(i)->SetStats(0);
  //   }
  // }

  TCanvas *c_generic = new TCanvas("c_generic", "c_generic", 1000, 1000);

  c_generic->cd();
  c_generic->Divide(3, 3);
  c_generic->Update();
  c_generic->Draw();

  for (int i = 0; i < 9; i++) {
    if (i == 4)
      continue;
    
    c_generic->cd(i + 1);

    pScintGeneric.at(i)->Draw("Hist");
    c_generic->Modified();
    c_generic->Update();

    TPaveStats* S_stat = (TPaveStats*)pScintGeneric.at(i)->FindObject("stats");
    S_stat->SetTextColor(kRed);
    S_stat->SetX1NDC(0.65);
    S_stat->SetX2NDC(0.95);
    S_stat->SetY1NDC(0.8);
    S_stat->SetY2NDC(1.0);
    S_stat->SaveStyle();

    pCerenGeneric.at(i)->Draw("Hist & sames");
    c_generic->Modified();
    c_generic->Update();

    TPaveStats* C_stat = (TPaveStats*)pCerenGeneric.at(i)->FindObject("stats");
    C_stat->SetTextColor(kBlue);
    C_stat->SetX1NDC(0.65);
    C_stat->SetX2NDC(0.95);
    C_stat->SetY1NDC(0.6);
    C_stat->SetY2NDC(0.8);
    C_stat->SaveStyle();

    c_generic->Modified();
    c_generic->Update();
  }

  TCanvas *c_mcppmt = new TCanvas("c_mcppmt", "c_mcppmt", 1000, 660);
  c_mcppmt->cd();
  c_mcppmt->Divide(3, 2);
  c_mcppmt->Update();
  c_mcppmt->Draw();

  for (int i = 0; i < 6; i++) {
    if (i == 0 || i == 2)
      continue;
    
    c_mcppmt->cd(i + 1);

    pCerenMCPPMT.at(i)->Draw("Hist");
    c_mcppmt->Modified();
    c_mcppmt->Update();

    TPaveStats* C_stat = (TPaveStats*)pCerenMCPPMT.at(i)->FindObject("stats");
    C_stat->SetTextColor(kBlue);
    C_stat->SetX1NDC(0.65);
    C_stat->SetX2NDC(0.95);
    C_stat->SetY1NDC(0.6);
    C_stat->SetY2NDC(0.8);
    C_stat->SaveStyle();

    pScintMCPPMT.at(i)->Draw("Hist & sames");
    c_mcppmt->Modified();
    c_mcppmt->Update();

    TPaveStats* S_stat = (TPaveStats*)pScintMCPPMT.at(i)->FindObject("stats");
    S_stat->SetTextColor(kRed);
    S_stat->SetX1NDC(0.65);
    S_stat->SetX2NDC(0.95);
    S_stat->SetY1NDC(0.8);
    S_stat->SetY2NDC(1.0);
    S_stat->SaveStyle();

    c_mcppmt->Modified();
    c_mcppmt->Update();
  }

  TRootCanvas *rc_generic = (TRootCanvas *)c_generic->GetCanvasImp();
  rc_generic->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

  TRootCanvas *rc_mcppmt = (TRootCanvas *)c_mcppmt->GetCanvasImp();
  rc_mcppmt->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

  app.Run();

  return 0;
}
