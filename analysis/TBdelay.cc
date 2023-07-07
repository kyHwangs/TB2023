#include "TStyle.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TText.h"
#include "TPaveText.h"

#include "drawFunction.h"

int main(int argc, char* argv[]) {
    TApplication app("app", nullptr, nullptr);

    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);

    std::string runNum = argv[1];
    std::string delay_clock  = argv[2];

    int delay_time = std::stoi(delay_clock) * 11;
    int delay_bin  = delay_time * 5;

    std::string fileName = "./ROOT/DQM_Run" + runNum + "_AvgTimeStruc.root";
    TFile* hist_file = TFile::Open(fileName.c_str());

    std::map<std::string, std::vector<int>> mapModuleConfig = getModuleConfigMap();

    std::vector<std::string> module_name_vec = {"H1-Ceren"};
    std::vector<TH1F*> module_hists;
    for (std::string module_name : module_name_vec) {
        std::vector<int> MIDandCh = mapModuleConfig.at(module_name);
        std::string MID = std::to_string(MIDandCh.at(0));
        std::string Ch  = std::to_string(MIDandCh.at(1));
        module_hists.emplace_back( GetHist(fileName, (TString)("AvgTimeStruc_Mid" + MID + "Ch" + Ch),  true, module_name) );
    }

    std::vector<std::string> aux_name_vec = {"C1", "C2", "T1N", "T2N"};
    std::vector<TH1F*> aux_hists;
    for (std::string aux_name : aux_name_vec) {
        std::vector<int> MIDandCh = mapModuleConfig.at(aux_name);
        std::string MID = std::to_string(MIDandCh.at(0));
        std::string Ch  = std::to_string(MIDandCh.at(1));
        aux_hists.emplace_back( GetHist(fileName, (TString)("AvgTimeStruc_Mid" + MID + "Ch" + Ch),  false, aux_name) );
    }

    TLegend* leg_1 = new TLegend(0.7, 0.4, 0.89, 0.55);
    TLegend* leg_2 = new TLegend(0.7, 0.4, 0.89, 0.55);

    TCanvas *c_generic = new TCanvas("avg_time_struc", "avg_time_struc", 1000, 800);
    c_generic->cd();
    c_generic->Divide(1,2);
    c_generic->Update();
    c_generic->Draw();

    c_generic->cd(1);
    for(int hist_idx = 0; hist_idx < module_hists.size(); hist_idx++) {
        if (hist_idx == 0) {
            module_hists.at(hist_idx)->SetLineWidth(2);
            module_hists.at(hist_idx)->SetTitle("Module avg. time structure");
            module_hists.at(hist_idx)->GetXaxis()->SetTitle("[0.2 ns / bin]");
            module_hists.at(hist_idx)->Draw("Hist & PLC");
            leg_1->AddEntry( module_hists.at(hist_idx), module_name_vec.at(hist_idx).c_str(), "l");
        }
        else {
            module_hists.at(hist_idx)->SetLineWidth(2);
            module_hists.at(hist_idx)->Draw("Hist & PLC & sames");
            leg_1->AddEntry( module_hists.at(hist_idx), module_name_vec.at(hist_idx).c_str(), "l");
        }

        c_generic->Modified();
        c_generic->Update();
    }
    leg_1->SetEntrySeparation(0.1);
    leg_1->Draw();
    c_generic->cd(1)->SetBottomMargin(0.1);
    c_generic->Modified();
    c_generic->Update();

    c_generic->cd(2);
    c_generic->cd(2)->SetTopMargin(0.1);
    c_generic->Modified();
    c_generic->Update();
    for(int hist_idx = 0; hist_idx < aux_hists.size(); hist_idx++) {
        if (hist_idx == 0) {
            aux_hists.at(hist_idx)->SetLineWidth(2);
            aux_hists.at(hist_idx)->SetTitle("");
            aux_hists.at(hist_idx)->Draw("Hist & PLC & X+");

            TAxis* x_axis = aux_hists.at(hist_idx)->GetXaxis();
            x_axis->SetTitle("");
            x_axis->Set( x_axis->GetNbins(), (x_axis->GetXmin() + delay_bin), (x_axis->GetXmax() + delay_bin) );

            leg_2->AddEntry( aux_hists.at(hist_idx), aux_name_vec.at(hist_idx).c_str(), "l");
        }
        else {
            aux_hists.at(hist_idx)->SetLineWidth(2);
            aux_hists.at(hist_idx)->Draw("Hist & PLC & sames & X+");

            TAxis* x_axis = aux_hists.at(hist_idx)->GetXaxis();
            x_axis->SetTitle("");
            x_axis->Set( x_axis->GetNbins(), (x_axis->GetXmin() + delay_bin), (x_axis->GetXmax() + delay_bin) );

            leg_2->AddEntry( aux_hists.at(hist_idx), aux_name_vec.at(hist_idx).c_str(), "l");
        }
        c_generic->Modified();
        c_generic->Update();
    }

    TText* text = new TText(0.35, 0.05, (TString) ("Aux avg. time structure - delay : " + std::to_string(delay_time) + " ns"));
    text->SetNDC();
    text->SetTextFont(42);
    text->Draw("same");

    leg_2->SetEntrySeparation(0.1);
    leg_2->Draw();
    c_generic->Modified();
    c_generic->Update();

    // TRootCanvas *rc_generic = (TRootCanvas *)c_generic->GetCanvasImp();
    // rc_generic->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    
    app.Run();

    return 0;
}