// A strict stratergy to exactly have 3 hits in upper plane and exactly have 3 hits in lower plane.
#include "EventAction.hh"
#include "RunAction.hh"
#include "TrackerHit.hh"  
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
  : G4UserEventAction(),
    fRunAction(runAction),
    fKE_Upper(0.),
    t_Upper(0.),
    fDir_Upper(),
    fUpperRecorded(false),
    fKE_Lower(0.),
    t_Lower(0.),
    fDir_Lower(),
    fLowerRecorded(false)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{    
  fKE_Upper = 0.;
  t_Upper = 0.;
  fDir_Upper = G4ThreeVector();
  fUpperRecorded = false;
  fKE_Lower = 0.;
  t_Lower = 0.;
  fDir_Lower = G4ThreeVector();
  fLowerRecorded = false;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::SetUpperDetectorEntry(G4double keU, const G4ThreeVector& dirU, G4double timeU) {
  if (!fUpperRecorded) {  // only record the first entry
    fKE_Upper = keU;
    t_Upper = timeU;
    fDir_Upper = dirU;
    fUpperRecorded = true;
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::SetLowerDetectorEntry(G4double keL, const G4ThreeVector& dirL, G4double timeL) {
  if (!fLowerRecorded) {  // only record the first entry
    fKE_Lower = keL;
    t_Lower = timeL;
    fDir_Lower = dirL;
    fLowerRecorded = true;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event* event)
{
  auto hce = event->GetHCofThisEvent();
  if (!hce) return;

  auto sdManager       = G4SDManager::GetSDMpointer();
  auto analysisManager = G4AnalysisManager::Instance();

  // Collect both hit collections
  static G4int upperHCID = -1, lowerHCID = -1;
  if (upperHCID < 0)
    upperHCID = sdManager->GetCollectionID("UpperTrackerSD/UpperHitsCollection");
  if (lowerHCID < 0)
    lowerHCID = sdManager->GetCollectionID("LowerTrackerSD/LowerHitsCollection");

  auto upperHC = (TrackerHitsCollection*)(hce->GetHC(upperHCID));
  auto lowerHC = (TrackerHitsCollection*)(hce->GetHC(lowerHCID));

  // Quick reject if no upper hits at all (no trigger)
  if (!upperHC || upperHC->entries() == 0) {
    return;
  }

  //------------------------------------------------------------------------------------
  // --- find the earliest upper hit time (t0) ---
  // --- Event Level Trigger and dump mechanism-----
  //------------------------------------------------------------------------------------
  G4double t0 = DBL_MAX;
  for (size_t i = 0; i < upperHC->entries(); ++i) {
    auto hit = (*upperHC)[i];
    if (!hit) continue;
    if (hit->GetTime() < t0) t0 = hit->GetTime();
  }
  if (t0 == DBL_MAX) return; // just in case

  const G4double window = 50.0 * ns;
  const G4double tmax   = t0 + window;

  // --- collect distinct detector copy numbers within the window ---
  std::unordered_set<int> upperCopiesInWindow;
  std::unordered_set<int> lowerCopiesInWindow;

  for (size_t i = 0; i < upperHC->entries(); ++i) {
    auto hit = (*upperHC)[i];
    if (!hit) continue;
    G4double t = hit->GetTime();
    if (t >= t0 && t <= tmax) upperCopiesInWindow.insert(hit->GetDetID());
  }

  if (lowerHC) {
    for (size_t i = 0; i < lowerHC->entries(); ++i) {
      auto hit = (*lowerHC)[i];
      if (!hit) continue;
      G4double t = hit->GetTime();
      if (t >= t0 && t <= tmax) lowerCopiesInWindow.insert(hit->GetDetID());
    }
  }

  // --- Decide acceptance: need >=3 distinct upper and >=3 distinct lower planes ---
  const size_t needUpper = 3;
  const size_t needLower = 3;
  bool acceptCoincidence =
    (upperCopiesInWindow.size() >= needUpper) &&
    (lowerCopiesInWindow.size() >= needLower);

  if (!acceptCoincidence) {
    // event rejected by multiplicity/coincidence -> do not write ntuple rows
    return;
  }

  // ---------------------------------------------------------------------------
  // Additional requirements for MC row:
  //  * primary muon must have recorded entries in BOTH detectors
  //  * upper-lower primary times within same 50 ns window
  // ---------------------------------------------------------------------------
  if (!(fUpperRecorded && fLowerRecorded)) {
    // We had hits in both trackers, but not from the primary in both
    return;
  }

  if (std::fabs(t_Lower - t_Upper) > window) {
    // Primary times not in coincidence window
    return;
  }
  G4int nHitsUpper = (upperHC ? upperHC->entries() : 0);
  G4int nHitsLower = (lowerHC ? lowerHC->entries() : 0);
  //  G4int nHitsTotal = nHitsUpper + nHitsLower;

  if (nHitsUpper != 3 || nHitsLower != 3)
    return;

  
  // ---------------------------------------------------------------------------
  // Event fully accepted at this point:
  //  * increment useful event counter
  //  * fill hit ntuple for all hits
  //  * fill ONE mcinfo row per event
  // ---------------------------------------------------------------------------
  fRunAction->CountEvents();
  auto eventID = event->GetEventID();

  // --- 1) Fill TrackerHits ntuple for ALL hits (as before) ---
  auto fillHits = [&](TrackerHitsCollection* hc) {
    if (!hc) return;
    for (size_t i = 0; i < hc->entries(); ++i) {
      auto hit = (*hc)[i];
      analysisManager->FillNtupleIColumn(0, 0, eventID);             // event ID
      analysisManager->FillNtupleIColumn(0, 1, hit->GetTrackID());
      analysisManager->FillNtupleIColumn(0, 2, hit->GetPDGCode());
      analysisManager->FillNtupleIColumn(0, 3, hit->GetPlaneID());
      analysisManager->FillNtupleIColumn(0, 4, hit->GetDetID());     // copy number
      analysisManager->FillNtupleDColumn(0, 5, hit->GetPos().x()/mm);
      analysisManager->FillNtupleDColumn(0, 6, hit->GetPos().y()/mm);
      analysisManager->FillNtupleDColumn(0, 7, hit->GetPos().z()/mm);
      analysisManager->FillNtupleDColumn(0, 8, hit->GetTime()/ns);
      analysisManager->FillNtupleDColumn(0, 9, hit->GetEdep()/keV);
      analysisManager->AddNtupleRow(0);
    }
  };

  fillHits(upperHC);
  fillHits(lowerHC);

  // --- 2) Fill ONE mcinfo row per accepted event ---
  // Use TrackID = 1 explicitly (primary muon),
  // or change if you later want something more sophisticated.
  analysisManager->FillNtupleIColumn(1, 0, eventID);
  analysisManager->FillNtupleIColumn(1, 1, 1);                  // primary track
  analysisManager->FillNtupleDColumn(1, 2, fKE_Upper / MeV);
  analysisManager->FillNtupleDColumn(1, 3, t_Upper / ns);
  analysisManager->FillNtupleDColumn(1, 4, fDir_Upper.x());
  analysisManager->FillNtupleDColumn(1, 5, fDir_Upper.y());
  analysisManager->FillNtupleDColumn(1, 6, fDir_Upper.z());
  analysisManager->FillNtupleDColumn(1, 7, fKE_Lower / MeV);
  analysisManager->FillNtupleDColumn(1, 8, t_Lower / ns);
  analysisManager->FillNtupleDColumn(1, 9, fDir_Lower.x());
  analysisManager->FillNtupleDColumn(1, 10, fDir_Lower.y());
  analysisManager->FillNtupleDColumn(1, 11, fDir_Lower.z());
  analysisManager->AddNtupleRow(1);
}
