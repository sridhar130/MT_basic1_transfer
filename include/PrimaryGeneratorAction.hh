#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include <memory>
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYData.h"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction(int masterSeed, long long eventOffset);
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
  std::unique_ptr<G4ParticleGun> fParticleGun;

  // CRY objects (per-thread)
  void InitializeCRY(int masterSeed, long long eventOffset);
  std::unique_ptr<CRYGenerator> fCryGen;        // replace with actual CRY type
  std::unique_ptr<CRYSetup> fCrySetup;         // optional: per-thread setup
  int fMasterSeed;
  long long fEventOffset;
  int fThreadId;
};

#endif
