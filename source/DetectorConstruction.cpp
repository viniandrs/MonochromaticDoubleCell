#include "../include/DetectorConstruction.h"

#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "CLHEP/Units/SystemOfUnits.h"

#include "G4MaterialPropertiesTable.hh"
#include "G4VisAttributes.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

using namespace CLHEP;
using namespace std;

/*
X: width Y: height Z: thickness
*/

// Constructor for member initializers
Detector::Detector() : G4VUserDetectorConstruction(), l_LAr(NULL), cryostat(NULL), cryostat_outter_radius(15.3 * cm), cryostat_inner_radius(14.9 * cm),
                       cryostat_height(70 * cm), arapuca_cell_height(49. * cm), z_source(24.5 * cm), x_source(3.5 * cm), nist_manager(G4NistManager::Instance()),
                       l_cryostat(NULL)
{
    GenerateMaterials();
}

Detector::Detector(G4String source_z_position) : G4VUserDetectorConstruction(), l_LAr(NULL), cryostat(NULL), cryostat_outter_radius(15.3 * cm), cryostat_inner_radius(14.9 * cm),
                                                 cryostat_height(70 * cm), arapuca_cell_height(49. * cm), x_source(3.5 * cm), z_source(std::stof(source_z_position) * cm),
                                                 nist_manager(G4NistManager::Instance()), l_cryostat(NULL)
{
    GenerateMaterials();
}

Detector::~Detector()
{
    delete nist_manager;
    delete teflon;
    delete stainless_steel;
    delete C;
    delete Si;
    delete Cr;
    delete Mn;
    delete Fe;
    delete Ni;
    delete cryostat;
    delete PVC;
    delete l_cryostat;
}

