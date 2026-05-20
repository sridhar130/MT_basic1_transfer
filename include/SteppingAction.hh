#ifndef SteppingAction_h
#define SteppingAction_h 1
#include "G4ThreeVector.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;
class G4Step;

/// Stepping action class
/// 

class SteppingAction : public G4UserSteppingAction
{
  public:
  SteppingAction(EventAction* eventAction);
  virtual ~SteppingAction() = default;

  // method from the base class
  virtual void UserSteppingAction(const G4Step* step) override;
private:
  EventAction*  fEventAction;
 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
