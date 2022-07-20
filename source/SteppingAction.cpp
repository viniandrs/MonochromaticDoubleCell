#include "../include/SteppingAction.h"
#include "../include/RunAction.h"
#include "../include/EventInformation.h"

#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepPoint.hh"
#include "g4root.hh"
#include "G4UnitsTable.hh"

#include <unistd.h>

#include <iostream>
#include <iomanip>

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *aStep)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    G4Track *track = aStep->GetTrack();
    G4String particle = track->GetDefinition()->GetParticleName();

    // PreStep volume info
    G4StepPoint *aPrePoint = aStep->GetPreStepPoint();
    G4VPhysicalVolume *aPrePV = aPrePoint->GetPhysicalVolume();
    G4String PreVolName = "";
    if (aPrePV)
        PreVolName = aPrePV->GetName();

    // PostStep volume info
    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *aPostPV = aPostPoint->GetPhysicalVolume();
    G4String PostVolName = "";
    if (aPostPV)
        PostVolName = aPostPV->GetName();

    // Alpha spectrum
    if (particle == "alpha")
    {
        // Registering the particle starting energy
        G4VUserEventInformation *eventInformationBaseClass = G4EventManager::GetEventManager()->GetUserInformation();
        EventInformation *eventInformation = static_cast<EventInformation *>(eventInformationBaseClass);        

        if (PreVolName == "Aluminum disk" && PostVolName == "Liquid Argon")
        {
            /* Get the kinetic energy an alpha particle has when it leaves the aluminum and
            register it on the spectrum */
            G4double kinEnergy = track->GetKineticEnergy();
            analysisManager->FillH1(1, kinEnergy);
            eventInformation->AlphaIsValid();
        }
    }

    // Counting the photons that entered the Arapuca and registering its alpha's energy
    else if (particle == "opticalphoton")
    {
        // Checking if the particle passed from the Argon to one of the detector screens
        if ((PreVolName == "Liquid Argon" && PostVolName == "X-Arapuca's front face") ||
            (PreVolName == "Liquid Argon" && PostVolName == "X-Arapuca's back face"))
        {
            G4VUserEventInformation *eventInformationBaseClass = G4EventManager::GetEventManager()->GetUserInformation();
            EventInformation *eventInformation = static_cast<EventInformation *>(eventInformationBaseClass);

            eventInformation->IncreasePhotonsDetected();
        }
    }
}