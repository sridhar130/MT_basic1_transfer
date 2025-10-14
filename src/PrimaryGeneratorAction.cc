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
  std::ifstream inputFile("/home/sridhar/G4Dev/cry_v1.7/configGaisser_short.txt");
  if (!inputFile.is_open()) {
    G4Exception("PrimaryGeneratorAction::InitializeCRY",
                "CRYConfig", FatalException, "Cannot open CRY config file");
  }
  std::string setupString((std::istreambuf_iterator<char>(inputFile)),
                          std::istreambuf_iterator<char>());

  // 2. Initialize CRY setup (pass data directory as string)
  std::string cryDataDir = "/home/sridhar/G4Dev/cry_v1.7/data";
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

  G4cout << "PrimaryGeneratorAction: CRY initialized (thread " << fThreadId
         << ") seed=" << (seed_thread & 0xffffffff) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  // Use CRY to generate particles for this event:
  std::vector<CRYParticle*> parts;
  fCryGen->genEvent(&parts);   // CRY API: fills vector with pointers to CRYParticle

  for (auto p : parts) {
    if (!p) continue;

    // Filter: only mu+ (PDG 13?) and mu- (PDG -13)
    int pdg = p->id(); // adjust accessor name to CRYParticle API
    if (pdg != 13 && pdg != -13) continue;

    G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* pdef = ptable->FindParticle((pdg == 13) ? "mu+" : "mu-");
    if (!pdef) continue;

    fParticleGun->SetParticleDefinition(pdef);

    // set energy and direction: convert units (CRY gives energy in MeV often)
    fParticleGun->SetParticleEnergy(p->ke() * MeV); // adapt accessor
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(p->u(), p->v(), p->w()));
    // muon generation position 
    G4double x0 = 142.0 * (G4UniformRand()-0.5);
    G4double y0 = 142.0 * (G4UniformRand()-0.5);
    G4double z0 = -41.*cm;
    fParticleGun->SetParticlePosition(G4ThreeVector(x0*cm,y0*cm,z0));
    //fParticleGun->SetParticlePosition(G4ThreeVector(p->x() * m, p->y() * m, p->z() * m));
    // time handling: CRY provides arrival time; set PrimaryVertex time if needed:
    //G4double time = p->t() * ns; // change accessor/unit as CRY uses
    //fParticleGun->SetParticleTime(time);

    fParticleGun->GeneratePrimaryVertex(anEvent);
  }

  // If CRY allocated particle pointers, free them if required by CRY API.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

