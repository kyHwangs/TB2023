#include "TBplotengine.h"

TBplotengine::TBplotengine(const YAML::Node fNodePlot_, int fRunNum_, TButility fUtility_, bool fUseExPed_)
		: fNodePlot(fNodePlot_), fRunNum(fRunNum_), fUtility(fUtility_), fUseExPed(fUseExPed_)
{

	init();
}

void TBplotengine::init()
{
	fUtility.loading("/Users/khwang/scratch/TB2023July/preparation/dev_230628/TB2023/mapping/mapping_TB2021July_v1.root");
	if (fUseExPed)
		fUtility.loadped("/Users/khwang/scratch/TB2023July/sample/Info/Run" + std::to_string(fRunNum) + "_pedestalHist_mean.root");

	std::cout << "starting INIT" << std::endl;

	for (const auto &aCase : fNodePlot["Cases"])
	{

		std::cout << " - Loading : " << aCase.first.as<std::string>() << std::endl;

		TBcid aCid = getCid(aCase.first.as<std::string>());
		std::vector<std::pair<PlotInfo, Plotter *>> aVec;

		for (const auto &aPlot : aCase.second)
		{
			std::cout << " --- Generating : " << aPlot.first.as<std::string>() << "...   ";

			PlotInfo which = getPlotInfo(aPlot.first.as<std::string>());
			Plotter *plot = getPlot(aCid, which, aPlot.second);

			aVec.push_back(std::make_pair(which, plot));

			std::cout << "Done!" << std::endl;
		}

		fPlotSet.insert(std::make_pair(aCid, aVec));
	}

	fMIDtoLoad.clear();
	for (auto aPlotter : fPlotSet)
	{
		fCIDtoPlot.push_back(aPlotter.first);
		int amid = aPlotter.first.mid();

		bool isMatch = false;
		for (int i = 0; i < fMIDtoLoad.size(); i++)
			if (fMIDtoLoad.at(i) == amid)
				isMatch = true;

		if (!isMatch)
			fMIDtoLoad.push_back(amid);
	}

	PrintInfo();
}

TBcid TBplotengine::getCid(std::string input)
{
	if (input.find("MID") != std::string::npos && input.find("CH") != std::string::npos)
	{
		int mid = std::stoi(input.substr(input.find("MID") + 3, input.find("CH") - input.find("MID") - 4));
		int ch = std::stoi(input.substr(input.find("CH") + 2, input.size() - input.find("CH") - 2));

		if (mid > 15 || mid < 1)
			throw std::runtime_error("TBplotengine::getCid - allowed MID {1..15}, input is : " + std::to_string(mid));

		if (ch > 32 || ch < 1)
			throw std::runtime_error("TBplotengine::getCid - allowed CH {1..32}, input is : " + std::to_string(ch));

		return TBcid(mid, ch);
	}

	if (input.find("LEGO") != std::string::npos)
	{
		int nChannel = -1;
		int isCeren = -1;
		if (input.find("L1") != std::string::npos)
			nChannel = 1;

		if (input.find("L2") != std::string::npos)
			nChannel = 2;

		if (input.find("L3") != std::string::npos)
			nChannel = 3;

		if (input.find("L4") != std::string::npos)
			nChannel = 4;

		if (input.find("Ceren") != std::string::npos)
			isCeren = 1;

		if (input.find("Scint") != std::string::npos)
			isCeren = 0;

		return fUtility.getcid(TBdetector::detid::LEGO, 11, nChannel, isCeren);
	}

	if (input.find("SFHS") != std::string::npos)
	{
		int nChannel = -1;
		int isCeren = -1;
		if (input.find("HW") != std::string::npos)
			nChannel = 0;

		if (input.find("H1") != std::string::npos)
			nChannel = 1;

		if (input.find("H2") != std::string::npos)
			nChannel = 2;

		if (input.find("H3") != std::string::npos)
			nChannel = 3;

		if (input.find("Ceren") != std::string::npos)
			isCeren = 1;

		if (input.find("Scint") != std::string::npos)
			isCeren = 0;

		return fUtility.getcid(TBdetector::detid::SFHS, 10, nChannel, isCeren);
	}

	if (input.find("MCPPMT") != std::string::npos)
	{
		int nChannel = -1;
		int isCeren = -1;
		if (input.find("mid") != std::string::npos)
			nChannel = 0;

		if (input.find("W1") != std::string::npos)
			nChannel = 1;

		if (input.find("W2") != std::string::npos)
			nChannel = 2;

		if (input.find("W3") != std::string::npos)
			nChannel = 3;

		if (input.find("Ceren") != std::string::npos)
			isCeren = 1;

		if (input.find("Scint") != std::string::npos)
			isCeren = 0;

		return fUtility.getcid(TBdetector::detid::MCPPMT_gen, 12, nChannel, isCeren);
	}

	return TBcid(1, 1);
}

