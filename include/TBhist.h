#ifndef TBhist_h
#define TBhist_h 1

#include "TString.h"
#include <TH1.h>

class TBhist
{
public:
	TBhist(TString title_, TString xLabel_, TString yLabel_, int binNum_, float low_, float high_);
	~TBhist();

	TH1F* getHist() { return hist; }
	void Fill(float x, float data);
	void Fill(float x);
	

private:
	void init();

	TH1F* hist;

	int binNum;
	float low;
	float high;
	float binWidth;

	TString title;
	TString xLabel;
	TString yLabel;
};

#endif