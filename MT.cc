#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "QBBC.hh"
#include "FTFP_BERT.hh"

#include "Randomize.hh"
#include "G4String.hh"
#include "G4Threading.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Random/Random.h"  // CLHEP
#include <cstring>
#include <cstdlib>
#include <cstdlib>   // for atoi, atof

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrintUsage() {
  G4cerr << "Usage: ./MT <macro> [nThreads] [seed]\n";
}

int main(int argc,char** argv)
{
  G4String macro;
  G4String session;
  G4bool verboseBestUnits = false; //true - to reduce outputs
  G4int nEvents = 0;
  G4String outputFile = "hits.root"; // Check - if here or in ActionInitialization
  G4int masterSeed = 12345;            // default
  long long eventOffset = 0;           // default

  // -------------------------------------------------------------------------
  // MULTI-THREADED BLOCK OPTIONS
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  for (G4int i = 1; i < argc; i++) {
    if (std::strcmp(argv[i], "-m") == 0 && i+1 < argc) {
      macro = argv[++i];
    }
    else if (std::strcmp(argv[i], "-u") == 0 && i+1 < argc) {
      session = argv[++i];
    }
#ifdef G4MULTITHREADED
    else if (std::strcmp(argv[i], "-t") == 0 && i+1 < argc) {
      nThreads = G4UIcommand::ConvertToInt(argv[++i]);
    }
#endif
    else if (std::strcmp(argv[i], "--n-events") == 0 && i+1 < argc) {
      nEvents = std::atoi(argv[++i]);
    }
    else if (std::strcmp(argv[i], "--event-offset") == 0 && i+1 < argc) {
      eventOffset = std::atoi(argv[++i]);
    }
    else if (std::strcmp(argv[i], "--random-seed") == 0 && i+1 < argc) {
      masterSeed = std::atoi(argv[++i]);
    }
    else if (std::strcmp(argv[i], "-o") == 0 && i+1 < argc) {
      outputFile = argv[++i];
    }
    else if (std::strcmp(argv[i], "-vDefault") == 0) {
      verboseBestUnits = false;
    }
    else {
      G4cerr << "Unknown option: " << argv[i] << G4endl;
      PrintUsage();
      return 1;
    }
  }
  // -------------------------------------------------------------------------  
  // Set CLHEP/Geant4 master seed (important for thread-local RNGs)
  CLHEP::HepRandom::setTheSeed(masterSeed);
  // -------------------------------------------------------------------------
  
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }
  // -------------------------------------------------------------------------
  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  // -------------------------------------------------------------------------
  // Construct the default run manager
  //
  auto runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if ( nThreads > 0 ) {
    runManager->SetNumberOfThreads(nThreads);
  }
#endif
  // -------------------------------------------------------------------------
  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  //  G4VModularPhysicsList* physicsList = new QBBC;
  // physicsList->SetVerboseLevel(1);
  //  runManager->SetUserInitialization(physicsList);

  auto physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization(masterSeed,eventOffset,outputFile));
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui) {
    // batch mode
    if (macro.empty()) {
      G4cerr << "Error: No macro file specified! Use -m <macro>." << G4endl;
      return 1;
    }
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + macro);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
