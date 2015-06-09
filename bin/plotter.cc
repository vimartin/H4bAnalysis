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
  std::vector<std::string> distribution_vec = convertStringToVector(reader.Get("General", "distribution", "UNKOWN"));
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
    address_map[bkg] =   Form("results/%s", reader.Get(bkg, "address", "UNKNOWN").c_str()); 
    isSignal_map[bkg] =  reader.GetBoolean(bkg, "isSignal", false);
    scale_map[bkg] =     reader.GetInteger(bkg, "scale", 1.0);
    lineColor[bkg] =     reader.GetInteger(bkg, "lineColor", 0);
    fillColor[bkg] =     reader.GetInteger(bkg, "fillColor", 0);
  }

  // Get Signal properties
  std::vector<std::string> sigName = convertStringToVector(reader.Get("Signals", "processes", "UNKNOWN"));
  for (auto sig : sigName){
    address_map[sig] =  Form("results/%s", reader.Get(sig, "address", "UNKNOWN").c_str()); 
    isSignal_map[sig] = reader.GetBoolean(sig, "isSignal", false);
    scale_map[sig] =    reader.GetInteger(sig, "scale", 1.0);
    lineColor[sig] =    reader.GetInteger(sig, "lineColor", 0);
    fillColor[sig] =    reader.GetInteger(sig, "fillColor", 0);

  }

  //--- P L O T T I N G

  plotStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  for (auto distribution : distribution_vec) {
    plotClass *plot = new plotClass(distribution);
    plot->setGlobalProperties(luminosity, doLogScale);
    plot->setSampleNames(bkgName, sigName);
    plot->setSampleProperties(address_map, isSignal_map, scale_map, lineColor, fillColor);
    plot->read();
    plot->prepareCanvas();
    plot->plotUpperPad();
    plot->plotLowerPad();
  }

  theApp.Run();
  return 0;
}
