#include <iostream>
#include <cstdio>
#include <algorithm>
#include <math.h>

#include "TBconfig.h"
#include "TBevt.h"
#include "TButility.h"
#include "TBload.h"
#include "TBhist.h"
#include "TBauxfactory.h"
#include "TBdwc.h"
#include "TBplotengine.h"

// #include "functions.cc"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>


#include "yaml-cpp/yaml.h"

int main() {

	// std::shared_ptr<TButility> utility(new TButility());
	TButility utility = TButility();

	std::cout << " Loading configs... ";
	TBconfig config = TBconfig("sample.yml", 682);
	std::cout << " Done!" << std::endl;

	std::cout << " Generating plotter... ";
	TBplotengine plotter = TBplotengine(config.GetConfig()["Plots"], utility);
	std::cout << " Done!" << std::endl;

	plotter.SaveAs("test.root");


	// const YAML::Node node = YAML::LoadFile("sample.yml");

	// const auto& aPlot = node["Plots"];
	// std::cout << typeid(aPlot).name() << std::endl;
	// std::cout << aPlot << std::endl;

	// for (const auto& aNode : node) {

	// 	// YAML::Node element = &aNode;
	// 	// std::cout << typeid(element).name() << std::endl;

	// 	std::cout << typeid(aNode).name() << std::endl;
	// 	std::cout << aNode.first.as<std::string>() << std::endl;
	// }

	// std::cout << " " << std::endl;

	// for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
  	// YAML::Node element = *it;
  	// // std::cout << it << std::endl;
  	// std::cout << typeid(it).name() << std::endl;
  	// std::cout << element << std::endl;
  	// std::cout << typeid(element).name() << std::endl;
 	// }

	// // const YAML::Node nodePlot = node["Plots"];
	// // const YAML::Node nodePlot = node["Plots"];
	// const YAML::Node nodePlot = node["test"];

	// std::cout << nodePlot << std::endl;
	// std::cout << typeid(nodePlot).name() << std::endl;
	// std::cout << typeid(node["test"]).name() << std::endl;
	// // std::cout << nodePlot.GetTag() << std::endl;

	// if ( node["test"] ) {
	// 	std::cout << "true?" << std::endl;
	// } else {
	// 	std::cout << "false?" << std::endl;
	// }

	// int i = 1;

	// for(auto& aCase : nodePlot["Cases"]) {
	// 	std::cout << " " << std::endl;
	// 	std::cout << " " << std::endl;
	// 	std::cout << " " << std::endl;
	// 	std::cout << i++ << std::endl;


	// 	std::cout << aCase << std::endl;
	// 	std::cout << aCase.first << std::endl;
	// 	std::cout << aCase.first.as<std::string>() << std::endl;

	// 	int j = 1;
	// 	for (auto& aPlot : aCase.second) {
	// 		std::cout << "  " << j++ << std::endl;
	// 		std::cout << "  " << aPlot.first << std::endl;
	// 		std::cout << " " << std::endl;
	// 		for (auto& aBin : aPlot.second) {
	// 			std::cout << "    " << aBin.first << std::endl;
	// 			std::cout << "    " << aBin.second << std::endl;
	// 		}
	// 	}
	// 	// std::cout << typeid(aNode.first).name() << std::endl;
	// 	// std::cout << aNode.first << std::endl;
	// 	// std::cout << typeid(aNode.second).name() << std::endl;
	// 	// std::cout << aNode.second << std::endl;
	// }

	// const auto& isGood = massBin.second.as<std::vector<float>>();

	// std::cout << node["mass bins"] << std::endl;
	// std::cout << typeid(node["mass bins"]).name() << std::endl;

	// std::vector<int> tmp = node["mass bins"].as<std::vector<int>>();
	// // std::cout << tmp << std::endl;

	// for(auto& a : tmp)
	// 	std::cout << a << std::endl;

	// std::cout << typeid(massBin).name() << std::endl;
	// std::cout << typeid(massBin.first).name() << std::endl;
	// std::cout << typeid(massBin.second).name() << std::endl;

	// for (const auto& aNode : node)
	// 	if (aNode.first.as<std::string>() == "mass bins") {
	// 		std::cout << typeid(aNode).name() << std::endl;
	// 		std::cout << aNode.first.as<std::string>() << std::endl;
	// 		std::cout << typeid(aNode.first).name() << std::endl;
	// 		std::cout << typeid(aNode.second).name() << std::endl;
	// 	}

	// for ( int i = 0; i < isGood.size(); i++ )
	// 	std::cout << " " << isGood.at(i) << std::endl;

	// for (const auto& keys : node) {
	// 	std::cout << keys.first.as<std::string>() << std::endl;

	// 	std::cout << keys << std::endl;
	// 	std::cout << typeid(keys).name() << std::endl;


	// 	const auto& key_second = keys.second;

	// 	for (const auto& keysOfvalues : key_second) {
	// 		std::cout << keysOfvalues.first.as<std::string>() << std::endl;
	// 	}
	// }

	// std::cout << "pt : " << muons["pt"].as<int>() << std::endl;
	// std::cout << "pt : " << muons["eta"].as<float>() << std::endl;

	// std::cout << "pt : " << muons["pt"].as<int>() << std::endl;
	// std::cout << "pt : " << muons["pt"].as<int>() << std::endl;
	// std::cout << "pt : " << muons["pt"].as<int>() << std::endl;
















	return 1;
}