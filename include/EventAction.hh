#ifndef EventAction_h
#define EventAction_h 1
#include "G4ThreeVector.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include <fstream>
class RunAction;
using namespace std;
/// Event action class
///

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction* runAction);
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  void AddEdep(G4double edep) { fEdep += edep; }
  void SetUpperDetectorEntry(G4double keU, const G4ThreeVector& dirU, G4double timeU);
  void SetLowerDetectorEntry(G4double keL, const G4ThreeVector& dirL, G4double timeL);
  
private:
  RunAction* fRunAction;
  G4double     fEdep;
  G4double fKE_Upper;
  G4double t_Upper;              
  G4ThreeVector fDir_Upper;       
  G4bool fUpperRecorded;          
  G4double fKE_Lower;
  G4double t_Lower;              
  G4ThreeVector fDir_Lower;       
  G4bool fLowerRecorded;
  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
