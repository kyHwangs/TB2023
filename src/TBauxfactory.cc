#include "TBauxfactory.h"

template<typename T>
AuxFactory<T>::AuxFactory(int runNum_, float psThres_) 
: fRunNum(runNum_), fMode(), fPSmipPosition(0), fMCmean(0), fMCsigma(0), fPSthres(psThres_), fPSadc(0), fMUadc(0), fPID(0)  {
	// fMode = new T;
	init();
}

template<typename T>
void AuxFactory<T>::init() {

	std::string path;
	
	if ( getName(fMode) == "TBwaveform") {
		path = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/Aux/Wave/Run" + std::to_string(fRunNum) + "_WaveRunInfo.root";
	} else {
		path = "dcap://cluster142.knu.ac.kr//pnfs/knu.ac.kr/data/cms/store/user/khwang/Aug2022TB/RunInfo/Aux/Fast/Run" + std::to_string(fRunNum) + "_FastRunInfo.root";
	}

	std::cout << "Loading AUX setting : " << path << std::endl;
	TH1F* psFit = (TH1F*)(TFile::Open((TString)path)->Get("preshower_fit"));
	fPSmipPosition = psFit->GetFunction("PSfit")->GetParameter("Mean");

	TH1F* muFit = (TH1F*)(TFile::Open((TString)path)->Get("muoncounter_fit"));
	fMCmean = muFit->GetFunction("MCfit")->GetParameter("Mean");
	fMCsigma = muFit->GetFunction("MCfit")->GetParameter("Sigma");
}

template<typename T>
void AuxFactory<T>::doPID() {

	if ( fMUadc > fMCmean + fMCsigma ) {
		fPID = 13;
	} else {
		if ( fPSadc > fPSthres * fPSmipPosition ) {
			fPID = 11;
		} else {
			fPID = 211;
		}
	}
}

template<typename T>
void AuxFactory<T>::SetAUXadc(float ps_, float mu_) {

	fPSadc = ps_;
	fMUadc = mu_;

	doPID();
}

template<typename T>
void AuxFactory<T>::print() {
	printf("  \n");
	printf("  ------------- AUX setting -------------\n");
	printf("  -----  1 mip position : %6f \n", fPSmipPosition);
	printf("  -----  PS cut thres   : %6f \n", fPSthres);
	printf("  -----  MU mean        : %6f \n", fMCmean);
	printf("  -----  MU sigma       : %6f \n", fMCsigma);
	printf("  ---------------------------------------\n");
	printf("  \n");
}

template class AuxFactory<TBwaveform>;
template class AuxFactory<TBfastmode>;