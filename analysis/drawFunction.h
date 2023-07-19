#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TH1.h"
#include "TPad.h"
#include "TString.h"
#include "TFile.h"
#include "TApplication.h"
#include "TRootCanvas.h"
#include "TPaveStats.h"

std::vector<int> myColorPalette {
  1, // "kBlack",
  2, // "kRed",
  4, // "kBlue",
  417, // "kGreen",
  616, // "kMagenta",
  433, // "kAzure"
  807, // "kOrange"
  399, // "kYellow"
  429 // kCyan
};

std::vector<std::string> getPlotInfo(std::string module_name) {
    
    std::string plot_type = module_name.substr(0, module_name.find("_"));
    std::string MIDandCh  = module_name.substr(module_name.find("_")+1);
    std::string MID = MIDandCh.substr( MIDandCh.find("Mid") + 3, MIDandCh.find("Ch") - MIDandCh.find("Mid") - 3 );
    std::string Ch  = MIDandCh.substr( MIDandCh.find("Ch") + 2);

    std::vector<std::string> plot_info;
    plot_info.emplace_back(plot_type);
    plot_info.emplace_back(MID);
    plot_info.emplace_back(Ch);

    return plot_info;
}

int getEntry(std::string fileName) {

    FILE *aFile = fopen(fileName.c_str(), "rb");
    fseek(aFile, 0L, SEEK_END);
    long long size = ftell(aFile);
    fclose(aFile);
    int nEvent = size / 65536;

    return nEvent;
}

std::string getFileName(std::string runNum, int MID, int fileIdx) {
    std::string baseDir = "/Users/yhep/scratch/YUdaq";
    std::string fileDir = baseDir + "/Run_" + runNum + "/Run_" + runNum + "_Wave/" + "Run_" + runNum + "_Wave_MID_" + std::to_string(MID) + "/";
    std::string fileName = fileDir + "Run_" + runNum +"_Wave_MID_" + std::to_string(MID) +"_FILE_" + std::to_string(fileIdx) + ".dat";

    return fileName;
}

TFile *ExistenceCheck(TString fileName, TString objectName)
{
  if (gSystem->AccessPathName(fileName))
  {
    std::cout << "*** [GetHist::" << fileName << "] fileName = [" << fileName << "]: not found ***" << std::endl;
    gApplication->Terminate();
  }

  TFile *f_input = TFile::Open(fileName);
  if (!f_input->GetListOfKeys()->Contains(objectName))
  {
    std::cout << "*** [GetHist::" << fileName << "] objectName = [" << objectName << "]: not found in [" << fileName << "] ***" << std::endl;
    gApplication->Terminate();
  }

  return f_input;
}

TH1F* GetHist(TString filename, TString histname, bool isCeren, TString newname = "")
{
  TFile *f_input = ExistenceCheck(filename, histname);
  TH1::AddDirectory(kFALSE);

  TH1F *h_temp = (TH1F *)f_input->Get(histname)->Clone();
  if (newname != "")
    h_temp->SetName(newname);

  f_input->Close();

  h_temp->SetLineWidth(2);

  if (isCeren)
  {
    h_temp->SetLineColor(kBlue);
  }
  else
  {
    h_temp->SetLineColor(kRed);
  }

  return h_temp;
}

void PadSet(TPad *aPad)
{
  aPad->Draw();
  aPad->cd();

  aPad->SetTopMargin(0.13);
  aPad->SetLeftMargin(0.13);
  aPad->SetRightMargin(0.13);
  aPad->SetBottomMargin(0.13);
}

float getPed(std::vector<short> waveform) {
  return std::accumulate( waveform.begin() + 1, waveform.begin() + 101, 0.) / 100.;
}

float getPedfromBack(std::vector<short> waveform) {
  return std::accumulate( waveform.end() - 24, waveform.end()  - 124, 0.) / 100.;
}

float getMin(std::vector<short> waveform) {
  return *(std::min_element(waveform.begin() + 1, waveform.end() - 23));
}

