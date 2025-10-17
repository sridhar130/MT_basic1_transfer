#include "PrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4AutoLock.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0), 
    fEnvelopeBox(0)
{

  // G4AutoLock lock(&PrimGenMutex);
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
 
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* mun = particleTable->FindParticle("mu-");
  G4ParticleDefinition* mup = particleTable->FindParticle("mu+");
  //  fParticleGun->SetParticleDefinition(particle);
 

  // muon generation position 
  G4double x0 = 142.0 * (G4UniformRand()-0.5);
  G4double y0 = 142.0 * (G4UniformRand()-0.5);
  G4double z0 = -150.*cm; //-41.0 cm
  fParticleGun->SetParticlePosition(G4ThreeVector(x0*cm,y0*cm,z0));


  // --- Cosmic-like muon direction generator ---

  G4double thetaMin = 0.*deg;
  G4double thetaMax = 70.*deg;
  G4double psiMin = 0.*deg;
  G4double psiMax = 360.*deg;

  // cos^2(theta) weighting
  G4double u = G4UniformRand();
  G4double cosTheta = std::pow(u, 1.0/3.0);
  G4double theta = std::acos(cosTheta);
  G4double psi = psiMin + G4UniformRand()*(psiMax - psiMin);

  G4double sinTheta = std::sin(theta);
  G4double ux = sinTheta * std::cos(psi);
  G4double uy = sinTheta * std::sin(psi);
  G4double uz = cosTheta;

  // Optional energy (E^-2.7 spectrum)
  G4double Emin = 1.*GeV, Emax = 100.*GeV;
  G4double r = G4UniformRand();
  G4double energy = std::pow((std::pow(Emax, -1.7) -
			      r*(std::pow(Emax, -1.7) - std::pow(Emin, -1.7))), -1./1.7);

  // Set in gun
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux, uy, uz));
  fParticleGun->SetParticleEnergy(energy);
  //  fParticleGun->SetParticleDefinition(G4MuonMinus::Definition());

  // Charge ratio (energy-dependent)
  G4double chargeRatio = 1.25 + 0.15 * std::log10(energy / GeV);
  G4double muPlusFraction = chargeRatio / (1. + chargeRatio);
  G4double rCharge = G4UniformRand();

  if (rCharge < muPlusFraction)
    fParticleGun->SetParticleDefinition(mup);
  else
    fParticleGun->SetParticleDefinition(mun);
  
  fParticleGun->GeneratePrimaryVertex(anEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

