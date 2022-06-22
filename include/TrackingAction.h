#pragma once

#include "G4UserTrackingAction.hh"  
#include "G4Types.hh"

class TrackingAction : public G4UserTrackingAction
{
public:
    TrackingAction();
    virtual ~TrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

private:
    G4int generatedPhotonsCounter;
};
