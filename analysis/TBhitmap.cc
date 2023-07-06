#include "TH2.h"
#include "TH1.h"

#include "drawFunction.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);
    gStyle->SetOptStat(0);

    std::string runNum = argv[1];
    std::string mode   = "0"; // 0 : Avg time stuct, 1 : PeakADC, 2 : IntADC
    std::string mode_text = "";
    if (mode == "0") mode_text = "Avg. Time Struc";
    if (mode == "1") mode_text = "Peak ADC";
    if (mode == "2") mode_text = "Int. ADC";

    TFile* root_file = TFile::Open((TString) ("./MCPPMT/DQM_MCPPMT_Wave_" + runNum + "_" + mode + ".root") );

    TH2F* ceren = (TH2F*) root_file->Get( (TString)("Heatmap_" + mode + "_Ceren") );
    TH2F* scint = (TH2F*) root_file->Get( (TString)("Heatmap_" + mode + "_Scint") );

    TCanvas* c = new TCanvas("c", "c", 800, 800);
    c->Divide(2,1);
    c->Modified();
    c->Update();
    c->Draw();

    c->cd(1);
    ceren->SetTitle( (TString)("Ceren_" + mode_text) );
    Double_t ceren_diff = ceren->GetMaximum() - ceren->GetMinimum();
    Double_t ceren_level[] = { ceren->GetMinimum(),
                               ceren->GetMinimum() + (ceren_diff * 0.1),
                               ceren->GetMinimum() + (ceren_diff * 0.2),
                               ceren->GetMinimum() + (ceren_diff * 0.3),
                               ceren->GetMinimum() + (ceren_diff * 0.4),
                               ceren->GetMinimum() + (ceren_diff * 0.5),
                               ceren->GetMinimum() + (ceren_diff * 0.6),
                               ceren->GetMinimum() + (ceren_diff * 0.7),
                               ceren->GetMinimum() + (ceren_diff * 0.8),
                               ceren->GetMinimum() + (ceren_diff * 0.9),
                               ceren->GetMaximum()}; // Hitmap Ceren and Scint both color map displayed in same ratio
    ceren->SetContour( ( sizeof(ceren_level) / sizeof(Double_t) ), ceren_level);
    ceren->Draw("colz & text");
    c->Modified();
    c->Update();

    c->cd(2);
    scint->SetTitle( (TString)("Scint_" + mode_text) );
    Double_t scint_diff = scint->GetMaximum() - scint->GetMinimum();
    Double_t scint_level[] = { scint->GetMinimum(),
                               scint->GetMinimum() + (scint_diff * 0.1),
                               scint->GetMinimum() + (scint_diff * 0.2),
                               scint->GetMinimum() + (scint_diff * 0.3),
                               scint->GetMinimum() + (scint_diff * 0.4),
                               scint->GetMinimum() + (scint_diff * 0.5),
                               scint->GetMinimum() + (scint_diff * 0.6),
                               scint->GetMinimum() + (scint_diff * 0.7),
                               scint->GetMinimum() + (scint_diff * 0.8),
                               scint->GetMinimum() + (scint_diff * 0.9),
                               scint->GetMaximum()};
    scint->SetContour( ( sizeof(scint_level) / sizeof(Double_t) ), scint_level);
    scint->Draw("colz & text");
    c->Modified();
    c->Update();

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
