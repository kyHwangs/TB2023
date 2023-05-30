import pydrcTB
import sys, os
import argparse
import ROOT

parser = argparse.ArgumentParser()

parser.add_argument('--RunNumber', '-rn', action='store', type=str, required=True, help='Run number')
parser.add_argument('--PedNumber', '-pn', action='store', type=str, required=True, help='Pedestal run number')
parser.add_argument('--FileNumber', '-fn', action='store', type=str, required=True, help='File nubver on run')
parser.add_argument('--evtNumber','-en', action='store', type=int, required=True, help='event number to plot')

args = parser.parse_args()

baseDir = os.getcwd();
baseDir = os.path.join(baseDir, 'monit/Run'+args.RunNumber)

rn=args.RunNumber
fn=args.FileNumber
nevt = args.evtNumber

if not (os.path.exists(baseDir)) :
	os.makedirs(baseDir)

WaveFiles=[]
FastFiles=[]

for mid in range(1,16):
	WaveDat=f"/Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_{rn}/Run_{rn}_Wave/Run_{rn}_Wave_MID_{mid}/Run_{rn}_Wave_MID_{mid}_FILE_{fn}.dat"
	FastDat=f"/Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_{rn}/Run_{rn}_Fast/Run_{rn}_Fast_MID_{mid}/Run_{rn}_Fast_MID_{mid}_FILE_{fn}.dat"
	if(os.path.isfile):WaveFiles.append(WaveDat)
	else:print(WaveDat,"not found")
	if(os.path.isfile):FastFiles.append(FastDat)
	else:print(FastDat,"not found")

reader = pydrcTB.TBread()
reader.ntuplizeWaveform(WaveFiles,"ntuple.root")

file = ROOT.TFile("ntuple.root")
atree = file.Get("events")

utility = pydrcTB.TButility()
utility.loading("mapping_data_MCPPMT_positiveSignal.csv")
utility.loadped("ped_343.csv")

channelsize = 32

output = ROOT.TFile("online.root")

atree.GetEntry(nevt)
anevt = getattr(atree,"TBevt")

for imid in range(anevt.size()):
    for ich in range(channelsize):
        # TODO cannot convert ROOT TBcid & boost::python TBcid automatically
        cidboost = pydrcTB.TBcid( imid+1, ich+1 ) # mid 1 - 15, ch 1 - 32
        cidroot  = ROOT.TBcid( imid+1, ich+1 )

        adet = utility.find(cidboost)

        # example: sum adc of all scint SiPMs
        if (not adet.isNull()) and adet.isModule() and adet.isSiPM():
            adata = anevt.data( cidroot )
            awave = adata.waveform()

            histo = ROOT.TH1D("wave_plate"+str(adet.plate())+"col"+str(adet.column()),
                              "wave_plate"+str(adet.plate())+"col"+str(adet.column()),1000,0,1000)

            for ibin in range(1000):
                histo.SetBinContent(ibin+1,awave[ibin])

            output.WriteTObject(histo)

        if (not adet.isNull()) and (not adet.isModule()):
            adata = anevt.data( cidroot )
            awave = adata.waveform()

            histo = ROOT.TH1D("detid"+str(adet.detType())+"no"+str(adet.column()),
                              "detid"+str(adet.detType())+"no"+str(adet.column()),1000,0,1000)

            for ibin in range(1000):
                histo.SetBinContent(ibin+1,awave[ibin])

            output.WriteTObject(histo)

output.Close()
file.Close()
