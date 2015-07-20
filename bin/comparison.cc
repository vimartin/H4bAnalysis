#include "ExoticHiggs/PlottingClass.h"
#include "ExoticHiggs/ComparisonClass.h"
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
    cout << "comparison.exe [configFile]" << endl;
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

  std::vector<std::string> sampleTitles;

  // Get Samples properties
  std::vector<std::string> samplesName = convertStringToVector(reader.Get("Sample", "processes", "UNKNOWN"));
  int counter = 0;
  for (auto sam : samplesName){
    std::cout<<sam<<std::endl;
    index_map[sam] = counter;
    address_map[sam] =   Form("results/%s", reader.Get(sam, "address", "UNKNOWN").c_str()); 
    isSignal_map[sam] =  reader.GetBoolean(sam, "isSignal", false); //dummie
    scale_map[sam] =     reader.GetInteger(sam, "scale", 1.0);
    lineColor[sam] =     reader.GetInteger(sam, "lineColor", 0);
    fillColor[sam] =     reader.GetInteger(sam, "fillColor", 0);
    counter++;

    if (sam=="WplusenuH_H125_a20a20_bbbb")
      sampleTitles.push_back("WH, H#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 20 GeV");
    else if (sam=="WplusenuH_H125_a60a60_bbbb")
      sampleTitles.push_back("WH, H#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 60 GeV");
    else if (sam=="ggF_H125_a20a20_bbmumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 20 GeV");
//      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow b#bar{b}b#bar{b}, m_{a} = 20 GeV");
    else if (sam=="ggF_H125_a60a60_bbmumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 60 GeV");
//      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow b#bar{b}b#bar{b}, m_{a} = 60 GeV");
    else if (sam=="ggF_H125_a01a01_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 1 GeV");
    else if (sam=="ggF_H125_a05a05_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 5 GeV");
    else if (sam=="ggF_H125_a10a10_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 10 GeV");
    else if (sam=="ggF_H125_a20a20_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 20 GeV");
    else if (sam=="ggF_H125_a30a30_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 30 GeV");
    else if (sam=="ggF_H125_a40a40_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 40 GeV");
    else if (sam=="ggF_H125_a50a50_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 50 GeV");
    else if (sam=="ggF_H125_a60a60_mumumumu")
      sampleTitles.push_back("ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}, m_{a} = 60 GeV");
    if (sam=="WplusenuH_H125_a20a20_bbbb_1")
      sampleTitles.push_back("Invariant mass 1 jet");
//      sampleTitles.push_back("No cut");
    if (sam=="WplusenuH_H125_a20a20_bbbb_2")
      sampleTitles.push_back("Invariant mass 2 jets");
//      sampleTitles.push_back("p_{T} > 26 GeV (Eff = 75%)");
    if (sam=="WplusenuH_H125_a20a20_bbbb_3")
      sampleTitles.push_back("p_{T} > 40 GeV (Eff = 52%)");
  }



  //--- P L O T T I N G

  plotStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  std::string histoXTitle;
  std::string histoYTitle("Entries (a.u.)");
  for (auto distribution : distribution_vec) {

    if (distribution=="h_bbar_dR")
      histoXTitle="#DeltaR(b, #bar{b})";
    else if (distribution=="h_mumu_dR")
      histoXTitle="#DeltaR(#mu^{+}, #mu^{-})";
    else if (distribution=="h_bparton_pt")
      histoXTitle="p_{T}(b)";
    else if (distribution=="h_muparton_pt")
      histoXTitle="p_{T}(#mu)";
    else if (distribution=="h_muparton_m")
      histoXTitle="m(#mu^{+}, #mu^{-})";
    else if (distribution=="h_aparton_pt")
      histoXTitle="p_{T}(a)";
    else if (distribution=="h_fatjet_trimmed_mass_verena-passLepton-passJets-passSubstr-passSubJet-passSubBJet")
      histoXTitle="Invariant mass";
    else if (distribution=="h_lepton_preselected_pt")
      histoXTitle="p_{T}(l from W)";
    else if (distribution=="h_dRmean_vs_mass"){
      histoXTitle="m_{a} [GeV]";
      histoYTitle="Mean #DeltaR(#mu^{+}, #mu^{-})";
    }
    else if (distribution=="h_pTmean_vs_mass"){
      histoXTitle="m_{a} [GeV]";
      histoYTitle="Mean p_{T}(#mu) [GeV]";
    }
    else{
      histoXTitle="<nothing>";
    }

    std::cout<<"\nPlotting "<<distribution<<std::endl;
    comparisonClass *plot = new comparisonClass(distribution);
    plot->setGlobalProperties(luminosity, doLogScale, savePlot);
    plot->setSampleNames(samplesName);
    plot->setSampleProperties(index_map, address_map, isSignal_map, scale_map, lineColor, fillColor);
    plot->setSamplesAndHistoTitles(histoXTitle, histoYTitle,  sampleTitles);
    plot->read();
    plot->prepareCanvas();
    plot->plotUpperPad();
    plot->finalize();
    if (savePlot) delete plot;
  }

  if (!savePlot){
    theApp.Run();
  }

  return 0;
}