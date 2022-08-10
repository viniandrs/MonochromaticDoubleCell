#include "../include/RunAction.h"

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
    analysisManager->CreateH1("hAlpha", "Alpha Spectrum", 50, 2, 7.0);               //0 Alpha spectrum

    // //NTuple
    analysisManager->SetFirstNtupleId(0);
    analysisManager->CreateNtuple("alpha", "Alpha particles that generated photons"); //NTuple 0: alpha data
    analysisManager->CreateNtupleIColumn("photonsGenerated");//0
    analysisManager->CreateNtupleIColumn("photonsDetectedUp");//1
    analysisManager->CreateNtupleIColumn("photonsDetectedDown");//2
    analysisManager->FinishNtuple(0);
}

void RunAction::EndOfRunAction(const G4Run *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}