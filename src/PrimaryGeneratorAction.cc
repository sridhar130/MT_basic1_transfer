#include "PrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4AutoLock.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4Threading.hh"
#include "CLHEP/Random/Random.h"
#include <random>
#include "CRYData.h"
#include "CRYSetup.h"      // adapt include names to your CRY installation
#include "CRYGenerator.h"
#include "CRYParticle.h"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(int masterSeed, long long eventOffset)
  : G4VUserPrimaryGeneratorAction(),
    fMasterSeed(masterSeed),
    fEventOffset(eventOffset),
    fParticleGun(std::make_unique<G4ParticleGun>(1))
{
  // determine thread id (0 for master if built that way; Geant4 worker threads get id >=1)
#ifdef G4MULTITHREADED
  fThreadId = G4Threading::G4GetThreadId();
#else
  fThreadId = 0;
#endif

  // Create & seed CRY generator for this thread
  InitializeCRY(masterSeed, eventOffset);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::InitializeCRY(int masterSeed, long long eventOffset)
{
  // 1. Load CRY configuration file

  std::ifstream inputFile("/home/sinjini/STProducts/G4Dev/cry_v1.7/configGaisser_short.txt");
  if (!inputFile.is_open()) {
    G4Exception("PrimaryGeneratorAction::InitializeCRY",
                "CRYConfig", FatalException, "Cannot open CRY config file");
  }
  std::string setupString((std::istreambuf_iterator<char>(inputFile)),
                          std::istreambuf_iterator<char>());

  // 2. Initialize CRY setup (pass data directory as string)
  std::string cryDataDir = "/home/sinjini/STProducts/G4Dev/cry_v1.7/data";
  fCrySetup = std::make_unique<CRYSetup>(setupString, cryDataDir);

  // 3. Create generator (constructor signature depends on CRY version)
  // If CRYGenerator takes CRYSetup*, use get(); if it takes CRYSetup&, use *fCrySetup
  fCryGen = std::make_unique<CRYGenerator>(fCrySetup.get());

  // 4. Make a deterministic seed per thread / per run
  unsigned long long seed_thread = static_cast<unsigned long long>(masterSeed)
    + static_cast<unsigned long long>(eventOffset)
    + static_cast<unsigned long long>(fThreadId + 1) * 1000003ull;

  // 5. If CRY uses std::rand internally, set srand; otherwise use CRY-specific seed method
  srand(static_cast<unsigned>(seed_thread & 0xffffffff));

  //  G4cout << "PrimaryGeneratorAction: CRY initialized (thread " << fThreadId
  //     << ") seed=" << (seed_thread & 0xffffffff) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  // choose detector half-size (match DetectorConstruction Det_sizeXY/2)
  const G4double det_half_xy = 80.0*cm; // 140 cm / 2
  std::vector<CRYParticle*> parts;
  fCryGen->genEvent(&parts);   // CRY API: fills vector with pointers to CRYParticle

  for (auto p : parts) {
  if (!p) continue;

  // CRY uses its own ID enums (not PDG). Accept CRY muons.
  // Use CRYParticle::Muon (and optionally MuonMinus / MuonPlus if defined).
  int cryid = p->id();
  if (cryid != CRYParticle::Muon) {
  delete p;
  continue;
  }

  // Energy: CRY ke() is in MeV -> convert to Geant4 units
  G4double energy = p->ke() * MeV;

  // Direction: CRY u,v,w are direction cosines.
  // Determine direction and make sure it points into the detector.
  G4ThreeVector dir(p->u(), p->v(), p->w());
  // In many CRY setups w<0 means downward;
  // an incoming cosmic from "above" should go from negative z -> positive z,
  // so flip sign if necessary to point towards +z (needed for current detector setup).
  if (dir.z() < 0) dir.setZ(-dir.z());   // flip if CRY uses negative-down convention
  dir = dir.unit();

  // Choose a generation Z that is above the upper tracker 
  // z = -25cm (upper) and +25cm (lower) so
  // plane at e.g. z = -50 cm 
  G4double genZ = -50.0*cm; 

  // Choose X,Y uniformly inside detector acceptance so particles actually hit
  G4double x = (G4UniformRand() - 0.5) * 2.0 * det_half_xy;
  G4double y = (G4UniformRand() - 0.5) * 2.0 * det_half_xy;
  G4ThreeVector pos(x, y, genZ);

  // Determine particle name from CRY charge (or from cryid if CRY encodes plus/minus)
  G4String pname;
  if (p->charge() > 0.0) pname = "mu+";
  else if (p->charge() < 0.0) pname = "mu-";
  else pname = "mu-"; // fallback

  // Debug print: CRY raw + chosen G4 pos/dir
  // G4cout << "[PGA] CRY id=" << cryid
  // << " E(MeV)=" << p->ke()
  // << " CRYpos(m)=(" << p->x() << "," << p->y() << "," << p->z() << ")"
  // << " -> G4 pos(cm)=(" << pos.x()/cm << "," << pos.y()/cm << "," << pos.z()/cm << ")"
  // << " dir=(" << dir.x() << "," << dir.y() << "," << dir.z() << ")"
  // << " charge=" << p->charge()
  // << G4endl;

  // Create G4 primary
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* pdef = ptable->FindParticle(pname);
  if (!pdef) pdef = ptable->FindParticle("mu-"); // safe fallback

  fParticleGun->SetParticleDefinition(pdef);
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticleMomentumDirection(dir);
  fParticleGun->SetParticlePosition(pos);
  fParticleGun->GeneratePrimaryVertex(anEvent);

  // free CRY particle if its API expects user deletion
  delete p;
  }

  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Alternate--------------------------
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  fParticleGun->SetParticleDefinition(particleTable->FindParticle("mu-"));
  //  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(3.0*GeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-50*cm));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

