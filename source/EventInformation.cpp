#include "../include/EventInformation.h"

#include "G4VHit.hh"

EventInformation::EventInformation() : photonsGenerated(0), photonsDetected(0), validAlpha(false) {}
EventInformation::~EventInformation() {}

void EventInformation::Print() const {}

void EventInformation::IncreasePhotonsDetected()
{
    photonsDetected++;
}

void EventInformation::SetPhotonsGenerated(G4int value)
{
    photonsGenerated = value;
}

void EventInformation::AlphaIsValid()
{
    validAlpha = true;
}