float getMinFrom(std::vector<short> waveform, int from, int end) {
  int minus = waveform.size() - end;
  return *(std::min_element(waveform.begin() + from, waveform.end() - minus));
}

int getMinIdx(std::vector<short> waveform) {
  return std::distance( waveform.begin(), std::min_element(waveform.begin() + 1, waveform.end() - 23) );
}

int getMinIdxFrom(std::vector<short> waveform, int from, int end) {
  int minus = waveform.size() - end;
  return std::distance( waveform.begin(), std::min_element(waveform.begin() + from, waveform.end() - minus) );
}

float interpolate(std::vector<short> waveform, int thrs_bin) {
  float x0 = (float) (thrs_bin - 1) * 0.2;
  float x1 = (float) thrs_bin * 0.2;
  float y0 = (float) getPed(waveform) - waveform.at(thrs_bin-1);
  float y1 = (float) getPed(waveform) - waveform.at(thrs_bin);

  float thrs = (getPed(waveform) - getMin(waveform)) * 0.3;

  return (float)( x0 + (thrs - y0) * (x1 - x0) / (y1 - y0) );
}

std::map<std::string, int> getPositionMap()
{
  std::map<std::string, int> return_map;

  return_map.insert(std::make_pair("L1-Ceren", 3));
  return_map.insert(std::make_pair("L2-Ceren", 1));
  return_map.insert(std::make_pair("L3-Ceren", 9));
  return_map.insert(std::make_pair("L4-Ceren", 7));

  return_map.insert(std::make_pair("HW-Ceren", 2));
  return_map.insert(std::make_pair("H1-Ceren", 6));
  return_map.insert(std::make_pair("H2-Ceren", 4));
  return_map.insert(std::make_pair("H3-Ceren", 8));

  return_map.insert(std::make_pair("mid-Ceren", 5));
  return_map.insert(std::make_pair("W1-Ceren", 2));
  return_map.insert(std::make_pair("W2-Ceren", 6));
  return_map.insert(std::make_pair("W3-Ceren", 4));

  return_map.insert(std::make_pair("L1-Scint", 3));
  return_map.insert(std::make_pair("L2-Scint", 1));
  return_map.insert(std::make_pair("L3-Scint", 9));
  return_map.insert(std::make_pair("L4-Scint", 7));

  return_map.insert(std::make_pair("HW-Scint", 2));
  return_map.insert(std::make_pair("H1-Scint", 6));
  return_map.insert(std::make_pair("H2-Scint", 4));
  return_map.insert(std::make_pair("H3-Scint", 8));

  return_map.insert(std::make_pair("mid-Scint", 5));
  return_map.insert(std::make_pair("W1-Scint", 2));
  return_map.insert(std::make_pair("W2-Scint", 6));
  return_map.insert(std::make_pair("W3-Scint", 4));

  return return_map;
}

