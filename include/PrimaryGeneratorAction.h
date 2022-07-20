#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    PrimaryGeneratorAction(G4float z);
    
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event *anEvent) override; // Need to translate it to the center of the disk

private:
    G4ParticleGun *m_newGun;
    G4float z_source;
};