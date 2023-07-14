#include "TH1.h"
#include "TH2.h"

#include "drawFunction.h"

#include "TBread.h"

int main(int argc, char* argv[]) {
    TApplication app("app", &argc, argv);

    gStyle->SetOptStat(11111111);

    std::string runNum = argv[1];
    int max_evt        = std::stoi(argv[2]);

    float t3_cut       = std::stof(argv[3]);
    float t4_cut       = std::stof(argv[4]);

    int logic          = std::stoi(argv[5]);

    TBcid t3_cid = TBcid(8, 19);
    TBcid t4_cid = TBcid(9, 19);
    TBread<TBwaveform> readerWave = TBread<TBwaveform>(std::stoi(runNum), max_evt, -1, "/Users/yhep/scratch/YUdaq", std::vector<int> {8, 9});
    std::cout << "Total # of entry : " << readerWave.GetMaxEvent() << std::endl;

    TH1F* t3_plot    = new TH1F("Trig_3", "Trig_3;PeakADC;Evt", 1024, 0., 4096.);
    TH1F* t4_plot    = new TH1F("Trig_4", "Trig_4;PeakADC;Evt", 1024, 0., 4096.);
    TH2F* t3_t4_plot = new TH2F("t3_t4_2D", "t3_t4_2D;t3_PeakADC;t4_PeakADC", 1024, 0., 4096., 1024, 0., 4096.);

    TH1F* t3_plot_cut    = new TH1F("Trig_3_cut", "Trig_3_cut;PeakADC;Evt", 1024, 0., 4096.);
    TH1F* t4_plot_cut    = new TH1F("Trig_4_cut", "Trig_4_cut;PeakADC;Evt", 1024, 0., 4096.);
    TH2F* t3_t4_plot_cut = new TH2F("t3_t4_2D_cut", "t3_t4_2D_cut;t3_PeakADC;t4_PeakADC", 1024, 0., 4096., 1024, 0., 4096.);


    int event_passed = 0;
    for(int iEvt = 0; iEvt < readerWave.GetMaxEvent(); iEvt++) {
        printProgress(iEvt, readerWave.GetMaxEvent());
        auto anEvent = readerWave.GetAnEvent();

        auto t3_waveform = anEvent.GetData(t3_cid).waveform();
        auto t4_waveform = anEvent.GetData(t4_cid).waveform();

        float t3_peak = GetPeak(t3_waveform, 300, 500);
        float t4_peak = GetPeak(t4_waveform, 300, 500);

        t3_plot->Fill(t3_peak);
        t4_plot->Fill(t4_peak);
        t3_t4_plot->Fill(t3_peak, t4_peak);

        switch(logic) {
            case 11 :
                if ( (t3_peak > t3_cut) && (t4_peak > t4_cut ) ) {
                    event_passed++;
                    t3_plot_cut->Fill(t3_peak);
                    t4_plot_cut->Fill(t4_peak);
                    t3_t4_plot_cut->Fill(t3_peak, t4_peak);
                }
                break;

            case 10 :
                if ( (t3_peak > t3_cut) && (t4_peak < t4_cut ) ) {
                    event_passed++;
                    t3_plot_cut->Fill(t3_peak);
                    t4_plot_cut->Fill(t4_peak);
                    t3_t4_plot_cut->Fill(t3_peak, t4_peak);
                }
                break;

            case 1 :
                if ( (t3_peak < t3_cut) && (t4_peak > t4_cut ) ) {
                    event_passed++;
                    t3_plot_cut->Fill(t3_peak);
                    t4_plot_cut->Fill(t4_peak);
                    t3_t4_plot_cut->Fill(t3_peak, t4_peak);
                }
                break;

            case 0 :
                if ( (t3_peak < t3_cut) && (t4_peak < t4_cut ) ) {
                    event_passed++;
                    t3_plot_cut->Fill(t3_peak);
                    t4_plot_cut->Fill(t4_peak);
                    t3_t4_plot_cut->Fill(t3_peak, t4_peak);
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
    t3_plot->Draw("Hist");
    c->Update();

    c->cd(2);
    t4_plot->Draw("Hist");
    c->Update();

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);
    c1->cd();
    c1->Divide(1, 2);

    c1->cd(1);
    t3_plot_cut->Draw("Hist");
    c1->Update();

    c1->cd(2);
    t4_plot_cut->Draw("Hist");
    c1->Update();

    // TCanvas* c2 = new TCanvas("c2", "c2", 800, 800);
    // c2->cd();
    // t3_t4_plot->Draw("colz");
    // c2->Update();

    // TCanvas* c3 = new TCanvas("c3", "c3", 800, 800);
    // c3->cd();
    // t3_t4_plot_cut->Draw("colz");
    // c3->Update();

    app.Run();
    return 0;
}