// Define all the properties related to refraction, scintillation and absortion in LAr.
void Detector::LArOptics()
{
    G4MaterialPropertiesTable *LAr_MPT = new G4MaterialPropertiesTable();

    // Scintillation properties
    G4double Scint_PE[14] = {9.2 * eV, 9.3 * eV, 9.4 * eV, 9.5 * eV,
                             9.6 * eV, 9.7 * eV, 9.76377 * eV, 9.8 * eV,
                             9.9 * eV, 10.0 * eV, 10.05 * eV, 10.1 * eV,
                             10.2 * eV, 10.3 * eV};

    G4double Scint_FAST[14] = {0.000856175, 0.00839924, 0.0528321,
                               0.213077, 0.551004, 0.913594, 1,
                               0.971252, 0.66205, 0.161926, 0.289355,
                               0.0810867, 0.0145697, 0.00167853};

    G4double Scint_SLOW[14] = {0.000856175, 0.00839924, 0.0528321,
                               0.213077, 0.551004, 0.913594, 1,
                               0.971252, 0.66205, 0.161926, 0.289355,
                               0.0810867, 0.0145697, 0.00167853};

    LAr_MPT->AddProperty("FASTCOMPONENT", Scint_PE, Scint_FAST, 14);
    LAr_MPT->AddProperty("SLOWCOMPONENT", Scint_PE, Scint_SLOW, 14);
    LAr_MPT->AddConstProperty("SCINTILLATIONYIELD", 3621.0 / MeV); // reduzido em de 10x - photons/energia
    LAr_MPT->AddConstProperty("RESOLUTIONSCALE", 1.);
    LAr_MPT->AddConstProperty("FASTTIMECONSTANT", 6. * ns);
    LAr_MPT->AddConstProperty("SLOWTIMECONSTANT", 1.8 * us);
    LAr_MPT->AddConstProperty("YIELDRATIO", 0.25);

    // Refraction Index

    // I'll discover why we're using those wavelengths
    G4double RIndex_photon_energy[35] = {1.8002708291 * eV, 1.9481638324 * eV, 1.8712998009 * eV, 2.031612752 * eV, 2.122530612 * eV,
                                         2.2219671291 * eV, 2.3311783976 * eV, 2.4516802288 * eV, 2.5853189354 * eV, 2.7343666014 * eV, 2.9016513225 * eV, 3.0907383696 * eV,
                                         3.306187123 * eV, 3.5539235215 * eV, 3.8417935227 * eV, 4.1804092554 * eV, 4.5844859047 * eV, 5.075036896 * eV, 5.6831472915 * eV,
                                         6.456829363 * eV, 7.3367183961 * eV, 8.1468599034 * eV, 8.7942508537 * eV, 9.3735970883 * eV, 9.7402642614 * eV, 10.0346587111 * eV,
                                         10.2939327045 * eV, 10.401432965 * eV, 10.5669602239 * eV, 10.7726825649 * eV, 10.7960361308 * eV, 10.8904717926 * eV, 11.0108653265 * eV,
                                         11.1339505034 * eV, 11.2217603734 * eV};

    G4double RIndex_LAr[35] = {1.2249193575067825, 1.224940655396017, 1.224940655396017, 1.2252814216237677, 1.22538791106994, 1.225686081519222,
                               1.2269000612055847, 1.2272834232118046, 1.2280714451134787, 1.2281779345596509, 1.2294345100244823, 1.2309466601601273, 1.2320328525110833,
                               1.2330125554158673, 1.2348441738900287, 1.2377406868259118, 1.2414465195527031, 1.2461320551842787, 1.2536928058625028, 1.2648954955998148,
                               1.28143463770843, 1.301258737209988, 1.3225062859789587, 1.345444112684444, 1.364484425660028, 1.3856545275590553, 1.4069737146827237,
                               1.4279201887447894, 1.4510731241315424, 1.4716468851320057, 1.5005409993414198, 1.5292534159333024, 1.550184981472904, 1.5699536822603057,
                               1.5907957966651227};

    LAr_MPT->AddProperty("RINDEX", RIndex_photon_energy, RIndex_LAr, 35);

    // Absorption
    G4double ABS_photon_energy[5] = {2.034 * eV, 2.341 * eV, 2.757 * eV, 3.353 * eV, 4.136 * eV};
    G4double abs_LAr[5] = {3448 * m, 4082 * m, 6329 * m, 9174 * m, 12346 * m};

    LAr_MPT->AddProperty("ABSLENGTH", ABS_photon_energy, abs_LAr, 5);

    LAr->SetMaterialPropertiesTable(LAr_MPT);

    LAr->GetIonisation()->SetBirksConstant(0.0000001 * mm / MeV);
}

