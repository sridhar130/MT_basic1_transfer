//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// This file is for the volume:
// HI-STORM-like CASK GEOMETRY
// Placed at center
// Vary Uranium rod thickness
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"
#include "TrackerSD.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fScoringVolume(0),fScoringVolume1(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  //******************* PARAMETERS AND MATERIALS*************************

  //-------------------------------------------------------------------------
  G4String symbol;             //a=mass of a mole;
  G4double density;      //z=mean number of protons;  
  // n=number of nucleons in an isotope;
  
  G4int ncomponents,natoms;
  G4double fractionmass;
  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  nist->SetVerbose(0);
  // //rust mixture ---

  G4Element* Fe = nist->FindOrBuildElement("Fe");
  G4Element* H = nist->FindOrBuildElement("H");
  G4Element* O = nist->FindOrBuildElement("O");
  
 
  // ScMaterial = mat->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material*  steel =  nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"); 



  G4Material*Rust = new G4Material
    ("Rust", density=4.0*g/cm3, ncomponents=3);
  Rust->AddElement(H , 4);
  Rust->AddElement(O , 5);
  Rust->AddElement(Fe , 2);
  
  
  G4Material* Iron = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* concrete = nist->FindOrBuildMaterial("G4_CONCRETE");  
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Ar");
  G4Material* Lead = nist->FindOrBuildMaterial("G4_Pb"); 
  G4Material* Aluminum   = nist->FindOrBuildMaterial("G4_Al");
  G4Material* helium = nist->FindOrBuildMaterial("G4_He");


  // Create approximate UO2 material
  G4Element* elU = nist->FindOrBuildElement("U");
  G4Element* elO = nist->FindOrBuildElement("O");
  G4Material* UO2 = new G4Material("UO2", 10.97*g/cm3, 2);
  UO2->AddElement(elU, 1);
  UO2->AddElement(elO, 2);

  // Zircaloy cladding (Zr)
  G4Element* elZr = nist->FindOrBuildElement("Zr");
  G4Material* Zircaloy = new G4Material("Zircaloy", 6.55*g/cm3, 1);
  Zircaloy->AddElement(elZr, 1);

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  //----------------------------------------------------------------------------------
  // Dimensions
  //----------------------------------------------------------------------------------

  G4double  Det_sizeXY = 600.0 *cm;
  G4double  Det_sizeZ  = 0.5 *cm;
  
  G4double world_sizeXY = Det_sizeXY + 600.0 *cm;
  G4double world_sizeZ  = 1300 *cm;  // Detectors span from 4 to -4m








  // Overlaps------------------------------------------------------------------------
  G4bool checkOverlaps = true;

  //
  // Rotation
  //
  //==================================================================
  //----rotation----------------------------------
  G4RotationMatrix* RotMat = new G4RotationMatrix();
  //  RotMat->rotateX(90.0*deg);
  //RotMat->rotateZ(90.0*deg);
  //RotMat->rotateY(90.0*deg);


  //     
  // World
  //

  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
	      0.6*world_sizeXY, 0.6*world_sizeXY, 0.6*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
 

  //     
  // UpperTracker
  //
  
   
  G4Box* solidUpperTracker =    
    new G4Box("UpperTracker",Det_sizeXY/2.0,Det_sizeXY/2.0,Det_sizeZ/2.0);
                
  G4LogicalVolume* logicUpperTracker =                         
    new G4LogicalVolume(solidUpperTracker,         //its solid
                        shape2_mat,          //its material
                        "UpperTracker");           //its name
  for (int kj =0;kj<3;kj++){              
    new G4PVPlacement(0,                       //no rotation
		      G4ThreeVector(0.,0.,400.0*cm-kj*25.0*cm),
		      logicUpperTracker,             //its logical volume
		      "UpperTracker",                //its name
		      logicWorld,                //its mother  volume
		      false,                   //no boolean operation
		      kj,                       //copy number
		      checkOverlaps);          //overlaps checking
  }

  //     
  // LowerTracker
  //
  
   
  G4Box* solidLowerTracker =    
    new G4Box("LowerTracker",Det_sizeXY/2.0,Det_sizeXY/2.0,Det_sizeZ/2.0);
                
  G4LogicalVolume* logicLowerTracker =                         
    new G4LogicalVolume(solidLowerTracker,         //its solid
                        shape2_mat,          //its material
                        "LowerTracker");           //its name
  for (int jk =0;jk<3;jk++){              
    new G4PVPlacement(0,                       //no rotation
		      G4ThreeVector(0.,0.,-400.0*cm+jk*25.0*cm),
		      logicLowerTracker,             //its logical volume
		      "LowerTracker",                //its name
		      logicWorld,                //its mother  volume
		      false,                   //no boolean operation
		      jk,                       //copy number
		      checkOverlaps);          //overlaps checking
  }

  //----------------------------------------------------------------------------------
  // HI-STORM-like CASK GEOMETRY
  //----------------------------------------------------------------------------------
  

  // Overall cask envelope
  const G4double caskOuterRadius = 1.75*m;   // outer radius of overpack
  const G4double caskHeight      = 4.5*m;    // total height (body + lids)

  // Radial layering for METCON overpack
  const G4double outerSteelThk   = 0.05*m;   // outer steel shell thickness
  const G4double concreteThk     = 0.25*m;   // concrete thickness
  const G4double innerSteelThk   = 0.05*m;   // inner steel shell thickness

  const G4double shellOuterInnerR = caskOuterRadius - outerSteelThk;       // inner radius of outer steel
  const G4double shellInnerOuterR = shellOuterInnerR - concreteThk;        // outer radius of inner steel
  const G4double cavityRadius     = shellInnerOuterR - innerSteelThk;      // inner radius of inner steel (cavity)

  // Axial layering: side body + top/bottom lids
  const G4double lidSteelThk   = 0.05*m;
  const G4double lidConcThk    = 0.25*m;
  const G4double lidStackThk   = lidSteelThk + lidConcThk;                 // each end
  const G4double bodyHeight    = caskHeight - 2.0*lidStackThk;             // cylindrical body between lids
  const G4double bodyHalfHeight = bodyHeight/2.0;

  // Cavity height (space where fuel can sit)
  const G4double cavityHeight  = bodyHeight - 2.0*0.05*m;                  // small axial clearance

  // Canister dimensions (simple cylinder inside cavity)
  const G4double canisterRad    = cavityRadius - 0.05*m;                   // radial gap between inner shell & canister
  const G4double canisterHeight = cavityHeight - 0.10*m;                   // axial gap top/bottom

  // ASSEMBLIES

  const G4int    nRows           = 6;
  const G4int    nCols           = 6;
  const G4double assemblyPitch   = 0.37*m;
  const G4double assemblyRadius  = 0.16*m;
  const G4double assemblyHeight  = canisterHeight - 0.10*m;
  const G4double assemblyZPos    = 0.0*m;                                  // center

  // Overpack body, outer steel, concrete, inner steel 

  // Outer steel shell (body)
  G4Tubs* solidOuterShell = new G4Tubs("OuterShell",
				       shellOuterInnerR,  // inner radius
				       caskOuterRadius,   // outer radius
				       bodyHalfHeight,    // half height
				       0.*deg, 360.*deg);
  G4LogicalVolume* logicOuterShell =
    new G4LogicalVolume(solidOuterShell, steel, "OuterShell");
  new G4PVPlacement(0, G4ThreeVector(0,0,0),
		    logicOuterShell, "OuterShell",
		    logicWorld, false, 0, checkOverlaps);

  // Concrete layer
  G4Tubs* solidConcrete = new G4Tubs("CaskConcrete",
				     shellInnerOuterR,    // inner radius
				     shellOuterInnerR,    // outer radius
				     bodyHalfHeight,
				     0.*deg, 360.*deg);
  G4LogicalVolume* logicConcrete =
    new G4LogicalVolume(solidConcrete, concrete, "CaskConcrete");
  new G4PVPlacement(0, G4ThreeVector(0,0,0),
		    logicConcrete, "CaskConcrete",
		    logicWorld, false, 0, checkOverlaps);

  // Inner steel shell
  G4Tubs* solidInnerShell = new G4Tubs("InnerShell",
				       cavityRadius,      // inner radius
				       shellInnerOuterR,  // outer radius
				       bodyHalfHeight,
				       0.*deg, 360.*deg);
  G4LogicalVolume* logicInnerShell =
    new G4LogicalVolume(solidInnerShell, steel, "InnerShell");
  new G4PVPlacement(0, G4ThreeVector(0,0,0),
		    logicInnerShell, "InnerShell",
		    logicWorld, false, 0, checkOverlaps);

  // Cavity (helium gap inside inner shell)

  G4Tubs* solidCavity = new G4Tubs("Cavity",
				   0.0,
				   cavityRadius,
				   cavityHeight/2.0,
				   0.*deg, 360.*deg);
  G4LogicalVolume* logicCavity =
    new G4LogicalVolume(solidCavity, helium, "Cavity");
  //new G4PVPlacement(0, G4ThreeVector(0,0,0),
  //              logicCavity, "Cavity",
  //            logicInnerShell, false, 0, checkOverlaps);

  // Inner stainless-steel canister

  G4Material* stainlessSteel = steel; 

  G4Tubs* solidCanister = new G4Tubs("Canister",
				     (canisterRad-0.05*m),
				     canisterRad,
				     canisterHeight/2.0,
				     0.*deg, 360.*deg);
  G4LogicalVolume* logicCanister =
    new G4LogicalVolume(solidCanister, stainlessSteel, "Canister");
  new G4PVPlacement(0, G4ThreeVector(0,0,0),
		    logicCanister, "Canister",
		    logicWorld, false, 0, checkOverlaps);

  // Top & bottom lids: steel + concrete 

  // Top steel lid (full outer radius)
  G4Tubs* solidTopSteel = new G4Tubs("TopSteelLid",
				     0.0,
				     caskOuterRadius,
				     lidSteelThk/2.0,
				     0.*deg, 360.*deg);
  G4LogicalVolume* logicTopSteel =
    new G4LogicalVolume(solidTopSteel, steel, "TopSteelLid");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0, bodyHalfHeight + lidSteelThk/2.0 + lidConcThk),
		    logicTopSteel, "TopSteelLid",
		    logicWorld, false, 0, checkOverlaps);

  // Top concrete lid between inner & outer shell
  G4Tubs* solidTopConc = new G4Tubs("TopConcreteLid",
				    0.0,
				    shellOuterInnerR,      // up to inner face of outer steel
				    lidConcThk/2.0,
				    0.*deg, 360.*deg);
  G4LogicalVolume* logicTopConc =
    new G4LogicalVolume(solidTopConc, concrete, "TopConcreteLid");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0, bodyHalfHeight + lidConcThk/2.0),
		    logicTopConc, "TopConcreteLid",
		    logicWorld, false, 0, checkOverlaps);

  // Bottom steel base
  G4Tubs* solidBotSteel = new G4Tubs("BottomSteelBase",
				     0.0,
				     caskOuterRadius,
				     lidSteelThk/2.0,
				     0.*deg, 360.*deg);
  G4LogicalVolume* logicBotSteel =
    new G4LogicalVolume(solidBotSteel, steel, "BottomSteelBase");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0, -bodyHalfHeight - lidSteelThk/2.0 - lidConcThk),
		    logicBotSteel, "BottomSteelBase",
		    logicWorld, false, 0, checkOverlaps);

  // Bottom concrete base
  G4Tubs* solidBotConc = new G4Tubs("BottomConcreteBase",
				    0.0,
				    shellOuterInnerR,
				    lidConcThk/2.0,
				    0.*deg, 360.*deg);
  G4LogicalVolume* logicBotConc =
    new G4LogicalVolume(solidBotConc, concrete, "BottomConcreteBase");
  new G4PVPlacement(0,
		    G4ThreeVector(0,0, -bodyHalfHeight - lidConcThk/2.0),
		    logicBotConc, "BottomConcreteBase",
		    logicWorld, false, 0, checkOverlaps);

  //Basket: assemblies placed INSIDE the canister

  const G4double gridWidth  = (nCols-1)*assemblyPitch;
  const G4double gridHeight = (nRows-1)*assemblyPitch;

  G4Tubs* solidAssembly =
    new G4Tubs("Assembly", 0.0, assemblyRadius,
	       assemblyHeight/2.0, 0.*deg, 360.*deg);
  G4LogicalVolume* logicAssembly =
    new G4LogicalVolume(solidAssembly, UO2, "Assembly");

  //G4VisAttributes* uo2Colour = new G4VisAttributes(G4Colour(1.0,0.5,0.0));
  //uo2Colour->SetForceSolid(true);
  //logicAssembly->SetVisAttributes(uo2Colour);

  for (G4int i=0; i<nCols; ++i) {
    for (G4int j=0; j<nRows; ++j) {
      G4double x = -gridWidth/2.0  + i*assemblyPitch;
      G4double y = -gridHeight/2.0 + j*assemblyPitch;

      // Keep assemblies inside canister radius with a small margin
      if (std::sqrt(x*x + y*y) + assemblyRadius > canisterRad - 0.06*m) continue;

      new G4PVPlacement(0, G4ThreeVector(x,y,assemblyZPos),
			logicAssembly, "AssemblyPV",
			logicWorld, false, i*100+j, checkOverlaps);
    }
  }



  //
  //always return the physical World
  //

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  auto sdManager = G4SDManager::GetSDMpointer();

  // --- Upper detector SD ---
  auto upperTrackerSD = new TrackerSD("UpperTrackerSD", "UpperHitsCollection",0);
  sdManager->AddNewDetector(upperTrackerSD);
  SetSensitiveDetector("UpperTracker", upperTrackerSD, true);

  // --- Lower detector SD ---
  auto lowerTrackerSD = new TrackerSD("LowerTrackerSD", "LowerHitsCollection",1);
  sdManager->AddNewDetector(lowerTrackerSD);
  SetSensitiveDetector("LowerTracker", lowerTrackerSD, true);
}
 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
