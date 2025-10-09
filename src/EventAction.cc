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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

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
    fRunAction(runAction)
    //fEdep(0.),
    //  fpos(0)

{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{    
  //  fEdep = 0.;
  //    fpos = G4ThreeVector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// old code for scoring and ASCII storing00000000000000000000
/*
  void EventAction::EndOfEventAction(const G4Event*event)
  {
  out1.open("raw.txt",std::ios::out|std::ios::app);
  G4int eventID= event->GetEventID();
  if (SetPosZ.size()>=8 && SetPosXR1!=0.0 && SetPosYR1!=0.0 && SetPosZR1!=0.0 && SetPosXR2!=0.0 && SetPosYR2!=0.0 && SetPosZR2!=0.0 && SetPosXR3!=0.0 && SetPosYR3!=0.0 && SetPosZR3!=0.0 && SetPosXR4!=0.0 && SetPosYR4!=0.0 && SetPosZR4!=0.0 && SetPosXR5!=0.0 && SetPosYR5!=0.0 && SetPosZR5!=0.0 && SetPosXR6!=0.0 && SetPosYR6!=0.0 && SetPosZR6!=0.0){
  out1<<eventID<<" "<<SetPosXR1<<" "<<SetPosYR1<<" "<<SetPosZR1<<" "<<SetPosXR2<<" "<<SetPosYR2<<" "<<SetPosZR2<<" "<<SetPosXR3<<" "<<SetPosYR3<<" "<<SetPosZR3<<" "<<SetPosXR4<<" "<<SetPosYR4<<" "<<SetPosZR4<<" "<<SetPosXR5<<" "<<SetPosYR5<<" "<<SetPosZR5<<" "<<SetPosXR6<<" "<<SetPosYR6<<" "<<SetPosZR6<<" "<<SetKE<<endl;
  out1.close();}
  cout<<"--- . End of event  :"<<eventID<<endl;
  SetPosZ.clear();
  eventID=0;
  SetPosXR1=SetPosYR1=SetPosZR1=SetPosXR2=SetPosYR2=SetPosZR2=SetPosXR3=SetPosYR3=SetPosZR3=SetPosXR4=SetPosYR4=SetPosZR4=SetPosXR5=SetPosYR5=SetPosZR5=SetPosXR6=SetPosYR6=SetPosZR6=SetKE=0.0;
  }
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event* event)
{
  auto hce = event->GetHCofThisEvent();
  if (!hce) return;

  auto sdManager = G4SDManager::GetSDMpointer();
  auto analysisManager = G4AnalysisManager::Instance();

  // Collect both hit collections
  static G4int upperHCID = -1, lowerHCID = -1;
  if (upperHCID < 0)
    upperHCID = sdManager->GetCollectionID("UpperTrackerSD/UpperHitsCollection");
  if (lowerHCID < 0)
    lowerHCID = sdManager->GetCollectionID("LowerTrackerSD/LowerHitsCollection");

  auto upperHC = (TrackerHitsCollection*)(hce->GetHC(upperHCID));
  auto lowerHC = (TrackerHitsCollection*)(hce->GetHC(lowerHCID));
  // If no hits at all -> discard quickly
  // Quick reject if no upper hits at all (no trigger)
  if (!upperHC || upperHC->entries() == 0) {
    return;
  }
  //------------------------------------------------------------------------------------
  // --- find the earliest upper hit time (t0) ---
  G4double t0 = DBL_MAX;
  for (size_t i = 0; i < upperHC->entries(); ++i) {
    auto hit = (*upperHC)[i];
    if (!hit) continue;
    if (hit->GetTime() < t0) t0 = hit->GetTime();
  }
  if (t0 == DBL_MAX) return; // just in case

  const G4double window = 50.0 * ns;
  const G4double tmax = t0 + window;

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
  bool accept = (upperCopiesInWindow.size() >= needUpper) && (lowerCopiesInWindow.size() >= needLower);
  /*
  // Debug: list DetIDs that fired in coincidence window
  G4cout << "Upper DetIDs: ";
  for (auto d : upperCopiesInWindow) G4cout << d << " ";
  G4cout << " | Lower DetIDs: ";
  for (auto d : lowerCopiesInWindow) G4cout << d << " ";
  G4cout << G4endl;
  // Debug prints for first few events (reduce verbosity)
  static G4int dbg_count = 0;
  if (dbg_count < 20) {
    G4cout << "Event " << event->GetEventID()
           << " t0(ns)=" << t0 / ns
           << " tmax(ns)=" << tmax / ns
           << " upper_hits=" << (upperHC?upperHC->entries():0)
           << " lower_hits=" << (lowerHC?lowerHC->entries():0)
           << " upperDistinctInWindow=" << upperCopiesInWindow.size()
           << " lowerDistinctInWindow=" << lowerCopiesInWindow.size()
           << " => accept=" << accept << G4endl;
    ++dbg_count;
  }
  */
  if (!accept) {
    // event rejected => do not write ntuple rows
    return;
  }

  // --- Passed coincidence: write hits to ntuple (as before) ---
  //------------------------------------------------------------------------------------

  
  auto eventID = event->GetEventID();
  // Fill ROOT ntuple with all hits
  auto fillHits = [&](TrackerHitsCollection* hc) {
    if (!hc) return;
    for (size_t i = 0; i < hc->entries(); ++i)
      {
	auto hit = (*hc)[i];
	analysisManager->FillNtupleIColumn(0, eventID);             // event ID
	analysisManager->FillNtupleIColumn(1, hit->GetTrackID());
	analysisManager->FillNtupleIColumn(2, hit->GetPlaneID());
	analysisManager->FillNtupleIColumn(3, hit->GetDetID());     // copy number
	analysisManager->FillNtupleDColumn(4, hit->GetPos().x()/mm);
	analysisManager->FillNtupleDColumn(5, hit->GetPos().y()/mm);
	analysisManager->FillNtupleDColumn(6, hit->GetPos().z()/mm);
	analysisManager->FillNtupleDColumn(7, hit->GetTime()/ns);
	analysisManager->FillNtupleDColumn(8, hit->GetEdep()/keV);
	analysisManager->AddNtupleRow();
      }
  };

  fillHits(upperHC);
  fillHits(lowerHC);
}