// Generate all the materials and elements used to build the geometry
void Detector::GenerateMaterials()
{
    teflon = nist_manager->FindOrBuildMaterial("G4_TEFLON");

    C = nist_manager->FindOrBuildElement("C");
    H = nist_manager->FindOrBuildElement("H");
    Cl = nist_manager->FindOrBuildElement("Cl");
    Si = nist_manager->FindOrBuildElement("Si");
    Cr = nist_manager->FindOrBuildElement("Cr");
    Mn = nist_manager->FindOrBuildElement("Mn");
    Fe = nist_manager->FindOrBuildElement("Fe");
    Ni = nist_manager->FindOrBuildElement("Ni");
    Al = nist_manager->FindOrBuildElement("Al");

    G4float density, fractionmass;
    G4int nAtoms, nComponents;

    // Creating the aluminum
    Aluminum = new G4Material("Aluminio", density = 2.70 * g / cm3, nComponents = 1);
    Aluminum->AddElement(Al, nAtoms = 1);

    // Creating stainless steel and its surface
    stainless_steel = new G4Material("stainless_steel", density = 8.06 * g / cm3, nComponents = 6);
    stainless_steel->AddElement(C, fractionmass = 0.001);
    stainless_steel->AddElement(Si, fractionmass = 0.007);
    stainless_steel->AddElement(Cr, fractionmass = 0.18);
    stainless_steel->AddElement(Mn, fractionmass = 0.01);
    stainless_steel->AddElement(Fe, fractionmass = 0.712);
    stainless_steel->AddElement(Ni, fractionmass = 0.09);

    G4double stainless_steel_PhotonEnergies[14] = {9.2 * eV, 9.3 * eV, 9.4 * eV, 9.5 * eV,
                                                   9.6 * eV, 9.7 * eV, 9.76377 * eV, 9.8 * eV,
                                                   9.9 * eV, 10.0 * eV, 10.05 * eV, 10.1 * eV,
                                                   10.2 * eV, 10.3 * eV};
    G4double stainless_steel_reflectivity[14] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2,
                                                 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};

    G4MaterialPropertiesTable *SS_MPT = new G4MaterialPropertiesTable();
    SS_MPT->AddProperty("REFLECTIVITY", stainless_steel_PhotonEnergies, stainless_steel_reflectivity, 14);

    G4OpticalSurface *stainless_steel_surface = new G4OpticalSurface("StainlessSteelSurface");
    stainless_steel_surface->SetModel(glisur);
    stainless_steel_surface->SetType(dielectric_metal);
    stainless_steel_surface->SetFinish(polished);
    stainless_steel_surface->SetMaterialPropertiesTable(SS_MPT);

    new G4LogicalSkinSurface("InoxProperty", l_cryostat, stainless_steel_surface);

    // Creating PVC and its properties
    PVC = new G4Material("PVC", density = 1.38 * g / cm3, nComponents = 3);
    PVC->AddElement(C, nAtoms = 2);
    PVC->AddElement(H, nAtoms = 3);
    PVC->AddElement(Cl, nAtoms = 1);

    G4double PhotonEnergy[14] = {4.96 * eV, 5.39 * eV, 6.20 * eV, 6.89 * eV,
                                 8.27 * eV, 8.86 * eV, 9.69 * eV, 10.3 * eV,
                                 11.3 * eV, 12.4 * eV, 13.8 * eV, 15.5 * eV, 24.8 * eV, 24.9 * eV};
    G4double abs_pvc[14] = {0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm,
                            0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm, 0.00001 * nm,
                            0.00001 * nm, 0.00001 * nm};

    G4MaterialPropertiesTable *PVC_MPT = new G4MaterialPropertiesTable();
    PVC_MPT->AddProperty("ABSLENGTH", PhotonEnergy, abs_pvc, 14);
    PVC->SetMaterialPropertiesTable(PVC_MPT);

    // LAr is more complex because we must define its optical properties ---------------
    LAr = nist_manager->FindOrBuildMaterial("G4_lAr");
    LArOptics();
}

