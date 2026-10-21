#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4RegionStore.hh"
#include "G4VisAttributes.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:fScorerLength(0), fScorerBins(0),
fMaterial_World(0), fMaterial_ScatteringFoil(0), fMaterial_Collimator1(0),
fMaterial_BeamPlug(0), fMaterial_BeamPipe(0), fMaterial_Box(0),
fMaterial_SEM(0), fMaterial_Collimator3(0), fMaterial_ExitWindow(0), fMaterial_AirBlock(0),
fMaterial_WaterTank(0), fMaterial_WaterPhantom(0),
 fPhysicalWorld(0), fDetectorMessenger(0)
{
    // materials
    DefineMaterials();
    
    // geometry
    GeometryParameters();
    
    // Commands
    fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
    G4double a, z, density;
    G4int ncomponents, natoms;
    G4double fractionmass;
    
    // define Elements
    G4Element* H  = new G4Element("Hydrogen", "H",  z= 1, a=   1.0079*g/mole);
    G4Element* C  = new G4Element("Carbon",   "C",  z= 6, a=  12.0107*g/mole);
    G4Element* N  = new G4Element("Nitrogen", "N",  z= 7, a=  14.0067*g/mole);
    G4Element* O  = new G4Element("Oxygen",   "O",  z= 8, a=  15.9994*g/mole);
    G4Element* Ar = new G4Element("Argon",    "Ar", z=18, a=  39.9480*g/mole);
    //G4Element* Ta = new G4Element("Tantalum", "Ta", z=73, a= 180.9479*g/mole);
    G4Element* Al = new G4Element("Aluminium","Al", z=13, a=  26.98154*g/mole);
    G4Element* Si = new G4Element("Silicon",  "Si", z=14, a=  28.085*g/mole);
    G4Element* S  = new G4Element("Sulfur",   "S",  z=16, a=  30.08*g/mole);
    G4Element* P  = new G4Element("Phosphorus","P", z=15, a=  30.973*g/mole);
    G4Element* Cr = new G4Element("Crhomium",  "Cr",z=24, a=  51.9961*g/mole);
    G4Element* Mn = new G4Element("Magnanese", "Mn",z=25, a=  54.938*g/mole);
    G4Element* Fe = new G4Element("Iron",      "Fe",z=26, a=  55.845*g/mole);
    G4Element* Ni = new G4Element("Nickel",    "Ni",z=28, a=  58.6934*g/mole);
    
    // Air
    G4Material* Air = new G4Material("Air", density= 1.20479*mg/cm3, ncomponents=4,
                                     kStateGas, 293.*kelvin, 1.*atmosphere);
    Air->AddElement(C, fractionmass=0.000124);
    Air->AddElement(N, fractionmass=0.755267);
    Air->AddElement(O, fractionmass=0.231781);
    Air->AddElement(Ar,fractionmass=0.012827);
    Air->GetIonisation()->SetMeanExcitationEnergy(85.7*eV);
    
    // Aluminium
    G4Material* Aluminium = new G4Material("Aluminium", density= 2.6989*g/cm3, ncomponents=1);
    Aluminium->AddElement(Al, fractionmass=1.0);
    
    // Carbon
    G4Material* Carbon = new G4Material("Carbon", density= 1.867*g/cm3, ncomponents=1);
    Carbon->AddElement(C, fractionmass=1.0);
    Carbon->GetIonisation()->SetMeanExcitationEnergy(78*eV);
    
    // Kapton
    G4Material* Kapton = new G4Material("Kapton", density=1.420*g/cm3, ncomponents=4);
    Kapton->AddElement(H, fractionmass=0.026362);
    Kapton->AddElement(C, fractionmass=0.691133);
    Kapton->AddElement(N, fractionmass=0.073270);
    Kapton->AddElement(O, fractionmass=0.209235);
    
    // Mylar
    G4Material* Mylar = new G4Material("Mylar", density= 1.40*g/cm3, ncomponents=3);
    Mylar->AddElement(H, fractionmass=0.041959);
    Mylar->AddElement(C, fractionmass=0.625017);
    Mylar->AddElement(O, fractionmass=0.333025);
    
    // Silicon
    G4Material* Silicon = new G4Material("Silicon", density= 2.33*g/cm3, ncomponents=1);
    Silicon->AddElement(Si, fractionmass=1.0);
    Silicon->GetIonisation()->SetMeanExcitationEnergy(173.0*eV);
    
    // Stainless steel
    G4Material* Steel = new G4Material("Steel", density=8.027*g/cm3, ncomponents=8);
    Steel->AddElement(C, fractionmass=0.0015);
    Steel->AddElement(Si,fractionmass=0.01);
    Steel->AddElement(P, fractionmass=0.00045);
    Steel->AddElement(S, fractionmass=0.0003);
    Steel->AddElement(Cr,fractionmass=0.19);
    Steel->AddElement(Mn,fractionmass=0.02);
    Steel->AddElement(Fe,fractionmass=0.67775);
    Steel->AddElement(Ni,fractionmass=0.1);
    
    // Tantalum
    G4Material* Tantalum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ta");
    
    // Vacuum
    G4Material* Vacuum = new G4Material("Vacuum", density=1e-25*g/cm3, ncomponents=4,
                                        kStateGas, 2.73*kelvin, 3.0e-18*pascal);
    Vacuum->AddElement(C, fractionmass=0.000124);
    Vacuum->AddElement(N, fractionmass=0.755268);
    Vacuum->AddElement(O, fractionmass=0.231781);
    Vacuum->AddElement(Ar,fractionmass=0.012827);
    
    // Water
    //G4Material* Water = new G4Material("Water", density= 0.998*g/cm3, ncomponents=2);
    G4Material* Water = new G4Material("Water", density= 1.0*g/cm3, ncomponents=2);
    Water->AddElement(H, natoms=2);
    Water->AddElement(O, natoms=1);
    Water->GetIonisation()->SetMeanExcitationEnergy(78*eV);
    //Water->GetIonisation()->SetMeanExcitationEnergy(83*eV); //78*eV);
    
    // assign materials
    fMaterial_World       = Vacuum;
    fMaterial_ScatteringFoil = Tantalum;
    fMaterial_Collimator1 = Carbon;
    fMaterial_BeamPlug    = Steel;
    fMaterial_BeamPipe    = Aluminium;
    fMaterial_Collimator2 = Steel;
    fMaterial_Box         = Aluminium;
    fMaterial_SEM         = Aluminium;
    fMaterial_Collimator3 = Steel;
    fMaterial_ExitWindow  = Kapton;
    fMaterial_AirBlock    = Air;
    fMaterial_WaterTank   = Mylar;
    fMaterial_WaterPhantom = Water;
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::GeometryParameters()
{
    fZback_ScatteringFoil = 0.0 * mm;
    fZback_Collimator1 = 30 * mm;
    fZback_BeamPlug = 710 * mm;
    fZback_BeamPipe = 3710 * mm;
    fZback_Collimator2 = 4040 * mm;
    fZback_Box_Plate1 = 4080 * mm;
    fZback_Box_Box = 4090 * mm;
    fZback_Box_Plate2 = 4580 * mm;
    fZback_SEM1 = 4140 * mm;
    fZback_SEM2 = 4165.4 * mm;
    fZback_SEM3 = 4190.8 * mm;
    fZback_Collimator3 = 4590 * mm;
    fZback_ExitWindow = 4640 * mm;
    fZback_AirBlock = 4640.127 * mm;
    fZback_WaterTank_Window = 4940 * mm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
    // Cleanup old geometry
    //
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    G4double length, zpos;
    
    // World
    G4Box* solid_World = new G4Box("World",
                                   30*cm,
                                   30*cm,
                                   600*cm);
    
    G4LogicalVolume* logical_World = new G4LogicalVolume(solid_World,
                                                         fMaterial_World,
                                                         "World");
    fPhysicalWorld = new G4PVPlacement(0,
                                       G4ThreeVector(),
                                       logical_World,
                                       "World",
                                       0,
                                       false,
                                       0);
    
    G4VisAttributes * visWorld = new G4VisAttributes(G4Colour(0.,1.,1.));
    visWorld->SetVisibility(false);
    logical_World->SetVisAttributes(visWorld);
    
    // Scattering foil
    length = fThickness_Foil;
    G4Tubs* solid_Foil = new G4Tubs("Foil",
                                    0*cm,
                                    101.6*mm,
                                    length/2.0,
                                    0*deg,
                                    360*deg);
    
    G4LogicalVolume* logical_Foil = new G4LogicalVolume(solid_Foil,
                                                        fMaterial_ScatteringFoil,
                                                        "Foil");
    
    zpos = fZback_ScatteringFoil + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_Foil,
                      "Foil",
                      logical_World,
                      false,
                      0);
    
    // Collimator1
    length = 25.4 * mm;
    G4Tubs* solid_Collimator1 = new G4Tubs("Collimator1",
                                           4.76*mm,
                                           51.0*mm,
                                           length/2.0,
                                           0*deg,
                                           360*deg);
    
    G4LogicalVolume* logical_Collimator1 = new G4LogicalVolume(solid_Collimator1,
                                                               fMaterial_Collimator1,
                                                               "Collimator1");
    
    zpos = fZback_Collimator1 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_Collimator1,
                      "Collimator1",
                      logical_World,
                      false,
                      0);
    
    
    // Beam plug
    length = 3000 * mm;
    G4Tubs* solid_BeamPlug = new G4Tubs("BeamPlug",
                                        63.5*mm,
                                        70.0*mm,
                                        length/2.0,
                                        0*deg,
                                        360*deg);
    
    G4LogicalVolume* logical_BeamPlug = new G4LogicalVolume(solid_BeamPlug,
                                                            fMaterial_BeamPlug,
                                                            "BeamPlug");
    
    zpos = fZback_BeamPlug + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_BeamPlug,
                      "BeamPlug",
                      logical_World,
                      false,
                      0);
    
    // BeamPipe
    length = 330 * mm;
    G4Tubs* solid_BeamPipe = new G4Tubs("BeamPipe",
                                        50.8*mm,
                                        70.0*mm,
                                        length/2.0,
                                        0*deg,
                                        360*deg);
    
    G4LogicalVolume* logical_BeamPipe = new G4LogicalVolume(solid_BeamPipe,
                                                            fMaterial_BeamPipe,
                                                            "BeamPipe");
    
    zpos = fZback_BeamPipe + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_BeamPipe,
                      "BeamPipe",
                      logical_World,
                      false,
                      0);
    
    // Collimator2
    length = 40.0 * mm;
    G4Tubs* solid_Collimator2 = new G4Tubs("Collimator2",
                                           34.9*mm,
                                           70.0*mm,
                                           length/2.0,
                                           0*deg,
                                           360*deg);
    
    G4LogicalVolume* logical_Collimator2 = new G4LogicalVolume(solid_Collimator2,
                                                               fMaterial_Collimator2,
                                                               "Collimator2");
    
    zpos = fZback_Collimator2 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_Collimator2,
                      "Collimator2",
                      logical_World,
                      false,
                      0);
    
    // Box/Plate1
    G4double lengthXY = 280 * mm;
    length = 10 * mm;
    G4Box* solid_BoxPlate1 = new G4Box("BoxPlate1",
                                       lengthXY/2,
                                       lengthXY/2,
                                       length/2);
    
    G4LogicalVolume* logical_BoxPlate1 = new G4LogicalVolume(solid_BoxPlate1,
                                                             fMaterial_Box,
                                                             "BoxPlate1");
    
    zpos = fZback_Box_Plate1 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_BoxPlate1,
                      "BoxPlate1",
                      logical_World,
                      false,
                      0);
    
    G4Tubs* solid_BoxPlate1Hole = new G4Tubs("BoxPlate1Hole",
                                             0.0*mm,
                                             50.0*mm,
                                             length/2,
                                             0*deg,
                                             360*deg);
    
    G4LogicalVolume* logical_BoxPlate1Hole = new G4LogicalVolume(solid_BoxPlate1Hole,
                                                                 fMaterial_World,
                                                                 "BoxPlate1Hole");
    
    new G4PVPlacement(0,
                      G4ThreeVector(),
                      logical_BoxPlate1Hole,
                      "BoxPlate1Hole",
                      logical_BoxPlate1,
                      false,
                      0);
    
    // Box/Box
    lengthXY = 280 * mm;
    length = 490 * mm;
    G4Box* solid_BoxBox = new G4Box("BoxBox",
                                    lengthXY/2,
                                    lengthXY/2,
                                    length/2);
    
    G4LogicalVolume* logical_BoxBox = new G4LogicalVolume(solid_BoxBox,
                                                          fMaterial_Box,
                                                          "BoxBox");
    
    zpos = fZback_Box_Box + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_BoxBox,
                      "BoxBox",
                      logical_World,
                      false,
                      0);
    
    // Box/Box/Hole
    lengthXY = 267 * mm;
    length = 490 * mm;
    G4Box* solid_BoxHole = new G4Box("BoxHole",
                                     lengthXY/2,
                                     lengthXY/2,
                                     length/2.0);
    
    G4LogicalVolume* logical_BoxHole = new G4LogicalVolume(solid_BoxHole,
                                                           fMaterial_World,
                                                           "BoxHole");
    
    new G4PVPlacement(0,
                      G4ThreeVector(),
                      logical_BoxHole,
                      "BoxHole",
                      logical_BoxBox,
                      false,
                      0);
    
    // Box/Plate2
    lengthXY = 280 * mm;
    length = 10 * mm;
    G4Box* solid_BoxPlate2 = new G4Box("BoxPlate2",
                                       lengthXY/2,
                                       lengthXY/2,
                                       length/2);
    
    G4LogicalVolume* logical_BoxPlate2 = new G4LogicalVolume(solid_BoxPlate2,
                                                             fMaterial_Box,
                                                             "BoxPlate2");
    
    zpos = fZback_Box_Plate2 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_BoxPlate2,
                      "BoxPlate2",
                      logical_World,
                      false,
                      0);
    
    // Box/Plate2/Hole
    G4Tubs* solid_BoxPlate2Hole = new G4Tubs("BoxPlate2Hole",
                                             0.0*mm,
                                             50.0*mm,
                                             length/2,
                                             0*deg,
                                             360*deg);
    
    G4LogicalVolume* logical_BoxPlate2Hole = new G4LogicalVolume(solid_BoxPlate2Hole,
                                                                 fMaterial_World,
                                                                 "BoxPlate2Hole");
    
    new G4PVPlacement(0,
                      G4ThreeVector(),
                      logical_BoxPlate2Hole,
                      "BoxPlate2Hole",
                      logical_BoxPlate2,
                      false,
                      0);
    
    // SEM1
    length = 6.35*um;
    G4Tubs* solid_SEM1 = new G4Tubs("SEM1",
                                    0*mm,
                                    101.6*mm,
                                    length/2.0,
                                    0*deg,
                                    360*deg);
    
    G4LogicalVolume* logical_SEM1 = new G4LogicalVolume(solid_SEM1,
                                                        fMaterial_SEM,
                                                        "SEM1");
    
    zpos = fZback_SEM1 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_SEM1,
                      "SEM1",
                      logical_World,
                      false,
                      0);
    
    // SEM2
    G4Tubs* solid_SEM2 = new G4Tubs("SEM2",
                                    0*mm,
                                    101.6*mm,
                                    length/2.0,
                                    0*deg,
                                    360*deg);
    
    G4LogicalVolume* logical_SEM2 = new G4LogicalVolume(solid_SEM2,
                                                        fMaterial_SEM,
                                                        "SEM2");
    zpos = fZback_SEM2 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_SEM2,
                      "SEM2",
                      logical_World,
                      false,
                      0);
    
    // SEM 3
    G4Tubs* solid_SEM3 = new G4Tubs("SEM3",
                                    0*mm,
                                    101.6*mm,
                                    length/2.0,
                                    0*deg,
                                    360*deg);
    
    G4LogicalVolume* logical_SEM3 = new G4LogicalVolume(solid_SEM3,
                                                        fMaterial_SEM,
                                                        "SEM3");
    
    
    zpos = fZback_SEM3 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_SEM3,
                      "SEM3",
                      logical_World,
                      false,
                      0);
    
    // Collimator3
    length = 50.0 * mm;
    G4Tubs* solid_Collimator3 = new G4Tubs("Collimator3",
                                           44.4*mm,
                                           60.0*mm,
                                           length/2.0,
                                           0*deg,
                                           360*deg);
    
    G4LogicalVolume* logical_Collimator3 = new G4LogicalVolume(solid_Collimator3,
                                                               fMaterial_Collimator3,
                                                               "Collimator3");
    
    zpos = fZback_Collimator3 + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_Collimator3,
                      "Collimator3",
                      logical_World,
                      false,
                      0);
    
    // Exitwindow
    lengthXY = 280 * mm;
    length = 0.127 * mm;
    G4Box* solid_ExitWindow = new G4Box("ExitWindow",
                                        lengthXY/2,
                                        lengthXY/2,
                                        length/2);
    
    G4LogicalVolume* logical_ExitWindow = new G4LogicalVolume(solid_ExitWindow,
                                                              fMaterial_ExitWindow,
                                                              "ExitWindow");
    
    zpos = fZback_ExitWindow + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_ExitWindow,
                      "ExitWindow",
                      logical_World,
                      false,
                      0);
    
    // AirBlock
    lengthXY = 280 * mm;
    length = 299.873 * mm;
    G4Box* solid_AirBlock = new G4Box("AirBlock",
                                      lengthXY/2,
                                      lengthXY/2,
                                      length/2);
    
    G4LogicalVolume* logical_AirBlock = new G4LogicalVolume(solid_AirBlock,
                                                            fMaterial_AirBlock,
                                                            "AirBlock");
    
    zpos = fZback_AirBlock + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_AirBlock,
                      "AirBlock",
                      logical_World,
                      false,
                      0);
    
    // Tank's wall
    lengthXY = 280 * mm;
    length = 0.25 * mm;
    G4Box* solid_Wall = new G4Box("Wall",
                                  lengthXY/2,
                                  lengthXY/2,
                                  length/2);
    
    G4LogicalVolume* logical_Wall = new G4LogicalVolume(solid_Wall,
                                                        fMaterial_WaterTank,
                                                        "Wall");
    
    zpos = fZback_WaterTank_Window + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_Wall,
                      "Wall",
                      logical_World,
                      false,
                      0);
    
    
    // Phantom
    lengthXY = 280 * mm;
    length = 50.0 * mm;
    G4Box* solid_WaterPhantom = new G4Box("WaterPhantom",
                                          lengthXY/2,
                                          lengthXY/2,
                                          length/2);
    
    G4LogicalVolume* logical_WaterPhantom = new G4LogicalVolume(solid_WaterPhantom,
                                                                fMaterial_WaterPhantom,
                                                                "WaterPhantom");
    
    zpos = fZback_WaterTank_Window +  0.25*mm + length/2.0;
    new G4PVPlacement(0,
                      G4ThreeVector(0*mm,0*mm,zpos),
                      logical_WaterPhantom,
                      "WaterPhantom",
                      logical_World,
                      false,
                      0);
    
    // Scoring regin
    lengthXY = 5 * mm;
    length = 50 * mm;
    G4int bins = 1000;
    G4double width = length/bins;
    
    fScorerBins = bins;
    fScorerLength = length;
    
    G4Box* solid_ScoringRegion = new G4Box("ScoringRegion",
                                           lengthXY/2,
                                           lengthXY/2,
                                           width/2);
    
    G4LogicalVolume* logical_ScoringRegion = new G4LogicalVolume(solid_ScoringRegion,
                                                                 fMaterial_WaterPhantom,
                                                                 "ScoringRegion");
    
    new G4PVReplica("ScoringRegion",
                    logical_ScoringRegion,
                    logical_WaterPhantom,
                    kZAxis,
                    bins,
                    width,
                    0);
    
    // Regions: 1 for production cuts and 1 for scoring in SteppingAction
    G4Region* region = G4RegionStore::GetInstance()->FindOrCreateRegion("beamplugregion");
    logical_BeamPlug->SetRegion(region);
    region->AddRootLogicalVolume(logical_BeamPlug);
    
    G4Region* scoringregion = G4RegionStore::GetInstance()->FindOrCreateRegion("scoringregion");
    logical_ScoringRegion->SetRegion(scoringregion);
    scoringregion->AddRootLogicalVolume(logical_ScoringRegion);
    
    // Visualization attributes
    G4VisAttributes * visCarbon = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    visCarbon->SetForceSolid(true);
    
    G4VisAttributes * visSteel = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
    visSteel->SetForceSolid(true);
    
    G4VisAttributes * visAluminium = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    visAluminium->SetForceSolid(true);
    
    G4VisAttributes * visAluminiumH = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    
    G4VisAttributes * visWater = new G4VisAttributes(G4Colour(175.0/255,124.0/255,1.0));
    
    G4VisAttributes * visMylar = new G4VisAttributes(G4Colour(1,1,0));
    visMylar->SetForceSolid(true);
    
    G4VisAttributes * visKapton = new G4VisAttributes(G4Colour(241.0/255,224.0/255,0));
    visKapton->SetForceSolid(true);
    
    G4VisAttributes * visTantalum = new G4VisAttributes(G4Colour(0,1,0));
    visTantalum->SetForceSolid(true);
    
    logical_Foil->SetVisAttributes(visTantalum);
    logical_Collimator1->SetVisAttributes(visCarbon);
    logical_Collimator2->SetVisAttributes(visSteel);
    logical_Collimator3->SetVisAttributes(visSteel);
    logical_BeamPlug->SetVisAttributes(visSteel);
    logical_BeamPipe->SetVisAttributes(visAluminium);
    logical_BoxBox->SetVisAttributes(visAluminiumH);
    logical_SEM1->SetVisAttributes(visAluminium);
    logical_SEM2->SetVisAttributes(visAluminium);
    logical_SEM3->SetVisAttributes(visAluminium);
    logical_ExitWindow->SetVisAttributes(visKapton);
    logical_Wall->SetVisAttributes(visMylar);
    logical_WaterPhantom->SetVisAttributes(visWater);
    
    return fPhysicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetThicknessFoil(G4double val)
{
    fThickness_Foil = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double DetectorConstruction::GetScorerLength()
{
    return fScorerLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4int DetectorConstruction::GetScorerBins()
{
    return fScorerBins;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

