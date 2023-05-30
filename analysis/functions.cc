#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

#include "TBevt.h"
#include "TButility.h"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>

constexpr double const_pi() { 
  return std::atan(1) * 4;
}

float peakADC(std::vector<short> waveform) {

  float ped = 0;
  for ( int i = 1; i < 101; i++ )
    ped += (float)waveform.at(i)/100.;

  
  return ped - (float)*std::min_element(waveform.begin()+1, waveform.end()-23);
}

float getPed(std::vector<short> waveform) {

  float ped = 0;
  for ( int i = 1; i < 101; i++ )
    ped += (float)waveform.at(i)/100.;

  return ped;
}

float getTime(float bin) {

  return 400. * (bin / 1000.);
}

float getLinearInterpolate(float thres, float i, float j, float fi, float fj) {

  return i + (j - i) * (thres - fi) / (fj - fi);
}

float getLeadingEdge(std::vector<float> waveform) {

  float LedingThres = (float)*std::max_element(waveform.begin()+1, waveform.end()-23) * 0.1;
  int LedingInx = std::max_element(waveform.begin()+1, waveform.end()-23) - waveform.begin();
  
  for ( int i = LedingInx; i > 0; i-- )
    if ( waveform.at(i) < LedingThres )
      return getTime(getLinearInterpolate(LedingThres, i, i+1, waveform.at(i), waveform.at(i+1)));

  return 0;
}

std::vector<float> getPosition(std::vector<TBwaveform> dwcSet, float xCoeff, float xConst, float yCoeff, float yConst) {

  std::vector<float> time;
  for ( int i = 0; i < dwcSet.size(); i++ )
    time.push_back(getLeadingEdge(dwcSet.at(i).pedcorrectedWaveform(getPed(dwcSet.at(i).waveform()))));

  return time;

}

float getDWCtimeWave(TBwaveform wave, float ped) {
  return getLeadingEdge(wave.pedcorrectedWaveform(ped));
}

std::vector<float> dwc_calib = {
                                  -0.1740806676,
                                  -0.1680572999,
                                  -0.17424779576,
                                  -0.053701300,
                                  -0.17257273,
                                  -0.579927452,
                                  -0.1741203164,
                                  -0.278179655
                                };


static float dwc1horizontalSlope = -0.1740806676;
static float dwc1horizontalOffset = -0.1680572999;
static float dwc1VerticalSlope = -0.17424779576;
static float dwc1VerticalOffset = -0.053701300;

static float dwc2horizontalSlope = -0.17257273;
static float dwc2horizontalOffset = -0.579927452;
static float dwc2VerticalSlope = -0.1741203164;
static float dwc2VerticalOffset = -0.278179655;

// static float dwc1Xoffset = 2.46;
// static float dwc1Yoffset = -3.191;
// static float dwc2Xoffset = 4.144;
// static float dwc2Yoffset = -10.54;

// Run624
static float dwc1Xoffset = 2.954;
static float dwc1Yoffset = -3.088;
static float dwc2Xoffset = 4.554;
static float dwc2Yoffset = -10.30;

// static float dwc1Xoffset = 0.;
// static float dwc1Yoffset = 0.;
// static float dwc2Xoffset = 0.;
// static float dwc2Yoffset = 0.;

std::vector<float> getDWCposition(std::vector<int> dwcTime) {

  std::vector<float> dec_pos;

  dec_pos.push_back( -((float)(dwcTime.at(0) - dwcTime.at(1))*25./1000. * dwc1horizontalSlope + dwc1horizontalOffset) + dwc1Xoffset );
  dec_pos.push_back( (float)(dwcTime.at(2) - dwcTime.at(3))*25./1000. * dwc1VerticalSlope + dwc1VerticalOffset + dwc1Yoffset );
  dec_pos.push_back( -((float)(dwcTime.at(4) - dwcTime.at(5))*25./1000. * dwc2horizontalSlope + dwc2horizontalOffset) + dwc2Xoffset );
  dec_pos.push_back( (float)(dwcTime.at(6) - dwcTime.at(7))*25./1000. * dwc2VerticalSlope + dwc2VerticalOffset + dwc2Yoffset );

  return dec_pos;
}

