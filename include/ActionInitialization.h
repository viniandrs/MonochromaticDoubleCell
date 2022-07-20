#pragma once

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"

using namespace std;

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();    
    ActionInitialization(G4String source_z_position);

    ~ActionInitialization();
    
    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
    G4float z_source;
    G4bool is_z_given;
};