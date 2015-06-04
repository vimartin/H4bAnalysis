#include "ExoticHiggs/PlottingClass.h"
#include "inih/INIReader.h"

#include "TApplication.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TColor.h"
#include "TCanvas.h"

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
  
  // Get luminosity
  double luminosity = reader.GetReal("General", "luminosity", 1.0);

  // Get distribution
  std::string distribution = reader.Get("General", "distribution", "UNKOWN");

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
    scale_map[bkg] =     reader.GetBoolean(bkg, "isSignal", false);
    lineColor[bkg] =     reader.GetInteger(bkg, "lineColor", 0);
    fillColor[bkg] =     reader.GetInteger(bkg, "fillColor", 0);

    sample_map[bkg].setDistribution(address_map[bkg], distribution);
    sample_map[bkg].setLuminosity(luminosity);
    sample_map[bkg].setIsSignal(isSignal_map[bkg]);
  }

  // Get Signal properties
  std::vector<std::string> sigName = convertStringToVector(reader.Get("Signals", "processes", "UNKNOWN"));
  for (auto sig : sigName){
    sample_map[sig] = process(); 

    address_map[sig] =  Form("results/%s", reader.Get(sig, "address", "UNKNOWN").c_str()); 
    isSignal_map[sig] = reader.GetBoolean(sig, "isSignal", false);
    scale_map[sig] =    reader.GetBoolean(sig, "isSignal", false);
    lineColor[sig] =    reader.GetInteger(sig, "lineColor", 0);
    fillColor[sig] =    reader.GetInteger(sig, "fillColor", 0);

    sample_map[sig].setDistribution(address_map[sig], distribution);
    sample_map[sig].setLuminosity(luminosity);
    sample_map[sig].setIsSignal(isSignal_map[sig]);
  }
//--- Check this part! Pointers??
  TH1F *mysignal = new TH1F(*sample_map["WplusenuH_H125_a20a20_bbbb"].getHistogram());
  cout<<typeid(sample_map["WplusenuH_H125_a20a20_bbbb"].getHistogram()).name()<<endl;
  cout<<mysignal->Integral()<<endl;
  TCanvas *c1 = new TCanvas();
  c1->cd();
  mysignal->Draw();


  theApp.Run();
  return 0;
}