std::vector<float> getPositionFromDWC(std::vector<float> dwcPos) {

  std::vector<float> centerLen;

  centerLen.push_back(std::sqrt( dwcPos.at(0)*dwcPos.at(0) + dwcPos.at(1)*dwcPos.at(1) ));
  centerLen.push_back(std::sqrt( dwcPos.at(2)*dwcPos.at(2) + dwcPos.at(3)*dwcPos.at(3) ));

  return centerLen;
}

float getLengthDiff(std::vector<float> dwcPos) {
  return std::sqrt( (dwcPos.at(0) - dwcPos.at(2))*(dwcPos.at(0) - dwcPos.at(2)) + (dwcPos.at(1) - dwcPos.at(3))*(dwcPos.at(1) - dwcPos.at(3)) );
}

bool inVetoWithAlign(std::vector<float> centerPos) {
  return (centerPos.at(0) <= 10) && (centerPos.at(1) <= 10);
}

bool inDWCaxisAlign(std::vector<float> dwcPos, double thres) {
  return ( std::abs(dwcPos.at(0) - dwcPos.at(2)) < thres && std::abs(dwcPos.at(1) - dwcPos.at(3)) < thres );
}


int getPID(double PS, double MU) {
  float muMean = 495.9;
  float muSigma = 444.6;
  float psMean = 8208;
  float psSigma = 1183;

  if ( MU < muMean + muSigma ) {
    if ( PS < psMean * 2.5 ) {
      return 211;
    } else {
      return 11;
    }
  } else {
    return 13;
  }

  return -1;
}

bool psCut(double PS, double thres) {
  if ( PS > 8208. *  thres )
    return true;

  return false;
}

bool muCut(double MU) {
  if ( MU < 495.9 + 444.6 )
    return true;

  return false;
}

std::vector<float> getPeakRegion(std::vector<float> wave, int begin, int end) {
  int peakIdx = std::max_element(wave.begin() + begin, wave.end() - end) - wave.begin();
  
  // std::cout << peakIdx - 30 << " " << peakIdx << " " << peakIdx + 120 << std::endl;
  
  if ( peakIdx < begin + 30 )
  peakIdx = begin + 30;

  if ( peakIdx > 880 )
    peakIdx =  880;

  std::vector<float> wave_refine;
  for ( int i = peakIdx - 60; i < peakIdx - 60 + 150; i++ )
    wave_refine.push_back(wave.at(i));

  return wave_refine;
}

std::vector<float> getLeadEdgeRegion(std::vector<float> wave, int begin, int end) {
  int peakIdx = std::max_element(wave.begin() + begin, wave.end() - end) - wave.begin();
  float thres = *std::max_element(wave.begin() + begin, wave.end() - end) * 0.1;

  int leIdx = peakIdx;
  for ( int i = peakIdx; i >= begin + 1; i-- ) {
    if ( thres > wave.at(i) ) {
      leIdx = i;
      break;
    }
  }

  if ( leIdx < 31 )
    leIdx = 31;

  if ( leIdx > 850 )
    leIdx = 850;

  // std::cout << leIdx - 30 << " " << leIdx << " " << leIdx + 120 << std::endl;
  std::vector<float> wave_refine;
  for ( int i = leIdx - 30; i < leIdx - 30 + 150; i++ )
    wave_refine.push_back(wave.at(i));

  return wave_refine;
}

float getDWCtimeWavePeak(std::vector<short> wave) {

  int peakidx = std::min_element(wave.begin()+10, wave.end()-23) - wave.begin();

  return getTime((float)peakidx);
}




