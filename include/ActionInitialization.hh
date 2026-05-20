#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization(int masterSeed, long long eventOffset, const G4String& fname);
  //  ActionInitialization(int masterSeed = 12345, long long eventOffset = 0, const G4String& fname);
  virtual ~ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  G4String fOutputFileName;
  int fMasterSeed;
  long long fEventOffset;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
