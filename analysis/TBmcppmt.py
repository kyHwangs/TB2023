import ROOT
from ROOT import TApplication
from ROOT import TRootCanvas

import sys, os
import argparse
import time
from datetime import datetime

ROOT.gApplication = ROOT.nullptr
app = TApplication("app", ROOT.nullptr, ROOT.nullptr)

print("Start at",datetime.now())
parser = argparse.ArgumentParser()

parser.add_argument('--RunNumber', '-rn', action='store', type=int, required=True, help='Run number')
parser.add_argument('--MaxEvent', '-me', action='store', type=int, default=-1, help='set maximum event to plot')
parser.add_argument('--StartEvent', '-se', action='store', type=int, default=0, help='set event to start from')
parser.add_argument('--event', '-e', action='store_true', default=False, help='if true, view evt-by-evt hitmap')
parser.add_argument('--save', '-s', action="store_true", default=False, help='if true with event option, saves hitmap & no display (only for evt-by-evt)')

args = parser.parse_args()
RunNumber = args.RunNumber
MaxEvent = args.MaxEvent
StartEvent = args.StartEvent

if (args.save) : ROOT.gROOT.SetBatch(True)

# 0 : Avg time struc, 1 : PeakADC, 2 : IntADC
if args.event :
	MCPPMT = ROOT.TBmcppmt('TBwaveform')(RunNumber, 2, 0)
else :
	MCPPMT = ROOT.TBmcppmt('TBwaveform')(RunNumber, 0, 0)

MCPPMT.SetMapping("/Users/yhep/scratch/DQM/TB2023/mapping/mapping_TB2021July_v1.root");

if MaxEvent != -1:
	MCPPMT.SetMaxEvent(MaxEvent + StartEvent)

if args.event :	
	MCPPMT.SetPlotRangeX(1000, -3000, 300000)
	MCPPMT.SetCalcRangeX(150, 300)

MCPPMT.PreparePlots()

if args.event :
	MCPPMT.SetEventHeatmap()
else :
	MCPPMT.SetHeatmap()

if args.event and not args.save :
	mode = MCPPMT.getMode()
	MCPPMT.PrepareEvtLoop()
	for iEvt in range(MaxEvent + StartEvent) :
		if (iEvt < StartEvent) : continue
		print("Evt : ", iEvt)
		MCPPMT.GetAnEvent(mode);
		MCPPMT.GetData(mode);
		MCPPMT.Fill(mode);
		canvas = MCPPMT.GetEventHeatMap(iEvt)
		canvas.Draw()
		key = input("Press enter to continue, q to quit")
		if (key == "q") : app.Terminate()
	app.Run()

elif args.event and args.save :
	mode = MCPPMT.getMode()
	MCPPMT.PrepareEvtLoop()
	ROOT.gSystem.mkdir("./HitMap/Run_" + str(RunNumber), True)
	for iEvt in range(MaxEvent + StartEvent) :
		if (iEvt < StartEvent) : continue
		print("Evt : ", iEvt)
		MCPPMT.GetAnEvent(mode);
		MCPPMT.GetData(mode);
		MCPPMT.Fill(mode);
		MCPPMT.SaveEventHeatMap(iEvt)
	MCPPMT.EndEvtLoop()

else :
	MCPPMT.Loop()

print("Done at",datetime.now())