#pragma once
#include "G4UserRunAction.hh"

using namespace std;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction();

  void BeginOfRunAction(const G4Run *aRun);
  void EndOfRunAction(const G4Run *aRun);
};