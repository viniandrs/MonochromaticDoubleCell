#include "../include/PrimaryGeneratorAction.h"
#include "../include/EventInformation.h"

#include "G4EventManager.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "g4root.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;
using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction(G4float z) : G4VUserPrimaryGeneratorAction(), m_newGun(new G4ParticleGun()), z_source(z) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete m_newGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    //------------------- Random initial position of the particles -------------------
    G4float aluminum_disk_thickness = 140 * um;
    G4float aluminum_disk_diameter = 1.0 * cm;

    // Choosing a random a cyllindrical coordinate
    const G4double pi = acos(-1.);
    G4double phi = 2 * pi * G4UniformRand();
    G4double r = 0.5 * sqrt(G4UniformRand()) * aluminum_disk_diameter; // The square root comes from the area dependence of the probability

    /* After some tests, it was found out that only particles which have been generated at x > 51 um could
    leave the aluminum, get at the LAr and emit photons. So, we'll generate alpha particles only in
    51um < x < 70um */
    const G4double xmin = 51 * um;
    const G4double xmax = 70 * um;

    // Converting to cartesian coordinates -> the x coordinate is fixed
    G4double x0 = 0;
    G4double y0 = r * cos(phi);
    G4double z0 = r * sin(phi);

    // Center of aluminum disk
    G4float support_thickness = 1.0 * cm;
    G4float arapuca_cell_height = 49. * cm;
    G4float x_source = 4 * cm;

    G4float disk_x = -x_source - 0.1 * um;
    G4float disk_y = 0;
    G4float disk_z = z_source;

    // the initial position of the particle
    m_newGun->SetParticlePosition(G4ThreeVector(disk_x + x0, disk_y + y0, disk_z + z0));

    // default particle kinematic
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    m_newGun->SetParticleDefinition(particleTable->FindParticle("alpha"));

    //------------------- Random initial momentum direction -------------------
    G4float phi2 = 2 * pi * G4UniformRand();
    G4float theta2 = pi * G4UniformRand();

    G4float x2 = cos(phi2) * sin(theta2);
    G4float y2 = sin(phi2) * sin(theta2);
    G4float z2 = cos(theta2);

    // Momentum vector with a random spherical direction
    G4ThreeVector direction = G4ThreeVector(x2, y2, z2);
    m_newGun->SetParticleMomentumDirection(direction);

    //------------------- Random initial particle energy -------------------

    /*
    In this simulation, we'll be using a monochromatic source of E = 5.45 * MeV
    */
    m_newGun->SetParticleEnergy(5.45 * MeV);

    // Generating the event
    m_newGun->GeneratePrimaryVertex(anEvent);
}