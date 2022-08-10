import ROOT
import os

# Counting the amount of output files
number_of_files = 0
for file_name in os.listdir(os.getcwd() + "/build/"):
    if "output_" in file_name:
        number_of_files = number_of_files + 1

# Building a chain with all the output files
chain = ROOT.TChain("alpha", "CompleteOutput")
for i in range(number_of_files):
    chain.Add(os.getcwd() + "/build/output_t"+str(i)+".root")

# Opening original file
file = ROOT.TFile(os.getcwd() + "/build/output.root")

# Creating the histograms from a TTree (TChain)
hPhDetUp = ROOT.TH1I(
    "hPhDetFromTuple", "Photons Detected by upper cell per alpha", 150, 0, 4500)
hPhDetDown = ROOT.TH1I(
    "hPhDetFromTuple", "Photons Detected by lower cell per alpha", 150, 0, 4500)
hPhDetTotal = ROOT.TH1I(
    "hPhDetFromTuple", "Total photons detected per alpha", 150, 0, 5000)
hAlphaSpectrum = file.Get("hAlpha")

for alpha in chain:
    photonsDetectedUp = alpha.photonsDetectedUp
    photonsDetectedDown = alpha.photonsDetectedDown
    hPhDetUp.Fill(photonsDetectedUp)
    hPhDetDown.Fill(photonsDetectedDown)
    hPhDetTotal.Fill(photonsDetectedUp + photonsDetectedDown)

# Editing histograms
hPhDetUp.GetXaxis().SetTitle("Photons/Alpha")
hPhDetUp.GetYaxis().SetTitle("#")

hPhDetDown.GetXaxis().SetTitle("Photons/Alpha")
hPhDetDown.GetYaxis().SetTitle("#")

hPhDetTotal.GetXaxis().SetTitle("Photons/Alpha")
hPhDetTotal.GetYaxis().SetTitle("#")

hAlphaSpectrum.GetXaxis().SetTitle("Alpha particle energy (MeV)")
hAlphaSpectrum.GetYaxis().SetTitle("#")

# Setup of the Canvas
c1 = ROOT.TCanvas("c1", "Histograms", 1)
c1.Divide(2,2)
c1.cd(1)
hAlphaSpectrum.Draw("HIST")
c1.cd(2)
hPhDetUp.Draw("HIST")
c1.cd(3)
hPhDetDown.Draw("HIST")
c1.cd(4)
hPhDetTotal.Draw("HIST")

c1.Draw()

input()