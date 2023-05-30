#ifndef TBmonit_h
#define TBmonit_h 1

#include <string>
#include <iostream>

// #include <boost/python.hpp>

#include "TFile.h"
#include "TPad.h"
#include "TBconfig.h"
#include "TBread.h"

template <class T>
class TBmonit {
public:
  TBmonit(const std::string& fConfig_, int fRunNum_);
  ~TBmonit() {}

  void Loop();

  void SetMaxEvent(int fMaxEvent_) { fMaxEvent = fMaxEvent_; }
  
private:
  TBconfig fConfig;

  int fRunNum;
  int fMaxEvent;
};

#endif
