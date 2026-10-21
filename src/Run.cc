#include "Run.hh"

#include "EventAction.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Material.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::Run()
: G4Run(),
fParticle(0), fEkin(0.),
fEdeposit(0.),  fEdeposit2(0.)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::~Run()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::SetPrimary (G4ParticleDefinition* particle, G4double energy)
{
    fParticle = particle;
    fEkin     = energy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddEdep (G4double e)
{
    fEdeposit  += e;
    fEdeposit2 += e*e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::Merge(const G4Run* run)
{
    const Run* localRun = static_cast<const Run*>(run);
    
    // pass information about primary particle
    fParticle = localRun->fParticle;
    fEkin     = localRun->fEkin;
    
    // accumulate sums
    fEdeposit   += localRun->fEdeposit;
    fEdeposit2  += localRun->fEdeposit2;
    
    G4Run::Merge(run);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::EndOfRun()
{
    std::ios::fmtflags mode = std::cout.flags();
    std::cout.setf(std::ios::fixed,std::ios::floatfield);
    G4int prec = std::cout.precision(2);
    
    //run conditions
    //
    G4String partName = fParticle->GetParticleName();
    
    std::cout << "\n ======================== run summary =====================\n";
    std::cout
    << "\n The run is " << numberOfEvent << " "<< partName << " of "
    << G4BestUnit(fEkin,"Energy") << std::endl;
    
    if (numberOfEvent == 0) {
        std::cout.setf(mode,std::ios::floatfield);
        std::cout.precision(prec);
        return;
    }
    
    fEdeposit /= numberOfEvent; fEdeposit2 /= numberOfEvent;
    G4double rms = fEdeposit2 - fEdeposit*fEdeposit;
    if (rms>0.) rms = std::sqrt(rms); else rms = 0.;
    
    std::cout.precision(3);
    std::cout
    << "\n Total Energy deposited        = " << G4BestUnit(fEdeposit,"Energy")
    << " +- "                                << G4BestUnit( rms,"Energy")
    << std::endl;
    
    // normalize histograms of longitudinal energy profile
    //
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4int ih = 1;
    G4double binWidth = analysisManager->GetH1Width(ih);
    G4double fac = (1./(numberOfEvent*binWidth))*(mm/MeV);
    analysisManager->ScaleH1(ih,fac);
    
    // reset default formats
    std::cout.setf(mode,std::ios::floatfield);
    std::cout.precision(prec);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
