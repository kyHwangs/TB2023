#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <map>
// #include <windows.h>

#include "TBevt.h"
#include "TBmid.h"
#include "TButility.h"
#include "TBread.h"
// #include "TBload.h"
// #include "TBhist.h"
// #include "TBauxfactory.h"
// #include "TBdwc.h"

// #include "functions.cc"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>

class Plotter
{
public:
	Plotter() = default;
	~Plotter() = default;

    virtual void Fill(std::vector<float> aWave) {}
    virtual void Set(std::vector<float> input) {}
};

class cIntADC : public Plotter
{
    float ped;
    float pStart;
    float pFinish;

public:
	cIntADC() {}
	~cIntADC() {}

	virtual void Fill(std::vector<float> aWave) {
        std::cout << "Int ADC Fill"  << std::endl;
        std::cout << ped << std::endl;
        std::cout << pStart << std::endl;
        std::cout << pFinish << std::endl;
    }   

    virtual void Set(std::vector<float> input) {
        ped = input.at(0);
        pStart = input.at(1);
        pFinish = input.at(2);
    }
};

class cPeakADC : public Plotter
{
    float ped;

public:
	cPeakADC() {}
	~cPeakADC() {}

	virtual void Fill(std::vector<float> aWave) {
        std::cout << "Int ADC Fill" << std::endl;
        std::cout << ped << std::endl;
	}
    
    virtual void Set(std::vector<float> input) {
        ped = input.at(0);
    }
};


int main(int argc, char** argv) {

    Plotter* intPlot = new cIntADC();
    Plotter* peakPlot = new cPeakADC();

    intPlot->Set(std::vector<float>{0.1, 0.2, 0.3});
    peakPlot->Set(std::vector<float>{0.1});

    std::vector<float> awave;

    intPlot->Fill(awave);
    peakPlot->Fill(awave);


    return 1;
}