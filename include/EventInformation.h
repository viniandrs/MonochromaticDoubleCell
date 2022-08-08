#pragma once

#include "G4VUserEventInformation.hh"
#include "G4Types.hh"

class EventInformation : public G4VUserEventInformation
{
public:
    EventInformation();
    ~EventInformation();

    void Print() const override;

    void IncreasePhotonsDetected();
    void SetPhotonsGenerated(G4int);
    void AlphaIsValid();

    inline G4int GetPhotonsDetected() { return photonsDetected; }
    inline G4int GetPhotonsGenerated() { return photonsGenerated; }
    inline G4int GetIsAlphaValid() { return validAlpha; }

public:
    G4int photonsGenerated, photonsDetected;
    G4bool validAlpha;
};
