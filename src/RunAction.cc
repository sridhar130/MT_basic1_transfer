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

RunAction::RunAction(const G4String& fname)
  : G4UserRunAction(),
    fOutputFileName(fname)
{
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
  G4String fileName = "hits.root";
  man->OpenFile(fOutputFileName);   // or set filename from ActionInitialization
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
