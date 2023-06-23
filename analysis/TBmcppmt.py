import ROOT
import sys, os
import argparse
from datetime import datetime

print("Start at",datetime.now())
parser = argparse.ArgumentParser()

parser.add_argument('--RunNumber', '-rn', action='store', type=int, required=True, help='Run number')
parser.add_argument('--MaxEvent', '-me', action='store', type=int, default=-1, help='set maximum event to plot')

args = parser.parse_args()
RunNumber = args.RunNumber
MaxEvent = args.MaxEvent

MCPPMT = ROOT.TBmcppmt('TBwaveform')(RunNumber, 0, 0)
MCPPMT.SetMapping("/Users/khwang/scratch/TB2023July/preparation/dev_230622/TB2023/mapping/mapping_TB2021July_v1.root");

if MaxEvent != -1:
	MCPPMT.SetMaxEvent(MaxEvent)

MCPPMT.PreparePlots()
MCPPMT.SetHeatmap()
MCPPMT.Loop()

print("Done at",datetime.now())


