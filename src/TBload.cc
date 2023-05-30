#include "TBload.h"

template<typename T>
TBload<T>::TBload(int runNum_)
: runNum(runNum_), aChain(0), aEvent(0) {
  init();
}

template<typename T>
void TBload<T>::init() {

  aChain = new TChain("events");

  TString baseDir = "";

  if ( getType(aEvent) == datamode::kWaveform ) {
    // baseDir = "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_ntuple/waveform/Run_" + std::to_string(runNum) + "/ntuple_Run_" + std::to_string(runNum) + "_Wave_";
    baseDir = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_ntuple/waveform/Run_" + std::to_string(runNum) + "/ntuple_Run_" + std::to_string(runNum) + "_Wave_";
    std::cout << "Loading waveform data for Run : " << runNum << std::endl;
  } else {
    // baseDir = "/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_ntuple/fastmode/Run_" + std::to_string(runNum) + "/ntuple_Run_" + std::to_string(runNum) + "_Fast_";
    baseDir = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_ntuple/fastmode/Run_" + std::to_string(runNum) + "/ntuple_Run_" + std::to_string(runNum) + "_Fast_";
    std::cout << "Loading fastmode data for Run : " << runNum << std::endl;
  }

  for ( int i = 0; i < 200; i++ ) {

    std::string filenameTmp = baseDir.Data() + std::to_string(i) + ".root";

    if ( !access(filenameTmp.c_str(),F_OK) ) {
      std::cout << i << "th file appending : " << filenameTmp << std::endl;
      aChain->Add(filenameTmp.c_str());
    } else {
      std::cout << "Loading done, total entries :  " << aChain->GetEntries() << std::endl;
      break;
    }
  }

  aChain->SetBranchAddress("TBevt",&aEvent);
  entries = aChain->GetEntries();
  currentEntry_ = 0;
}

template<typename T>
void TBload<T>::loading() {
  aChain->GetEntry(currentEntry_);
  currentEntry_++;
}

template<typename T>
void TBload<T>::close() {
  delete aChain;
  delete aEvent;
}

template class TBload<TBwaveform>;
template class TBload<TBfastmode>;
