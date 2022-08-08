#include "../include/EventAction.h"
#include "../include/EventInformation.h"

#include "G4EventManager.hh"
#include "g4root.hh"

EventAction::EventAction() {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event *)
{
    // Set a pointer of EventInformation class object to G4Event through
    // SetUserEventInformation() method of G4EventManager
    auto *eventInformation = new EventInformation();
    G4EventManager::GetEventManager()->SetUserInformation(eventInformation);
}

// This function will be run at the end of the processing of a full particle stack created
// when one alpha particle is generated
void EventAction::EndOfEventAction(const G4Event *){
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    G4VUserEventInformation *eventInformationBaseClass = G4EventManager::GetEventManager()->GetUserInformation();
    EventInformation *eventInformation = static_cast<EventInformation *>(eventInformationBaseClass);

    if(eventInformation->GetPhotonsGenerated() == 0 ||
       eventInformation->GetPhotonsDetected() == 0 || 
       eventInformation->GetIsAlphaValid() == false) return;
    
    //Fill the NTuple and the histogram
    analysisManager->FillNtupleIColumn(0, eventInformation->GetPhotonsGenerated());
    analysisManager->FillNtupleIColumn(1, eventInformation->GetPhotonsDetected());
    analysisManager->AddNtupleRow();
}