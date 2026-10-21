#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
: fFileName("UCSFRaw")
{
    Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
    delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
    // Create or get analysis manager
    // The choice of analysis technology is done via selection of a namespace
    // in HistoManager.hh
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetFileName(fFileName);
    analysisManager->SetDefaultFileType(".root");
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetActivation(true);
    // enable inactivation of histograms
    
    // Define histograms start values
    const G4int kMaxHisto = 2;
    const G4String id[] = { "h0", "h1"};
    
    const G4String title[] =
    { "dummy",                                       //0
        "Energy deposit at depth",                //1
    };
    
    // Default values (to be reset via /analysis/h1/set command)
    G4int nbins = 100;
    G4double vmin = 1.;
    G4double vmax = 100.;
    
    // Create all histograms as inactivated
    // as we have not yet set nbins, vmin, vmax
    for (G4int k=0; k<kMaxHisto; k++) {
        G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
        analysisManager->SetH1Activation(ih, false);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
