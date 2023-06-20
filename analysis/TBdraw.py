import ROOT
from ROOT import TFile
from ROOT import TCanvas
from ROOT import TH1
from ROOT import TList
from ROOT import TApplication
from ROOT import TRootCanvas

import argparse

app = TApplication("app", ROOT.nullptr, ROOT.nullptr)

parser = argparse.ArgumentParser()
parser.add_argument("--runNum", "-rn", type=str, required=True, action="store", help="run number of DQM file")
args = parser.parse_args()

DQM_file = TFile.Open("DQM_Run" + args.runNum + ".root")
hist_list = DQM_file.GetListOfKeys()

hist_name_list = []
for obj in hist_list :
    hist_name_list.append(obj.GetName())

c = TCanvas("c", "c", 1200, 800)
c.Divide( len(hist_name_list) - int(len(hist_name_list)/2), int(len(hist_name_list)/2) )
c.Update()
c.Draw()

pad_idx = 1
for hist_name in hist_name_list :
    plot_type = hist_name[ : hist_name.find("_")]
    MID       = hist_name[hist_name.find("Mid")+3 : hist_name.find("Ch")]
    Ch        = hist_name[hist_name.find("Ch")+2 : ]
    print("Drawing plot : ", plot_type, " MID : ", MID, " Ch : ", Ch)

    c.cd(pad_idx)
    hist = DQM_file.Get(hist_name)
    hist.SetTitle(plot_type)
    if (plot_type == "AvgTimeStruc") :
        hist.SetStats(0)
    hist.Draw("Hist")
    c.Modified()
    c.Update()
    pad_idx += 1

rc = c.GetCanvasImp()
rc.Connect("CloseWindow()", "TApplication", ROOT.gApplication, "Terminate()");
app.Run()

DQM_file.Close()