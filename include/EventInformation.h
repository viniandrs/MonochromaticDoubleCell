#pragma once

#include "G4VUserEventInformation.hh"
#include "G4Types.hh"

class EventInformation : public G4VUserEventInformation
{
public:
    EventInformation();
    ~EventInformation();

    virtual void Print() const override;

    void UpperDetection();
    void LowerDetection();
    
    void SetPhotonsGenerated(G4int);
    void AlphaIsValid();

    inline G4int GetIsAlphaValid() { return validAlpha; }

public:
    G4int photonsGenerated;
    G4int photonsDetected_up;
    G4int photonsDetected_down;

    G4bool validAlpha;
};
