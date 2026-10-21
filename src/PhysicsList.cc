#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
#include "StepMax.hh"

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"
#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmParameters.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4AutoDelete.hh"
#include "G4HadronInelasticQBBC.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList:: PhysicsList() : G4VModularPhysicsList()
{
    G4LossTableManager::Instance();
    defaultCutValue = 0.05*mm;
    fCutForGamma     = defaultCutValue;
    fCutForElectron  = defaultCutValue;
    fCutForPositron  = defaultCutValue;
    fCutForProton    = defaultCutValue;
    fCutForRegion = defaultCutValue;
    fCutForRegionWater = defaultCutValue;
    
    fMessenger = new  PhysicsListMessenger(this);
    
    SetVerboseLevel(1);
    
    
    // ******     Definition of defaults for the physics processes *****
    // ******     in case no physics is called by the macro file   *****
    //
    // Based on Zacharatou and Paganetti IEEE.
    
    // Elecromagnetic physics
    //
    fEmPhysicsList = new G4EmStandardPhysics_option3();
    fEmName = G4String("emstandard_opt3");
    
    // Hadronic physics
    //
    fProtonPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
    fProtonPhys.push_back( new G4DecayPhysics());
    fProtonPhys.push_back( new G4IonBinaryCascadePhysics());
    fProtonPhys.push_back( new G4HadronElasticPhysicsHP());
    fProtonPhys.push_back( new G4StoppingPhysics());
    
    // Decay physics
    fDecay_List = new G4DecayPhysics();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~ PhysicsList()
{
    delete fMessenger;
    delete fEmPhysicsList;
    delete fDecay_List;
    fProtonPhys.clear();
    
    for(size_t i=0; i<fProtonPhys.size(); i++) {
        delete fProtonPhys[i];
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  PhysicsList::ConstructParticle()
{
    fDecay_List -> ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  PhysicsList::ConstructProcess()
{
    // Transportation
    //
    AddTransportation();
    
    // Electromagnetic physics
    //
    fEmPhysicsList -> ConstructProcess();
    fEmConf.AddModels();
    
    // Hadronic physics
    //
    for(size_t i=0; i < fProtonPhys.size(); i++)
    {
        fProtonPhys[i] -> ConstructProcess();
    }
    
    AddStepMax();
    
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  PhysicsList::AddPhysicsList(const G4String& name)
{
    if (verboseLevel>1) {
        std::cout << "PhysicsList::AddPhysicsList: <" << name << ">" << std::endl;
    }
    if (name == fEmName) return;
    
    if (name == "standard_opt4") {
        fEmName = name;
        delete fEmPhysicsList;
        fProtonPhys.clear();
        fEmPhysicsList = new G4EmStandardPhysics_option4();
        G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
        
    } else if (name == "standard_opt3") {
        fEmName = name;
        delete fEmPhysicsList;
        fProtonPhys.clear();
        fEmPhysicsList = new G4EmStandardPhysics_option3();
        G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
    } else if (name == "livermore" ) {
        fEmName = name;
        delete fEmPhysicsList;
        fProtonPhys.clear();
        fEmPhysicsList = new G4EmLivermorePhysics();
        G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
    } else if (name == "penelope" ) {
        fEmName = name;
        delete fEmPhysicsList;
        fProtonPhys.clear();
        fEmPhysicsList = new G4EmPenelopePhysics();
        G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
    } else if (name == "proton_em_standard_opt4") {
        // QGSP_BIC_HP + EmStandard_opt4
        
        AddPhysicsList("standard_opt4");
        fProtonPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
        fProtonPhys.push_back( new G4DecayPhysics());
        fProtonPhys.push_back( new G4IonBinaryCascadePhysics());
        fProtonPhys.push_back( new G4HadronElasticPhysicsHP());
        fProtonPhys.push_back( new G4StoppingPhysics());
//        fProtonPhys.push_back( new G4RadioactiveDecayPhysics());
//        fProtonPhys.push_back( new G4HadronInelasticQBBC());
 
        std::cout << "proton_em_standard_opt4 PHYSICS LIST has been activated" << std::endl;
        
        
    } else if (name == "proton_em_standard_opt3") {
        // QGSP_BIC_HP + EmStandard_opt3
        
        AddPhysicsList("standard_opt3");
        fProtonPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
        fProtonPhys.push_back( new G4DecayPhysics());
        fProtonPhys.push_back( new G4IonBinaryCascadePhysics());
        fProtonPhys.push_back( new G4HadronElasticPhysicsHP());
        fProtonPhys.push_back( new G4StoppingPhysics());
        //fProtonPhys.push_back( new G4RadioactiveDecayPhysics());
        //fProtonPhys.push_back( new G4HadronInelasticQBBC());
        
        std::cout << "proton_em_standard_opt3 PHYSICS LIST has been activated" << std::endl;
    } else if (name == "proton_em_livermore") {
        // QGSP_BIC_HP + EmStandard_opt3
        
        AddPhysicsList("livermore");
        fProtonPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
        fProtonPhys.push_back( new G4DecayPhysics());
        fProtonPhys.push_back( new G4IonBinaryCascadePhysics());
        fProtonPhys.push_back( new G4HadronElasticPhysicsHP());
        fProtonPhys.push_back( new G4StoppingPhysics());
        //fProtonPhys.push_back( new G4RadioactiveDecayPhysics());
        //fProtonPhys.push_back( new G4HadronInelasticQBBC());
        
        std::cout << "proton_em_livermore PHYSICS LIST has been activated" << std::endl;
    } else if (name == "proton_em_penelope") {
        // QGSP_BIC_HP + EmStandard_opt3
        
        AddPhysicsList("penelope");
        fProtonPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
        fProtonPhys.push_back( new G4DecayPhysics());
        fProtonPhys.push_back( new G4IonBinaryCascadePhysics());
        fProtonPhys.push_back( new G4HadronElasticPhysicsHP());
        fProtonPhys.push_back( new G4StoppingPhysics());
        //fProtonPhys.push_back( new G4RadioactiveDecayPhysics());
        //fProtonPhys.push_back( new G4HadronInelasticQBBC());
        
        std::cout << "proton_em_penelope PHYSICS LIST has been activated" << std::endl;
          
    } else if (name == "hadron_em_standard_opt4") {
        // A'la hadrontherapy advanced example
        
        AddPhysicsList("standard_opt4");
        fProtonPhys.push_back( new G4DecayPhysics());
        fProtonPhys.push_back( new G4RadioactiveDecayPhysics());
        fProtonPhys.push_back( new G4IonBinaryCascadePhysics());
        fProtonPhys.push_back( new G4EmExtraPhysics());
        fProtonPhys.push_back( new G4HadronElasticPhysicsHP());
        fProtonPhys.push_back( new G4StoppingPhysics());
        fProtonPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
        
        std::cout << "hadron_em_standard_opt4 PHYSICS LIST has been activated" << std::endl;
    } else {
        std::cout << "PhysicsList::AddPhysicsList: <" << name << ">"
        << " is not defined"
        << std::endl;
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  PhysicsList::AddStepMax()
{
    // Step limitation seen as a process
    // This process must exist in all threads.
    //
    StepMax* stepMaxProcess  = new  StepMax();
    G4AutoDelete::Register( stepMaxProcess );
    
    auto theParticleIterator = GetParticleIterator();
    theParticleIterator->reset();
    while ((*theParticleIterator)()){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        
        if (stepMaxProcess->IsApplicable(*particle) && pmanager)
        {
            pmanager ->AddDiscreteProcess(stepMaxProcess);
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  PhysicsList::SetCuts()
{
    
    if (verboseLevel >0){
        std::cout << "PhysicsList::SetCuts:";
        std::cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << std::endl;
    }
    
    // Production thresholds for detector regions
    // The G4Regions, for which you want define a given cut via de macro command
    // '/run/setCutForRegion <G4Region name> <cut value>'
    // must be defined here
    //
    SetCutValue(fCutForGamma, "gamma");
    SetCutValue(fCutForElectron, "e-");
    SetCutValue(fCutForPositron, "e+");
    SetCutValue(fCutForProton, "proton");
    
    // Only Beam's plug volume is defined as G4Region
    G4Region* reg1 = G4RegionStore::GetInstance()->GetRegion("beamplugregion");
    G4ProductionCuts* cuts1 = new G4ProductionCuts;
    cuts1->SetProductionCut(fCutForRegion,idxG4ElectronCut);
    cuts1->SetProductionCut(fCutForRegion,idxG4GammaCut);
    cuts1->SetProductionCut(fCutForRegion,idxG4ProtonCut);
    cuts1->SetProductionCut(fCutForRegion,idxG4PositronCut);
    reg1->SetProductionCuts(cuts1);
    
    G4Region* reg2 = G4RegionStore::GetInstance()->GetRegion("scoringregion");
    G4ProductionCuts* cuts2 = new G4ProductionCuts;
    cuts2->SetProductionCut(fCutForRegionWater,idxG4ElectronCut);
    cuts2->SetProductionCut(fCutForRegionWater,idxG4GammaCut);
    cuts2->SetProductionCut(fCutForRegionWater,idxG4ProtonCut);
    cuts2->SetProductionCut(fCutForRegionWater,idxG4PositronCut);
    reg2->SetProductionCuts(cuts2);
    
    
   G4EmParameters::Instance()->SetMinEnergy(100.0*eV);
   G4EmParameters::Instance()->SetMaxEnergy(500.0*MeV);
   G4EmParameters::Instance()->SetNumberOfBinsPerDecade(20);
   G4EmParameters::Instance()->SetFluo(false);
   G4EmParameters::Instance()->SetAuger(false);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutsForRegion(G4double val)
{
    fCutForRegion = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutsForRegionWater(G4double val)
{
    fCutForRegionWater = val;
}

