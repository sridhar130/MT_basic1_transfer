//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

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
  G4ParticleDefinition* particle
    = particleTable->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particle);
 
 



    // muon generation position 
    G4double x0 = 142.0 * (G4UniformRand()-0.5);
    G4double y0 = 142.0 * (G4UniformRand()-0.5);
    G4double z0 = -41.*cm;
    fParticleGun->SetParticlePosition(G4ThreeVector(x0*cm,y0*cm,z0));



  //direction uniform in solid angle
  //

  //solid angle
  //
  G4double alphaMin =  0*deg;      //alpha in [0,pi]
  G4double alphaMax = 10*deg;
  G4double fCosAlphaMin = std::cos(alphaMin);
  G4double fCosAlphaMax = std::cos(alphaMax);
  
  G4double fPsiMin = 80*deg;       //psi in [0, 2*pi]
  G4double fPsiMax = 100*deg;
  
  G4double cosAlpha = fCosAlphaMin-G4UniformRand()*(fCosAlphaMin-fCosAlphaMax);
  G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
  G4double psi = fPsiMin + G4UniformRand()*(fPsiMax - fPsiMin);

  G4double ux = sinAlpha*std::cos(psi);
  G4double uy = sinAlpha*std::sin(psi);
  G4double uz = cosAlpha;

  G4double pz = 0.9+ 0.09*G4UniformRand();
  G4double pm = sqrt(1-pz*pz);
G4double px = pm/sqrt(2.0)+0.01*(G4UniformRand()-0.5);
G4double py = sqrt(pm*pm-px*px);


 // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz)); //ux

  G4double EN =750.0+500.0*(G4UniformRand()-0.5); // lets assume a small uniform variation 500-1000 MeV


   fParticleGun->SetParticleEnergy(EN *MeV);  
   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.0));
   fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

