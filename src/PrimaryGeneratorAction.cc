#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
:fParticleGun(0)
{
    fParticleGun  = new G4ParticleGun(1);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("proton");
    fParticleGun->SetParticleDefinition(particle);
    
    fEnergy = 67.5 * MeV;
    fSpread = 0.2516509037890427;
    fBeamX = 0.5 * cm;
    fBeamY = 1.0 * cm;
    fPosZ = -5.0 * mm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4double posX, posY, kEnergy;
    
    G4bool resample = true;
    while (resample) {
        posX = CLHEP::RandFlat::shoot(-0.5*fBeamX, 0.5*fBeamX);
        posY = CLHEP::RandFlat::shoot(-0.5*fBeamY, 0.5*fBeamY);
        if ( (posX*posX)/(fBeamX*fBeamX/4) + (posY*posY)/(fBeamY*fBeamY/4) < 1.0)
            resample = false;
    }
    
    kEnergy = CLHEP::RandGauss::shoot(fEnergy, fSpread);
    fParticleGun->SetParticleEnergy(kEnergy);
    fParticleGun->SetParticlePosition(G4ThreeVector(posX, posY, fPosZ));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
