#include "TLegend.h"
#include "TGraph.h"
#include "TH2.h"

#include "drawFunction.h"

#include "TBread.h"


int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    std::string runNum   = argv[1];
    int start_evt        = std::stoi(argv[2]);
    int max_evt          = std::stoi(argv[3]); 
    std::string channel_name = argv[4];
    gStyle->SetOptStat(0);
    gStyle->SetMarkerStyle(1);

    auto map = getModuleConfigMap();
    if (map.find(channel_name) == map.end()) {
        std::cout << "Plot Name error : " << channel_name << " not found" << std::endl;
        return 0;
    }

    TGraph* plot = new TGraph();
    plot->SetNameTitle( (TString)(channel_name) , (TString)(channel_name) );
    std::vector<int> MIDandCh = map.at(channel_name);
    int MID = MIDandCh.at(0);
    int Ch  = MIDandCh.at(1);
   
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), start_evt + max_evt, -1, "/Users/yhep/scratch/YUdaq", std::vector<int> {MID} );
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TCanvas* c = new TCanvas("c", "c", 800, 600);

    for(int iEvt = 0; iEvt < start_evt + max_evt; iEvt++) {
        printProgress(iEvt + 1, start_evt + max_evt);
        if (iEvt < start_evt) continue;
        if (iEvt > start_evt + max_evt) gApplication->Terminate();

        auto anEvent = readerWave.GetAnEvent();
        TBcid cid = TBcid(MID, Ch);
        auto single_waveform = anEvent.GetData(cid).waveform();

        for(int bin = 0; bin < single_waveform.size()-23; bin++) {
            plot->AddPoint(bin, single_waveform.at(bin+1));
        }
    }

    c->cd();
    plot->SetMaximum(4096.);
    plot->SetMinimum(0.);
    plot->GetXaxis()->SetRangeUser(0., 1000.);
    plot->GetXaxis()->SetTitle("bin");
    plot->GetYaxis()->SetTitle("ADC");
    plot->Draw("AP");
    c->Update();
    // c->SaveAs("test.png");

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
