import pydrcTB
import ROOT

c = ROOT.TCanvas()
c.Update()
plotter1 = pydrcTB.TBplot(1000,1000,"HitMapAccuWave","hitmap")
plotter1.openFile("output.root")
plotter1.loadTH("HitMapAccuWave0",27)
plotter1.Draw()

plotter2 = pydrcTB.TBplot(1000,500,"disMapAccuWave","distribution")
plotter2.openFile("output.root")
plotter2.loadTH("disMapAccuWave1",26)
plotter2.Draw()

plotter3 = pydrcTB.TBplot(1000,500,"wavMapWave","waveform")
plotter3.openFile("output.root")
plotter3.loadTH("wavMapWave2",26)
plotter3.Draw()

plotter4 = pydrcTB.TBplot(1000,1000,"HitMapAccuFast","hitmap")
plotter4.openFile("output.root")
plotter4.loadTH("HitMapAccuFast0",27)
plotter4.Draw()

plotter5 = pydrcTB.TBplot(1000,500,"disMapAccuFast","distribution")
plotter5.openFile("output.root")
plotter5.loadTH("disMapAccuFast1",26)
plotter5.Draw()
