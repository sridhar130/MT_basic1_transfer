#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ActionInitialization::ActionInitialization(int masterSeed, long long eventOffset,const G4String& fname)
  : G4VUserActionInitialization(),
    fMasterSeed(masterSeed),
    fEventOffset(eventOffset),
    fOutputFileName(fname)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  //  SetUserAction(new RunAction(fOutputFileName));
  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
   SetUserAction(new PrimaryGeneratorAction(fMasterSeed, fEventOffset));
  // SetUserAction(new RunAction(fOutputFileName));
  // auto runAction = new RunAction(fOutputFileName);
  // SetUserAction(new EventAction(runAction));

  //SetUserAction(new PrimaryGeneratorAction());

  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
  
  EventAction* eventAction = new EventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new SteppingAction(eventAction)); 
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
