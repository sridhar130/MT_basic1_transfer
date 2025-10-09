// TrackerHit.hh
#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4Threading.hh"
#include "globals.hh"

class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
    virtual ~TrackerHit();
    TrackerHit(const TrackerHit&);
    const TrackerHit& operator=(const TrackerHit&);
    G4bool operator==(const TrackerHit&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // --- Data members (add what you need) ---
    G4int fDetID;          // Det Copy number
    G4int fTrackID;          // Track ID
    G4int fPlaneID;          // e.g. 0 = upper, 1 = lower (optional but useful)
    G4ThreeVector fPos;      // Hit position
    G4double fTime;          // Global time of hit
    G4double fEdep;          // Energy deposited

    // --- Accessors ---
    void SetDetID(G4int id)          { fDetID = id; }
    void SetTrackID(G4int id)        { fTrackID = id; }
    void SetPlaneID(G4int id)        { fPlaneID = id; }
    void SetPos(G4ThreeVector pos)   { fPos = pos; }
    void SetTime(G4double t)         { fTime = t; }
    void SetEdep(G4double e)         { fEdep = e; }

    G4int GetDetID() const           { return fDetID; }
    G4int GetTrackID() const         { return fTrackID; }
    G4int GetPlaneID() const         { return fPlaneID; }
    G4ThreeVector GetPos() const     { return fPos; }
    G4double GetTime() const         { return fTime; }
    G4double GetEdep() const         { return fEdep; }

  private:
    // nothing special
};

using TrackerHitsCollection = G4THitsCollection<TrackerHit>;

// --- Memory management for MT ---
extern G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;

inline void* TrackerHit::operator new(size_t)
{
  if (!TrackerHitAllocator)
    TrackerHitAllocator = new G4Allocator<TrackerHit>;
  return (void*)TrackerHitAllocator->MallocSingle();
}

inline void TrackerHit::operator delete(void* hit)
{
  TrackerHitAllocator->FreeSingle((TrackerHit*)hit);
}

#endif
