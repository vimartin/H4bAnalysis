#include "ExoticHiggs/PlottingClass.h"
#include "inih/INIReader.h"

#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "THStack.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TPad.h"

#include <algorithm>
#include <iostream> // needed for io
#include <cstdio>   // needed for io
#include <fstream>
#include <cmath>
#include <map>

#include "TString.h"

using namespace std;

int main(int argc, char** argv){

  // Parameters
  if (argc != 2) {
    cout << "plotter.exe [configFile]" << endl;
    return 0;
  }
  INIReader reader(argv[1]);
  if (reader.ParseError() < 0) {
    std::cout << "Can't load " << argv[1] << endl;
    return 1;
  }

  // Allow showing TCanvas
  TApplication theApp("tapp", &argc, argv);
  
  // Get general info
  double luminosity = reader.GetReal("General", "luminosity", 1.0);
  std::string distribution = reader.Get("General", "distribution", "UNKOWN");
  bool doLogScale = reader.GetBoolean("General", "doLogScale", false);

  // Initialize maps
  std::map<std::string, process> sample_map;
  std::map<std::string, std::string> address_map;
  std::map<std::string, bool>        isSignal_map;
  std::map<std::string, float>       scale_map;
  std::map<std::string, int>      lineColor;
  std::map<std::string, int>      fillColor;

  // Get Background properties
  std::vector<std::string> bkgName = convertStringToVector(reader.Get("Backgrounds", "processes", "UNKNOWN"));
  for (auto bkg : bkgName){
    sample_map[bkg] = process(); 

    address_map[bkg] =   Form("results/%s", reader.Get(bkg, "address", "UNKNOWN").c_str()); 
    isSignal_map[bkg] =  reader.GetBoolean(bkg, "isSignal", false);
    scale_map[bkg] =     reader.GetInteger(bkg, "scale", 1.0);
    lineColor[bkg] =     reader.GetInteger(bkg, "lineColor", 0);
    fillColor[bkg] =     reader.GetInteger(bkg, "fillColor", 0);

    sample_map[bkg].setDistribution(address_map[bkg], distribution);
    sample_map[bkg].setLuminosity(luminosity);
    sample_map[bkg].setIsSignal(isSignal_map[bkg]);
    sample_map[bkg].setProperties(scale_map[bkg], lineColor[bkg], fillColor[bkg]);
  }

  // Get Signal properties
  std::vector<std::string> sigName = convertStringToVector(reader.Get("Signals", "processes", "UNKNOWN"));
  for (auto sig : sigName){
    sample_map[sig] = process(); 

    address_map[sig] =  Form("results/%s", reader.Get(sig, "address", "UNKNOWN").c_str()); 
    isSignal_map[sig] = reader.GetBoolean(sig, "isSignal", false);
    scale_map[sig] =    reader.GetInteger(sig, "scale", 1.0);
    lineColor[sig] =    reader.GetInteger(sig, "lineColor", 0);
    fillColor[sig] =    reader.GetInteger(sig, "fillColor", 0);

    sample_map[sig].setDistribution(address_map[sig], distribution);
    sample_map[sig].setLuminosity(luminosity);
    sample_map[sig].setIsSignal(isSignal_map[sig]);
    sample_map[sig].setProperties(scale_map[sig], lineColor[sig], fillColor[sig]);
  }


  //--- P L O T T I N G

  plotStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  //--- Define canvas and pads  
  TCanvas *c1 = new TCanvas(Form("c1_%s", distribution.c_str()), Form("c1_%s", distribution.c_str()), 700, 600);
  c1->cd();

  TPad *pad1 = new TPad(Form("pad1_%s", distribution.c_str()), Form("pad1_%s", distribution.c_str()), 0, 0.305, .99, 1);
  pad1->SetBottomMargin(0);
  pad1->Draw("same");

  TPad *pad2 = new TPad(Form("pad2_%s", distribution.c_str()), Form("pad2_%s", distribution.c_str()), 0, 0.01, .99, 0.295);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->SetGrid(0,1);
  pad2->Draw("same");

  //--- Create THStack for Bkg and signal processes.
  pad1->cd();
  float n_totalbkg = 0.;
  THStack *sbkg = new THStack(Form("sbkg_%s", distribution.c_str()), "");

  TH1F *hbkg;
  TH1F *hsig;
  for (auto bkg : bkgName){
    TH1F *htemp = (TH1F*) sample_map[bkg.c_str()].getHistogram()->Clone();
    hbkg = new TH1F(Form("h_bkg_%s", distribution.c_str()), "", htemp->GetSize()-2, htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax());
    hsig = new TH1F(Form("h_sig_%s", distribution.c_str()), "", htemp->GetSize()-2, htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax());
    htemp->Delete();
    break;
  }

  for (auto bkg : bkgName){
    sbkg->Add(sample_map[bkg.c_str()].getHistogram());
    hbkg->Add(sample_map[bkg.c_str()].getHistogram());
    n_totalbkg += sample_map[bkg.c_str()].getHistogram()->Integral();
  }

  THStack *ssig = new THStack(Form("ssig_%s", distribution.c_str()), "");
  float n_totalsig = 0.;
  for (auto sig : sigName){
    ssig->Add(sample_map[sig.c_str()].getHistogram());
    hsig->Add(sample_map[sig.c_str()].getHistogram());
    n_totalsig += sample_map[sig.c_str()].getHistogram()->Integral();
  }

  //--- Set Log scale, maximum and minimum for the plot in pad1
  float ymax = 0.;
  for (int ibin=1; ibin<hbkg->GetSize()-1; ibin++){
    if (hbkg->GetBinContent(ibin)>ymax) ymax = hbkg->GetBinContent(ibin);
  }
  for (int ibin=1; ibin<hsig->GetSize()-1; ibin++){
    if (hsig->GetBinContent(ibin)>ymax) ymax = hsig->GetBinContent(ibin);
  }
  sbkg->SetMaximum(ymax*1.2);
  sbkg->SetMinimum(0.);
  if (doLogScale){
    sbkg->SetMinimum(min(n_totalbkg, n_totalsig)/100.);
    gPad->SetLogy();
  }

  //--- Draw THStacks and histograms
  sbkg->Draw("histo");
  ssig->Draw("histosame");

  //--- Set cosmetics for THStack
  sbkg->GetHistogram()->GetYaxis()->SetLabelSize(0.04);
  sbkg->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
  sbkg->GetHistogram()->GetYaxis()->SetTitleOffset(0.7);
  sbkg->GetHistogram()->SetYTitle("Events");


  //---
  //---
  //--- Ratio plot (only shape)
  pad2->cd();
  if (hbkg->Integral()>0.) hbkg->Scale(1./hbkg->Integral());
  if (hsig->Integral()>0.) hsig->Scale(1./hsig->Integral());
  hsig->Divide(hbkg);

  //--- Set cosmetics for ratio plot
  hsig->GetXaxis()->SetTitleSize(0.145);
  hsig->GetXaxis()->SetTitleOffset(0.93);
  hsig->GetXaxis()->SetLabelSize(0.1);
  hsig->GetXaxis()->SetTitle(distribution.c_str());

  hsig->GetYaxis()->SetNdivisions(3, 5, 0, true);
  hsig->GetYaxis()->SetTitle("S/B (norm.)");
  hsig->GetYaxis()->SetTitleSize(0.15);
  hsig->GetYaxis()->SetTitleOffset(0.24);
  hsig->GetYaxis()->SetLabelSize(0.1);
  hsig->SetMaximum(1.7);
  hsig->SetMinimum(0.5);

  //--- Draw ratio plot
  hsig->Draw();


  c1->Update();
  theApp.Run();
  return 0;
}
