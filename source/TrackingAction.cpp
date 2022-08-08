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

void TrackingAction::PreUserTrackingAction(const G4Track *track)
{
    G4String particleName = track->GetDefinition()->GetParticleName();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    /* If CreatorProcess is null, this means the track is created by the event generator, i.e. the primary track.
    If it is not, it points to the process which created the track.*/
    if (particleName == "opticalphoton" && track->GetCreatorProcess() != NULL)
    {
        // Building the Liquid argon scintillation spectrum
        G4double photonEnergy = track->GetTotalEnergy() / eV;
        analysisManager->FillH1(0, photonEnergy);
    }
    // else if (particleName == "alpha" && track->GetCreatorProcess() == NULL)
    // {   
    //     // preparing starting position data
    //     G4ThreeVector initialPosition = track->GetVertexPosition();

    //     G4float aluminum_disk_thickness = 140 * um;
    //     G4float support_thickness = 1.0 * cm;
    //     G4float x_source = 3.5 * cm;
    //     G4float disk_x = -x_source + 0.5 * (support_thickness - aluminum_disk_thickness);

    //     G4float x0 = initialPosition.x() - disk_x;
    //     G4float y0 = initialPosition.y();
    //     G4float z0 = initialPosition.z();
    //     // Registering particle's initial position
    //     analysisManager->FillNtupleFColumn(2, x0); 
    //     analysisManager->FillNtupleFColumn(3, y0);
    //     analysisManager->FillNtupleFColumn(4, z0);
        
    // }
}

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