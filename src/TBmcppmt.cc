#include "TBmcppmt.h"


template<typename T>
TBmcppmt<T>::TBmcppmt(int fRunNum_, calculator fCalc_, bool fUseExPed_)
: fMode(), fRunNum(fRunNum_), fCalc(fCalc_), fUseExPed(fUseExPed_), fMaxEvent(-1), fUtility(TButility()), fNBin(1000), fPlotXmin(0.), fPlotXmax(4096.), fCalcRangeMin(0), fCalcRangeMax(1000), fDoHeatMap(false)
{
    fPlotCeren.reserve(5 * 10);
    fPlotScint.reserve(5 * 10);

    if ( fCalc == calculator::AvgTimeStruc ) {
        fNBin = 1000;
        fPlotXmin = 0.;
        fPlotXmax = 1000.;
    } 
    if ( fCalc == calculator::PeakADC ) {
        fNBin = 1000;
        fPlotXmin = 0.;
        fPlotXmax = 4096.;        
    }
    if ( fCalc == calculator::IntADC ) {
        fNBin = 1000;
        fPlotXmin = -3000.;
        fPlotXmax = 300000.;      
    }

    SetReader(fMode);
}

template<typename T>
void TBmcppmt<T>::SetPlotRangeX(int nbin, float min, float max) {
    fNBin = nbin;
    fPlotXmin = min;
    fPlotXmax = max;
}

template<typename T>
void TBmcppmt<T>::SetCalcRangeX(float min, float max) {
    fCalcRangeMin = min;
    fCalcRangeMax = max;
}

template<typename T>
void TBmcppmt<T>::PreparePlots() {

    for ( int nRow = 1; nRow <= 10; nRow++ ) {
        for ( int nCol = 1; nCol <= 5; nCol++ ) {
            fPlotCeren.emplace_back(new TH1F((TString)("Ceren_row" + std::to_string(nRow) + "_col" +std::to_string(nCol)), "", fNBin, fPlotXmin, fPlotXmax));
            fPlotScint.emplace_back(new TH1F((TString)("Scint_row" + std::to_string(nRow) + "_col" +std::to_string(nCol)), "", fNBin, fPlotXmin, fPlotXmax));
        }
    }
}

template<typename T>
void TBmcppmt<T>::GetData(TBwaveform mode) {

    for (int i = 0; i < fCIDCeren.size(); i++)
        fChCerenWave.push_back(fAnWaveEvent.GetData(fCIDCeren.at(i)));

    for (int i = 0; i < fCIDScint.size(); i++)
        fChScintWave.push_back(fAnWaveEvent.GetData(fCIDScint.at(i)));
}

template<typename T>
void TBmcppmt<T>::GetData(TBfastmode mode) {

    for (int i = 0; i < fCIDCeren.size(); i++)
        fChCerenFast.push_back(fAnFastEvent.GetData(fCIDCeren.at(i)));

    for (int i = 0; i < fCIDScint.size(); i++)
        fChScintFast.push_back(fAnFastEvent.GetData(fCIDScint.at(i)));
}

template<typename T>
void TBmcppmt<T>::Fill(TBfastmode mode) {

}

template<typename T>
void TBmcppmt<T>::Fill(TBwaveform mode) {
    if ( fCalc == calculator::AvgTimeStruc ) FillAverageTimeStruc();
    if ( fCalc == calculator::PeakADC ) FillPeakADC();
    if ( fCalc == calculator::IntADC ) FillIntADC();
}

template<typename T>
void TBmcppmt<T>::FillAverageTimeStruc() {

    for (int i = 0; i < fChCerenWave.size(); i++) {
        auto aWave = fChCerenWave.at(i).waveform();
        for (int j = 1; j <= 1000; j++) {
            fPlotCeren.at(i)->Fill(j - 1, aWave.at(j));
        }
    }

    for (int i = 0; i < fChScintWave.size(); i++) {
        auto aWave = fChScintWave.at(i).waveform();
        for (int j = 1; j <= 1000; j++) {
            fPlotScint.at(i)->Fill(j - 1, aWave.at(j));
        }
    }

}

