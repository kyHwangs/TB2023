#include "TLegend.h"
#include "TLatex.h"
#include "TColor.h"

#include "drawFunction.h"

#include "TBread.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    gStyle->SetPalette(kVisibleSpectrum);

    std::string runNum = argv[1];
    int start_evt      = std::stoi(argv[2]);
    int max_evt        = std::stoi(argv[3]);
    int start_bin      = std::stoi(argv[4]);
    int end_bin        = std::stoi(argv[5]);

    std::vector<std::string> channel_names;
    myColorPalette.clear();
    for(int plot_args = 6; plot_args < argc; plot_args++ ) {
        channel_names.push_back(argv[plot_args]);
        myColorPalette.push_back(gStyle->GetColorPalette( (plot_args-6) * ((float)gStyle->GetNumberOfColors() / ((float)argc - 6.)) ));
    }
    gStyle->SetOptStat(0);

    auto map = getModuleConfigMap();
    for(int idx = 0; idx < channel_names.size(); idx++) {
        if (map.find(channel_names.at(idx)) == map.end()) {
            std::cout << "Plot Name error : " << channel_names.at(idx) << " not found" << std::endl;
            return 0;
        }
    }

    std::vector<int> MIDs;
    std::vector<int> unique_MIDs;
    std::vector<int> Chs;
    std::vector<TH1F*> plots;
    for(int idx = 0; idx < channel_names.size(); idx++) {
        std::vector<int> MIDandCh = map.at(channel_names.at(idx));
        MIDs.push_back(MIDandCh.at(0));
        unique_MIDs.push_back(MIDandCh.at(0));
        Chs.push_back(MIDandCh.at(1));
        plots.push_back(new TH1F( (TString)channel_names.at(idx), (TString)channel_names.at(idx), 1024, 0., 4096.) );
    }
    unique_MIDs.erase( std::unique( unique_MIDs.begin(), unique_MIDs.end() ), unique_MIDs.end() );
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), start_evt + max_evt, -1, "/Users/yhep/scratch/YUdaq", unique_MIDs);
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TCanvas* c = new TCanvas("c", "c", 800, 600);
    c->cd();

    TLatex* text = new TLatex();
    text->SetTextSize(0.025);
    float height = 0.87;

    TLegend* leg = new TLegend(0.75, 0.2, 0.9, 0.4);

    for(int idx = 0 ; idx < plots.size(); idx++) {
        plots.at(idx)->SetTitle( "" );
        plots.at(idx)->GetXaxis()->SetTitle("PeakADC");
        plots.at(idx)->GetYaxis()->SetTitle("Evt");
        plots.at(idx)->SetLineWidth(2);
        plots.at(idx)->SetLineColor(myColorPalette.at(idx));

        TBcid cid = TBcid(MIDs.at(idx), Chs.at(idx));

        for(int iEvt = 0; iEvt < start_evt + max_evt; iEvt++) {
            printProgress(iEvt, start_evt + max_evt);
            if (iEvt < start_evt) continue;

            auto anEvent = readerWave.GetAnEvent();
            auto single_waveform = anEvent.GetData(cid).waveform();

            float PeakADC = GetPeak(single_waveform, start_bin, end_bin);
            plots.at(idx)->Fill(PeakADC);
        }

        c->cd();

        if(idx == 0) plots.at(idx)->Draw("Hist ");
        else plots.at(idx)->Draw("Hist & sames");

        leg->AddEntry(plots.at(idx), channel_names.at(idx).c_str(), "l");

        int meanOp = (int)(plots.at(idx)->GetMean());
        int meanBp = (int)(plots.at(idx)->GetMean() * 100) - meanOp * 100;

        int stdOp  = (int)(plots.at(idx)->GetStdDev());
        int stdBp  = (int)(plots.at(idx)->GetStdDev() * 100) - stdOp * 100;

        int entries = plots.at(idx)->GetEntries();
        text->DrawLatexNDC( 0.5 , height ,(TString)("#color[" + std::to_string( myColorPalette.at(idx) ) + "]{" + channel_names.at(idx) + " Ent : " + entries +  " / M : " + std::to_string(meanOp) + "." + std::to_string(meanBp) + " / Std : " + std::to_string(stdOp) + "." + std::to_string(stdBp) + "}" ));
        c->Update();

        height -= 0.03;
    }
    leg->Draw("sames");
    c->Update();

    TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
