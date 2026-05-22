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
#include <sstream>
#include <iomanip>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::RunAction()
  : G4UserRunAction(),
    fEventCount(0),    //
    fTimer(new G4Timer)
{

  G4AccumulableManager::Instance()->RegisterAccumulable(fEventCount);
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  analysisManager->CreateNtuple("TrackerHits", "Hit data");
  analysisManager->CreateNtupleIColumn("runID");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleIColumn("TrackID");
  analysisManager->CreateNtupleIColumn("ParentID");
  analysisManager->CreateNtupleIColumn("pdg");
  analysisManager->CreateNtupleIColumn("PlaneID");
  analysisManager->CreateNtupleIColumn("DetID");
  analysisManager->CreateNtupleDColumn("x_mm");
  analysisManager->CreateNtupleDColumn("y_mm");
  analysisManager->CreateNtupleDColumn("z_mm");
  analysisManager->CreateNtupleDColumn("time_ns");
  analysisManager->CreateNtupleDColumn("edep_keV");
  analysisManager->CreateNtupleDColumn("KE");
  analysisManager->FinishNtuple();
  // mc info
  analysisManager->CreateNtuple("mcinfo", "mc data"); 
  analysisManager->CreateNtupleIColumn("runID");     //0 
  analysisManager->CreateNtupleIColumn("eventID");   //1
  analysisManager->CreateNtupleDColumn("keu");       //2
  analysisManager->CreateNtupleDColumn("timeu");     //3
  analysisManager->CreateNtupleDColumn("upx");       //4
  analysisManager->CreateNtupleDColumn("upy");       //5
  analysisManager->CreateNtupleDColumn("upz");       //6
  analysisManager->CreateNtupleDColumn("kel");       //7
  analysisManager->CreateNtupleDColumn("timel");     //8
  analysisManager->CreateNtupleDColumn("lpx");       //9
  analysisManager->CreateNtupleDColumn("lpy");       //10
  analysisManager->CreateNtupleDColumn("lpz");       //11
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void RunAction::BeginOfRunAction(const G4Run* run)
{
  fTimer->Start();
  fEventCount = 0;  // reset the accumulable

  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  auto man = G4AnalysisManager::Instance();

  G4int runID = run->GetRunID();

  std::ostringstream fname;
  fname << "hits_run"
        << std::setw(4) << std::setfill('0') << runID
        << ".root";

  man->OpenFile(fname.str());

  G4cout << "Opened output file: " << fname.str() << G4endl;
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
