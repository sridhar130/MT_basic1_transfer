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
#include "G4AccumulableManager.hh"
#include "G4Timer.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::RunAction()
  : G4UserRunAction(),
    fEventCount(0),    // ✅ initialize accumulable
    fTimer(new G4Timer)
{

  G4AccumulableManager::Instance()->RegisterAccumulable(fEventCount);
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
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
  // mc info
  analysisManager->CreateNtuple("mcinfo", "mc data");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleIColumn("TrackID");
  analysisManager->CreateNtupleDColumn("keu");
  analysisManager->CreateNtupleDColumn("timeu");
  analysisManager->CreateNtupleDColumn("upx");
  analysisManager->CreateNtupleDColumn("upy");
  analysisManager->CreateNtupleDColumn("upz");
  analysisManager->CreateNtupleDColumn("kel");
  analysisManager->CreateNtupleDColumn("timel");
  analysisManager->CreateNtupleDColumn("lpx");
  analysisManager->CreateNtupleDColumn("lpy");
  analysisManager->CreateNtupleDColumn("lpz");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  fTimer->Start();
  fEventCount = 0;   // reset the accumulable
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  auto man = G4AnalysisManager::Instance();
  man->OpenFile("hits.root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  fTimer->Stop();
  G4int nofEvents = run->GetNumberOfEvent();
  G4AccumulableManager::Instance()->Merge();
  G4int totalAccepted = fEventCount.GetValue();
  G4double det_size = 140.0; // cm verify from DetectorConstruction
  G4double MuonExposure = nofEvents/(det_size*det_size*60); // in hrs
  
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
  G4cout << "\n==================================================\n";
  G4cout << " Run Summary:\n";
  G4cout << "  Total events processed: " << nofEvents << G4endl;
  G4cout << "  Useful muon tracks:     " << totalAccepted << G4endl;
  G4cout << "  Exposure in hrs:        " << MuonExposure << G4endl;
  G4cout << "### Run time: " << fTimer->GetRealElapsed() << " s (real), "
         << fTimer->GetSystemElapsed() << " s (CPU)" << G4endl;
  G4cout << "==================================================\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// G4Run* RunAction::GenerateRun() {
//     return new Run;
// }
