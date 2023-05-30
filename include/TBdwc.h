#ifndef TBdwc_h
#define TBdwc_h 1

#include <vector>
#include <string>

#include "TBmid.h"
#include "TBevt.h"
#include "TBdetector.h"

#include <TH1.h>
#include <TF1.h>
#include <TFile.h>

template <class T>
class DWCset
{
public:
	DWCset(int runNum_, std::vector<float> calib_);
	~DWCset();

	void SetDWC(std::vector<T> dwc1_, std::vector<T> dwc2_);
	void SetOffset(std::vector<float> offset) { fPhysicalOffset = offset; }
	void print();

	bool inVeto();
	bool inAlign(float thres);

	std::vector<float> GetPosition() { return fDWCposition; }

private:

	void init();

	void CalcPosition();
		
	void GetTime(TBwaveform onedwc);
	void GetTime(TBfastmode onedwc);

	std::string getName(TBwaveform set) { return "TBwaveform"; }
	std::string getName(TBfastmode set) { return "TBfastmode"; }

	int fRunNum;
	T fMode;

	std::vector<float> fDWCclaib;
	// 0 : float dwc1horizontalSlope;  // = -0.1740806676;
	// 1 : float dwc1horizontalOffset;  //= -0.1680572999;
	// 2 : float dwc1VerticalSlope;  //   = -0.17424779576;
	// 3 : float dwc1VerticalOffset;  //  = -0.053701300;

	// 4 : float dwc2horizontalSlope;  // = -0.17257273;
	// 5 : float dwc2horizontalOffset;  //= -0.579927452;
	// 6 : float dwc2VerticalSlope;  //   = -0.1741203164;
	// 7 : float dwc2VerticalOffset;  //  = -0.278179655;

	std::vector<float> fPhysicalOffset;
	std::vector<float> fDWCtime;
	std::vector<float> fDWCposition;
};


#endif