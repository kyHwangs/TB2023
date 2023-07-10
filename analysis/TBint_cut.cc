#include "TLegend.h"
#include "TLatex.h"
#include "TColor.h"

#include "drawFunction.h"

#include "TBread.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    gStyle->SetPalette(kVisibleSpectrum);

    std::string runNum = argv[1];
    int max_evt        = std::stoi(argv[2]);
    int start_bin      = std::stoi(argv[3]);
    int end_bin        = std::stoi(argv[4]);
    float c1_cut       = std::stof(argv[5]);
    float c2_cut       = std::stof(argv[6]);
    float t3_cut       = std::stof(argv[7]);

    int logic          = std::stoi(argv[8]);

    std::vector<std::string> channel_names;
    myColorPalette.clear();
    for(int plot_args = 9; plot_args < argc; plot_args++ ) {
        channel_names.push_back(argv[plot_args]);
        myColorPalette.push_back(gStyle->GetColorPalette( (plot_args-9) * ((float)gStyle->GetNumberOfColors() / ((float)argc - 9.)) ));
    }
    gStyle->SetOptStat(1);
    gStyle->SetStatFormat("6.6g");

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

    std::map<int, int> keyMap;

    for(int idx = 0; idx < channel_names.size(); idx++) {
        std::vector<int> MIDandCh = map.at(channel_names.at(idx));
        MIDs.push_back(MIDandCh.at(0));
        Chs.push_back(MIDandCh.at(1));

        if (keyMap.find(MIDandCh.at(0)) == keyMap.end()){
            keyMap.insert(std::make_pair(MIDandCh.at(0), 1));
            unique_MIDs.push_back(MIDandCh.at(0));
        }

        plots.push_back(new TH1F( (TString)channel_names.at(idx), (TString)channel_names.at(idx), 500, -5000., 200000.) );
    }

    unique_MIDs.push_back(12);


    // unique_MIDs.erase( std::unique( unique_MIDs.begin(), unique_MIDs.end() ), unique_MIDs.end() );
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), max_evt, -1, "/Users/yhep/scratch/YUdaq", unique_MIDs);
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TCanvas* c = new TCanvas("c", "c", 800, 600);
    c->cd();

    TLatex* text = new TLatex();
    text->SetTextSize(0.025);
    float height = 0.87;

    TLegend* leg = new TLegend(0.75, 0.2, 0.9, 0.4);

    for(int iEvt = 0; iEvt < readerWave.GetMaxEvent(); iEvt++) {
        printProgress(iEvt, readerWave.GetMaxEvent());

        auto anEvent = readerWave.GetAnEvent();

        auto c1_waveform = anEvent.GetData(TBcid(12, 8)).waveform();
        auto c2_waveform = anEvent.GetData(TBcid(12, 16)).waveform();
        auto t3_waveform = anEvent.GetData(TBcid(8, 19)).waveform();

        float c1_peakADC = GetPeak(c1_waveform, 350, 700);
        float c2_peakADC = GetPeak(c2_waveform, 200, 500);
        float t3_peakADC = GetPeak(t3_waveform, 300, 500);

        // if (!( (c1_peakADC < c1_cut) && (c2_peakADC < c2_cut) ) ) continue; // Logic : 00
        // if (!( (c1_peakADC < c1_cut) && (c2_peakADC < c2_cut) ) ) continue; // Logic : 10

        if (logic == 11) {
            if (!( (c1_peakADC > c1_cut) && (c2_peakADC > c2_cut) ) ) continue;
        }
        if (logic == 10) {
            if (!( (c1_peakADC > c1_cut) && (c2_peakADC < c2_cut) ) ) continue;
        }
        if (logic == 1) {
            if (!( (c1_peakADC < c1_cut) && (c2_peakADC > c2_cut) ) ) continue;
        }
        if (logic == 0) {
            if (!( (c1_peakADC < c1_cut) && (c2_peakADC < c2_cut) ) ) continue;
        }
        if ( t3_peakADC < t3_cut ) continue;

        for (int idx = 0; idx < plots.size(); idx++) {
            TBcid cid = TBcid(MIDs.at(idx), Chs.at(idx));
            auto single_waveform = anEvent.GetData(cid).waveform();

            float IntADC = GetInt(single_waveform, start_bin, end_bin);
            plots.at(idx)->Fill(IntADC);
        }
    }
    
    int maxEntry_idx = -1;
    int tmpMaxEntry = 0;
    for(int idx = 0; idx < plots.size(); idx++) {
        int currentEntry = plots.at(idx)->GetMaximum();
        if (tmpMaxEntry <= currentEntry) {
            tmpMaxEntry = currentEntry;
            maxEntry_idx = idx;
        }
    }

    plots.at(maxEntry_idx)->SetTitle((TString)("Run" + runNum + "_cut"));
    plots.at(maxEntry_idx)->GetXaxis()->SetTitle("IntADC");
    plots.at(maxEntry_idx)->GetYaxis()->SetTitle("Evt");
    plots.at(maxEntry_idx)->SetLineWidth(2);
    plots.at(maxEntry_idx)->SetLineColor(myColorPalette.at(maxEntry_idx));

    c->cd();
    plots.at(maxEntry_idx)->Draw("Hist");
    leg->AddEntry(plots.at(maxEntry_idx), channel_names.at(maxEntry_idx).c_str(), "l");
    int meanOp = (int)(plots.at(maxEntry_idx)->GetMean());
    int meanBp = (int)(plots.at(maxEntry_idx)->GetMean() * 100) - meanOp * 100;
    int stdOp  = (int)(plots.at(maxEntry_idx)->GetStdDev());
    int stdBp  = (int)(plots.at(maxEntry_idx)->GetStdDev() * 100) - stdOp * 100;
    int entries = plots.at(maxEntry_idx)->GetEntries();
    text->DrawLatexNDC( 0.5 , height ,(TString)("#color[" + std::to_string( myColorPalette.at(maxEntry_idx) ) + "]{" + channel_names.at(maxEntry_idx) + " Ent : " + entries +  " / M : " + std::to_string(meanOp) + "." + std::to_string(meanBp) + " / Std : " + std::to_string(stdOp) + "." + std::to_string(stdBp) + "}" ));
    c->Update();

    height -= 0.03;

    for(int idx = 0 ; idx < plots.size(); idx++) {
        if (idx == maxEntry_idx) continue;
        plots.at(idx)->SetTitle( "" );
        plots.at(idx)->GetXaxis()->SetTitle("IntADC");
        plots.at(idx)->GetYaxis()->SetTitle("Evt");
        plots.at(idx)->SetLineWidth(2);
        plots.at(idx)->SetLineColor(myColorPalette.at(idx));

        c->cd();
        plots.at(idx)->Draw("Hist & sames");

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
    c->Draw();

    //TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
    //rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();

    return 0;
}
