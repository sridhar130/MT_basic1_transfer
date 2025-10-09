//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
#include <fstream>
#include "G4Event.hh"
#include "G4RunManager.hh"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
  //fEdep(0.),
  //  fpos(0)

{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{    
  //  fEdep = 0.;
  //    fpos = G4ThreeVector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void EventAction::EndOfEventAction(const G4Event*event)
{
  out1.open("raw.txt",std::ios::out|std::ios::app);
  G4int eventID= event->GetEventID();
  if (SetPosZ.size()>=8 && SetPosXR1!=0.0 && SetPosYR1!=0.0 && SetPosZR1!=0.0 && SetPosXR2!=0.0 && SetPosYR2!=0.0 && SetPosZR2!=0.0 && SetPosXR3!=0.0 && SetPosYR3!=0.0 && SetPosZR3!=0.0 && SetPosXR4!=0.0 && SetPosYR4!=0.0 && SetPosZR4!=0.0 && SetPosXR5!=0.0 && SetPosYR5!=0.0 && SetPosZR5!=0.0 && SetPosXR6!=0.0 && SetPosYR6!=0.0 && SetPosZR6!=0.0){
    out1<<eventID<<" "<<SetPosXR1<<" "<<SetPosYR1<<" "<<SetPosZR1<<" "<<SetPosXR2<<" "<<SetPosYR2<<" "<<SetPosZR2<<" "<<SetPosXR3<<" "<<SetPosYR3<<" "<<SetPosZR3<<" "<<SetPosXR4<<" "<<SetPosYR4<<" "<<SetPosZR4<<" "<<SetPosXR5<<" "<<SetPosYR5<<" "<<SetPosZR5<<" "<<SetPosXR6<<" "<<SetPosYR6<<" "<<SetPosZR6<<" "<<SetKE<<endl;
    out1.close();}
  cout<<"--- . End of event  :"<<eventID<<endl;
  SetPosZ.clear();
  eventID=0;
  SetPosXR1=SetPosYR1=SetPosZR1=SetPosXR2=SetPosYR2=SetPosZR2=SetPosXR3=SetPosYR3=SetPosZR3=SetPosXR4=SetPosYR4=SetPosZR4=SetPosXR5=SetPosYR5=SetPosZR5=SetPosXR6=SetPosYR6=SetPosZR6=SetKE=0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
