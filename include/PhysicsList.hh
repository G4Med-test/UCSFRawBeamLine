#ifndef  PhysicsList_h
#define  PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class StepMax;
class PhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class  PhysicsList: public G4VModularPhysicsList
{
public:
    
    PhysicsList();
    virtual ~ PhysicsList();
    
    void ConstructParticle();
    void SetCuts();
    void SetCutsForRegion(G4double);
    void SetCutsForRegionWater(G4double);
    void AddPhysicsList(const G4String& name);
    void ConstructProcess();
    void AddStepMax();
    void AddPackage(const G4String& name);
    
private:
    
    G4EmConfigurator fEmConf;
    
    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;
    G4double fCutForProton;
    G4double fCutForRegion;
    G4double fCutForRegionWater;
    
    G4String      fEmName;
    G4VPhysicsConstructor* fEmPhysicsList;
    G4VPhysicsConstructor* fDecay_List;
    
    std::vector<G4VPhysicsConstructor*>  fProtonPhys;
    
    PhysicsListMessenger* fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
