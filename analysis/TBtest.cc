#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <map>

#include "TBevt.h"
#include "TBmid.h"
#include "TButility.h"
#include "TBread.h"
#include "TBconfig.h"
#include "TBread.h"
#include "TBplotengine.h"
#include "TBmonit.h"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TCanvas.h>




int main(int argc, char** argv) {

    TBmonit<TBwaveform> aMonit = TBmonit<TBwaveform>("sample.yml", 624);
    aMonit.SetMaxEvent(1000);
    aMonit.Loop();

    return 1;
}