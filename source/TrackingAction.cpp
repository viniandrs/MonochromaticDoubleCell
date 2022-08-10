#include "../include/TrackingAction.h"
#include "../include/EventInformation.h"

#include "G4SystemOfUnits.hh"
#include "G4Types.hh"
#include "G4EventManager.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "g4root.hh"

#include <string>
#include <sstream>

TrackingAction::TrackingAction() : generatedPhotonsCounter(0) {}
TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track *track){}

void TrackingAction::PostUserTrackingAction(const G4Track *track)
{
    G4String particleName = track->GetDefinition()->GetParticleName();

    auto *eventInformationBaseClass = G4EventManager::GetEventManager()->GetUserInformation();
    auto *eventInformation = static_cast<EventInformation *>(eventInformationBaseClass);

    if (particleName == "alpha")
    {
        // Counting the number of secondary photons generated
        G4TrackVector *secondaryTracks = fpTrackingManager->GimmeSecondaries();

        if (!secondaryTracks)
            return;

        size_t nmbSecTracks = (*secondaryTracks).size();
        generatedPhotonsCounter = 0;
        for (size_t i = 0; i < nmbSecTracks; i++)
        {
            if ((*secondaryTracks)[i]->GetDefinition()->GetParticleName() == "opticalphoton")
                generatedPhotonsCounter++;
        }

        // Saving the information of how many photons were created in EventInformation object
        eventInformation->SetPhotonsGenerated(generatedPhotonsCounter);
    }
}