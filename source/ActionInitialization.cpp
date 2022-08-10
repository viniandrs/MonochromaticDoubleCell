#include "../include/ActionInitialization.h"

#include "../include/PrimaryGeneratorAction.h"
#include "../include/RunAction.h"
#include "../include/EventAction.h"
#include "../include/SteppingAction.h"
#include "../include/TrackingAction.h"

#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;
using namespace std;

ActionInitialization::ActionInitialization(G4float z_position) : G4VUserActionInitialization(), z_source(z_position){}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const
{
    RunAction *runAction = new RunAction();
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
    PrimaryGeneratorAction *generator = new PrimaryGeneratorAction(z_source);
    SetUserAction(generator);
    
    RunAction *runAction = new RunAction();
    SetUserAction(runAction);

    EventAction *eventAction = new EventAction();
    SetUserAction(eventAction);

    SteppingAction *steppingAction = new SteppingAction();
    SetUserAction(steppingAction);

    TrackingAction *trackingAction = new TrackingAction();
    SetUserAction(trackingAction);
}