template<typename T>
void TBmcppmt<T>::FillPeakADC() {

    for (int i = 0; i < fChCerenWave.size(); i++) {

        auto aWave = fChCerenWave.at(i).waveform();

        float ped = 0;
        if ( fUseExPed ) {
            ped = fUtility.retrievePed(fCIDCeren.at(i));
        } else {
            ped = std::accumulate(aWave.begin() + 1, aWave.begin() + 101, 0.) / 100.;
        }
    
        auto aWavePed = fChCerenWave.at(i).pedcorrectedWaveform(ped);

        fPlotCeren.at(i)->Fill(*std::min_element(aWavePed.begin() + fCalcRangeMin, aWavePed.begin() + fCalcRangeMax));
    }

    for (int i = 0; i < fChScintWave.size(); i++) {

        auto aWave = fChScintWave.at(i).waveform();

        float ped = 0;
        if ( fUseExPed ) {
            ped = fUtility.retrievePed(fCIDScint.at(i));
        } else {
            ped = std::accumulate(aWave.begin() + 1, aWave.begin() + 101, 0.) / 100.;
        }
    
        auto aWavePed = fChScintWave.at(i).pedcorrectedWaveform(ped);

        fPlotScint.at(i)->Fill(*std::min_element(aWavePed.begin() + fCalcRangeMin, aWavePed.begin() + fCalcRangeMax));
    }

}

template<typename T>
void TBmcppmt<T>::FillIntADC() {

    for (int i = 0; i < fChCerenWave.size(); i++) {

        auto aWave = fChCerenWave.at(i).waveform();

        float ped = 0;
        if ( fUseExPed ) {
            ped = fUtility.retrievePed(fCIDCeren.at(i));
        } else {
            ped = std::accumulate(aWave.begin() + 1, aWave.begin() + 101, 0.) / 100.;
        }
    
        auto aWavePed = fChCerenWave.at(i).pedcorrectedWaveform(ped);

        fPlotCeren.at(i)->Fill(std::accumulate(aWavePed.begin() + fCalcRangeMin, aWavePed.begin() + fCalcRangeMax, 0.));
    }

    for (int i = 0; i < fChScintWave.size(); i++) {

        auto aWave = fChScintWave.at(i).waveform();

        float ped = 0;
        if ( fUseExPed ) {
            ped = fUtility.retrievePed(fCIDScint.at(i));
        } else {
            ped = std::accumulate(aWave.begin() + 1, aWave.begin() + 101, 0.) / 100.;
        }
    
        auto aWavePed = fChScintWave.at(i).pedcorrectedWaveform(ped);

        fPlotScint.at(i)->Fill(std::accumulate(aWavePed.begin() + fCalcRangeMin, aWavePed.begin() + fCalcRangeMax, 0.));
    }

}

template<typename T>
void TBmcppmt<T>::Loop() {

    for ( int nRow = 1; nRow <= 10; nRow++ ) {
        for ( int nCol = 1; nCol <= 5; nCol++ ) {
            fCIDCeren.push_back(fUtility.getcid(TBdetector::detid::MCPPMT, 13, nCol, nRow, 1));
            fCIDScint.push_back(fUtility.getcid(TBdetector::detid::MCPPMT, 13, nCol, nRow, 0));
        }
    }

    std::cout << fCIDCeren.size() << " " << fCIDScint.size() << std::endl;

    std::chrono::time_point time_begin = std::chrono::system_clock::now(); 

    if ( fReaderWave == nullptr )
        if ( fMaxEvent == -1 || fMaxEvent > fReaderFast->GetMaxEvent() )
            fMaxEvent = fReaderFast->GetMaxEvent();

    if ( fReaderFast == nullptr )
        if ( fMaxEvent == -1 || fMaxEvent > fReaderWave->GetMaxEvent() )
            fMaxEvent = fReaderWave->GetMaxEvent();

    for (int i = 0; i < fMaxEvent; i++) {
        if ( i > 0 && i % 10 == 0 ) {
            std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; //delete
            float percent_done = 1. * i / (float)fMaxEvent;
            std::chrono::duration time_left = time_taken * ( 1 / percent_done - 1 );
            std::chrono::minutes minutes_left = std::chrono::duration_cast<std::chrono::minutes>(time_left);
            std::chrono::seconds seconds_left = std::chrono::duration_cast<std::chrono::seconds>(time_left - minutes_left);
            std::cout << "\r\033[F"<< " " << i << " events  "<< minutes_left.count() << ":";
            printf("%02d left   %.1f%%            \n", int(seconds_left.count()), percent_done * 100);
        }        
        
        fChCerenWave.clear();
        fChScintWave.clear();
        fChCerenFast.clear();
        fChScintFast.clear();

        GetAnEvent(fMode);
        GetData(fMode);
        Fill(fMode);
    }

    EndOfLoop();

    std::chrono::duration time_taken = std::chrono::system_clock::now() - time_begin; //delete
    std::chrono::minutes minutes_taken = std::chrono::duration_cast<std::chrono::minutes>(time_taken);
    std::chrono::seconds seconds_taken = std::chrono::duration_cast<std::chrono::seconds>(time_taken - minutes_taken);

    std::cout << "Time Taken : " << minutes_taken.count() << ":" << seconds_taken.count() << std::endl;
}

