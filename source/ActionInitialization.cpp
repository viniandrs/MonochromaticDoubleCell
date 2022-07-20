#include "../include/ActionInitialization.h"

#include "../include/PrimaryGeneratorAction.h"
#include "../include/RunAction.h"
#include "../include/EventAction.h"
#include "../include/SteppingAction.h"
#include "../include/TrackingAction.h"

#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;
using namespace std;

ActionInitialization::ActionInitialization() : G4VUserActionInitialization(), is_z_given(false) {}

ActionInitialization::ActionInitialization(G4String source_z_position) : G4VUserActionInitialization(), is_z_given(true)
{
    G4float try_z = std::stof(source_z_position) * cm;
    if (try_z > 1 * cm && try_z < 48 * cm)
        z_source = try_z;
    else
    {
        G4cout << "INVALID SOURCE POSITION! \n"
               << G4endl;
        exit(EXIT_FAILURE);
    }
}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const
{
    RunAction *runAction = new RunAction();
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
    if (is_z_given)
    {
        PrimaryGeneratorAction *generator = new PrimaryGeneratorAction(z_source);
        SetUserAction(generator);
    }
    else
    {
        PrimaryGeneratorAction *generator = new PrimaryGeneratorAction();
        SetUserAction(generator);
    }

    RunAction *runAction = new RunAction();
    SetUserAction(runAction);

    EventAction *eventAction = new EventAction();
    SetUserAction(eventAction);

    SteppingAction *steppingAction = new SteppingAction();
    SetUserAction(steppingAction);

    TrackingAction *trackingAction = new TrackingAction();
    SetUserAction(trackingAction);
}