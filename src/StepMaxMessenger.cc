#include "StepMaxMessenger.hh"
#include "StepMax.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::StepMaxMessenger(StepMax* stepM)
:fStepMax(stepM)
{
    fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/testem/step/waterPhantomStepMax",this);
    fStepMaxCmd->SetGuidance("Set max allowed step length");
    fStepMaxCmd->SetParameterName("mxStep",false);
    fStepMaxCmd->SetRange("mxStep>0.");
    fStepMaxCmd->SetUnitCategory("Length");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::~StepMaxMessenger()
{
    delete fStepMaxCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMaxMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fStepMaxCmd)
    { fStepMax->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));}
}

