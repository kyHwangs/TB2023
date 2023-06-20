#include <string>
#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TPad.h"
#include "TString.h"
#include "TFile.h"
#include "TApplication.h"
#include "TRootCanvas.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    std::string runNum = argv[1];
    TFile* hist_file = TFile::Open( (("DQM_Run") + runNum + ".root").c_str() );

    TList* hist_list = hist_file->GetListOfKeys();
    std::vector<std::string> hist_name_list;
    for(int idx = 0; idx < hist_list->GetEntries(); idx++) {
        hist_name_list.emplace_back( (std::string) hist_list->At(idx)->GetName() );
        std::cout << hist_name_list.at(idx) << std::endl;
    }

    TCanvas* c = new TCanvas("c", "c", 1200, 800);
    c->Divide(hist_name_list.size() - hist_name_list.size()/2, hist_name_list.size()/2);
    c->Update();
    c->Draw();
    int pad_idx = 1;
    for (std::string hist_name : hist_name_list) {
        c->cd(pad_idx);
        std::string plot_type = hist_name.substr(0, hist_name.find("_"));
        std::string MIDandCh  = hist_name.substr(hist_name.find("_")+1);
        std::string MID = MIDandCh.substr( MIDandCh.find("Mid") + 3, MIDandCh.find("Ch") - MIDandCh.find("Mid") - 3 );
        std::string Ch  = MIDandCh.substr( MIDandCh.find("Ch") + 2);
        std::cout << "Drawing plot : " << plot_type << " with MID : " << MID << " Ch : " << Ch << std::endl;
        
        TH1F* hist = (TH1F*) hist_file->Get(hist_name.c_str());
        hist->SetTitle(plot_type.c_str());
        if (plot_type == "AvgTimeStruc") hist->SetStats(0);
        hist->Draw("Hist");
        c->Modified();
        c->Update();
        pad_idx++;
    }

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();
    
    hist_file->Close();
    return 0;
}