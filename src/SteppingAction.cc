#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"

#include "G4RegionStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det, EventAction* event)
:G4UserSteppingAction(),fDetector(det), fEventAction(event)
{
    fScoringRegion = G4RegionStore::GetInstance()->FindOrCreateRegion("scoringregion");
    fBinWidth = fDetector->GetScorerLength()/fDetector->GetScorerBins();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return;
    
    
    if ( step->GetTrack()->GetVolume()->GetLogicalVolume()->GetRegion() == fScoringRegion ) {
        
        fBinWidth = fDetector->GetScorerLength()/fDetector->GetScorerBins(); // dirty fix to work in sigle-threaded mode
        //total energy deposit
        fEventAction->AddEdep(edep);
        
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        
        G4StepPoint* prePoint  = step->GetPreStepPoint();
        const G4TouchableHandle touchable = prePoint->GetTouchableHandle();
        G4int copyID = touchable->GetCopyNumber();
        
        analysisManager->FillH1(1, copyID*fBinWidth, edep);
    }
    else {
        return;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


