#include "../include/RunAction.h"
#include "../include/Constants.h"

#include "g4root.hh"
#include "G4Run.hh"

using namespace std;

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run *)
{
    //-------------------------Root setup--------------------------------//
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("output.root");

    // // Cria histogramas    -- (nome, titulo, nbins, xmin, xmax)
    analysisManager->CreateH1("hEnergy", "Energy of scintillation photon", 2000, 4, 16); //0 Liquid Argon scintillation spectrum
    analysisManager->CreateH1("hAlpha", "Alpha Spectrum", 100, -1.0, 6.0);               //1 Alpha spectrum

    // //NTuple
    analysisManager->SetFirstNtupleId(0);
    analysisManager->CreateNtuple("alpha", "Alpha particles data"); //NTuple 0: alpha data
    analysisManager->CreateNtupleIColumn("photonsGenerated");//0
    analysisManager->CreateNtupleIColumn("photonsDetected");//1
    analysisManager->CreateNtupleFColumn("alphaXGen");//2
    analysisManager->CreateNtupleFColumn("alphaYGen");//3
    analysisManager->CreateNtupleFColumn("alphaZGen");//4
    analysisManager->FinishNtuple(0);
}

void RunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}