std::map<std::string, std::vector<int>> getModuleConfigMap() {
    std::map<std::string, std::vector<int>> map_btw_MIDCH_and_Name;

    map_btw_MIDCH_and_Name.insert(std::make_pair("HW-Ceren", std::vector<int>{9, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H1-Ceren", std::vector<int>{9, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H2-Ceren", std::vector<int>{9, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H3-Ceren", std::vector<int>{9, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("HW-Scint", std::vector<int>{8, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H1-Scint", std::vector<int>{8, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H2-Scint", std::vector<int>{8, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("H3-Scint", std::vector<int>{8, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("L1-Ceren", std::vector<int>{9, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L2-Ceren", std::vector<int>{9, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L3-Ceren", std::vector<int>{9, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L4-Ceren", std::vector<int>{9, 18}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("L1-Scint", std::vector<int>{8, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L2-Scint", std::vector<int>{8, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L3-Scint", std::vector<int>{8, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("L4-Scint", std::vector<int>{8, 18}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("mid-Ceren", std::vector<int>{9, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W1-Ceren", std::vector<int>{9, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W2-Ceren", std::vector<int>{9, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W3-Ceren", std::vector<int>{9, 4}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("mid-Scint", std::vector<int>{8, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W1-Scint", std::vector<int>{8, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W2-Scint", std::vector<int>{8, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("W3-Scint", std::vector<int>{8, 4}));
    
    map_btw_MIDCH_and_Name.insert(std::make_pair("1-Ceren",  std::vector<int>  {3, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("2-Ceren",  std::vector<int>  {3, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("3-Ceren",  std::vector<int>  {4, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("4-Ceren",  std::vector<int>  {4, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("5-Ceren",  std::vector<int>  {4, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("6-Ceren",  std::vector<int>  {3, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("7-Ceren",  std::vector<int>  {3, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("8-Ceren",  std::vector<int>  {4, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("9-Ceren",  std::vector<int>  {4, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("10-Ceren", std::vector<int>  {4, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("11-Ceren", std::vector<int>  {3, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("12-Ceren", std::vector<int>  {3, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("13-Ceren", std::vector<int>  {4, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("14-Ceren", std::vector<int>  {4, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("15-Ceren", std::vector<int>  {4, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("16-Ceren", std::vector<int>  {3, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("17-Ceren", std::vector<int>  {3, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("18-Ceren", std::vector<int>  {4, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("19-Ceren", std::vector<int>  {4, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("20-Ceren", std::vector<int>  {4, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("21-Ceren", std::vector<int>  {3, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("22-Ceren", std::vector<int>  {3, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("23-Ceren", std::vector<int>  {3, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("24-Ceren", std::vector<int>  {3, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("25-Ceren", std::vector<int>  {3, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("26-Ceren", std::vector<int>  {3, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("27-Ceren", std::vector<int>  {3, 24}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("28-Ceren", std::vector<int>  {4, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("29-Ceren", std::vector<int>  {4, 8} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("30-Ceren", std::vector<int>  {4, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("31-Ceren", std::vector<int>  {3, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("32-Ceren", std::vector<int>  {3, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("33-Ceren", std::vector<int>  {4, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("34-Ceren", std::vector<int>  {4, 6} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("35-Ceren", std::vector<int>  {4, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("36-Ceren", std::vector<int>  {3, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("37-Ceren", std::vector<int>  {3, 20}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("38-Ceren", std::vector<int>  {4, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("39-Ceren", std::vector<int>  {4, 4} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("40-Ceren", std::vector<int>  {4, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("41-Ceren", std::vector<int>  {3, 8} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("42-Ceren", std::vector<int>  {3, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("43-Ceren", std::vector<int>  {3, 6} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("44-Ceren", std::vector<int>  {3, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("45-Ceren", std::vector<int>  {3, 4} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("46-Ceren", std::vector<int>  {3, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("47-Ceren", std::vector<int>  {3, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("48-Ceren", std::vector<int>  {4, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("49-Ceren", std::vector<int>  {4, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("50-Ceren", std::vector<int>  {4, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("1-Scint",  std::vector<int>  {1, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("2-Scint",  std::vector<int>  {1, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("3-Scint",  std::vector<int>  {1, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("4-Scint",  std::vector<int>  {2, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("5-Scint",  std::vector<int>  {2, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("6-Scint",  std::vector<int>  {1, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("7-Scint",  std::vector<int>  {1, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("8-Scint",  std::vector<int>  {1, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("9-Scint",  std::vector<int>  {2, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("10-Scint", std::vector<int>  {2, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("11-Scint", std::vector<int>  {1, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("12-Scint", std::vector<int>  {1, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("13-Scint", std::vector<int>  {1, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("14-Scint", std::vector<int>  {2, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("15-Scint", std::vector<int>  {2, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("16-Scint", std::vector<int>  {1, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("17-Scint", std::vector<int>  {1, 7}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("18-Scint", std::vector<int>  {1, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("19-Scint", std::vector<int>  {2, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("20-Scint", std::vector<int>  {2, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("21-Scint", std::vector<int>  {2, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("22-Scint", std::vector<int>  {2, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("23-Scint", std::vector<int>  {2, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("24-Scint", std::vector<int>  {2, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("25-Scint", std::vector<int>  {2, 24} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("26-Scint", std::vector<int>  {1, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("27-Scint", std::vector<int>  {1, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("28-Scint", std::vector<int>  {1, 24} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("29-Scint", std::vector<int>  {2, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("30-Scint", std::vector<int>  {2, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("31-Scint", std::vector<int>  {1, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("32-Scint", std::vector<int>  {1, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("33-Scint", std::vector<int>  {1, 22} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("34-Scint", std::vector<int>  {2, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("35-Scint", std::vector<int>  {2, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("36-Scint", std::vector<int>  {1, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("37-Scint", std::vector<int>  {1, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("38-Scint", std::vector<int>  {1, 20} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("39-Scint", std::vector<int>  {2, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("40-Scint", std::vector<int>  {2, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("41-Scint", std::vector<int>  {2, 23} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("42-Scint", std::vector<int>  {2, 21} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("43-Scint", std::vector<int>  {2, 22} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("44-Scint", std::vector<int>  {2, 19} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("45-Scint", std::vector<int>  {2, 20} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("46-Scint", std::vector<int>  {1, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("47-Scint", std::vector<int>  {1, 3}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("48-Scint", std::vector<int>  {1, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("49-Scint", std::vector<int>  {2, 18} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("50-Scint", std::vector<int>  {2, 2}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("T1", std::vector<int> {8, 24}));  // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 1
    map_btw_MIDCH_and_Name.insert(std::make_pair("T2", std::vector<int> {9, 24})); // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 2
    map_btw_MIDCH_and_Name.insert(std::make_pair("T3", std::vector<int> {8, 19})); // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 3
    map_btw_MIDCH_and_Name.insert(std::make_pair("T4", std::vector<int> {9, 19})); // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 4

    map_btw_MIDCH_and_Name.insert(std::make_pair("C1", std::vector<int> {12, 8}));  // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 1
    map_btw_MIDCH_and_Name.insert(std::make_pair("C2", std::vector<int> {12, 16}));  // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 1

    map_btw_MIDCH_and_Name.insert(std::make_pair("T1N", std::vector<int> {8, 7})); // FIXME!! : TEMPORARY MAPPING FOR C COUNTER 1
    map_btw_MIDCH_and_Name.insert(std::make_pair("T2N", std::vector<int> {9, 7})); // FIXME!! : TEMPORARY MAPPING FOR C COUNTER 2
    map_btw_MIDCH_and_Name.insert(std::make_pair("T3N", std::vector<int> {9, 15})); // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 2
    // map_btw_MIDCH_and_Name.insert(std::make_pair("T4N", std::vector<int> {9, 15})); // NOT USING THIS
    map_btw_MIDCH_and_Name.insert(std::make_pair("Coin", std::vector<int> {8, 15})); // FIXME!! : TEMPORARY MAPPING FOR C COUNTER 2

    map_btw_MIDCH_and_Name.insert(std::make_pair("1C",  std::vector<int> {5, 1}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("2C",  std::vector<int> {5, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("3C",  std::vector<int> {5, 3}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("4C",  std::vector<int> {5, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("5C",  std::vector<int> {5, 5}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("6C",  std::vector<int> {5, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("7C",  std::vector<int> {5, 7}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("8C",  std::vector<int> {5, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("9C",  std::vector<int> {5, 9}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("10C", std::vector<int> {5, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("11C", std::vector<int> {5, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("12C", std::vector<int> {5, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("13C", std::vector<int> {5, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("14C", std::vector<int> {5, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("15C", std::vector<int> {5, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("16C", std::vector<int> {5, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("17C", std::vector<int> {5, 17}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("18C", std::vector<int> {5, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("19C", std::vector<int> {5, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("20C", std::vector<int> {5, 20}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("21C", std::vector<int> {5, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("22C", std::vector<int> {5, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("23C", std::vector<int> {5, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("24C", std::vector<int> {5, 24}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("25C", std::vector<int> {5, 25}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("26C", std::vector<int> {5, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("27C", std::vector<int> {5, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("28C", std::vector<int> {5, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("29C", std::vector<int> {5, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("30C", std::vector<int> {5, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("31C", std::vector<int> {5, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("32C", std::vector<int> {5, 32}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("1S",  std::vector<int> {6, 1}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("2S",  std::vector<int> {6, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("3S",  std::vector<int> {6, 3}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("4S",  std::vector<int> {6, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("5S",  std::vector<int> {6, 5}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("6S",  std::vector<int> {6, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("7S",  std::vector<int> {6, 7}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("8S",  std::vector<int> {6, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("9S",  std::vector<int> {6, 9}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("10S", std::vector<int> {6, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("11S", std::vector<int> {6, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("12S", std::vector<int> {6, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("13S", std::vector<int> {6, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("14S", std::vector<int> {6, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("15S", std::vector<int> {6, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("16S", std::vector<int> {6, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("17S", std::vector<int> {6, 17}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("18S", std::vector<int> {6, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("19S", std::vector<int> {6, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("20S", std::vector<int> {6, 20}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("21S", std::vector<int> {6, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("22S", std::vector<int> {6, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("23S", std::vector<int> {6, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("24S", std::vector<int> {6, 24}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("25S", std::vector<int> {6, 25}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("26S", std::vector<int> {6, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("27S", std::vector<int> {6, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("28S", std::vector<int> {6, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("29S", std::vector<int> {6, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("30S", std::vector<int> {6, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("31S", std::vector<int> {6, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("32S", std::vector<int> {6, 32}));

    return map_btw_MIDCH_and_Name;
}

void printProgress(const int currentStep, const int totalStep)
{
    // print progress
    float progress = (float)currentStep / totalStep;
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; i++)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "]  " << currentStep << "/" << totalStep << "  " << int(progress * 100.0) << "%\r";
    std::cout.flush();
}

float GetInt(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.begin() + 1, waveform.begin() + 101, 0.) / 100.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (std::accumulate(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin, 0.));
}

float GetInt50ped(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.begin() + 1, waveform.begin() + 51, 0.) / 50.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (std::accumulate(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin, 0.));
}

float GetIntFromBack(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.end() - 124, waveform.end() - 24, 0.) / 100.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (std::accumulate(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin, 0.));
}

std::vector<float> GetAvg(std::vector<short> waveform, int maxEntry)
{
  std::vector<float> scaled_waveform;
  for(int i = 0; i < waveform.size(); i++)
    scaled_waveform.push_back( ( (float) waveform.at(i) / (float) maxEntry) );
  return scaled_waveform;
}

float GetPeak(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.begin() + 1, waveform.begin() + 101, 0.) / 100.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (*std::max_element(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin));
}

float GetPeak50ped(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.begin() + 1, waveform.begin() + 51, 0.) / 50.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (*std::max_element(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin));
}

float GetPeakFromBack(std::vector<short> waveform, int startBin, int endBin)
{
  float ped = std::accumulate(waveform.end() - 24, waveform.begin() - 124, 0.) / 100.;
  
  std::vector<float> pedCorrectedWave;
  for (int i = 0; i < waveform.size(); i++)
    pedCorrectedWave.push_back(ped - waveform.at(i));

  return (*std::max_element(pedCorrectedWave.begin() + startBin, pedCorrectedWave.begin() + endBin));
}

std::vector<std::string> mcppmt_c_names = { "1-Ceren", 
                                            "2-Ceren", 
                                            "3-Ceren", 
                                            "4-Ceren", 
                                            "5-Ceren", 
                                            "6-Ceren", 
                                            "7-Ceren", 
                                            "8-Ceren", 
                                            "9-Ceren", 
                                            "10-Ceren",
                                            "11-Ceren",
                                            "12-Ceren",
                                            "13-Ceren",
                                            "14-Ceren",
                                            "15-Ceren",
                                            "16-Ceren",
                                            "17-Ceren",
                                            "18-Ceren",
                                            "19-Ceren",
                                            "20-Ceren",
                                            "21-Ceren",
                                            "22-Ceren",
                                            "23-Ceren",
                                            "24-Ceren",
                                            "25-Ceren",
                                            "26-Ceren",
                                            "27-Ceren",
                                            "28-Ceren",
                                            "29-Ceren",
                                            "30-Ceren",
                                            "31-Ceren",
                                            "32-Ceren",
                                            "33-Ceren",
                                            "34-Ceren",
                                            "35-Ceren",
                                            "36-Ceren",
                                            "37-Ceren",
                                            "38-Ceren",
                                            "39-Ceren",
                                            "40-Ceren",
                                            "41-Ceren",
                                            "42-Ceren",
                                            "43-Ceren",
                                            "44-Ceren",
                                            "45-Ceren",
                                            "46-Ceren",
                                            "47-Ceren",
                                            "48-Ceren",
                                            "49-Ceren",
                                            "50-Ceren"};

std::vector<std::string> mcppmt_s_names = { "1-Scint", 
                                            "2-Scint", 
                                            "3-Scint", 
                                            "4-Scint", 
                                            "5-Scint", 
                                            "6-Scint", 
                                            "7-Scint", 
                                            "8-Scint", 
                                            "9-Scint", 
                                            "10-Scint",
                                            "11-Scint",
                                            "12-Scint",
                                            "13-Scint",
                                            "14-Scint",
                                            "15-Scint",
                                            "16-Scint",
                                            "17-Scint",
                                            "18-Scint",
                                            "19-Scint",
                                            "20-Scint",
                                            "21-Scint",
                                            "22-Scint",
                                            "23-Scint",
                                            "24-Scint",
                                            "25-Scint",
                                            "26-Scint",
                                            "27-Scint",
                                            "28-Scint",
                                            "29-Scint",
                                            "30-Scint",
                                            "31-Scint",
                                            "32-Scint",
                                            "33-Scint",
                                            "34-Scint",
                                            "35-Scint",
                                            "36-Scint",
                                            "37-Scint",
                                            "38-Scint",
                                            "39-Scint",
                                            "40-Scint",
                                            "41-Scint",
                                            "42-Scint",
                                            "43-Scint",
                                            "44-Scint",
                                            "45-Scint",
                                            "46-Scint",
                                            "47-Scint",
                                            "48-Scint",
                                            "49-Scint",
                                            "50-Scint"};

std::vector<std::string> sipm_c_names = { "1C",
                                          "2C",
                                          "3C",
                                          "4C",
                                          "5C",
                                          "6C",
                                          "7C",
                                          "8C",
                                          "9C",
                                          "10C",
                                          "11C",
                                          "12C",
                                          "13C",
                                          "14C",
                                          "15C",
                                          "16C",
                                          "17C",
                                          "18C",
                                          "19C",
                                          "20C",
                                          "21C",
                                          "22C",
                                          "23C",
                                          "24C",
                                          "25C",
                                          "26C",
                                          "27C",
                                          "28C",
                                          "29C",
                                          "30C",
                                          "31C",
                                          "32C"};

std::vector<std::string> sipm_s_names = { "1S",
                                          "2S",
                                          "3S",
                                          "4S",
                                          "5S",
                                          "6S",
                                          "7S",
                                          "8S",
                                          "9S",
                                          "10S",
                                          "11S",
                                          "12S",
                                          "13S",
                                          "14S",
                                          "15S",
                                          "16S",
                                          "17S",
                                          "18S",
                                          "19S",
                                          "20S",
                                          "21S",
                                          "22S",
                                          "23S",
                                          "24S",
                                          "25S",
                                          "26S",
                                          "27S",
                                          "28S",
                                          "29S",
                                          "30S",
                                          "31S",
                                          "32S"};

int IdxToRow(int idx) {
  return (int) ( 5 - (idx % 5));
}

int IdxToCol(int idx) {
  return (int) ( (idx / 5) + 1);
}
