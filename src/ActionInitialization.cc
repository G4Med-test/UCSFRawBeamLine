#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "SteppingVerbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(DetectorConstruction* detector,
                                           PhysicsList* physics)
: G4VUserActionInitialization(),
fDetector(detector),
fPhysics(physics)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
    RunAction* runAction = new RunAction(fDetector, fPhysics, 0);
    SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
    PrimaryGeneratorAction* kinematics = new PrimaryGeneratorAction();
    SetUserAction(kinematics);
    
    SetUserAction(new RunAction(fDetector, fPhysics, kinematics));
    
    EventAction* eventAction = new EventAction();
    SetUserAction(eventAction);
    
    SetUserAction(new TrackingAction(fDetector));
    
    SetUserAction(new SteppingAction(fDetector, eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose* ActionInitialization::InitializeSteppingVerbose() const
{
    return new SteppingVerbose();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
