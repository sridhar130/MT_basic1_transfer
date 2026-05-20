#ifndef RunAction_h
#define RunAction_h 1
#include "G4Run.hh"
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "G4AccumulableManager.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "G4Timer.hh"
#include <vector>

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction();

  virtual void BeginOfRunAction(const G4Run*) override;
  virtual void EndOfRunAction(const G4Run*) override;

  // Increment counter
  void CountEvents() { fEventCount += 1; }

private:
  G4Accumulable<G4int> fEventCount;  // ✅ accumulable counter
  G4Timer* fTimer;
};

#endif
