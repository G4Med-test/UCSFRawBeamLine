#include "TrackingAction.hh"

#include "DetectorConstruction.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4Track.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(DetectorConstruction*)// det)
:G4UserTrackingAction()//,fDetector(det)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track*) // track)
{
    /*G4int trackID = track->GetTrackID();
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    Run* run
    = static_cast<Run*>(
                        G4RunManager::GetRunManager()->GetNonConstCurrentRun());*/
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

