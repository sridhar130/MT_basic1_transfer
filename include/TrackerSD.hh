#ifndef TrackerSD_h
#define TrackerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TrackerHit.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class TrackerSD : public G4VSensitiveDetector
{
  public:
    TrackerSD(const G4String& name, const G4String& hcName, G4int planeID);
    virtual ~TrackerSD() = default;

    virtual void Initialize(G4HCofThisEvent* hce) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* hce) override;

  private:
    TrackerHitsCollection* fHitsCollection = nullptr;
    G4int fHCID = -1;
    G4int fPlaneID;  // 0 = upper, 1 = lower (or whatever mapping you prefer)
};

#endif
