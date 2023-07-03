import ROOT
import sys, os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--RunNumber', '-rn', action='store', type=int, required=True, help='Run number')
parser.add_argument('--MaxEntry', '-me', action='store', type=int, default=-1, help='Maximum entry to ntuplize, use full event if -1')
parser.add_argument('--FileEntry', '-fe', action='store', type=int, default=-1, help='Entry per splitted file, store all event in single file if -1')
parser.add_argument('--doFast', '-f', action='store_true', default=False, help='If true, make fast mode ntupler')
parser.add_argument('--output', '-o', action='store', type=str, default='./', help='Set output file dir path')
args = parser.parse_args()

rn = args.RunNumber
mode = "Fast" if args.doFast else "Wave"

outputPath = ""
if args.output.endswith("/") : 
    outputPath = args.output + f"Run_{rn}/Run_{rn}_{mode}/"
else :
    outputPath = args.output + f"/Run_{rn}/Run_{rn}_{mode}/"
if not os.path.exists(outputPath) :
    os.makedirs(outputPath)
outFileName = f"ntuple_Run_{rn}_{mode}"

ntupler = ROOT.TBntupler("/Users/yhep/scratch/YUdaq", outputPath, outFileName, rn, args.MaxEntry, args.FileEntry)
if args.doFast :
    ntupler.ntuplizeFastmode()
else :
    ntupler.ntuplizeWaveform()
