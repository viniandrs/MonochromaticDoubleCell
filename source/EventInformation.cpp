#include "../include/EventInformation.h"

#include "G4VHit.hh"

EventInformation::EventInformation() : photonsGenerated(0), photonsDetected_up(0), photonsDetected_down(0), validAlpha(false) {}
EventInformation::~EventInformation() {}

void EventInformation::Print() const{}

void EventInformation::UpperDetection()
{
    photonsDetected_up++;
}

void EventInformation::LowerDetection()
{
    photonsDetected_down++;
}

void EventInformation::SetPhotonsGenerated(G4int value)
{
    photonsGenerated = value;
}

void EventInformation::AlphaIsValid()
{
    validAlpha = true;
}