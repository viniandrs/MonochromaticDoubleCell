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

# Creating the histogram from a TTree (TChain)
hPhGenFromTuple = ROOT.TH1I(
    "hPhDetFromTuple", "Photons Detected per alpha", 200, 0, 5000)

for alpha in chain:
    photonsDetected = alpha.photonsDetected
    hPhGenFromTuple.Fill(photonsDetected)

# Editing histograms
hPhGenFromTuple.GetXaxis().SetTitle("Photons/Alpha")
hPhGenFromTuple.GetYaxis().SetTitle("#")

# Setup of the Canvas

c1 = ROOT.TCanvas("c1", "Histograms", 1)
hPhGenFromTuple.Draw("HIST")
c1.Draw()

input()