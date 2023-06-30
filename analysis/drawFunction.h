#include <string>
#include <iostream>
#include <vector>

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
  3, // "kGreen",
  6, // "kMagenta",
  7, // "kAzure"

  1, // "kBlack",
  2, // "kRed",
  4, // "kBlue",
  3, // "kGreen",
  6, // "kMagenta",
  7 // "kAzure"
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
    std::string baseDir = "/Users/swkim/DRC/2022_TB_at_CERN/data_sample";
    //std::string baseDir = "/Users/yhep/scratch/YUdaq";
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

std::map<std::string, int> getPositionMap()
{
  std::map<std::string, int> return_map;

  return_map.insert(std::make_pair("LEGO-L1-Ceren", 3));
  return_map.insert(std::make_pair("LEGO-L2-Ceren", 1));
  return_map.insert(std::make_pair("LEGO-L3-Ceren", 9));
  return_map.insert(std::make_pair("LEGO-L4-Ceren", 7));

  return_map.insert(std::make_pair("SFHS-HW-Ceren", 2));
  return_map.insert(std::make_pair("SFHS-H1-Ceren", 6));
  return_map.insert(std::make_pair("SFHS-H2-Ceren", 4));
  return_map.insert(std::make_pair("SFHS-H3-Ceren", 8));

  return_map.insert(std::make_pair("MCPPMT_gen-mid-Ceren", 5));
  return_map.insert(std::make_pair("MCPPMT_gen-W1-Ceren", 2));
  return_map.insert(std::make_pair("MCPPMT_gen-W2-Ceren", 6));
  return_map.insert(std::make_pair("MCPPMT_gen-W3-Ceren", 4));

  return_map.insert(std::make_pair("LEGO-L1-Scint", 3));
  return_map.insert(std::make_pair("LEGO-L2-Scint", 1));
  return_map.insert(std::make_pair("LEGO-L3-Scint", 9));
  return_map.insert(std::make_pair("LEGO-L4-Scint", 7));

  return_map.insert(std::make_pair("SFHS-HW-Scint", 2));
  return_map.insert(std::make_pair("SFHS-H1-Scint", 6));
  return_map.insert(std::make_pair("SFHS-H2-Scint", 4));
  return_map.insert(std::make_pair("SFHS-H3-Scint", 8));

  return_map.insert(std::make_pair("MCPPMT_gen-mid-Scint", 5));
  return_map.insert(std::make_pair("MCPPMT_gen-W1-Scint", 2));
  return_map.insert(std::make_pair("MCPPMT_gen-W2-Scint", 6));
  return_map.insert(std::make_pair("MCPPMT_gen-W3-Scint", 4));

  return return_map;
}

