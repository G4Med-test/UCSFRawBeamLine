#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
:fPhysicsList(pPhys)
{
    fPhysDir = new G4UIdirectory("/UCSFRawBeam/PhysicsList/");
    fPhysDir->SetGuidance("Commands to activate physics models and set cuts");
    
    fListCmd = new G4UIcmdWithAString("/UCSFRawBeam/PhysicsList/AddPhysics",this);
    fListCmd->SetGuidance("Add physics list.");
    fListCmd->SetParameterName("PList",false);
    fListCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    fCutCmd = new G4UIcmdWithADoubleAndUnit("/UCSFRawBeam/PhysicsList/CutForRegion/BeamPlug",this);
    fCutCmd->SetGuidance("Change the production cut for the beam plug's region");
    fCutCmd->SetParameterName("cut",false);
    fCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    fCutWaterCmd = new G4UIcmdWithADoubleAndUnit("/UCSFRawBeam/PhysicsList/CutForRegion/WaterPhantom",this);
    fCutWaterCmd->SetGuidance("Change the production cut for the water phantom's region");
    fCutWaterCmd->SetParameterName("cut",false);
    fCutWaterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
    delete fPhysDir;
    delete fListCmd;
    delete fCutCmd;
    delete fCutWaterCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                       G4String newValue)
{
    if( command == fListCmd )
        fPhysicsList->AddPhysicsList(newValue);
    else if ( command == fCutCmd )
        fPhysicsList->SetCutsForRegion(fCutCmd->GetNewDoubleValue(newValue));
    else if ( command == fCutWaterCmd )
        fPhysicsList->SetCutsForRegionWater(fCutWaterCmd->GetNewDoubleValue(newValue));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
