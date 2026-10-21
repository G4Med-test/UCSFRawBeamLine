#include "RunAction.hh"
#include "PhysicsList.hh"
#include "StepMax.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "Run.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EmCalculator.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction*, PhysicsList*,
                     PrimaryGeneratorAction* kin)
:G4UserRunAction(),fPrimary(kin),fRun(0),
fHistoManager(0)
{
    // Book predefined histograms
    fHistoManager = new HistoManager(); 
    stepMax =0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  //  delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{
    fRun = new Run();
    return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
    // save Rndm status
    ////G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    if (isMaster)  G4Random::showEngineStatus();
    
    // keep run condition
    if ( fPrimary ) {
        G4ParticleDefinition* particle
        = fPrimary->GetParticleGun()->GetParticleDefinition();
        G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
        fRun->SetPrimary(particle, energy);
    }
    
    //histograms
    //
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if ( analysisManager->IsActive() ) {
        analysisManager->OpenFile();
    }
    
    if (!fPrimary) return;
    
    //set StepMax from histos 1 and 8
    //
    stepMax = DBL_MAX;
    G4int ih = 1;
    if (analysisManager->GetH1Activation(ih)) {
        stepMax = analysisManager->GetH1Width(ih);
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
    if (isMaster) fRun->EndOfRun();
    
    // save histograms
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if ( analysisManager->IsActive() ) {
        analysisManager->Write();
        analysisManager->CloseFile();
    }
    
    // show Rndm status
    if (isMaster) G4Random::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
