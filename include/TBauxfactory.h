#ifndef TBauxfactory_h
#define TBauxfactory_h 1

#include <vector>
#include <string>

#include "TBmid.h"
#include "TBevt.h"
#include "TBdetector.h"

#include <TH1.h>
#include <TF1.h>
#include <TFile.h>

template <class T>
class AuxFactory
{
public:
	AuxFactory(int runNum_, float psThres_);
	~AuxFactory();

	void SetPSmipPosition(float right) { fPSmipPosition = right; }
	float GetPSmipPosition() { return fPSmipPosition; }

	void SetMCmean(float right) { fMCmean = right; }
	float GetMCmean() { return fMCmean; }

	void SetMCsigma(float right) { fMCsigma = right; }
	float GetMCsigma() { return fMCsigma; }

	void SetPSthres(float right) { fPSthres = right; }
	float GetPSthres() { return fPSthres; }

	void SetAUXadc(float ps_, float mu_);
	void SetPSadc(float right) { fPSadc = right; }
	void SetMUadc(float right) { fMUadc = right; }

	int GetPID() { return  fPID; };

	void print();

private:

	void init();
	void doPID();

	std::string getName(TBwaveform set) { return "TBwaveform"; }
	std::string getName(TBfastmode set) { return "TBfastmode"; }

	int fRunNum;
	T fMode;

	float fPSmipPosition;
	float fMCmean;
	float fMCsigma;
	float fPSthres;

	float fPSadc;
	float fMUadc;

	int fPID;
};


#endif