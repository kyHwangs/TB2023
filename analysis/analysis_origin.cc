#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLatex.h>
#include <TList.h>
#include <TCanvas.h>

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "ROOT/RDataFrame.hxx"

using cRVecF = const ROOT::RVecF &;

int main(int argc, char** argv) {
	std::cout << " doing " << argv[1] << " ... ";

	ROOT::EnableImplicitMT();

	std::string runNum = argv[1];
	std::string filePath = "./TreeRun" + runNum + "_RunInfo.root";
	std::string outoutPath = "./RDF_Run" + runNum + ".root";

	std::map<int, std::vector<float>> dwcOffset = {
		std::make_pair(624, (std::vector<float>){-3.340, 3.069046, -5.727, 10.215829}),
		std::make_pair(676, (std::vector<float>){-4.163, 2.920384, -6.730, 10.598635}),
		std::make_pair(700, (std::vector<float>){-2.964, 3.273742, -5.377, 10.665751}),
		std::make_pair(702, (std::vector<float>){-3.005, 3.316395, -5.483, 10.671460}),
		std::make_pair(704, (std::vector<float>){-2.946, 3.309556, -5.357, 10.651037})
	};

	std::vector<float> scintEq = {
		0.00123373018320893, // GeV / ADC
		0.00127950866867123, // p.e. / GeV
		0.00152045005321575,
		0.00123426314490249
	};

	std::vector<float> cerenEq = {
		0.002039567611666,
		0.002157962883038,
		0.002542588354945,
		0.002067183462532
	};

	if ( dwcOffset.find(std::stoi(runNum)) == dwcOffset.end() )
		return -1;

	auto dwcOffsetRun = dwcOffset.at(std::stoi(runNum));

	auto dwcOffsetNormX1 = [&dwcOffsetRun](float pos) { return pos - dwcOffsetRun.at(0); };
	auto dwcOffsetNormY1 = [&dwcOffsetRun](float pos) { return pos - dwcOffsetRun.at(1); };
	auto dwcOffsetNormX2 = [&dwcOffsetRun](float pos) { return pos - dwcOffsetRun.at(2); };
	auto dwcOffsetNormY2 = [&dwcOffsetRun](float pos) { return pos - dwcOffsetRun.at(3); };

	auto m1t1EqS = [&scintEq](float adc) { return adc * scintEq.at(0); };
	auto m1t2EqS = [&scintEq](float adc) { return adc * scintEq.at(1); };
	auto m1t3EqS = [&scintEq](float adc) { return adc * scintEq.at(2); };
	auto m1t4EqS = [&scintEq](float adc) { return adc * scintEq.at(3); };

	auto m1t1EqC = [&cerenEq](float adc) { return adc * cerenEq.at(0); };
	auto m1t2EqC = [&cerenEq](float adc) { return adc * cerenEq.at(1); };
	auto m1t3EqC = [&cerenEq](float adc) { return adc * cerenEq.at(2); };
	auto m1t4EqC = [&cerenEq](float adc) { return adc * cerenEq.at(3); };

	auto totalE = [](float m1, float m2, float m3, float m4) { return m1 + m2 + m3 + m4; };

	ROOT::RDataFrame df("data", filePath);

	auto df_dwcNormed = df.Define("dwc_1_X_norm", dwcOffsetNormX1, {"dwc_1_x"})
								 .Define("dwc_1_Y_norm", dwcOffsetNormY1, {"dwc_1_y"})
								 .Define("dwc_2_X_norm", dwcOffsetNormX2, {"dwc_2_x"})
								 .Define("dwc_2_Y_norm", dwcOffsetNormY2, {"dwc_2_y"})
								 .Define("dwc_X_diff", {"dwc_1_X_norm - dwc_2_X_norm"})
								 .Define("dwc_Y_diff", {"dwc_1_Y_norm - dwc_2_Y_norm"})
								 .Define("dwc_X_diff_origin", {"dwc_1_x - dwc_2_x"})
								 .Define("dwc_Y_diff_origin", {"dwc_1_y - dwc_2_y"});

	auto df_dwcCut = df_dwcNormed.Filter("dwc_X_diff < 1.5 && dwc_X_diff > -1.5", "x position cut at 1.5mm")
															 .Filter("dwc_Y_diff < 1.5 && dwc_Y_diff > -1.5", "y position cut at 1.5mm");

	auto df_muCut = df_dwcCut.Filter("muADC < 2000.", "Muon Counter Cut at 2000 ADC");

	auto df_psCut = df_muCut.Filter("psADC > 34000.", "Muon Counter Cut at 2000 ADC");

	auto df_psCutHard = df_muCut.Filter("psADC > 60000.", "Muon Counter Cut at 2000 ADC");

	auto df_eq = df_psCut.Define("m1t1C_Eq", m1t1EqC, {"m1t1c"})
											 .Define("m1t2C_Eq", m1t2EqC, {"m1t2c"})
											 .Define("m1t3C_Eq", m1t3EqC, {"m1t3c"})
											 .Define("m1t4C_Eq", m1t4EqC, {"m1t4c"})

											 .Define("m1t1S_Eq", m1t1EqS, {"m1t1s"})
											 .Define("m1t2S_Eq", m1t2EqS, {"m1t2s"})
											 .Define("m1t3S_Eq", m1t3EqS, {"m1t3s"})
											 .Define("m1t4S_Eq", m1t4EqS, {"m1t4s"})

											 .Define("m1C_Eq", totalE, {"m1t1C_Eq", "m1t2C_Eq", "m1t3C_Eq", "m1t4C_Eq"})
											 .Define("m1S_Eq", totalE, {"m1t1S_Eq", "m1t2S_Eq", "m1t3S_Eq", "m1t4S_Eq"});

	auto df_eqHardPS = df_psCutHard.Define("m1t1C_Eq", m1t1EqC, {"m1t1c"})
																 .Define("m1t2C_Eq", m1t2EqC, {"m1t2c"})
																 .Define("m1t3C_Eq", m1t3EqC, {"m1t3c"})
																 .Define("m1t4C_Eq", m1t4EqC, {"m1t4c"})

																 .Define("m1t1S_Eq", m1t1EqS, {"m1t1s"})
																 .Define("m1t2S_Eq", m1t2EqS, {"m1t2s"})
																 .Define("m1t3S_Eq", m1t3EqS, {"m1t3s"})
																 .Define("m1t4S_Eq", m1t4EqS, {"m1t4s"})

																 .Define("m1C_Eq", totalE, {"m1t1C_Eq", "m1t2C_Eq", "m1t3C_Eq", "m1t4C_Eq"})
																 .Define("m1S_Eq", totalE, {"m1t1S_Eq", "m1t2S_Eq", "m1t3S_Eq", "m1t4S_Eq"});

	auto h_dwc_1_X = df_dwcNormed.Histo1D({"DWC 1 X", "DWC1 X pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_1_X_norm");
	auto h_dwc_1_Y = df_dwcNormed.Histo1D({"DWC 1 Y", "DWC1 Y pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_1_Y_norm");
	auto h_dwc_2_X = df_dwcNormed.Histo1D({"DWC 2 X", "DWC2 X pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_2_X_norm");
	auto h_dwc_2_Y = df_dwcNormed.Histo1D({"DWC 2 Y", "DWC2 Y pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_2_Y_norm");

	auto h_dwc_1_X_ori = df_dwcNormed.Histo1D({"DWC 1 X origin", "DWC1 X pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_1_x");
	auto h_dwc_1_Y_ori = df_dwcNormed.Histo1D({"DWC 1 Y origin", "DWC1 Y pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_1_y");
	auto h_dwc_2_X_ori = df_dwcNormed.Histo1D({"DWC 2 X origin", "DWC2 X pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_2_x");
	auto h_dwc_2_Y_ori = df_dwcNormed.Histo1D({"DWC 2 Y origin", "DWC2 Y pos.;mm;N_{Events}", 720, -120., 120.}, "dwc_2_y");

	auto h_dwc_X_diff = df_dwcNormed.Histo1D({"DWC X diff. norm", "X postion diff.;mm;N_{Events}", 720, -120., 120.}, "dwc_X_diff");
	auto h_dwc_Y_diff = df_dwcNormed.Histo1D({"DWC Y diff. norm", "Y postion diff.;mm;N_{Events}", 720, -120., 120.}, "dwc_Y_diff");

	auto h_dwc_X_diff_ori = df_dwcNormed.Histo1D({"DWC X diff.", "X postion diff.;mm;N_{Events}", 720, -120., 120.}, "dwc_X_diff_origin");
	auto h_dwc_Y_diff_ori = df_dwcNormed.Histo1D({"DWC Y diff.", "Y postion diff.;mm;N_{Events}", 720, -120., 120.}, "dwc_Y_diff_origin");

	auto h_psADC = df_dwcNormed.Histo1D({"psADC", "PS int. ADC;int. ADC;N_{Events}", 1200, -6000., 300000.}, "psADC");
	auto h_muADC = df_dwcNormed.Histo1D({"muADC", "MU int. ADC;int. ADC;N_{Events}", 1500, -20000., 40000.}, "muADC");

	auto h_dwc_1_2D = df_dwcNormed.Histo2D({"DWC 1 2D.", "DWC 1 position;DWC1 X [mm];DWC1 Y [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_1_x", "dwc_1_y");
	auto h_dwc_2_2D = df_dwcNormed.Histo2D({"DWC 2 2D.", "DWC 2 position;DWC2 X [mm];DWC2 Y [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_2_x", "dwc_2_y");

	auto h_dwc_X_2D = df_dwcNormed.Histo2D({"DWC X 2D.", "DWC X position;DWC1 X [mm];DWC2 X [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_1_x", "dwc_2_x");
	auto h_dwc_Y_2D = df_dwcNormed.Histo2D({"DWC Y 2D.", "DWC Y position;DWC1 Y [mm];DWC2 Y [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_1_y", "dwc_2_y");

	auto h_C_m1t1 = df_dwcNormed.Histo1D({"M1T1 Cerenkov", "M1T1 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1c");
	auto h_C_m1t2 = df_dwcNormed.Histo1D({"M1T2 Cerenkov", "M1T2 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2c");
	auto h_C_m1t3 = df_dwcNormed.Histo1D({"M1T3 Cerenkov", "M1T3 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3c");
	auto h_C_m1t4 = df_dwcNormed.Histo1D({"M1T4 Cerenkov", "M1T4 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4c");

	auto h_S_m1t1 = df_dwcNormed.Histo1D({"M1T1 Scintillation", "M1T1 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1s");
	auto h_S_m1t2 = df_dwcNormed.Histo1D({"M1T2 Scintillation", "M1T2 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2s");
	auto h_S_m1t3 = df_dwcNormed.Histo1D({"M1T3 Scintillation", "M1T3 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3s");
	auto h_S_m1t4 = df_dwcNormed.Histo1D({"M1T4 Scintillation", "M1T4 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4s");

	auto h_dwc_1_2D_dwcCut = df_dwcCut.Histo2D({"DWC 1 2D. (DWC cut)", "DWC 1 position;DWC1 X [mm];DWC1 Y [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_1_x", "dwc_1_y");
	auto h_dwc_2_2D_dwcCut = df_dwcCut.Histo2D({"DWC 2 2D. (DWC cut)", "DWC 2 position;DWC2 X [mm];DWC2 Y [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_2_x", "dwc_2_y");

	auto h_dwc_X_2D_dwcCut = df_dwcCut.Histo2D({"DWC X 2D. (DWC cut)", "DWC X position;DWC1 X [mm];DWC2 X [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_1_x", "dwc_2_x");
	auto h_dwc_Y_2D_dwcCut = df_dwcCut.Histo2D({"DWC Y 2D. (DWC cut)", "DWC Y position;DWC1 Y [mm];DWC2 Y [mm]", 720, -120., 120., 720, -120., 120.}, "dwc_1_y", "dwc_2_y");

	auto h_C_m1t1_dwcCut = df_dwcCut.Histo1D({"M1T1 Cerenkov (DWC cut)", "M1T1 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1c");
	auto h_C_m1t2_dwcCut = df_dwcCut.Histo1D({"M1T2 Cerenkov (DWC cut)", "M1T2 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2c");
	auto h_C_m1t3_dwcCut = df_dwcCut.Histo1D({"M1T3 Cerenkov (DWC cut)", "M1T3 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3c");
	auto h_C_m1t4_dwcCut = df_dwcCut.Histo1D({"M1T4 Cerenkov (DWC cut)", "M1T4 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4c");

	auto h_S_m1t1_dwcCut = df_dwcCut.Histo1D({"M1T1 Scintillation (DWC cut)", "M1T1 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1s");
	auto h_S_m1t2_dwcCut = df_dwcCut.Histo1D({"M1T2 Scintillation (DWC cut)", "M1T2 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2s");
	auto h_S_m1t3_dwcCut = df_dwcCut.Histo1D({"M1T3 Scintillation (DWC cut)", "M1T3 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3s");
	auto h_S_m1t4_dwcCut = df_dwcCut.Histo1D({"M1T4 Scintillation (DWC cut)", "M1T4 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4s");

	auto h_psADC_dwcCut = df_dwcCut.Histo1D({"psADC (DWC cut)", "PS int. ADC;int. ADC;N_{Events}", 1200, -6000., 300000.}, "psADC");
	auto h_muADC_dwcCut = df_dwcCut.Histo1D({"muADC (DWC cut)", "MU int. ADC;int. ADC;N_{Events}", 1500, -20000., 40000.}, "muADC");

	auto h_C_m1t1_mcCut = df_muCut.Histo1D({"M1T1 Cerenkov (MC cut)", "M1T1 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1c");
	auto h_C_m1t2_mcCut = df_muCut.Histo1D({"M1T2 Cerenkov (MC cut)", "M1T2 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2c");
	auto h_C_m1t3_mcCut = df_muCut.Histo1D({"M1T3 Cerenkov (MC cut)", "M1T3 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3c");
	auto h_C_m1t4_mcCut = df_muCut.Histo1D({"M1T4 Cerenkov (MC cut)", "M1T4 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4c");

	auto h_S_m1t1_mcCut = df_muCut.Histo1D({"M1T1 Scintillation (MC cut)", "M1T1 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1s");
	auto h_S_m1t2_mcCut = df_muCut.Histo1D({"M1T2 Scintillation (MC cut)", "M1T2 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2s");
	auto h_S_m1t3_mcCut = df_muCut.Histo1D({"M1T3 Scintillation (MC cut)", "M1T3 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3s");
	auto h_S_m1t4_mcCut = df_muCut.Histo1D({"M1T4 Scintillation (MC cut)", "M1T4 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4s");

	auto h_C_m1t1_psCut = df_psCut.Histo1D({"M1T1 Cerenkov (PS cut at 34000)", "M1T1 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1c");
	auto h_C_m1t2_psCut = df_psCut.Histo1D({"M1T2 Cerenkov (PS cut at 34000)", "M1T2 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2c");
	auto h_C_m1t3_psCut = df_psCut.Histo1D({"M1T3 Cerenkov (PS cut at 34000)", "M1T3 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3c");
	auto h_C_m1t4_psCut = df_psCut.Histo1D({"M1T4 Cerenkov (PS cut at 34000)", "M1T4 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4c");

	auto h_S_m1t1_psCut = df_psCut.Histo1D({"M1T1 Scintillation (PS cut at 34000)", "M1T1 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1s");
	auto h_S_m1t2_psCut = df_psCut.Histo1D({"M1T2 Scintillation (PS cut at 34000)", "M1T2 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2s");
	auto h_S_m1t3_psCut = df_psCut.Histo1D({"M1T3 Scintillation (PS cut at 34000)", "M1T3 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3s");
	auto h_S_m1t4_psCut = df_psCut.Histo1D({"M1T4 Scintillation (PS cut at 34000)", "M1T4 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4s");

	auto h_C_m1t1_psHardCut = df_psCutHard.Histo1D({"M1T1 Cerenkov (PS cut at 60000)", "M1T1 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1c");
	auto h_C_m1t2_psHardCut = df_psCutHard.Histo1D({"M1T2 Cerenkov (PS cut at 60000)", "M1T2 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2c");
	auto h_C_m1t3_psHardCut = df_psCutHard.Histo1D({"M1T3 Cerenkov (PS cut at 60000)", "M1T3 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3c");
	auto h_C_m1t4_psHardCut = df_psCutHard.Histo1D({"M1T4 Cerenkov (PS cut at 60000)", "M1T4 Cerenkov;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4c");

	auto h_S_m1t1_psHardCut = df_psCutHard.Histo1D({"M1T1 Scintillation (PS cut at 60000)", "M1T1 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t1s");
	auto h_S_m1t2_psHardCut = df_psCutHard.Histo1D({"M1T2 Scintillation (PS cut at 60000)", "M1T2 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t2s");
	auto h_S_m1t3_psHardCut = df_psCutHard.Histo1D({"M1T3 Scintillation (PS cut at 60000)", "M1T3 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t3s");
	auto h_S_m1t4_psHardCut = df_psCutHard.Histo1D({"M1T4 Scintillation (PS cut at 60000)", "M1T4 Scintillation;Int. ADC;N_{Events}", 1100, -5000., 50000.}, "m1t4s");

	auto h_m1t1_C_eq = df_eq.Histo1D({"M1T1 Cerenkov (Eq const)", "M1T1 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t1C_Eq");
	auto h_m1t2_C_eq = df_eq.Histo1D({"M1T2 Cerenkov (Eq const)", "M1T2 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t2C_Eq");
	auto h_m1t3_C_eq = df_eq.Histo1D({"M1T3 Cerenkov (Eq const)", "M1T3 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t3C_Eq");
	auto h_m1t4_C_eq = df_eq.Histo1D({"M1T4 Cerenkov (Eq const)", "M1T4 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t4C_Eq");

	auto h_m1t1_S_eq = df_eq.Histo1D({"M1T1 Scintillation (Eq const)", "M1T1 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t1S_Eq");
	auto h_m1t2_S_eq = df_eq.Histo1D({"M1T2 Scintillation (Eq const)", "M1T2 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t2S_Eq");
	auto h_m1t3_S_eq = df_eq.Histo1D({"M1T3 Scintillation (Eq const)", "M1T3 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t3S_Eq");
	auto h_m1t4_S_eq = df_eq.Histo1D({"M1T4 Scintillation (Eq const)", "M1T4 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t4S_Eq");

	auto h_m1C_eq = df_eq.Histo1D({"M1 Cerenkov (Eq const)", "M1 Cerenkov;E [GeV];N_{Events}", 2000, -10., 90.}, "m1C_Eq");
	auto h_m1S_eq = df_eq.Histo1D({"M1 Scintillation (Eq const)", "M1 Scintillation;E [GeV];N_{Events}", 2000, -10., 90.}, "m1S_Eq");

	auto h_m1t1_C_eqHardCut = df_eqHardPS.Histo1D({"M1T1 Cerenkov (Eq const PS cut at 60000)", "M1T1 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t1C_Eq");
	auto h_m1t2_C_eqHardCut = df_eqHardPS.Histo1D({"M1T2 Cerenkov (Eq const PS cut at 60000)", "M1T2 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t2C_Eq");
	auto h_m1t3_C_eqHardCut = df_eqHardPS.Histo1D({"M1T3 Cerenkov (Eq const PS cut at 60000)", "M1T3 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t3C_Eq");
	auto h_m1t4_C_eqHardCut = df_eqHardPS.Histo1D({"M1T4 Cerenkov (Eq const PS cut at 60000)", "M1T4 Cerenkov;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t4C_Eq");

	auto h_m1t1_S_eqHardCut = df_eqHardPS.Histo1D({"M1T1 Scintillation (Eq const PS cut at 60000)", "M1T1 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t1S_Eq");
	auto h_m1t2_S_eqHardCut = df_eqHardPS.Histo1D({"M1T2 Scintillation (Eq const PS cut at 60000)", "M1T2 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t2S_Eq");
	auto h_m1t3_S_eqHardCut = df_eqHardPS.Histo1D({"M1T3 Scintillation (Eq const PS cut at 60000)", "M1T3 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t3S_Eq");
	auto h_m1t4_S_eqHardCut = df_eqHardPS.Histo1D({"M1T4 Scintillation (Eq const PS cut at 60000)", "M1T4 Scintillation;E [GeV];N_{Events}", 1000, -10., 40.}, "m1t4S_Eq");

	auto h_m1C_eqHardCut = df_eqHardPS.Histo1D({"M1 Cerenkov (Eq const PS cut at 60000)", "M1 Cerenkov;E [GeV];N_{Events}", 2000, -10., 90.}, "m1C_Eq");
	auto h_m1S_eqHardCut = df_eqHardPS.Histo1D({"M1 Scintillation (Eq const PS cut at 60000)", "M1 Scintillation;E [GeV];N_{Events}", 2000, -10., 90.}, "m1S_Eq");


	TFile* outputFile = new TFile(outoutPath.c_str(), "RECREATE");
	outputFile->cd();
	
	h_dwc_1_X->Write();
	h_dwc_1_Y->Write();
	h_dwc_2_X->Write();
	h_dwc_2_Y->Write();
	h_dwc_1_X_ori->Write();
	h_dwc_1_Y_ori->Write();
	h_dwc_2_X_ori->Write();
	h_dwc_2_Y_ori->Write();
	h_dwc_X_diff->Write();
	h_dwc_Y_diff->Write();
	h_dwc_X_diff_ori->Write();
	h_dwc_Y_diff_ori->Write();

	h_dwc_1_2D->Write();
	h_dwc_2_2D->Write();
	h_dwc_1_2D_dwcCut->Write();
	h_dwc_2_2D_dwcCut->Write();

	h_C_m1t1->Write();
	h_C_m1t2->Write();
	h_C_m1t3->Write();
	h_C_m1t4->Write();
	h_S_m1t1->Write();
	h_S_m1t2->Write();
	h_S_m1t3->Write();
	h_S_m1t4->Write();

	h_C_m1t1_dwcCut->Write();
	h_C_m1t2_dwcCut->Write();
	h_C_m1t3_dwcCut->Write();
	h_C_m1t4_dwcCut->Write();
	h_S_m1t1_dwcCut->Write();
	h_S_m1t2_dwcCut->Write();
	h_S_m1t3_dwcCut->Write();
	h_S_m1t4_dwcCut->Write();

	h_psADC->Write();
	h_muADC->Write();

	h_psADC_dwcCut->Write();
	h_muADC_dwcCut->Write();

	h_C_m1t1_mcCut->Write();
	h_C_m1t2_mcCut->Write();
	h_C_m1t3_mcCut->Write();
	h_C_m1t4_mcCut->Write();
	h_S_m1t1_mcCut->Write();
	h_S_m1t2_mcCut->Write();
	h_S_m1t3_mcCut->Write();
	h_S_m1t4_mcCut->Write();
	h_C_m1t1_psCut->Write();
	h_C_m1t2_psCut->Write();
	h_C_m1t3_psCut->Write();
	h_C_m1t4_psCut->Write();
	h_S_m1t1_psCut->Write();
	h_S_m1t2_psCut->Write();
	h_S_m1t3_psCut->Write();
	h_S_m1t4_psCut->Write();
	h_C_m1t1_psHardCut->Write();
	h_C_m1t2_psHardCut->Write();
	h_C_m1t3_psHardCut->Write();
	h_C_m1t4_psHardCut->Write();
	h_S_m1t1_psHardCut->Write();
	h_S_m1t2_psHardCut->Write();
	h_S_m1t3_psHardCut->Write();
	h_S_m1t4_psHardCut->Write();

	h_m1t1_C_eq->Write();
	h_m1t2_C_eq->Write();
	h_m1t3_C_eq->Write();
	h_m1t4_C_eq->Write();
	h_m1t1_S_eq->Write();
	h_m1t2_S_eq->Write();
	h_m1t3_S_eq->Write();
	h_m1t4_S_eq->Write();
	h_m1C_eq->Write();
	h_m1S_eq->Write();

	h_m1t1_C_eqHardCut->Write();
	h_m1t2_C_eqHardCut->Write();
	h_m1t3_C_eqHardCut->Write();
	h_m1t4_C_eqHardCut->Write();
	h_m1t1_S_eqHardCut->Write();
	h_m1t2_S_eqHardCut->Write();
	h_m1t3_S_eqHardCut->Write();
	h_m1t4_S_eqHardCut->Write();
	h_m1C_eqHardCut->Write();
	h_m1S_eqHardCut->Write();

	h_dwc_X_2D->Write();
	h_dwc_Y_2D->Write();
	h_dwc_X_2D_dwcCut->Write();
	h_dwc_Y_2D_dwcCut->Write();

	outputFile->Close();

	std::cout << " DONE ! " << std::endl;


	return 1;
}

