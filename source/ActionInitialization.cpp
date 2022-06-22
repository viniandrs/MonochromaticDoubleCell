#include "../include/ActionInitialization.h"

#include "../include/PrimaryGeneratorAction.h"
#include "../include/RunAction.h"
#include "../include/EventAction.h"
#include "../include/SteppingAction.h"
#include "../include/TrackingAction.h"

ActionInitialization::ActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const
{
    RunAction *runAction = new RunAction();
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
    PrimaryGeneratorAction *generator = new PrimaryGeneratorAction();
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