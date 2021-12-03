#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


void Mom_Gen()
{

  long int evt;
  double R1XX,R1YY,R1ZZ,R2XX,R2YY,R2ZZ,R3XX,R3YY,R3ZZ,R4XX,R4YY,R4ZZ,R5XX,R5YY,R5ZZ,R6XX,R6YY,R6ZZ,mom,R7XX,R7YY,R7ZZ,R8XX,R8YY,R8ZZ;
  std::ifstream in;
  ofstream out1,out2,out3;
  double f_mom1,f_mom2,f_mom3;
  TCanvas *c1 = new TCanvas("c1","",0,0,1000,1000);
  TH1F* histo = new TH1F("","",200,0.,2.9);

  out1.open("Mom_mix_gen.txt");
  //  out2.open("/home/sridhar/MT/ConcreteApplications/MT_concrete_Rust2/Mom_Exp1.txt");
  //  out3.open("/home/sridhar/MT/ConcreteApplications/MT_concrete_Rust2/Mom_Mid.txt");
  cout<<"--Reached--"<<endl;

  for (int i =0;i<100230;i++)
    {
      double med_mom = 700.0;
      double RMS_mom = 100.0;
      //      f_mom3 = gRandom->Gaus(med_mom,RMS_mom);      
      //     out3<<f_mom3<<endl;
  //------ Momentum Scaling-----
     
      // double par1[2] = {9.37,-2.18e-03}; 
      // double par2[3] = {4.36e+03,3.42e+02,3.265e+02};
      // TF1* f1 = new TF1("f1","expo",700.0,3100.0);
      // TF1 * f2 = new TF1("f2","gaus",0.0,699.0);
      // f1->SetParameters(par1);
      // f2->SetParameters(par2);
      // f_mom1 = f1->GetRandom();
      // f_mom2 = f2->GetRandom();
      // out1<<f_mom1<<endl;
      // out2<<f_mom2<<endl;
   double parcf[5]={3.14e+03,2.48e-01,-6.06e-1,7.33,-8.37e-01};
      double par4[2]= {6.85,-1.82e-03};
      double par5[2]= {5.8,-7.23e-03};
      TF1*tot = new TF1("total","gaus(0)+expo(3)",0.02,2.9);
      tot->SetParameters(parcf); //it was parnew1
      //f2->SetParameters(par2);
      // TF1* f1 = new TF1("f1","expo",601.0,2500.0);
      //f1->SetParameters(par1);
      //TF1* f2 = new TF1("f2","gaus",30.0,600.0);
      //f2->SetParameters(par2);
      //      f_mom1 = tot->GetRandom();
      f_mom1 = tot->GetRandom();
      histo->Fill(f_mom1);
         out1<<f_mom1<<endl;
    
    }
 out1.close(); 
  histo->Draw();
  histo->GetXaxis()->SetTitle("momentum(GeV)");
  //  c1->SaveAs("Gen_mix_frej.png");
  //  out2.close(); 
  //out3.close(); 
}