template<typename T>
void TBmcppmt<T>::EndOfLoop() {

    if ( fCalc == calculator::AvgTimeStruc ) {
        for ( int i = 0; i < fPlotCeren.size(); i++ ) {
            fPlotCeren.at(i)->Scale(1. / fMaxEvent);
            fPlotScint.at(i)->Scale(1. / fMaxEvent);
        }
    }

    TH2F* heatScint = nullptr;
    TH2F* heatCeren = nullptr;

    TFile* oFile = new TFile(GetOutputName(fMode) , "RECREATE");

    if ( fDoHeatMap ) {
        heatScint = GetScintHeatmap();
        heatCeren = GetCerenHeatmap();

        oFile->cd();
        heatScint->Write();
        heatCeren->Write();
    }

    oFile->cd();
    for ( int i = 0; i < fPlotCeren.size(); i++ ) {
        fPlotCeren.at(i)->Write();
        fPlotScint.at(i)->Write();
    }

    oFile->Close();
}

template<typename T>
void TBmcppmt<T>::SetPedestal(std::string path) {
    if (fUseExPed)
        fUtility.loadped(path);
}

template<typename T>
TH2F* TBmcppmt<T>::GetScintHeatmap() {

    TH2F* heatmap = new TH2F((TString)("Heatmap_" + std::to_string(static_cast<int>(fCalc)) + "_Scint" ), "", 5, 0, 5, 10, 0, 10);

    for ( int i = 0; i < fPlotScint.size(); i++ ) {
        int row = GetRow(i);
        int col = GetCol(i);

        float value = 0;

        if ( fCalc == calculator::AvgTimeStruc ) {
            TH1F* aHist = (TH1F*)fPlotScint.at(i)->Clone();

            float tmpPed = 0;
            for ( int j = 1; j <= 100; j++ )
                tmpPed += aHist->GetBinContent(j) / 100.;

            value = tmpPed - aHist->GetBinContent(aHist->GetMinimumBin());
        }

        heatmap->SetBinContent(col, row, value);
    }

    for ( int i = 1; i <= 5; i++)
        heatmap->GetXaxis()->SetBinLabel(i, std::to_string(i).data());

    for ( int i = 1; i <= 10; i++)
        heatmap->GetYaxis()->SetBinLabel(i, std::to_string(i).data());

    return heatmap;

}

template<typename T>
TH2F* TBmcppmt<T>::GetCerenHeatmap() {

    TH2F* heatmap = new TH2F((TString)("Heatmap_" + std::to_string(static_cast<int>(fCalc)) + "_Ceren" ), "", 5, 0, 5, 10, 0, 10);

    for ( int i = 0; i < fPlotCeren.size(); i++ ) {
        int row = GetRow(i);
        int col = GetCol(i);

        float value = 0;

        if ( fCalc == calculator::AvgTimeStruc ) {
            TH1F* aHist = (TH1F*)fPlotCeren.at(i)->Clone();

            float tmpPed = 0;
            for ( int j = 1; j <= 100; j++ )
                tmpPed += aHist->GetBinContent(j) / 100.;

            value = tmpPed - aHist->GetBinContent(aHist->GetMinimumBin());
        }

        heatmap->SetBinContent(col, row, value);
    }

    for ( int i = 1; i <= 5; i++)
        heatmap->GetXaxis()->SetBinLabel(i, std::to_string(i).data());

    for ( int i = 1; i <= 10; i++)
        heatmap->GetYaxis()->SetBinLabel(i, std::to_string(i).data());

    return heatmap;

}


template class TBmcppmt<TBwaveform>;
template class TBmcppmt<TBfastmode>;