#include "TBdwc.h"

template<typename T>
DWCset<T>::DWCset(int runNum_, std::vector<float> calib_) 
: fRunNum(runNum_), fMode(), fDWCclaib(calib_), fPhysicalOffset(0), fDWCtime(0), fDWCposition(0) {
	init();
}

template<typename T>
void DWCset<T>::init() {

	std::string path;
	
	if ( getName(fMode) == "TBwaveform") {
		path = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/Aux/Wave/Run" + std::to_string(fRunNum) + "_WaveRunInfo.root";
	} else {
		path = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/Aux/Fast/Run" + std::to_string(fRunNum) + "_FastRunInfo.root";
	}

	fPhysicalOffset.push_back( ((TH1F*)(TFile::Open((TString)path)->Get("dwc1_X")))->GetMean() );
	fPhysicalOffset.push_back( ((TH1F*)(TFile::Open((TString)path)->Get("dwc1_Y")))->GetMean() );
	fPhysicalOffset.push_back( ((TH1F*)(TFile::Open((TString)path)->Get("dwc2_X")))->GetMean() );
	fPhysicalOffset.push_back( ((TH1F*)(TFile::Open((TString)path)->Get("dwc2_Y")))->GetMean() );
}

template<typename T>
void DWCset<T>::SetDWC(std::vector<T> dwc1_, std::vector<T> dwc2_) {

	fDWCtime.clear();
	fDWCposition.clear();

	for ( int i = 0; i < 4; i++ )
		GetTime(dwc1_.at(i));

	for ( int i = 0; i < 4; i++ )
		GetTime(dwc2_.at(i));

	CalcPosition();
}

template<typename T>
void DWCset<T>::print() {
	printf("  \n");
	printf("  ---------- DWC offset setting ----------\n");
	printf("  -----  DWC 1 X : %6f \n", fPhysicalOffset.at(0));
	printf("  -----  DWC 1 Y : %6f \n", fPhysicalOffset.at(1));
	printf("  -----  DWC 2 X : %6f \n", fPhysicalOffset.at(2));
	printf("  -----  DWC 2 Y : %6f \n", fPhysicalOffset.at(3));
	printf("  ----------------------------------------\n");
	printf("  \n");

	printf("  ------------ DWC calibration ------------\n");
	printf("  -  DWC 1 Horizontal Slope  : %6f \n", fDWCclaib.at(0));
	printf("  -  DWC 1 Horizontal Offset : %6f \n", fDWCclaib.at(1));
	printf("  -  DWC 1 Vertical Slope    : %6f \n", fDWCclaib.at(2));
	printf("  -  DWC 1 Vertical Offset   : %6f \n", fDWCclaib.at(3));
	printf("  -  DWC 2 Horizontal Slope  : %6f \n", fDWCclaib.at(4));
	printf("  -  DWC 2 Horizontal Offset : %6f \n", fDWCclaib.at(5));
	printf("  -  DWC 2 Vertical Slope    : %6f \n", fDWCclaib.at(6));
	printf("  -  DWC 2 Vertical Offset   : %6f \n", fDWCclaib.at(7));
	printf("  -----------------------------------------\n");
	printf("  \n");
}



template<typename T>
void DWCset<T>::CalcPosition() {

  // fDWCposition.push_back( -(fDWCtime.at(0) - fDWCtime.at(1)) * fDWCclaib.at(0) + fDWCclaib.at(1) - fPhysicalOffset.at(0) );
  // fDWCposition.push_back(  (fDWCtime.at(2) - fDWCtime.at(3)) * fDWCclaib.at(2) + fDWCclaib.at(3) - fPhysicalOffset.at(1) );
  // fDWCposition.push_back( -(fDWCtime.at(4) - fDWCtime.at(5)) * fDWCclaib.at(4) + fDWCclaib.at(5) - fPhysicalOffset.at(2) );
  // fDWCposition.push_back(  (fDWCtime.at(6) - fDWCtime.at(7)) * fDWCclaib.at(6) + fDWCclaib.at(7) - fPhysicalOffset.at(3) );

  fDWCposition.push_back( -(fDWCtime.at(0) - fDWCtime.at(1)) * fDWCclaib.at(0) + fDWCclaib.at(1) );
  fDWCposition.push_back(  (fDWCtime.at(2) - fDWCtime.at(3)) * fDWCclaib.at(2) + fDWCclaib.at(3) );
  fDWCposition.push_back( -(fDWCtime.at(4) - fDWCtime.at(5)) * fDWCclaib.at(4) + fDWCclaib.at(5) );
  fDWCposition.push_back(  (fDWCtime.at(6) - fDWCtime.at(7)) * fDWCclaib.at(6) + fDWCclaib.at(7) );
}

template<typename T>
void DWCset<T>::GetTime(TBwaveform onedwc) {
	auto wave = onedwc.waveform();

	fDWCtime.push_back( 0.4 * (float)(std::min_element(wave.begin() + 1, wave.end() - 23) - wave.begin()) );
}

template<typename T>
void DWCset<T>::GetTime(TBfastmode onedwc) {
	
	fDWCtime.push_back( onedwc.timing()  * 25./1000. );
}

template<typename T>
bool DWCset<T>::inVeto() {

	float center_1 = std::sqrt( (fDWCposition.at(0) - fPhysicalOffset.at(0)) * (fDWCposition.at(0) - fPhysicalOffset.at(0)) + (fDWCposition.at(1) - fPhysicalOffset.at(1)) * (fDWCposition.at(1) - fPhysicalOffset.at(1)) );
	float center_2 = std::sqrt( (fDWCposition.at(2) - fPhysicalOffset.at(2)) * (fDWCposition.at(2) - fPhysicalOffset.at(2)) + (fDWCposition.at(3) - fPhysicalOffset.at(3)) * (fDWCposition.at(3) - fPhysicalOffset.at(3)) );

	if ( center_1 < 20 && center_2 < 20 )
		return true;

	return false;
}

template<typename T>
bool DWCset<T>::inAlign(float thres) {
	if ( std::abs(fDWCposition.at(0) - fPhysicalOffset.at(0) - fDWCposition.at(2) + fPhysicalOffset.at(2)) < thres && std::abs(fDWCposition.at(1) - fPhysicalOffset.at(1) - fDWCposition.at(3) + fPhysicalOffset.at(3)) < thres )
		return true;

	return false;
}

template class DWCset<TBwaveform>;
template class DWCset<TBfastmode>;