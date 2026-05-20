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
int cc =0;

TH1D * projh2X;
TH1D * projh2Y;
TPad *right_pad, *top_pad;

void MomDist1()
{

  TCanvas *c1 = new TCanvas("c1","",0,0,1000,1000);
  // TCanvas *c2 = new TCanvas("c2","",0,0,1000,1000);
  //  c1-> Divide(2,1);
  // TPad *center_pad = new TPad("center_pad", "center_pad",0.0,0.0,0.6,0.6);
  // center_pad->Draw();
   gStyle->SetOptStat(1);
  //right_pad = new TPad("right_pad", "right_pad",0.55,0.0,1.0,0.6);
  // right_pad->Draw();

  //top_pad = new TPad("top_pad", "top_pad",0.0,0.55,0.6,1.0);
  //top_pad->Draw();
   
   //  TH2F* histo = new TH2F("","",75,-250.,250.,75,-250.,250.);
   // TH2F* histo1 = new TH2F("","",45,-150.,150.,45,-150.,150.);
   // TH1D* h1 = new TH1D("< 3 GeV","",100,-50.0,50.);
   //TH1D* h2 = new TH1D("> 3 GeV","",100,-50.0,50.);
   //TH1F* h3 = new TH1F("no threshold","",100,0.,10.);
   //TH1F* h4 = new TH1F("5 mrad","",100,0.,10.);
   TH1F* h5 = new TH1F("10 mrad","",60,0.,3.);
  int evt;
  double R1XX,R1YY,R1ZZ,R2XX,R2YY,R2ZZ,R3XX,R3YY,R3ZZ,R4XX,R4YY,R4ZZ,R5XX,R5YY,R5ZZ,R6XX,R6YY,R6ZZ,mom,R7XX,R7YY,R7ZZ,R8XX,R8YY,R8ZZ;

  long double div,u1,u2,u3,v1,v2,v3,w01,w02,w03,a,b,c,d,e,s,t,P1,P2,P3,Q1,Q2,Q3,M1,M2,M3,kxu,kyu,kxl,kyl,kxs,kys,thetax,thetax1,thetay,theta,corr_theta,thetaxs,thetays,kTheta =0.;
  int line_no =0;
  std::fstream outfile;
  std::ifstream in;
  //outfile.open("Det45PbFull.txt",std::ios::out|std::ios::app);//
  //random noise for spatial resolution.
  double noise = 0.2;

  in.open ("raw.txt");

  while (!in.eof())
    {
      in>>evt>>R1XX>>R1YY>>R1ZZ>>R2XX>>R2YY>>R2ZZ>>R3XX>>R3YY>>R3ZZ>>R4XX>>R4YY>>R4ZZ>>R5XX>>R5YY>>R5ZZ>>R6XX>>R6YY>>R6ZZ>>R7XX>>R7YY>>R7ZZ>>R8XX>>R8YY>>R8ZZ>>mom;

      // include noise

      R1XX = gRandom->Gaus(R1XX,noise);
      R1YY = gRandom->Gaus(R1YY,noise);
      R1ZZ = gRandom->Gaus(R1ZZ,noise);
      R2XX = gRandom->Gaus(R2XX,noise);
      R2YY = gRandom->Gaus(R2YY,noise);
      R2ZZ = gRandom->Gaus(R2ZZ,noise);
      R3XX = gRandom->Gaus(R3XX,noise);
      R3YY = gRandom->Gaus(R3YY,noise);
      R3ZZ = gRandom->Gaus(R3ZZ,noise);
      R4XX = gRandom->Gaus(R4XX,noise);
      R4YY = gRandom->Gaus(R4YY,noise);
      R4ZZ = gRandom->Gaus(R4ZZ,noise);
      R5XX = gRandom->Gaus(R5XX,noise);
      R5YY = gRandom->Gaus(R5YY,noise);
      R5ZZ = gRandom->Gaus(R5ZZ,noise);
      R6XX = gRandom->Gaus(R6XX,noise);
      R6YY = gRandom->Gaus(R6YY,noise);
      R6ZZ = gRandom->Gaus(R6ZZ,noise);
      R7XX = gRandom->Gaus(R7XX,noise);
      R7YY = gRandom->Gaus(R7YY,noise);
      R7ZZ = gRandom->Gaus(R7ZZ,noise);
      
      

      //----scattering angle calculation--------------------------------------------
      kxu = (R3ZZ-R1ZZ)/(R3XX-R1XX);
      kyu = (R3ZZ-R1ZZ)/(R3YY-R1YY);
      kxl = (R6ZZ-R4ZZ)/(R6XX-R4XX);
      kyl = (R6ZZ-R4ZZ)/(R6YY-R4YY);
      thetax = atan(abs((kxl-kxu)/(1+kxl*kxu)));
      thetax1 = atan((kxl-kxu)/(1+kxl*kxu));
      thetay = atan(abs((kyl-kyu)/(1+kyl*kyu)));
      theta = sqrt(thetax*thetax+thetay*thetay)/2.0;
      corr_theta = theta*(mom*mom/1000000);
      

      // known scatterer-----------------------------------------------------------
      kxs = (R7ZZ-R6ZZ)/(R7XX-R6XX);
      kyu = (R7ZZ-R6ZZ)/(R7YY-R6YY);
      thetaxs = atan((kxs-kxl)/(1+kxs*kxl));
      thetays = atan(abs((kys-kyl)/(1+kys*kyl)));
      kTheta = sqrt(thetaxs*thetaxs+thetays*thetays)/2.0;
     
     //  h3-> Fill(mom/1000.0);
//       //      cout<<mom<<endl;

//       if (mom<3000.0)
// 	{h1->Fill(thetax1*1000);
// 	 //cout<<thetaxs<<endl;
// }

//       if (mom>3000.0)
// 	{h2->Fill(thetax1*1000);}



      
//       if (theta >= 0.005) 
// 	{  h4-> Fill(mom/1000.0);
// 	}
      if (theta >= 0.01)
	{h5-> Fill(mom/1000.0);}
      //  if (evt >= 60000000) break;  //500000000
     
    }
  in.close();
  //  outfile.close();
  
    // cout <<"---line no = "<<evt<<endl;
    // double scale = 1.0/(h2->Integral());
    // h1->Scale(scale);
    // h2->Scale(scale);

    // double scale1 = 1.0/(h3->Integral());
    // h3->Scale(scale1);
    // h4->Scale(scale1);
    // h5->Scale(scale1);
    // c1->cd();
    // h2->Draw();
    // h1->Draw("same");
    // c2->cd();
    // h3->Draw();
    // h4->Draw("same");
    h5->Draw();

    // h1->SetLineColor(2);
    //h2->SetLineColor(6);
    //h3->SetLineColor(2);
    //h4->SetLineColor(6);
    h5->SetLineColor(4);
    
    //h1->SetLineWidth(2);
    //h2->SetLineWidth(2);
    //h3->SetLineWidth(2);
    //h4->SetLineWidth(2);
    h5->SetLineWidth(2);


    TF1*f1 = new TF1("f1","gaus",0.02,0.5);
    TF1*f2 = new TF1("f2","expo",0.0,0.4);
    TF1*f4 = new TF1("f4","expo",0.501,2.9);
    TF1*tot = new TF1("total","gaus(0)+expo(3)",0.02,2.9);
    h5->Fit("f1","R");
    h5->Fit("f4","R+");
    double par[5];
    f1->GetParameters(&par[0]);
    f4->GetParameters(&par[3]);
    tot->SetParameters(par);
    h5->Fit(tot,"R+");


    //   h2->SetTitle ("Scattering Angle, Concrete");
    //h2->GetXaxis()->SetTitle ("Scattering angle (mrad)");
    //h2->GetYaxis()->SetTitle ("Entries");
   // h3->SetTitle ("Momentum Distribution");
   //h3->GetXaxis()->SetTitle ("Momentum (GeV)");
   //h3->GetYaxis()->SetTitle ("Entries");

  // histo1->GetYaxis()->SetTitle ("y-axis (mm)");
      
     

  // top_pad->cd();
  // // projh2X->SetFillColor(kBlue+1);
  // projh2X->Draw("bar");
  // histo1->GetZaxis()->SetRangeUser(0.0, 700.0);
  // histo->GetZaxis()->SetRangeUser(0.0, 700.0);
      

  // right_pad->cd();
  // //projh2Y->SetFillColor(kBlue-2);
  // projh2Y->Draw("bar");
  // projh2Y->GetXaxis()->SetRangeUser(-150.0,150.0);
  // c1->cd();
  // TLatex *t = new TLatex();
  // t->SetTextFont(42);
  // t->SetTextSize(0.02);
  // t->DrawLatex(0.6,0.88,"This example demonstrates how to display");
  // t->DrawLatex(0.6,0.85,"a histogram and its two projections.");

      //      c1->SaveAs("r2.pdf");
      
}



