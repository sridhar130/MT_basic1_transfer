#include "TrackerHit.hh"

G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator = nullptr;

TrackerHit::TrackerHit()
 : G4VHit(),
   fDetID(-1),
   fTrackID(-1),
   fPlaneID(-1),
   fPos(G4ThreeVector()),
   fTime(0.),
   fEdep(0.)
{}

TrackerHit::~TrackerHit() {}

TrackerHit::TrackerHit(const TrackerHit& right) : G4VHit()
{
  fDetID   = right.fDetID;
  fTrackID = right.fTrackID;
  fPlaneID = right.fPlaneID;
  fPos     = right.fPos;
  fTime    = right.fTime;
  fEdep    = right.fEdep;
}

const TrackerHit& TrackerHit::operator=(const TrackerHit& right)
{
  fDetID   = right.fDetID;
  fTrackID = right.fTrackID;
  fPlaneID = right.fPlaneID;
  fPos     = right.fPos;
  fTime    = right.fTime;
  fEdep    = right.fEdep;
  return *this;
}

G4bool TrackerHit::operator==(const TrackerHit& right) const
{
  return (this == &right);
}
