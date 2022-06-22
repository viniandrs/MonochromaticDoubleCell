#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"

class Detector : public G4VUserDetectorConstruction
{
public:
    Detector();
    ~Detector();

    G4VPhysicalVolume *Construct() override;

private:
    // Methods to construct each component separatedly
    void PlaceXArapucaCell(G4ThreeVector center);
    void PlaceCryostat(G4ThreeVector origin,
                                          G4LogicalVolume *mother_volume);
    void FillCryostatWithLAr(G4LogicalVolume *mother_volume);                                          
    void PlaceModuleBases(G4ThreeVector origin,
                                          G4LogicalVolume *mother_volume);
    void PlaceSupportWithSource();                                       

    // Materials used to build the geometry
    void GenerateMaterials();
    void LArOptics();

    G4NistManager *nist_manager; // data from NIST table

    G4Material *teflon;
    G4Material *stainless_steel;
    G4Material *LAr;
    G4Material *PVC;
    G4Material *Aluminum;

    G4Element *H;
    G4Element *Cl;
    G4Element *C;
    G4Element *Si;
    G4Element *Cr;
    G4Element *Mn;
    G4Element *Fe;
    G4Element *Ni;
    G4Element *Al;

    // Components of the geometry
    G4VSolid *cryostat;
    G4LogicalVolume *l_LAr;
    G4LogicalVolume *l_cryostat;

    // Geometric parameters (I shall initialize them on the constructor)
    G4float cryostat_outter_radius;
    G4float cryostat_inner_radius;
    G4float cryostat_height;
    G4float arapuca_cell_height;
    G4float z_source;
    G4float x_source;
};
