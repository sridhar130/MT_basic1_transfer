#ifndef RunAction_h
#define RunAction_h 1
#include "G4Run.hh"
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"
#include <vector>

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RunAction : public G4UserRunAction
{
public:
  RunAction(const G4String& fname);
  ~RunAction();

  // virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run*) override;
  virtual void   EndOfRunAction(const G4Run*) override;
  // virtual G4Run* GenerateRun();

  // vectors that will hold per-event hit info (bound to ntuple columns)
  std::vector<double> fHitTime;
  std::vector<double> fHitX, fHitY, fHitZ;
  std::vector<int>    fHitTrackID;

private:
  G4int fNtupleId;
  G4String fOutputFileName;
};

#endif

