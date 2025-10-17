#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include <vector>
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
  : G4UserSteppingAction(),
    fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  auto track = step->GetTrack();

  // Only record for the primary particle
  if (track->GetTrackID() != 1)
    return;

  // Get the pre-step physical volume name
  auto preStepPoint = step->GetPreStepPoint();
  auto volume = preStepPoint->GetTouchableHandle()->GetVolume();
  if (!volume)
    return;

  G4String volName = volume->GetName();
  G4int copyNo = preStepPoint->GetTouchableHandle()->GetCopyNumber();

  // Check if entering upper detector (change name as appropriate)g
  if (volName == "UpperTracker" && copyNo == 0)
    {
      // Get kinetic energy and direction
      G4double keU = track->GetDynamicParticle()->GetKineticEnergy();
      G4ThreeVector dirU = track->GetMomentumDirection();
      G4double timeU = track->GetGlobalTime();
      // Store this info in EventAction
      if (fEventAction)
	fEventAction->SetUpperDetectorEntry(keU, dirU, timeU);
      // G4cout << "[USTEP] volume=" << volName
      //  << " copyNo=" << copyNo
      // 	     << " KE=" << track->GetKineticEnergy()/CLHEP::MeV << " MeV"
      //  << G4endl;
    }

  if (volName == "LowerTracker" && copyNo == 2)
    {
      // Get kinetic energy and direction
      G4double keL = track->GetDynamicParticle()->GetKineticEnergy();
      G4ThreeVector dirL = track->GetMomentumDirection();
      G4double timeL = track->GetGlobalTime();
      // Store this info in EventAction
      if (fEventAction)
	fEventAction->SetLowerDetectorEntry(keL, dirL,timeL);
      // G4cout << "[LSTEP] volume=" << volName
      //  << " copyNo=" << copyNo
      // 	     << " KE=" << track->GetKineticEnergy()/CLHEP::MeV << " MeV"
      //  << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