// Place the XArapuca cell into the LAr
void Detector::PlaceXArapucaCell(G4ThreeVector origin)
{
    // Frame ----------------------------------------------------------------------------------------------------------
    // Geometric constants
    G4float frame_width = 10.0 * cm;

    G4float hole_height = 46.7 * cm;
    G4float hole_width = 7.5 * cm;

    G4float thickness = 1.5 * cm;

    // Geometry
    G4Box *frame_box = new G4Box("arapuca_cell:frame:solid:box", 0.5 * thickness, 0.5 * frame_width, 0.5 * arapuca_cell_height);
    G4Box *frame_hole = new G4Box("arapuca_cell:frame:solid:hole", 0.5 * (thickness + 1.0 * cm), 0.5 * hole_width, 0.5 * hole_height);

    G4SubtractionSolid *frame = new G4SubtractionSolid("arapuca_cell:frame:solid_outter_frame", frame_box, frame_hole, 0, G4ThreeVector(0, 0, 0));

    // Creating the divisories between the cells
    G4float div_width = 10 * cm;
    G4float div_height = 0.34 * cm;
    G4float div_thickness = 0.25 * thickness;

    G4Box *frame_div = new G4Box("arapuca_cell:frame:divs", 0.5 * div_thickness, 0.5 * div_width, 0.5 * div_height);

    for (int i = 0; i < 5; i++)
    {
        
    }

    // Placing the supercell
    G4LogicalVolume *l_frame = new G4LogicalVolume(frame, teflon, "arapuca_cell:frame:logical");

    G4VPhysicalVolume *p_frame = new G4PVPlacement(0,                      // Rotation matrix
                                                   G4ThreeVector(0, 0, 0), // Position
                                                   l_frame,                // Logical volume
                                                   "X-Arapuca's frame",    // its name
                                                   l_LAr,                  // its mother volume
                                                   true,                   // no boolean operation
                                                   0,                      // copy number
                                                   true);

    // Visualization attributes
    G4VisAttributes *arapuca_frame_vis = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
    l_frame->SetVisAttributes(arapuca_frame_vis);

    // Faces ----------------------------------------------------------------------------------------------------------

    // Geometric constants
    G4float face_thickness = 0.25 * thickness;

    // Positions
    G4ThreeVector position_front_face = G4ThreeVector(
        -0.25 * thickness,
        0,
        0);

    G4ThreeVector position_back_face = G4ThreeVector(
        0.25 * thickness,
        0,
        0);

    // face facing the detector
    G4Box *face = new G4Box("arapuca_cell:face:solid", 0.5 * face_thickness, 0.5 * hole_width, 0.5 * hole_height);

    G4LogicalVolume *l_face = new G4LogicalVolume(face, teflon, "arapuca_cell:face:logical");

    G4VPhysicalVolume *p_front_face = new G4PVPlacement(0,                        // no rotation
                                                        position_front_face,      // at position
                                                        l_face,                   // its logical volume
                                                        "X-Arapuca's front face", // its name
                                                        l_LAr,                    // its mother  volume
                                                        true,                     // no boolean operation
                                                        0,                        // copy number
                                                        true);

    // face back to the detector
    G4VPhysicalVolume *p_back_face = new G4PVPlacement(0,                       // no rotation
                                                       position_back_face,      // at position
                                                       l_face,                  // its logical volume
                                                       "X-Arapuca's back face", // its name
                                                       l_LAr,                   // its mother  volume
                                                       true,                    // no boolean operation
                                                       0,                       // copy number
                                                       true);

    // Visualization attributes
    G4VisAttributes *arapuca_face_vis = new G4VisAttributes(G4Colour(.7, .3, .2, 0.2));
    l_face->SetVisAttributes(arapuca_face_vis);
}

void Detector::PlaceCryostat(G4ThreeVector origin,
                             G4LogicalVolume *mother_volume)
{
    G4RotationMatrix *rotation = new G4RotationMatrix();
    // rotation->rotateY(0. * degree);
    //  Cryostat wall ----------------------------------------------------------------------------------------------------------

    // Geometric constants
    G4float cryostat_base_height = 1.5 * cm;

    G4double fractionmass, density, nComponents;

    // Geometry
    G4Tubs *cryostat_wall = new G4Tubs("cryostat:outter_cyllinder:solid",
                                       0.5 * cryostat_inner_radius,
                                       0.5 * cryostat_outter_radius,
                                       0.5 * cryostat_height,
                                       0.0 * deg,
                                       360 * deg);

    G4Tubs *cryostat_base = new G4Tubs("cryostat:base:solid",
                                       0,
                                       0.5 * cryostat_inner_radius,
                                       0.5 * cryostat_base_height,
                                       0.0 * deg,
                                       360 * deg);

    cryostat = new G4UnionSolid("cryostat:solid", cryostat_wall, cryostat_base, 0,
                                G4ThreeVector(0, 0, 0.5 * (cryostat_base_height - cryostat_height)));

    l_cryostat = new G4LogicalVolume(cryostat, stainless_steel, "cryostat:logical");

    G4VPhysicalVolume *p_cryostat = new G4PVPlacement(rotation,               // Rotation matrix
                                                      G4ThreeVector(0, 0, 0), // Position
                                                      l_cryostat,             // Logical volume
                                                      "Cryostat",             // its name
                                                      mother_volume,          // its mother volume
                                                      true,                   // no boolean operation
                                                      0,                      // copy number
                                                      true);

    // Cryostat visualization attributes ----------------------------------------------------------------------------------------------------------
    G4VisAttributes *cryostat_vis = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
    cryostat_vis->SetForceSolid(false);
    l_cryostat->SetVisAttributes(cryostat_vis);
}