std::map<std::string, std::vector<int>> getModuleConfigMap() {
    std::map<std::string, std::vector<int>> map_btw_MIDCH_and_Name;

    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-HW-Ceren", std::vector<int>{9, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-H1-Ceren", std::vector<int>{9, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-H2-Ceren", std::vector<int>{9, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-H3-Ceren", std::vector<int>{9, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-HW-Scint", std::vector<int>{8, 2}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-H1-Scint", std::vector<int>{8, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-H2-Scint", std::vector<int>{8, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("SFHS-H3-Scint", std::vector<int>{8, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L1-Ceren", std::vector<int>{9, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L2-Ceren", std::vector<int>{9, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L3-Ceren", std::vector<int>{9, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L4-Ceren", std::vector<int>{9, 18}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L1-Scint", std::vector<int>{8, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L2-Scint", std::vector<int>{8, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L3-Scint", std::vector<int>{8, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("LEGO-L4-Scint", std::vector<int>{8, 18}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-mid-Ceren", std::vector<int>{9, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-W1-Ceren", std::vector<int>{9, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-W2-Ceren", std::vector<int>{9, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-W3-Ceren", std::vector<int>{9, 4}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-mid-Scint", std::vector<int>{8, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-W1-Scint", std::vector<int>{8, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-W2-Scint", std::vector<int>{8, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT_gen-W3-Scint", std::vector<int>{8, 4}));
    
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-1-Ceren",  std::vector<int>  {3, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-2-Ceren",  std::vector<int>  {3, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-3-Ceren",  std::vector<int>  {4, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-4-Ceren",  std::vector<int>  {4, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-5-Ceren",  std::vector<int>  {4, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-6-Ceren",  std::vector<int>  {3, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-7-Ceren",  std::vector<int>  {3, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-8-Ceren",  std::vector<int>  {4, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-9-Ceren",  std::vector<int>  {4, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-10-Ceren", std::vector<int>  {4, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-11-Ceren", std::vector<int>  {3, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-12-Ceren", std::vector<int>  {3, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-13-Ceren", std::vector<int>  {4, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-14-Ceren", std::vector<int>  {4, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-15-Ceren", std::vector<int>  {4, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-16-Ceren", std::vector<int>  {3, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-17-Ceren", std::vector<int>  {3, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-18-Ceren", std::vector<int>  {4, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-19-Ceren", std::vector<int>  {4, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-20-Ceren", std::vector<int>  {4, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-21-Ceren", std::vector<int>  {3, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-22-Ceren", std::vector<int>  {3, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-23-Ceren", std::vector<int>  {3, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-24-Ceren", std::vector<int>  {3, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-25-Ceren", std::vector<int>  {3, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-26-Ceren", std::vector<int>  {3, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-27-Ceren", std::vector<int>  {3, 24}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-28-Ceren", std::vector<int>  {4, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-29-Ceren", std::vector<int>  {4, 8} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-30-Ceren", std::vector<int>  {4, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-31-Ceren", std::vector<int>  {3, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-32-Ceren", std::vector<int>  {3, 22}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-33-Ceren", std::vector<int>  {4, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-34-Ceren", std::vector<int>  {4, 6} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-35-Ceren", std::vector<int>  {4, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-36-Ceren", std::vector<int>  {3, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-37-Ceren", std::vector<int>  {3, 20}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-38-Ceren", std::vector<int>  {4, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-39-Ceren", std::vector<int>  {4, 4} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-40-Ceren", std::vector<int>  {4, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-41-Ceren", std::vector<int>  {3, 8} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-42-Ceren", std::vector<int>  {3, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-43-Ceren", std::vector<int>  {3, 6} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-44-Ceren", std::vector<int>  {3, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-45-Ceren", std::vector<int>  {3, 4} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-46-Ceren", std::vector<int>  {3, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-47-Ceren", std::vector<int>  {3, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-48-Ceren", std::vector<int>  {4, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-49-Ceren", std::vector<int>  {4, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-50-Ceren", std::vector<int>  {4, 20}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-1-Scint",  std::vector<int>  {1, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-2-Scint",  std::vector<int>  {1, 31}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-3-Scint",  std::vector<int>  {1, 32}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-4-Scint",  std::vector<int>  {2, 15}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-5-Scint",  std::vector<int>  {2, 16}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-6-Scint",  std::vector<int>  {1, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-7-Scint",  std::vector<int>  {1, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-8-Scint",  std::vector<int>  {1, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-9-Scint",  std::vector<int>  {2, 13}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-10-Scint", std::vector<int>  {2, 14}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-11-Scint", std::vector<int>  {1, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-12-Scint", std::vector<int>  {1, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-13-Scint", std::vector<int>  {1, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-14-Scint", std::vector<int>  {2, 11}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-15-Scint", std::vector<int>  {2, 12}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-16-Scint", std::vector<int>  {1, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-17-Scint", std::vector<int>  {1, 7}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-18-Scint", std::vector<int>  {1, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-19-Scint", std::vector<int>  {2, 29}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-20-Scint", std::vector<int>  {2, 10}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-21-Scint", std::vector<int>  {2, 27}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-22-Scint", std::vector<int>  {2, 28}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-23-Scint", std::vector<int>  {2, 30}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-24-Scint", std::vector<int>  {2, 26}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-25-Scint", std::vector<int>  {2, 24} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-26-Scint", std::vector<int>  {1, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-27-Scint", std::vector<int>  {1, 23}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-28-Scint", std::vector<int>  {1, 24} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-29-Scint", std::vector<int>  {2, 7} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-30-Scint", std::vector<int>  {2, 8}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-31-Scint", std::vector<int>  {1, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-32-Scint", std::vector<int>  {1, 21}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-33-Scint", std::vector<int>  {1, 22} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-34-Scint", std::vector<int>  {2, 5} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-35-Scint", std::vector<int>  {2, 6}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-36-Scint", std::vector<int>  {1, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-37-Scint", std::vector<int>  {1, 19}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-38-Scint", std::vector<int>  {1, 20} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-39-Scint", std::vector<int>  {2, 3} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-40-Scint", std::vector<int>  {2, 4}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-41-Scint", std::vector<int>  {2, 23} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-42-Scint", std::vector<int>  {2, 21} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-43-Scint", std::vector<int>  {2, 22} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-44-Scint", std::vector<int>  {2, 19} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-45-Scint", std::vector<int>  {2, 20} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-46-Scint", std::vector<int>  {1, 2} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-47-Scint", std::vector<int>  {1, 3}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-48-Scint", std::vector<int>  {1, 18}));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-49-Scint", std::vector<int>  {2, 18} ));
    map_btw_MIDCH_and_Name.insert(std::make_pair("MCPPMT-50-Scint", std::vector<int>  {2, 2}));

    map_btw_MIDCH_and_Name.insert(std::make_pair("EXT-T1", std::vector<int> {12, 2}));  // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 1
    map_btw_MIDCH_and_Name.insert(std::make_pair("EXT-T2", std::vector<int> {12, 10})); // FIXME!! : TEMPORARY MAPPING FOR TRIGGER 2

    map_btw_MIDCH_and_Name.insert(std::make_pair("AUX-C1", std::vector<int> {12, 23})); // FIXME!! : TEMPORARY MAPPING FOR C COUNTER 1
    map_btw_MIDCH_and_Name.insert(std::make_pair("AUX-C2", std::vector<int> {12, 31})); // FIXME!! : TEMPORARY MAPPING FOR C COUNTER 2

    return map_btw_MIDCH_and_Name;
}
