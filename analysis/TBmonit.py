import ROOT
import sys, os
import argparse
from datetime import datetime

print("Start at",datetime.now())
parser = argparse.ArgumentParser()

parser.add_argument('--RunNumber', '-rn', action='store', type=int, required=True, help='Run number')
parser.add_argument('--Config', '-c', action='store', type=str, required=True, help='YAML config file')
parser.add_argument('--MaxEvent', '-me', action='store', type=int, default=-1, help='set maximum event to plot')

args = parser.parse_args()
RunNumber = args.RunNumber
MaxEvent = args.MaxEvent
Config = args.Config

monit = ROOT.TBmonit('TBwaveform')(Config, RunNumber)

if MaxEvent != -1:
	monit.SetMaxEvent(MaxEvent)

monit.Loop()

print("Done at",datetime.now())
