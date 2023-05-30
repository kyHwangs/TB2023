#include "TBhist.h"

TBhist::TBhist(TString title_, TString xLabel_, TString yLabel_, int binNum_, float low_, float high_)
: title(title_), xLabel(xLabel_), yLabel(yLabel_), binNum(binNum_), low(low_), high(high_) {
	init();
}

void TBhist::init() {

	binWidth = (high - low) / (float)binNum;
	binNum = binNum + 2;
	low = low - binWidth;
	high = high + binWidth;

	TString label = title + ";" + xLabel + ";" + yLabel;
	hist = new TH1F(title, label, binNum, low, high);
}

void TBhist::Fill(float x, float data) {

	if ( x < low )
		x = hist->GetBinCenter(1);

	if ( x > high )
		x = hist->GetBinCenter(binNum);

	hist->Fill(x, data);
}

void TBhist::Fill(float x) {

	if ( x < low )
		x = hist->GetBinCenter(1);

	if ( x > high )
		x = hist->GetBinCenter(binNum);

	hist->Fill(x);
}