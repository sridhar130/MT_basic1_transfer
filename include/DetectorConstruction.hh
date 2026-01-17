#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  void ConstructSDandField() override;    
  G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
  G4LogicalVolume* GetScoringVolume1() const { return fScoringVolume1; }
  


protected:
  G4LogicalVolume*  fScoringVolume;
  G4LogicalVolume*  fScoringVolume1;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

