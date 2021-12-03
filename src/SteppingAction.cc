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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include <vector>
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),fScoringVolume1(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume && !fScoringVolume1) { 
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
    fScoringVolume1 = detectorConstruction->GetScoringVolume1();
    
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();


      
  // check if we are in scoring volume
  if (volume == fScoringVolume || volume==fScoringVolume1) //return;
    {

  //fEventAction->AddEdep(edepStep);
  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
  G4double posX = pos.x();
  G4double posY = pos.y();
  G4double posZ = pos.z();
  
  G4int pid =  step->GetTrack()->GetDefinition()->GetPDGEncoding();
  // G4int cop =  step->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber();
   G4double KE = step->GetTrack()->GetDynamicParticle()->GetKineticEnergy();
    
  if (pid ==13 || pid ==-13)
    {
      // cout<<" x = "<<posX<<" y = "<<posY<<" z = "<<posZ<<endl;
      fEventAction->SetKE=KE;
      fEventAction->SetPosZ.push_back(posZ);
      if (posZ <-387.0 && posZ >-393.0)
	{
      fEventAction->SetPosXR1=posX;
      fEventAction->SetPosYR1=posY;
      fEventAction->SetPosZR1=posZ;
	}
     if (posZ <-317.0 && posZ >-323.0)
	{
      fEventAction->SetPosXR2=posX;
      fEventAction->SetPosYR2=posY;
      fEventAction->SetPosZR2=posZ;
	}
     if (posZ <-247.0 && posZ >-253.0)
	{
      fEventAction->SetPosXR3=posX;
      fEventAction->SetPosYR3=posY;
      fEventAction->SetPosZR3=posZ;
	}
     if (posZ >247.0 && posZ <253.0)
	{
      fEventAction->SetPosXR4=posX;
      fEventAction->SetPosYR4=posY;
      fEventAction->SetPosZR4=posZ;
	}
     if (posZ >317.0 && posZ <323.0)
       {
	 fEventAction->SetPosXR5=posX;
	 fEventAction->SetPosYR5=posY;
	 fEventAction->SetPosZR5=posZ;
       }
     if (posZ >387.0 && posZ <393.0)
       {
	 fEventAction->SetPosXR6=posX;
	 fEventAction->SetPosYR6=posY;
	 fEventAction->SetPosZR6=posZ;
       }
    
     
    }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

