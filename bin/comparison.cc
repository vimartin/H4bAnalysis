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
#include <boost/algorithm/string.hpp>
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
  bool normalizeUnity = reader.GetBoolean("General", "normalizeUnity", false);
  int drawMode = reader.GetReal("General", "drawMode", 0);
  bool drawLegend = reader.GetBoolean("General", "drawLegend", false);
  std::string drawLatex = reader.Get("General", "drawLatex","");
  std::string saveFolder = reader.Get("General", "saveFolder","");

  boost::replace_all(drawLatex, "\"", "");
  boost::replace_all(saveFolder, "\"", "");


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
    std::cout<<address_map[sam]<<std::endl;
    isSignal_map[sam] =  reader.GetBoolean(sam, "isSignal", false); //dummie
    scale_map[sam] =     reader.GetInteger(sam, "scale", 1.0);
    lineColor[sam] =     reader.GetInteger(sam, "lineColor", 0);
    fillColor[sam] =     reader.GetInteger(sam, "fillColor", 0);
    counter++;

    if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_ggH_H125_a20a20_bbbb")
      sampleTitles.push_back("ggF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 20 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_ggH_H125_a30a30_bbbb")
      sampleTitles.push_back("ggF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 30 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_ggH_H125_a40a40_bbbb")
      sampleTitles.push_back("ggF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 40 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_ggH_H125_a50a50_bbbb")
      sampleTitles.push_back("ggF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 50 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_ggH_H125_a60a60_bbbb")
      sampleTitles.push_back("ggF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 60 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_VBF_H125_a20a20_bbbb")
      sampleTitles.push_back("VBF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 20 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_VBF_H125_a30a30_bbbb")
      sampleTitles.push_back("VBF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 30 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_VBF_H125_a40a40_bbbb")
      sampleTitles.push_back("VBF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 40 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_VBF_H125_a50a50_bbbb")
      sampleTitles.push_back("VBF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 50 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_VBF_H125_a60a60_bbbb")
      sampleTitles.push_back("VBF, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 60 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WplusmunuH_H125_a20a20_bbbb")
      sampleTitles.push_back("W^{+}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 20 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WplusmunuH_H125_a30a30_bbbb")
      sampleTitles.push_back("W^{+}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 30 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WplusmunuH_H125_a40a40_bbbb")
      sampleTitles.push_back("W^{+}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 40 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WplusmunuH_H125_a50a50_bbbb")
      sampleTitles.push_back("W^{+}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 50 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WplusmunuH_H125_a60a60_bbbb")
      sampleTitles.push_back("W^{+}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 60 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WminusmunuH_H125_a20a20_bbbb")
      sampleTitles.push_back("W^{-}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 20 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WminusmunuH_H125_a30a30_bbbb")
      sampleTitles.push_back("W^{-}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 30 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WminusmunuH_H125_a40a40_bbbb")
      sampleTitles.push_back("W^{-}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 40 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WminusmunuH_H125_a50a50_bbbb")
      sampleTitles.push_back("W^{-}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 50 GeV");
    else if (sam=="PowhegPy8EG_AZNLOCTEQ6L1_WminusmunuH_H125_a60a60_bbbb")
      sampleTitles.push_back("W^{-}(#mu#nu)h, h#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 60 GeV");


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
    else if (distribution=="h_aa_dR")
      histoXTitle="#DeltaR(a, a)";
    else if (distribution=="h_mumu_dR")
      histoXTitle="#DeltaR(#mu^{+}, #mu^{-})";
    else if (distribution=="h_aa_meanDeltaR"){
      histoXTitle="m_{a} [GeV]";
      histoYTitle="Mean #DeltaR(a, a)";}
    else if (distribution=="h_bbar_meanDeltaR"){
      histoXTitle="m_{a} [GeV]";
      histoYTitle="Mean #DeltaR(b, #bar{b})";}
    else if (distribution=="h_bparton_pt")
      histoXTitle="p_{T}(b) [GeV]";
    else if (distribution=="h_bparton_eta")
      histoXTitle="#eta(b)";
    else if (distribution=="h_hparton_pt")
      histoXTitle="p_{T}(h) [GeV]";
    else if (distribution=="h_hparton_eta")
      histoXTitle="#eta(h)";
    else if (distribution=="h_aparton_pt")
      histoXTitle="p_{T}(a) [GeV]";
    else if (distribution=="h_aparton_eta")
      histoXTitle="#eta(a)";
    else if (distribution=="h_wparton_pt")
      histoXTitle="p_{T}(W) [GeV]";
    else if (distribution=="h_wparton_eta")
      histoXTitle="#eta(W)";
    else if (distribution=="h_muparton_pt")
      histoXTitle="p_{T}(#mu) [GeV]";
    else if (distribution=="h_muparton_m")
      histoXTitle="m(#mu^{+}, #mu^{-}) [GeV]";
    else if (distribution=="h_aparton_pt")
      histoXTitle="p_{T}(a)";
    else if (distribution=="h_fatjet_trimmed_mass_verena-passLepton-passJets-passSubstr-passSubJet-passSubBJet")
      histoXTitle="Invariant mass [GeV]";
    else if (distribution=="h_lepton_preselected_pt")
      histoXTitle="p_{T}(l from W) [GeV]";
    else if (distribution.find("h_lepton_pt") != std::string::npos)
      histoXTitle="Lepton p_{T} [GeV]";
    else if (distribution.find("nbjets") != std::string::npos)
      histoXTitle="B-jet multiplicity";
    else if (distribution.find("bjets_pt") != std::string::npos)
      histoXTitle="B-jets p_{T} [GeV]";
    else if (distribution.find("bjets_eta") != std::string::npos)
      histoXTitle="B-jets #eta";
    else if (distribution.find("nljets") != std::string::npos)
      histoXTitle="Light jet multiplicity";
    else if (distribution.find("ljets_pt") != std::string::npos)
      histoXTitle="Light jets p_{T} [GeV]";
    else if (distribution.find("ljets_eta") != std::string::npos)
      histoXTitle="Light jets #eta";
    else if (distribution.find("nvbfjets") != std::string::npos)
      histoXTitle="VBF jet multiplicity";
    else if (distribution.find("vbfjets_pt") != std::string::npos)
      histoXTitle="VBF jets p_{T} [GeV]";
    else if (distribution.find("vbfjets_eta") != std::string::npos)
      histoXTitle="VBF jets #eta";
    else if (distribution.find("njets") != std::string::npos)
      histoXTitle="Jet multiplicity";
    else if (distribution.find("jets_pt") != std::string::npos)
      histoXTitle="Jets p_{T} [GeV]";
    else if (distribution.find("jets_eta") != std::string::npos)
      histoXTitle="Jets #eta";
    else if (distribution.find("HMass") != std::string::npos)
      histoXTitle="Reconstructed H mass [GeV]";
    else if (distribution.find("AMass") != std::string::npos)
      histoXTitle="Reconstructed a mass [GeV]";
    else if (distribution.find("vbfjets_deltaEta") != std::string::npos)
      histoXTitle="VBF jets |#Delta#eta|";
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
    plot->setGlobalProperties(luminosity, doLogScale, savePlot, normalizeUnity, drawMode, drawLegend, drawLatex, saveFolder);
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
