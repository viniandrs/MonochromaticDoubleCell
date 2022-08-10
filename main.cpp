#include <iostream>
using namespace std;

// User Action classes
#include "include/SteppingAction.h"
#include "include/RunAction.h"
#include "include/TrackingAction.h"
#include "include/EventAction.h"
#include "include/ActionInitialization.h"

// User Initialization classes
#include "include/DetectorConstruction.h"
#include "include/PrimaryGeneratorAction.h"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4ScoringManager.hh"

// User Interface classes
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

// Visualization managers
#define G4VIS_USE_OPENGLQT
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

// Physics configuration
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

int main(int argc, char **argv)
{
#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager();
#else
    G4RunManager *runManager = new G4RunManager();
#endif
    G4long myseed = 3453540;
    CLHEP::HepRandom::setTheSeed(myseed);

    // Physics list setup
    G4VModularPhysicsList *physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option2());

    G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
    // opticalPhysics->Configure(kCerenkov, true);
    opticalPhysics->Configure(kScintillation, true);
    // physicsList->DumpList();
    physicsList->RegisterPhysics(new G4OpticalPhysics());

    runManager->SetUserInitialization(physicsList);

    // User action and initialization classes
    if (argc == 3)
    {   
        G4float z_source;
        G4int try_z = std::stoi(argv[2]);

        // We're interested in three available positions for the alpha source
        switch (try_z)
        {
        case 1:
            z_source = -0.5 * 11 * cm;
            break;

        case 2:
            z_source = 0;
            break;

        case 3:
            z_source = 0.5 * 11 * cm;
            break;

        default:
            z_source = 0;
            break;
        }
        runManager->SetUserInitialization(new ActionInitialization(z_source));
        runManager->SetUserInitialization(new Detector(z_source));
    }
    else
    {
        G4cout << "WRONG PARAMETERS!!" << G4endl;
        return 1;
    }

    // Initializing the visualization manager
    auto *visManager = new G4VisExecutive();
    visManager->Initialize();

    // User interface
    auto *uiExecutive = new G4UIExecutive(argc, argv, "Qt");
    auto *uiManager = G4UImanager::GetUIpointer();

    // Running the macro file passed as parameter
    uiManager->ApplyCommand("/control/execute " + G4String(argv[1]));
    uiExecutive->SessionStart();

    delete opticalPhysics;
    delete physicsList;
    delete visManager;
    delete uiExecutive;
    delete runManager;
    return 0;
}
