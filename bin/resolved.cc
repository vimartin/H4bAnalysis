// Resolved Analysis

#include "fastjet/ClusterSequence.hh"

#include "generator/McEventCollection_p5.h"
#include "generator/GenParticle_p5.h"
#include "generator/GenEvent_p5.h"

#include "inih/INIReader.h"

#include "ExoticHiggs/LeptonUtils.h"
#include "ExoticHiggs/JetUtils.h"
#include "ExoticHiggs/PlotUtilities.h"

#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TH1D.h"
#include "TH1.h"
#include "TH2D.h"
#include "TF1.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TString.h"
#include "TFileCollection.h"

#include <algorithm>
#include <iostream> // needed for io
#include <cstdio>   // needed for io
#include <cmath>
#include <fstream>

using namespace std;


int main(int argc, char** argv){

  // Parameters
  if (argc != 2) {
    cout << "resolved.exe [configFile]" << endl;
    return 0;
  }
  INIReader reader(argv[1]);
  if (reader.ParseError() < 0) {
    std::cout << "Can't load " << argv[1] << endl;
    return 1;
  }

  // Jet definition
  double jet_R = reader.GetReal("jet", "R", 0.4);
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, jet_R);

  // Jet selection
  double jet_ptmin = reader.GetReal("jet", "ptmin", 40000.0);
  double jet_etamax = reader.GetReal("jet", "etamax", 2.6);

  // Lepton selection
  double lep_ptmin = reader.GetReal("lepton", "lep_ptmin", 26000.0);
  double lep_etamax = reader.GetReal("lepton", "lep_etamax", 2.47);
  double lep_isomax = reader.GetReal("lepton", "lep_isomax", 0.5); // mini iso limit

  // Ghost factor for truth-level B-jet tagging
  double ghost_factor = reader.GetReal("btagging", "ghost_factor", 1.e-21);

  // Use track jets
  bool use_only_charged = reader.GetBoolean("jet", "use_only_charged", false);

  // files
  std::string dataFileName = reader.Get("io", "data_file_name", "UNKNOWN");
  std::string outputFileName = reader.Get("io", "result_file_name", "UNKNOWN");
  bool isSignal = reader.GetBoolean("io", "isSignal", false);
  bool isPythia6 = reader.GetBoolean("io", "isPythia6", false);

  // Used for the track jets
  TDatabasePDG *db= TDatabasePDG::Instance();

  // Read TTree and get the input
  TChain* CollectionTree = new TChain("CollectionTree");
  TFileCollection fc("dum","",Form("JOs/inputList/%s", dataFileName.c_str()));
  CollectionTree->AddFileInfoList((TCollection*) fc.GetList());
  //  TBranch* branch = CollectionTree->GetBranch("McEventCollection_p5_GEN_EVENT");
  McEventCollection_p5* event = 0;

  // I don't care about the other information
  CollectionTree->SetBranchStatus("*", 0);
  CollectionTree->SetBranchStatus("McEventCollection_p5_GEN_EVENT", 1);
  CollectionTree->SetBranchAddress("McEventCollection_p5_GEN_EVENT", &event);

  Long64_t nentries = CollectionTree->GetEntries();
  cout << "Reading " << nentries << " events" << endl;

  // Define maps for the histograms
  std::map<std::string, TH1*> h_1d; 
  std::map<std::string, TH2D*> h_2d;

  // Define cutflow bin titles
  std::vector<std::string> cutflow_bin_title;
  cutflow_bin_title.push_back("all");
  cutflow_bin_title.push_back("1 lep");
  cutflow_bin_title.push_back("lep pt");
  cutflow_bin_title.push_back("lep eta");
  cutflow_bin_title.push_back("lep iso");
  cutflow_bin_title.push_back("4 jets");
  cutflow_bin_title.push_back("4 bjets");

  // Define particle vectors
  std::vector<fastjet::PseudoJet> input_particles;
  std::vector<particleJet>        selected_jets;
  std::vector<particleJet>        selected_bjets;
  std::vector<particleJet>        selected_ljets;
  std::vector<particleLepton>     pre_selected_lepton;
  std::vector<particleLepton>     selected_lepton;

  // Cross section and weights
  double mc_weight = 1.;
  double xsec = reader.GetReal("eventInfo", "crossSection", 1.);

  // Loop on events
  for (Long64_t ievt=0; ievt<nentries; ievt++) {

    // all of it
    plot1D_cutflow("cutflow", 0, h_1d, "Cut flow", cutflow_bin_title);

    if (ievt % 500 == 0) cout << "Event  " << ievt << endl;

    CollectionTree->GetEntry(ievt);

    // Extract weight
    for (auto ev : event->m_genEvents){
      ev.m_weights.at(0)>0. ? mc_weight=1. : mc_weight=-1.;
      break;
    }

    // Clear particle vectors
    input_particles.clear();
    selected_jets.clear();
    selected_bjets.clear();
    selected_ljets.clear();
    pre_selected_lepton.clear();
    selected_lepton.clear();

    // Find the index of the b-quarks from the A decay
    std::vector<std::vector<int>> theBHdecays = findBHdecays(event->m_genParticles);

    // TLorentzVector for the particle
    TLorentzVector partvec;

    // Find the index of the hard scatter leptons
    std::vector<int> theHL = findHardScatterLeptons(event->m_genParticles, isPythia6);
    particleLepton lepton;

    // Loop over all particles in the event
    int index = -1;
    for (auto part : event->m_genParticles) {
      index++;
      partvec.SetXYZM(part.m_px, part.m_py, part.m_pz, part.m_m);

      if (isSignal) {
        if (index == theBHdecays[0][0]) {
          partvec *= ghost_factor;
          fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
          bghost.set_user_index(-50001);
          input_particles.push_back(bghost);
          continue;
        } else if (index == theBHdecays[0][1]) {
          partvec *= ghost_factor;
          fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
          bghost.set_user_index(-50002);
          input_particles.push_back(bghost);
          continue;
        } else if (index == theBHdecays[1][0]) {
          partvec *= ghost_factor;
          fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
          bghost.set_user_index(-50003);
          input_particles.push_back(bghost);
          continue;
        } else if (index == theBHdecays[1][1]) {
          partvec *= ghost_factor;
          fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
          bghost.set_user_index(-50004);
          input_particles.push_back(bghost);
          continue;
        }
      }
      if (isBhadron(part.m_pdgId)) {
        partvec *= ghost_factor;
        fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
        bghost.set_user_index(-5);
        input_particles.push_back(bghost);
        continue;
      }

      // Get TLorentz vector of the lepton
      if (std::find(theHL.begin(), theHL.end(), index)!=theHL.end()) {
        lepton.setLeptonRaw(partvec);
        lepton.setGenParticle(part);
        lepton.setIndexLepton(index);
        lepton.dressLepton(event->m_genParticles, isPythia6, 0.1);

        pre_selected_lepton.push_back(lepton);
        continue;
      }

      // Only stable particles
      if (part.m_status != 1) continue;

      // Not neutrinos
      if (abs(part.m_pdgId) == 12) continue;
      if (abs(part.m_pdgId) == 14) continue;
      if (abs(part.m_pdgId) == 16) continue;

      // Not dressing of the electron
      std::vector<int> dressing_index = lepton.getDressingIndex();
      if (std::find(dressing_index.begin(), dressing_index.end(), index)!=dressing_index.end()) continue;

      // Track jets
      if (use_only_charged){
        if (db->GetParticle(part.m_pdgId)->Charge() == 0) continue;
      }

      // The rest of particles, consider them to cluster
      input_particles.push_back(fastjet::PseudoJet(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E()));
      input_particles.back().set_user_index(index);

    } // End particle loop


    // Truth level, before selection
    if (isSignal) {
      TLorentzVector b1, b2, b3, b4;
      b1.SetXYZM(event->m_genParticles[theBHdecays[0][0]].m_px, event->m_genParticles[theBHdecays[0][0]].m_py, event->m_genParticles[theBHdecays[0][0]].m_pz, event->m_genParticles[theBHdecays[0][0]].m_m);
      b2.SetXYZM(event->m_genParticles[theBHdecays[0][1]].m_px, event->m_genParticles[theBHdecays[0][1]].m_py, event->m_genParticles[theBHdecays[0][1]].m_pz, event->m_genParticles[theBHdecays[0][1]].m_m);
      b3.SetXYZM(event->m_genParticles[theBHdecays[1][0]].m_px, event->m_genParticles[theBHdecays[1][0]].m_py, event->m_genParticles[theBHdecays[1][0]].m_pz, event->m_genParticles[theBHdecays[1][0]].m_m);
      b4.SetXYZM(event->m_genParticles[theBHdecays[1][1]].m_px, event->m_genParticles[theBHdecays[1][1]].m_py, event->m_genParticles[theBHdecays[1][1]].m_pz, event->m_genParticles[theBHdecays[1][1]].m_m);

      plot1D("h_bparton_pt", b1.Pt()/1000., 1., h_1d, "b-parton pT", 100, 0, 150);
      plot1D("h_bparton_pt", b2.Pt()/1000., 1., h_1d, "b-parton pT", 100, 0, 150);
      plot1D("h_bparton_pt", b3.Pt()/1000., 1., h_1d, "b-parton pT", 100, 0, 150);
      plot1D("h_bparton_pt", b4.Pt()/1000., 1., h_1d, "b-parton pT", 100, 0, 150);

      plot1D("h_bparton_eta", b1.Eta(), 1., h_1d, "b-parton eta", 100, -5, 5);
      plot1D("h_bparton_eta", b2.Eta(), 1., h_1d, "b-parton eta", 100, -5, 5);
      plot1D("h_bparton_eta", b3.Eta(), 1., h_1d, "b-parton eta", 100, -5, 5);
      plot1D("h_bparton_eta", b4.Eta(), 1., h_1d, "b-parton eta", 100, -5, 5);

      plot1D("h_bbar_dR", b1.DeltaR(b2), 1., h_1d, "b-bbar dR", 100, 0, 5);
      plot1D("h_bbar_dR", b3.DeltaR(b4), 1., h_1d, "b-bbar dR", 100, 0, 5);

      plot2D("h_bbar_dR_vs_H_pT", b1.DeltaR(b2), (b1+b2+b3+b4).Pt()/1000., 1., h_2d, "bbar dR vs Higgs pT", 100, 0., 5., 100, 0., 150.);
      plot2D("h_bbar_dR_vs_H_pT", b3.DeltaR(b4), (b1+b2+b3+b4).Pt()/1000., 1., h_2d, "bbar dR vs Higgs pT", 100, 0., 5., 100, 0., 150.);
    }

    // cluster the jets
    fastjet::ClusterSequence clust_seq(input_particles, jet_def);    
    vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(jet_ptmin));

    // Check pt and eta effect in Njets
    for (int j=0; j<4; j++){
      float _pt = (5+j*5)*1000;
      fastjet::ClusterSequence clust_seq_test(input_particles, jet_def);    
      vector<fastjet::PseudoJet> inclusive_jets_test = sorted_by_pt(clust_seq_test.inclusive_jets(_pt));
      for (int i=0; i<25; i++){
        float _eta = i*0.2;
        int _njets=0;
        int _nbjets=0;
        for (auto jet : inclusive_jets_test) {
          TLorentzVector jetvec(jet.px(), jet.py(), jet.pz(), jet.e());
          if (fabs(jetvec.Eta())>_eta) continue;
          _njets++;
          if (!isBjet(jet)) continue;
          _nbjets++;
        }
        plot1D(Form("h_njets_ptmin%.1f_abseta%.1f",_pt, _eta), _njets, 1., h_1d, Form("Njets with ptmin<%.1f and |eta|<%.1f", _pt,  _eta), 10, 0., 10.);
        plot1D(Form("h_nbjets_ptmin%.1f_abseta%.1f",_pt, _eta), _nbjets, 1., h_1d, Form("Nbjets with ptmin<%.1f and |eta|<%.1f", _pt,  _eta), 10, 0., 10.);
      }
      inclusive_jets_test.clear();
    }

    // Jet reconstruction
    for (auto jet : inclusive_jets) {
      TLorentzVector jetvec(jet.px(), jet.py(), jet.pz(), jet.e());
      if (fabs(jetvec.Eta()) > jet_etamax) continue;
      particleJet partJet;
      partJet.jet = jetvec;
      partJet.isBjet = isBjet(jet);
      partJet.pseudoJet = jet;

      if (isSignal) {
        if (isBpartonJet(jet,1)) {
          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[0][0]]));
        } else if (isBpartonJet(jet,2)) {
          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[0][1]]));
        } else if (isBpartonJet(jet,3)) {
          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[1][0]]));
        } else if (isBpartonJet(jet,4)) {
          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[1][1]]));
        }
      }
      selected_jets.push_back(partJet);
    }

    // Lepton reconstruction
    for (auto lepton : pre_selected_lepton){
      // Compute isolations
      lepton.compute_miniIsolation(event->m_genParticles);
      lepton.compute_isolation(selected_jets, 0.01, 0.2);

      plot1D_cutflow("cutflow", 1, h_1d, "Cut flow", cutflow_bin_title);
      TLorentzVector lepton_tlv = lepton.getLeptonDressed();
      GenParticle_p5 lepton_part = lepton.getLeptonGenPart();
      if (lepton_tlv.Pt() > lep_ptmin) {
        plot1D_cutflow("cutflow", 2, h_1d, "Cut flow", cutflow_bin_title);
        if (fabs(lepton_tlv.Eta()) < lep_etamax) {
          plot1D_cutflow("cutflow", 3, h_1d, "Cut flow", cutflow_bin_title);
          if (lepton.getMiniIsolation() < lep_isomax) {
            selected_lepton.push_back(lepton);
          }
        }
      }
    }

    // Define the b-jets
    for (auto jet : selected_jets) {
      if (jet.isBjet) {
        selected_bjets.push_back(jet);
      }
    } 

    // P L O T S
    std::string pass;
    //--- Pass lepton requirement
    selected_lepton.size()>0 ? pass="passLepton" : pass="failLepton";
    if (pass.find(std::string("passLepton")) != std::string::npos){
      plot1D_cutflow("cutflow", 4, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(0, pass, h_1d, h_2d, selected_jets, selected_bjets, selected_lepton, mc_weight*xsec);

    //--- Pass lepton + jets requirements
    selected_jets.size()>=4  ? pass=Form("%s-passJets", pass.c_str()) : pass=Form("%s-failJets", pass.c_str());
    if (pass.find(std::string("passLepton-passJets")) != std::string::npos){
      plot1D_cutflow("cutflow", 5, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(0, pass, h_1d, h_2d, selected_jets, selected_bjets, selected_lepton, mc_weight*xsec);

    //--- Pass lepton + jets + b-jets requirements
    selected_bjets.size()>=4  ? pass=Form("%s-passBJets", pass.c_str()) : pass=Form("%s-failBJets", pass.c_str());
    if (pass.find(std::string("passLepton-passJets-passBJets")) != std::string::npos){
      plot1D_cutflow("cutflow", 6, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(0, pass, h_1d, h_2d, selected_jets, selected_bjets, selected_lepton, mc_weight*xsec);

  } // end loop entries



  cout << "---- REPORT ----" << endl;
  cout << "Acceptance: " << endl << "   " << 100*h_1d["cutflow"]->GetBinContent(7)/h_1d["cutflow"]->GetBinContent(1) << "%" << endl;

  TFile* resultFile = TFile::Open(Form("results/%s", outputFileName.c_str()), "RECREATE");

  // Writing and deleting all histos
  std::map<std::string, TH1*>::iterator it1d;
  for(it1d=h_1d.begin(); it1d!=h_1d.end(); it1d++) {
    it1d->second->Write(); 
    delete it1d->second;
  }
  std::map<std::string, TH2D*>::iterator it2d;
  for(it2d=h_2d.begin(); it2d!=h_2d.end(); it2d++) {
    it2d->second->Write(); 
    delete it2d->second;
  }

  resultFile->Close();
  return 0;

}
