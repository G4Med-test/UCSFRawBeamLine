#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

#include "globals.hh"

class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    DetectorConstruction();
    ~DetectorConstruction();
    
public:
    
    void SetThicknessFoil(G4double);
    virtual G4VPhysicalVolume* Construct();
    G4double    GetScorerLength();
    G4int       GetScorerBins();
    
    void UpdateGeometry();
    
public:
    
    G4double    GetThicknessFoil()       {return fThickness_Foil;};

    
    const G4VPhysicalVolume* GetPhysicalWorld() {return fPhysicalWorld;};
    
private:
    G4double    fThickness_Foil;
    G4double    fScorerLength;
    G4double    fScorerBins;
    
    G4double    fZback_ScatteringFoil;
    G4double    fZback_Collimator1;
    G4double    fZback_BeamPlug;
    G4double    fZback_BeamPipe;
    G4double    fZback_Collimator2;
    G4double    fZback_Box_Plate1;
    G4double    fZback_Box_Box;
    G4double    fZback_Box_Plate2;
    G4double    fZback_SEM1;
    G4double    fZback_SEM2;
    G4double    fZback_SEM3;
    G4double    fZback_Collimator3;
    G4double    fZback_ExitWindow;
    G4double    fZback_AirBlock;
    G4double    fZback_WaterTank_Window;
    
    G4Material*     fMaterial_World;
    G4Material*     fMaterial_ScatteringFoil;
    G4Material*     fMaterial_Collimator1;
    G4Material*     fMaterial_BeamPlug;
    G4Material*     fMaterial_BeamPipe;
    G4Material*     fMaterial_Collimator2;
    G4Material*     fMaterial_Box;
    G4Material*     fMaterial_SEM;
    G4Material*     fMaterial_Collimator3;
    G4Material*     fMaterial_ExitWindow;
    G4Material*     fMaterial_AirBlock;
    G4Material*     fMaterial_WaterTank;
    G4Material*     fMaterial_WaterPhantom;
    
    G4VPhysicalVolume* fPhysicalWorld;
    
    DetectorMessenger* fDetectorMessenger;
    
    void DefineMaterials();
    void GeometryParameters();
    G4VPhysicalVolume* ConstructVolumes();
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

