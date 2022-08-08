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
hPhDetFromTuple = ROOT.TH1I(
    "hPhDetFromTuple", "Photons Detected per alpha", 150, 0, 1300)
hAlphaSpectrum = file.Get("hAlpha")
hPhotonSpectrum = file.Get("hEnergy")

for alpha in chain:
    photonsDetected = alpha.photonsDetected
    hPhDetFromTuple.Fill(photonsDetected)

# Editing histograms
hPhDetFromTuple.GetXaxis().SetTitle("Photons/Alpha")
hPhDetFromTuple.GetYaxis().SetTitle("#")

hAlphaSpectrum.GetXaxis().SetTitle("Alpha particle energy (MeV)")
hAlphaSpectrum.GetYaxis().SetTitle("#")

hPhotonSpectrum.GetXaxis().SetTitle("Photon energy (MeV)")
hPhotonSpectrum.GetYaxis().SetTitle("#")

# Setup of the Canvas
c1 = ROOT.TCanvas("c1", "Histograms", 1)
c1.Divide(3,1)
c1.cd(1)
hPhDetFromTuple.Draw("HIST")
c1.cd(2)
hAlphaSpectrum.Draw("HIST")
c1.cd(3)
hPhotonSpectrum.Draw("HIST")

c1.Draw()

input()