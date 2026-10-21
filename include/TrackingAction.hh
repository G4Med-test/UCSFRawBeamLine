#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "DetectorConstruction.hh"
#include "G4UserTrackingAction.hh"
#include "globals.hh"

class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {
    
public:
    TrackingAction(DetectorConstruction*);
    ~TrackingAction() {};
    
    virtual void PostUserTrackingAction(const G4Track*);
    
private:
    //DetectorConstruction* fDetector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
