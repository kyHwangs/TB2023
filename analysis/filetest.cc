#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <TH1F.h>
#include <TString.h>


int filetest() {

    TH1F* aHist = new TH1F("test", ";;", 1000, 0., 1000.);

    std::vector<short> aWave{0,1,2,3,4,5,6,7,8,9,10};

    for(int i = 0; i < aWave.size(); i++) {
        aHist->Fill(i, aWave.at(i));
    }

    for (int i = 1; i < aWave.size() +1; i++)
        std::cout << aHist->GetBinContent(i) << std::endl;


    return 1;
}
