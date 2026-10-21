#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
:fDetector(det),
fDetDir(0), fThickCmd(0), fUpdateCmd(0)
{
    
    fDetDir = new G4UIdirectory("/UCSFRawBeam/ScatteringFoil/");
    fDetDir->SetGuidance("detector construction commands");
    
    fThickCmd = new G4UIcmdWithADoubleAndUnit("/UCSFRawBeam/Geometry/ScatteringFoil/SetThickness",this);
    fThickCmd->SetGuidance("Set Thickness of the Scattering Foil");
    fThickCmd->SetParameterName("thickness",false);
    fThickCmd->SetRange("thickness>0.");
    fThickCmd->SetUnitCategory("Length");
    
    fUpdateCmd = new G4UIcmdWithoutParameter("/UCSFRawBeam/Geometry/Update",this);
    fUpdateCmd->SetGuidance("Update geometry.");
    fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
    fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
    delete fThickCmd;
    delete fUpdateCmd;
    delete fDetDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if ( command == fThickCmd )
        fDetector->SetThicknessFoil(fThickCmd->GetNewDoubleValue(newValue));
    
    if  ( command == fUpdateCmd )
        fDetector->UpdateGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
