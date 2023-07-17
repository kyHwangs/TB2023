#include "TH1.h"
#include "TH2.h"

#include "drawFunction.h"

#include "TBread.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    gStyle->SetOptStat(11111111);

    std::string runNum = argv[1];
    int max_evt        = std::stoi(argv[2]);

    float c1_cut       = std::stof(argv[3]);
    float c2_cut       = std::stof(argv[4]);

    int logic          = std::stoi(argv[5]);

    TBcid c1_cid = TBcid(12, 8);
    TBcid c2_cid = TBcid(12, 16);
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), max_evt, -1, "/Users/yhep/scratch/YUdaq", std::vector<int> {12});
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TH1F* c1_plot    = new TH1F("C_counter_1", "C_counter_1;PeakADC;Evt", 1024, 0., 4096.);
    TH1F* c2_plot    = new TH1F("C_counter_2", "C_counter_2;PeakADC;Evt", 1024, 0., 4096.);
    TH2F* c1_c2_plot = new TH2F("C1_C2_2D", "C1_C2_2D;C1_PeakADC;C2_PeakADC", 1024, 0., 4096., 1024, 0., 4096.);

    TH1F* c1_plot_cut    = new TH1F("C_counter_1_cut", "C_counter_1_cut;PeakADC;Evt", 1024, 0., 4096.);
    TH1F* c2_plot_cut    = new TH1F("C_counter_2_cut", "C_counter_2_cut;PeakADC;Evt", 1024, 0., 4096.);
    TH2F* c1_c2_plot_cut = new TH2F("C1_C2_2D_cut", "C1_C2_2D_cut;C1_PeakADC;C2_PeakADC", 1024, 0., 4096., 1024, 0., 4096.);


    int event_passed = 0;
    for(int iEvt = 0; iEvt < readerWave.GetMaxEvent(); iEvt++) {
        printProgress(iEvt, readerWave.GetMaxEvent());
        auto anEvent = readerWave.GetAnEvent();

        auto c1_waveform = anEvent.GetData(c1_cid).waveform();
        auto c2_waveform = anEvent.GetData(c2_cid).waveform();

        float c1_peak = GetPeak(c1_waveform, 350, 700);
        float c2_peak = GetPeak(c2_waveform, 200, 500);

        c1_plot->Fill(c1_peak);
        c2_plot->Fill(c2_peak);
        c1_c2_plot->Fill(c1_peak, c2_peak);

        switch(logic) {
            case 11 :
                if ( (c1_peak > c1_cut) && (c2_peak > c2_cut ) ) {
                    event_passed++;
                    c1_plot_cut->Fill(c1_peak);
                    c2_plot_cut->Fill(c2_peak);
                    c1_c2_plot_cut->Fill(c1_peak, c2_peak);
                }
                break;

            case 10 :
                if ( (c1_peak > c1_cut) && (c2_peak < c2_cut ) ) {
                    event_passed++;
                    c1_plot_cut->Fill(c1_peak);
                    c2_plot_cut->Fill(c2_peak);
                    c1_c2_plot_cut->Fill(c1_peak, c2_peak);
                }
                break;

            case 1 :
                if ( (c1_peak < c1_cut) && (c2_peak > c2_cut ) ) {
                    event_passed++;
                    c1_plot_cut->Fill(c1_peak);
                    c2_plot_cut->Fill(c2_peak);
                    c1_c2_plot_cut->Fill(c1_peak, c2_peak);
                }
                break;

            case 0 :
                if ( (c1_peak < c1_cut) && (c2_peak < c2_cut ) ) {
                    event_passed++;
                    c1_plot_cut->Fill(c1_peak);
                    c2_plot_cut->Fill(c2_peak);
                    c1_c2_plot_cut->Fill(c1_peak, c2_peak);
                }
                break;    
        }
    }
    std::cout << "Event passed = " << event_passed << std::endl;
    std::cout << "Eff = " << (100. * (float) event_passed / (float) readerWave.GetMaxEvent()) << std::endl;


    TCanvas* c = new TCanvas("c", "c", 800, 800);
    c->cd();
    c->Divide(1, 2);

    c->cd(1);
    c1_plot->Draw("Hist");
    c->Update();

    c->cd(2);
    c2_plot->Draw("Hist");
    c->Update();

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);
    c1->cd();
    c1->Divide(1, 2);

    c1->cd(1);
    c1_plot_cut->Draw("Hist");
    c1->Update();

    c1->cd(2);
    c2_plot_cut->Draw("Hist");
    c1->Update();

    // TCanvas* c2 = new TCanvas("c2", "c2", 800, 800);
    // c2->cd();
    // c1_c2_plot->Draw("colz");
    // c2->Update();

    // TCanvas* c3 = new TCanvas("c3", "c3", 800, 800);
    // c3->cd();
    // c1_c2_plot_cut->Draw("colz");
    // c3->Update();

    app.Run();
    return 0;
}
