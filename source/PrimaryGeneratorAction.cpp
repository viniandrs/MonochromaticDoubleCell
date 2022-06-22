#include "../include/PrimaryGeneratorAction.h"
#include "../include/EventInformation.h"

#include "G4EventManager.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "g4root.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;
using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(),
                                                   m_newGun(new G4ParticleGun()) {} // Instantiating our particle gun

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
    G4double r = 0.5 * sqrt(G4UniformRand()) * aluminum_disk_diameter; //I'll try to take its sqrt

    // Converting to cartesian coordinates
    G4double x0 = aluminum_disk_thickness * (G4UniformRand() - 0.5); // de - 0.5 a 0.5
    G4double y0 = r * cos(phi);
    G4double z0 = r * sin(phi);

    // Center of aluminum disk
    G4float support_thickness = 1.0 * cm;
    G4float arapuca_cell_height = 49. * cm;
    G4float z_source = 24.5 * cm;
    G4float x_source = 3.5 * cm;

    G4float disk_x = -x_source + 0.5 * (support_thickness - aluminum_disk_thickness);
    G4float disk_y = 0;
    G4float disk_z = z_source - 0.5 * arapuca_cell_height;

    // the initial position of the particle
    m_newGun->SetParticlePosition(G4ThreeVector(disk_x + x0, disk_y + y0, disk_z + z0));

    // default particle kinematic
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    m_newGun->SetParticleDefinition(particleTable->FindParticle("alpha"));

    //------------------- Random initial momentum direction -------------------
    G4float phi2 = 2 * pi * G4UniformRand();
    G4float theta2 = pi * G4UniformRand();
    G4float r2 = 1;//G4UniformRand();

    G4float x2 = r2 * cos(phi2) * sin(theta2);
    G4float y2 = r2 * sin(phi2) * sin(theta2);
    G4float z2 = r2 * cos(theta2);

    // Momentum vector with a random spherical direction
    G4ThreeVector direction = G4ThreeVector(x2, y2, z2);
    m_newGun->SetParticleMomentumDirection(direction);

    //------------------- Random initial particle energy -------------------

    /*
    For this, we'll recall that alpha particles emitted by uranium have a distribution of 2.2%, 48.9% and 48.9%,
    for the respective energies  4.464, 4.187 and 4.759 MeV.
    */
    G4double random = 100 * G4UniformRand();

    if (random <= 2.2)
    {
        m_newGun->SetParticleEnergy(4.464 * MeV);
    }
    else if (random > 2.2 && random < 51.1)
    {
        m_newGun->SetParticleEnergy(4.187 * MeV);
    }
    else
    {
        m_newGun->SetParticleEnergy(4.759 * MeV);
    }

    // Generating the event
    m_newGun->GeneratePrimaryVertex(anEvent);
}