// Fill the cryostat volume with LAr
void Detector::FillCryostatWithLAr(G4LogicalVolume *l_world)
{
    // Geometry: I'll make a base cylinder with the volume of the cryostat. The LAr geometry will be
    // the boolean subtraction of the cryostat from the base cylinder
    auto *lar_cylinder = new G4Tubs("lar:bool_solid1",
                                    0, 0.5 * cryostat_inner_radius,
                                    0.5 * cryostat_height,
                                    0.0 * deg, 360 * deg);

    auto *s_LAr = new G4SubtractionSolid("lar:solid", lar_cylinder, cryostat);

    l_LAr = new G4LogicalVolume(s_LAr, LAr, "lar:logical");

    auto *lar_vis = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.1));
    l_LAr->SetVisAttributes(lar_vis);

    auto *p_LAr = new G4PVPlacement(0,                      // Rotation matrix
                                    G4ThreeVector(0, 0, 0), // Position
                                    l_LAr,                  // Logical volume
                                    "Liquid Argon",         // its name
                                    l_world,                // its mother volume
                                    true,                   // no boolean operation
                                    0,                      // copy number
                                    true);
}

// Create the bases and place the XArapuca cell between them. I'll call "module" the cell and the bases.
void Detector::PlaceModuleBases(G4ThreeVector origin,
                                G4LogicalVolume *mother_volume)
{
    // Bases ----------------------------------------------------------------------------------------------------------

    // Geometric constants
    G4float base_radius = 14.5 * cm;
    G4float base_height = 2 * cm;

    G4ThreeVector position1 = G4ThreeVector(0, 0, 0.5 * (arapuca_cell_height + base_height));
    G4ThreeVector position2 = G4ThreeVector(0, 0, -0.5 * (arapuca_cell_height + base_height));

    // Geometry

    G4Tubs *base_cylinder = new G4Tubs("arapuca:base:solid",
                                       0, 0.5 * base_radius,
                                       0.5 * base_height,
                                       0.0 * deg, 360 * deg);

    G4LogicalVolume *l_base = new G4LogicalVolume(base_cylinder, PVC, "arapuca:base:logical");

    G4VPhysicalVolume *p_base1 = new G4PVPlacement(0,             // no rotation
                                                   position1,     // at position
                                                   l_base,        // its logical volume
                                                   "Upper base",  // its name
                                                   mother_volume, // its mother  volume
                                                   true,          // no boolean operation
                                                   0,             // copy number
                                                   true);

    G4VPhysicalVolume *p_base2 = new G4PVPlacement(0,             // no rotation
                                                   position2,     // at position
                                                   l_base,        // its logical volume
                                                   "Lower base",  // its name
                                                   mother_volume, // its mother  volume
                                                   true,          // no boolean operation
                                                   0,             // copy number
                                                   true);

    G4VisAttributes *base_vis = new G4VisAttributes(G4Colour(.0, .0, 0.6));
    l_base->SetVisAttributes(base_vis);
}

