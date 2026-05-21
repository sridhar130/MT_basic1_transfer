//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// This file is for the volume:
// Rusted Rebar
// Placed at center
// Used in EPJ plus paper
// Vary defect thickness
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
  G4Material*  Steel =  nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"); 



  G4Material*Rust = new G4Material
    ("Rust", density=4.0*g/cm3, ncomponents=3);
  Rust->AddElement(H , 4);
  Rust->AddElement(O , 5);
  Rust->AddElement(Fe , 2);
  
  G4double  Det_sizeXY = 140.0 *cm;
  G4double  Det_sizeZ = 0.2 *cm;
  G4double cubesize = 10.0*cm;
  G4Material* Iron = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");  
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Ar");
  G4Material* Lead = nist->FindOrBuildMaterial("G4_Pb"); 
  G4Material* Aluminum   = nist->FindOrBuildMaterial("G4_Al"); 

  // Envelope parameters
  //
  G4double env_sizeXY = 1.4*m, env_sizeZ = 1.4*m;

  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Rusted Rod parameters

  // scaling
  G4double scale = 2.0;  // make everything twice larger

  G4double Concrete_h_length = 12.5*cm * scale; 
  G4double Concrete_h_x = 5.0*cm * scale;
  G4double Concrete_h_y = 5.0*cm * scale;
  G4double Rebar_h_length = 12.0*cm * scale;
  G4double tol = 0.0001* cm * scale;
  G4double Steel_l = 1.5* cm * scale;
  G4double Rust_l = 2.0* cm * scale;
  G4double RodRad = 1.5 * cm * scale; // Total Rebar thickness
  // don't scale these
  G4double CenterRad = 0.7*RodRad; // Un-rusted/ core rebar
  G4double DefectFrac = 0.3; // Defect percent in rebar
  G4double xi,xii = 0.0;
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // Rotation
  //
  //==================================================================
  //----rotation----------------------------------
  G4RotationMatrix* RotMat = new G4RotationMatrix();
  //  RotMat->rotateX(90.0*deg);
  //RotMat->rotateZ(90.0*deg);
  RotMat->rotateY(90.0*deg);


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
  for (int jk =0;jk<3;jk++){              
    new G4PVPlacement(0,                       //no rotation
		      G4ThreeVector(0.,0.,-25.0*cm-jk*7.0*cm),
		      logicUpperTracker,             //its logical volume
		      "UpperTracker",                //its name
		      logicWorld,                //its mother  volume
		      false,                   //no boolean operation
		      jk,                       //copy number
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
  for (int kj =0;kj<3;kj++){              
    new G4PVPlacement(0,                       //no rotation
		      G4ThreeVector(0.,0.,25.0*cm+kj*7.0*cm),
		      logicLowerTracker,             //its logical volume
		      "LowerTracker",                //its name
		      logicWorld,                //its mother  volume
		      false,                   //no boolean operation
		      kj,                       //copy number
		      checkOverlaps);          //overlaps checking
  }
  




  //----------------------------------------------------------------------------------
  //--------------------scatAngleFile---------------------------------------------

  //-------------------- concrete slab iron rod rust defect------------------------------------------------------------------
  // Subtraction volume to remove the concrete from the rusted_rebar region



  G4VSolid* box = new G4Box("box",Concrete_h_x,Concrete_h_x,Concrete_h_length);
  //G4Tubs *box2 = new G4Tubs("box2",0.0*cm,1.0*cm, 4.5*cm, 0, 2*M_PI); // l: 1 cm
  G4Tubs *box2 = new G4Tubs("box2",0.0*cm,RodRad,Rebar_h_length,0,2*M_PI);


  G4SubtractionSolid*RCCS= new  G4SubtractionSolid ("RCCS",box,box2,0,G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));


  G4LogicalVolume* RCCLV
    = new G4LogicalVolume(
			  RCCS,             // its solid
			  Concrete,      // its material
			  "RCCLV");         // its name
                                   
  new G4PVPlacement(
		    RotMat,                // no rotation
		    G4ThreeVector(), // its position
		    RCCLV,            // its logical volume                         
		    "RCC",            // its name
		    logicWorld,          // its mother  volume
		    false,            // no boolean operation
		    0,                // copy number
		    0);  // checking overlaps 

  

  //--------------------------------center rod------------------------------------------------------------
  // Center of the Rod of smaller rad and non defected.

  G4Tubs * brod2m = new G4Tubs("brod2m", 0.0*cm,CenterRad,Rebar_h_length, 0, 2*M_PI); // r: 0 -> 50      

  G4LogicalVolume*brod2Logicalm
    = new G4LogicalVolume(brod2m,Steel,"brod2m");

  new G4PVPlacement(RotMat,G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),brod2Logicalm,//
		    "brod2Physicalm",logicWorld,//logicWorld,
		    false,0,0); 

  //--------------------------Steel outers-----------------------------------------
  //outers with steel at -10.5 ,-3.5, 3.5 & 10.5
  G4Tubs * brod2a = new G4Tubs("brod2a",CenterRad,RodRad, Steel_l-tol, 0, 2*M_PI); // r: 0 -> 50      

  for (int i=0;i<4;i++){
    xi = (-10.5 + 7.0*i)*scale;  
    G4LogicalVolume*brod2Logicala
      = new G4LogicalVolume(brod2a,Steel,"brod2a");//rust

    new G4PVPlacement(RotMat,G4ThreeVector(xi*cm,0.0*cm,0.0*cm),brod2Logicala,//
		      "brod2Physicala",logicWorld,//logicWorld,
		      false,0,0);
  } 

  //--------------------------Rust outers-----------------------------------------
  //outers with rust at -7 ,0, 7
  G4Tubs * brod2b = new G4Tubs("brod2b",CenterRad,RodRad, Rust_l-tol, 0, 2*M_PI); // r: 0 -> 50      

  for (int i=0;i<3;i++){
    xii = (-7 + 7.0*i)*scale;  
    G4LogicalVolume*brod2Logicalb
      = new G4LogicalVolume(brod2b,Rust,"brod2b");//rust

    new G4PVPlacement(RotMat,G4ThreeVector(xii*cm,0.0*cm,0.0*cm),brod2Logicalb,//
		      "brod2Physicalb",logicWorld,//logicWorld,
		      false,0,0);
  } 



  //  fScoringVolume = logicShape3;
  //  fScoringVolume1 = logicShape2;
    
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
