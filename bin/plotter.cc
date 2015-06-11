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
#include "TColor.h"
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
  bool doLogScale = reader.GetBoolean("General", "doLogScale", false);
  bool savePlot = reader.GetBoolean("General", "savePlot", false);

  // Get distributions
  std::vector<std::string> distribution_vec = convertStringToVector(reader.Get("General", "distribution", "UNKOWN"));
  std::vector<std::string> new_distribution_vec;
  if (distribution_vec.size()>0){
    if (distribution_vec.at(0).find(".txt") != std::string::npos){
      ifstream distr_file;
      distr_file.open(Form("JOs/plotter/%s",distribution_vec.at(0).c_str()));
      std::string d;
      while (distr_file >> d){
        new_distribution_vec.push_back(d);
        d.clear();
      }
      distr_file.close();
      distribution_vec = new_distribution_vec;
    }
  }

  // Initialize maps
  std::map<std::string, int>         index_map;
  std::map<std::string, std::string> address_map;
  std::map<std::string, bool>        isSignal_map;
  std::map<std::string, float>       scale_map;
  std::map<std::string, int>         lineColor;
  std::map<std::string, int>         fillColor;


  // Get Background properties
  std::vector<std::string> bkgName = convertStringToVector(reader.Get("Backgrounds", "processes", "UNKNOWN"));
  int counter = 0;
  for (auto bkg : bkgName){
    index_map[bkg] = counter;
    address_map[bkg] =   Form("results/%s", reader.Get(bkg, "address", "UNKNOWN").c_str()); 
    isSignal_map[bkg] =  reader.GetBoolean(bkg, "isSignal", false);
    scale_map[bkg] =     reader.GetInteger(bkg, "scale", 1.0);
    lineColor[bkg] =     reader.GetInteger(bkg, "lineColor", 0);
    fillColor[bkg] =     reader.GetInteger(bkg, "fillColor", 0);
    counter++;
  }

  // Get Signal properties
  std::vector<std::string> sigName = convertStringToVector(reader.Get("Signals", "processes", "UNKNOWN"));
  counter = 0;
  for (auto sig : sigName){
    index_map[sig] = counter;
    address_map[sig] =  Form("results/%s", reader.Get(sig, "address", "UNKNOWN").c_str()); 
    isSignal_map[sig] = reader.GetBoolean(sig, "isSignal", false);
    scale_map[sig] =    reader.GetInteger(sig, "scale", 1.0);
    lineColor[sig] =    reader.GetInteger(sig, "lineColor", 0);
    fillColor[sig] =    reader.GetInteger(sig, "fillColor", 0);
    counter++;
  }

  //--- P L O T T I N G

  plotStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  for (auto distribution : distribution_vec) {
    std::cout<<"\nPlotting "<<distribution<<std::endl;
    plotClass *plot = new plotClass(distribution);
    plot->setGlobalProperties(luminosity, doLogScale, savePlot);
    plot->setSampleNames(bkgName, sigName);
    plot->setSampleProperties(index_map, address_map, isSignal_map, scale_map, lineColor, fillColor);
    plot->read();
    plot->prepareCanvas();
    plot->plotUpperPad();
    plot->plotLowerPad();
    plot->plotSidePad();
    plot->finalize();
    if (distribution.find("total_events") != std::string::npos){
      plot->printSummary();
    }
    if (savePlot) delete plot;
  }

  if (!savePlot){
    theApp.Run();
  }

  return 0;
}
