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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "TrackerHit.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
  : G4UserRunAction()
{
  // auto man = G4AnalysisManager::Instance();
  // man->SetVerboseLevel(1);
  // man->SetNtupleMerging(true); // important for MT
  // fNtupleId = man->CreateNtuple("Hits", "Hit per-event vectors");
  // // scalar columns:
  // man->CreateNtupleIColumn(fNtupleId, "eventID");
  // man->CreateNtupleIColumn(fNtupleId, "nHits");
  // // vector columns (bind to member vectors)
  // man->CreateNtupleDColumn(fNtupleId, "hitTime", fHitTime);
  // man->CreateNtupleDColumn(fNtupleId, "hitX", fHitX);
  // man->CreateNtupleDColumn(fNtupleId, "hitY", fHitY);
  // man->CreateNtupleDColumn(fNtupleId, "hitZ", fHitZ);
  // man->CreateNtupleIColumn(fNtupleId, "hitTrackID", fHitTrackID);
  // man->FinishNtuple();

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true); // important for MT
  analysisManager->CreateNtuple("TrackerHits", "Hit data");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleIColumn("TrackID");
  analysisManager->CreateNtupleIColumn("PlaneID");
  analysisManager->CreateNtupleIColumn("DetID");
  analysisManager->CreateNtupleDColumn("x_mm");
  analysisManager->CreateNtupleDColumn("y_mm");
  analysisManager->CreateNtupleDColumn("z_mm");
  analysisManager->CreateNtupleDColumn("time_ns");
  analysisManager->CreateNtupleDColumn("edep_keV");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  auto man = G4AnalysisManager::Instance();
  man->OpenFile("hits.root");   // or set filename from ActionInitialization
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;


  // const DetectorConstruction* detectorConstruction
  //  = static_cast<const DetectorConstruction*>
  //    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  // G4double dose = edep/mass;
  // G4double rmsDose = rms/mass;

  auto man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();      
  // Print
  //  
  if (IsMaster()) {
    G4cout
      << G4endl
      << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
      << G4endl
      << "--------------------End of Local Run------------------------";
  }
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// G4Run* RunAction::GenerateRun() {
//     return new Run;
// }