TBplotengine::PlotInfo TBplotengine::getPlotInfo(std::string input)
{
	if (input == "average time structure")
		return PlotInfo::kAvgTimeStruc;

	if (input == "integrated ADC")
		return PlotInfo::kIntADC;

	if (input == "average time structure")
		return PlotInfo::kPeakADC;
}

Plotter *TBplotengine::getPlot(TBcid cid, TBplotengine::PlotInfo which, const YAML::Node node)
{
	if (which == TBplotengine::PlotInfo::kAvgTimeStruc)
	{
		TString name = "AvgTimeStruc_Mid" + std::to_string(cid.mid()) + "Ch" + std::to_string(cid.channel());
		float xScale = node["xScale"].as<float>();

		std::vector<float> input;
		input.push_back(xScale);

		Plotter *aPlotter = new cAvgTimeStruc();
		aPlotter->Set(input);
		aPlotter->SetHisto(new TH1F(name, (TString)(";bins [" + std::to_string(xScale) + "ns / bin]; ADC"), 1000, 0., 1000.));

		return aPlotter;
	}

	if (which == TBplotengine::PlotInfo::kIntADC)
	{
		TString name = "IntADC_Mid" + std::to_string(cid.mid()) + "Ch" + std::to_string(cid.channel());
		float ped = -1;
		if (fUseExPed)
			fUtility.retrievePed(cid);

		int nBin = node["nBin"].as<int>();
		float xMin = node["xMin"].as<float>();
		float xMax = node["xMax"].as<float>();
		std::vector<int> interval = node["interval"].as<std::vector<int>>();

		std::vector<float> input;
		input.push_back(ped);
		input.push_back(interval.at(0));
		input.push_back(interval.at(1));

		Plotter *aPlotter = new cIntADC();
		aPlotter->Set(input);
		aPlotter->SetHisto(new TH1F(name, (TString)(";Int. ADC; N_{events}"), nBin, xMin, xMax));

		return aPlotter;
	}

	if (which == TBplotengine::PlotInfo::kPeakADC)
	{
		TString name = "PeakADC_Mid" + std::to_string(cid.mid()) + "Ch" + std::to_string(cid.channel());
		float ped = -1;
		if (fUseExPed)
			fUtility.retrievePed(cid);

		float nBin = node["nBin"].as<int>();
		float xMin = node["xMin"].as<float>();
		float xMax = node["xMax"].as<float>();
		std::vector<int> interval = node["interval"].as<std::vector<int>>();

		std::vector<float> input;
		input.push_back(ped);
		input.push_back(interval.at(0));
		input.push_back(interval.at(1));

		Plotter *aPlotter = new cPeakADC();
		aPlotter->Set(input);
		aPlotter->SetHisto(new TH1F(name, (TString)(";Peak ADC; N_{events}"), nBin, xMin, xMax));

		return aPlotter;
	}

	return new Plotter();
}

void TBplotengine::PrintInfo()
{

	std::cout << " " << std::endl;
	std::cout << " ### Plots to draw ### " << std::endl;
	for (auto aSet : fPlotSet)
	{

		std::cout << " ------------------------------ " << std::endl;
		std::cout << " - ";
		aSet.first.print();

		for (auto aPlot : aSet.second)
		{
			std::cout << " -- | " << aPlot.first << " " << aPlot.second->GetHisto()->GetName() << std::endl;
		}
	}
	std::cout << " ============================== " << std::endl;
	std::cout << " " << std::endl;
}

void TBplotengine::Fill(TBevt<TBwaveform> anEvent)
{
	for (int i = 0; i < fCIDtoPlot.size(); i++)
		for (int j = 0; j < fPlotSet.at(fCIDtoPlot.at(i)).size(); j++)
			fPlotSet.at(fCIDtoPlot.at(i)).at(j).second->Fill(anEvent.GetData(fCIDtoPlot.at(i)).waveform());
}

void TBplotengine::SaveAs(TString output)
{

	TFile *outoutFile = new TFile(output, "RECREATE");

	outoutFile->cd();
	for (int i = 0; i < fCIDtoPlot.size(); i++)
		for (int j = 0; j < fPlotSet.at(fCIDtoPlot.at(i)).size(); j++)
			fPlotSet.at(fCIDtoPlot.at(i)).at(j).second->GetHisto()->Write();

	outoutFile->Close();
}
