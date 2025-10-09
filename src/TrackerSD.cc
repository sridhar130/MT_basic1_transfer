#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4TouchableHandle.hh"

TrackerSD::TrackerSD(const G4String& name,
                     const G4String& hcName,
                     G4int planeID)
  : G4VSensitiveDetector(name), fPlaneID(planeID)
{
  collectionName.insert(hcName);  // required for hit collection registration
}

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (fHCID < 0)
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  hce->AddHitsCollection(fHCID, fHitsCollection);
}

G4bool TrackerSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep == 0.) return false;
  G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
  auto hit = new TrackerHit();
  hit->SetDetID(copyNo);
  hit->SetTrackID(step->GetTrack()->GetTrackID());
  hit->SetPlaneID(fPlaneID);
  hit->SetPos(step->GetPreStepPoint()->GetPosition());
  hit->SetTime(step->GetPreStepPoint()->GetGlobalTime());
  hit->SetEdep(edep);

  fHitsCollection->insert(hit);
  /*
  auto touchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4int depth = touchable->GetHistoryDepth();
  G4int copyNo1 = (depth > 0) ? touchable->GetCopyNumber(0) : -1;

  G4cout << "DEBUG TrackerSD: copyNo=" << copyNo1
       << " depth=" << depth
       << " volName=" << touchable->GetVolume()->GetName() << G4endl;
  */

  
  return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
#ifdef G4VERBOSE
  if (verboseLevel > 1)
    {
      G4cout << ">>> TrackerSD Plane " << fPlaneID
	     << " : " << fHitsCollection->entries() << " hits recorded." << G4endl;
    }
#endif
}