void Detector::PlaceSupportWithSource()
{
    // Geometric constants
    G4float support_thickness = 1.0 * cm;
    G4float support_width = 5.0 * cm;

    G4float aluminum_disk_diameter = 1.0 * cm;
    G4float aluminum_disk_thickness = 140 * um;

    G4RotationMatrix *aluminum_disk_rotation = new G4RotationMatrix();
    aluminum_disk_rotation->rotateY(-90 * degree);

    // First create the support
    auto *base_support = new G4Box("source:support:base", 0.5 * support_thickness, 0.5 * support_width, 0.5 * arapuca_cell_height);

    // Second create the aluminum disk
    auto *aluminum_disk = new G4Tubs("aluminio_alpha", 0, 0.5 * aluminum_disk_diameter, 0.5 * aluminum_disk_thickness, 0.0 * deg, 360 * deg);

    // Subtracting the disk from the support
    auto *subtraction_disk = new G4Tubs("subtraction_disk", 0, 0.5 * aluminum_disk_diameter, 0.6 * aluminum_disk_thickness, 0.0 * deg, 360 * deg);
    auto s_support = new G4SubtractionSolid("source:support:solid", base_support, subtraction_disk, aluminum_disk_rotation,
                                            G4ThreeVector(0.5 * (support_thickness + 0.9 * aluminum_disk_thickness), 0, z_source - 0.5 * arapuca_cell_height));

    // Placing the support
    auto *l_support = new G4LogicalVolume(s_support, PVC, "source:support:logical");
    auto *p_support = new G4PVPlacement(0,                              // no rotation
                                        G4ThreeVector(-x_source, 0, 0), // at position
                                        l_support,                      // its logical volume
                                        "Support",                      // its name
                                        l_LAr,                          // its mother  volume
                                        true,                           // no boolean operation
                                        0,                              // copy number
                                        true);

    // Now we place the aluminum at height z_source
    auto *l_aluminum_disk = new G4LogicalVolume(aluminum_disk, Aluminum, "source:aluminum_disk:logical");
    auto *p_aluminumm_disk = new G4PVPlacement(aluminum_disk_rotation,                                                                                                  // no rotation
                                               G4ThreeVector(-x_source + 0.5 * (support_thickness - aluminum_disk_thickness), 0, z_source - 0.5 * arapuca_cell_height), // at position
                                               l_aluminum_disk,                                                                                                         // its logical volume
                                               "Aluminum disk",                                                                                                         // its name
                                               l_LAr,                                                                                                                   // its mother  volume
                                               true,                                                                                                                    // no boolean operation
                                               0,                                                                                                                       // copy number
                                               true);

    G4VisAttributes *support_vis = new G4VisAttributes(G4Colour(.8, .8, .8));
    l_support->SetVisAttributes(support_vis);

    G4VisAttributes *disk_vis = new G4VisAttributes(G4Colour(.5, .5, 0.5));
    l_aluminum_disk->SetVisAttributes(disk_vis);
}

G4VPhysicalVolume *Detector::Construct()
{
    // Generating world ----------------------------------------------------------------------------------------------------------

    // Geometric constants
    G4float world_size = 1 * m;

    // Geometry
    auto *world = new G4Box("world:solid", world_size, world_size, world_size);

    auto *l_world = new G4LogicalVolume(world, LAr, "World");

    auto *p_world = new G4PVPlacement(0,                      // Rotation matrix
                                      G4ThreeVector(0, 0, 0), // Position
                                      l_world,                // Logical volume
                                      "World",                // its name
                                      0,                      // its mother volume
                                      true,                   // no boolean operation
                                      0,                      // copy number
                                      true);

    // Generating the cryostat ----------------------------------------------------------------------------------------------------------
    PlaceCryostat(G4ThreeVector(0, 0, 0), l_world);

    // Filling up the cryostat with LAr -----------------------------------------------------------------------------
    FillCryostatWithLAr(l_world);

    // Placing the X-ARAPUCA module into the LAr -----------------------------------------------------------------------------
    PlaceXArapucaCell(G4ThreeVector(0, 0, 0));
    PlaceModuleBases(G4ThreeVector(0, 0, 0), l_LAr);

    // Building the alpha source support and the source itself -------------------------------------------------------------------------------------------------
    PlaceSupportWithSource();

    // The alpha source itself -------------------------------------------------------------------------------------------
    return p_world;
}