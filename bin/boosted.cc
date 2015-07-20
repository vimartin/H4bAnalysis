// Analysis strategy

/*


  1) Reconstruct C/A with 1.0
  2) Do some sort of substructure tagging
  3) Reconstruct the subjets
  4) b-tag them
  5) Reconstruct the mass of the two subjets  

 */ 


#include "fastjet/ClusterSequence.hh"
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"

#include "generator/McEventCollection_p5.h"
#include "generator/GenParticle_p5.h"

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
#include "TH2D.h"
#include "TF1.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
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
    cout << "analysis.exe [configFile]" << endl;
    return 0;
  }
  INIReader reader("JOs/boosted_analysis.cfg");
  if (reader.ParseError() < 0) {
    std::cout << "Can't load JOs/boosted_analysis.cfg" << endl;
    return 1;
  }

  INIReader reader_sample(argv[1]);
  if (reader_sample.ParseError() < 0) {
    std::cout << "Can't load " << argv[1] << endl;
    return 1;
  }
  
  // Fat jet definition
  double fat_R = reader.GetReal("fatjet", "R", 1.0);
  fastjet::JetDefinition fat_jet_def(fastjet::cambridge_algorithm, fat_R);

  // Fat jet selection
  double jet_ptmin = reader.GetReal("fatjet", "ptmin", 40000.0);
  double jet_etamax = reader.GetReal("fatjet", "etamax", 2.6);

  // MD tagger
  bool doMDtagger = reader.GetBoolean("mdtagger", "doMDtagger", true);
  double R_mdtagger = reader.GetReal("mdtagger", "R", 0.4);
  fastjet::JetDefinition sub_jet_def_mdtagged(fastjet::antikt_algorithm, R_mdtagger);
  double mu_thr = reader.GetReal("mdtagger", "mu_thr", 0.667);
  double y_thr = reader.GetReal("mdtagger", "y_thr", 0.09);    

  // Prunning
  bool doPrunning = reader.GetBoolean("prunning", "doPrunning", false);
  double R_pruning = reader.GetReal("prunning", "R", 0.4);
  fastjet::JetDefinition sub_jet_def_pruned(fastjet::cambridge_algorithm, R_pruning);
  double zcut = reader.GetReal("prunning", "zcut", 0.1);
  double rcut_factor = reader.GetReal("prunning", "rcut_factor", 0.5);

  // BDRS
  bool doBDRS = reader.GetBoolean("BDRS", "doBDRS", false);
  double R_BDRS = reader.GetReal("BDRS", "R", 0.4);
  fastjet::JetDefinition sub_jet_def_BDRS(fastjet::cambridge_algorithm, R_BDRS);

  // Trimming
  bool doTrimming = reader.GetBoolean("trimming", "doTrimming", false);
  double ptfrac_trim = reader.GetReal("trimming", "ptfrac", 0.05);
  double R_trim = reader.GetReal("trimming", "R", 0.2);
  fastjet::JetDefinition sub_jet_def_trimmed(fastjet::kt_algorithm, R_trim);

  // Sub jet definition
  double R_matched = reader.GetReal("subjet", "R", 0.4);
  fastjet::JetDefinition sub_jet_def_matched(fastjet::antikt_algorithm, R_matched);
  double sub_jet_ptmin = reader.GetReal("subjet", "ptmin", 5000.0);
  bool use_only_charged = reader.GetBoolean("subjet", "use_only_charged", false);
  bool doFatMatching = reader.GetBoolean("subjet", "doFatMatching", true);
  
  // Lepton selection
  double lep_ptmin = reader.GetReal("lepton", "lep_ptmin", 26000.0);
  double lep_etamax = reader.GetReal("lepton", "lep_etamax", 2.4);
  double lep_isomax = reader.GetReal("lepton", "lep_isomax", 0.5); // mini iso limit

  // Ghost factor for truth-level B-jet tagging
  double ghost_factor = reader.GetReal("btagging", "ghost_factor", 1.e-21);

  // files
  std::string dataFileName = reader_sample.Get("io", "data_file_name", "UNKNOWN");
  std::string outputFileName = Form("boosted_%s", reader_sample.Get("io", "result_file_name", "UNKNOWN").c_str());
  double hypMass = reader_sample.GetReal("io", "hyp_mass", 20.);
  bool isSignal = reader_sample.GetBoolean("io", "isSignal", false);
  bool isPythia6 = reader_sample.GetBoolean("io", "isPythia6", false);
  bool isSherpa = reader_sample.GetBoolean("io", "isSherpa", false);

  TDatabasePDG *db= TDatabasePDG::Instance();

  // Gets the input and stores in fastjet
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
  cutflow_bin_title.push_back("2 jets");
  cutflow_bin_title.push_back("substructure");
  cutflow_bin_title.push_back("2 subjets");
  cutflow_bin_title.push_back("2 b subjets");

  // Define particle vectors
  std::vector<fastjet::PseudoJet> input_particles_fat;
  std::vector<fastjet::PseudoJet> input_particles_sub;
  std::vector<particleJet>        selected_jets_fat;
  std::vector<particleJet>        selected_bjets_fat;
  std::vector<particleJet>        selected_ljets_fat;
  std::vector<particleJet>        selected_jets_sub;
  std::vector<particleLepton>     pre_selected_lepton;
  std::vector<particleLepton>     selected_lepton;

  // Cross section and weights
  double mc_weight = 1.;
  double xsec = reader_sample.GetReal("eventInfo", "crossSection", 1.);

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
    input_particles_fat.clear();
    input_particles_sub.clear();
    selected_jets_fat.clear();
    selected_bjets_fat.clear();
    selected_ljets_fat.clear();
    selected_jets_sub.clear();
    pre_selected_lepton.clear();
    selected_lepton.clear();


    // Find the index of the b-quarks from the A decay
    std::vector<std::vector<int>> theBHdecays = findBHdecays(event->m_genParticles);

    // TLorentzVector for the particle
    TLorentzVector partvec;

    // Find the index of the hard scatter leptons
    std::vector<int> theHL = findHardScatterLeptons(event->m_genParticles, isSherpa, isPythia6);
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
          input_particles_fat.push_back(bghost);
          input_particles_sub.push_back(bghost);
          continue;
        } else if (index == theBHdecays[0][1]) {
          partvec *= ghost_factor;
          fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
          bghost.set_user_index(-50002);
          input_particles_fat.push_back(bghost);
          input_particles_sub.push_back(bghost);
          continue;
        } else if (index == theBHdecays[1][0]) {
          partvec *= ghost_factor;
          fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
          bghost.set_user_index(-50003);
          input_particles_fat.push_back(bghost);
          input_particles_sub.push_back(bghost);
          continue;
        } else if (index == theBHdecays[1][1]) {
          partvec *= ghost_factor;
          fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
          bghost.set_user_index(-50004);
          input_particles_fat.push_back(bghost);
          input_particles_sub.push_back(bghost);
          continue;
        }
      }
      if (isBhadron(part.m_pdgId)) {
        partvec *= ghost_factor;
        fastjet::PseudoJet bghost(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E());
        bghost.set_user_index(-5);
        input_particles_fat.push_back(bghost);
        input_particles_sub.push_back(bghost);
        continue;
      }

      // Get TLorentz vector of the lepton
      if (std::find(theHL.begin(), theHL.end(), index)!=theHL.end()) {
        lepton.setLeptonRaw(partvec);
        lepton.setGenParticle(part);
        lepton.setIndexLepton(index);
        lepton.dressLepton(event->m_genParticles, isPythia6, 0.1);

        pre_selected_lepton.push_back(lepton);
//        if (partvec.Pt()>40000){
        plot1D("h_lepton_preselected_pt", partvec.Pt()/1000., 1., h_1d, "pre-selected lepton pT", 30, 0, 150);
//        }
        plot1D("h_lepton_preselected_eta", partvec.Eta(), 1., h_1d, "pre-selected lepton eta", 100, -5, 5);
        continue;
      }

      // Stable particles
      if (part.m_status != 1) continue;

      // Not neutrinos
      if (abs(part.m_pdgId) == 12) continue;
      if (abs(part.m_pdgId) == 14) continue;
      if (abs(part.m_pdgId) == 16) continue;

      // Not consider dressing of the electron
      std::vector<int> dressing_index = lepton.getDressingIndex();
      if (std::find(dressing_index.begin(), dressing_index.end(), index)!=dressing_index.end()) continue;

      // The rest of particles, consider them to cluster: fat jets
      input_particles_fat.push_back(fastjet::PseudoJet(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E()));
      input_particles_fat.back().set_user_index(index);

      // The rest of particles, consider them to cluster: small jets
      input_particles_sub.push_back(fastjet::PseudoJet(partvec.Px(),partvec.Py(),partvec.Pz(),partvec.E()));
      input_particles_sub.back().set_user_index(index);
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

      plot1D("h_bbar_dR", b1.DeltaR(b2), 1., h_1d, "b-bbar dR", 100, 0, 5);
      plot1D("h_bbar_dR", b3.DeltaR(b4), 1., h_1d, "b-bbar dR", 100, 0, 5);

      plot2D("h_bbar_dR_vs_H_pT", b1.DeltaR(b2), (b1+b2+b3+b4).Pt()/1000., 1., h_2d, "bbar dR vs Higgs pT", 100, 0., 5., 100, 0., 150.);
      plot2D("h_bbar_dR_vs_H_pT", b3.DeltaR(b4), (b1+b2+b3+b4).Pt()/1000., 1., h_2d, "bbar dR vs Higgs pT", 100, 0., 5., 100, 0., 150.);
      plot2D("h_bbar_dR_vs_A_pT", b1.DeltaR(b2), (b1+b2).Pt()/1000., 1., h_2d, "bbar dR vs pseudoscalar pT", 100, 0., 5., 100, 0., 150.);
      plot2D("h_bbar_dR_vs_A_pT", b3.DeltaR(b4), (b3+b4).Pt()/1000., 1., h_2d, "bbar dR vs pseudoscalar pT", 100, 0., 5., 100, 0., 150.);
      if (((b1+b2).Pt()>0. && (b1+b2).Pt()<20000.) || ((b3+b4).Pt()>0. && (b3+b4).Pt()<20000.)){
        plot1D("h_bbar_dR_A_pT_0_20", b1.DeltaR(b2), 1., h_1d, "bbar dR for 0 < ApT < 20", 100, 0, 5.);
        plot1D("h_bbar_dR_A_pT_0_20", b3.DeltaR(b4), 1., h_1d, "bbar dR for 0 < ApT < 20", 100, 0, 5.);
      }
      if (((b1+b2).Pt()>20000. && (b1+b2).Pt()<40000.) || ((b3+b4).Pt()>20000. && (b3+b4).Pt()<40000.)){
        plot1D("h_bbar_dR_A_pT_20_40", b1.DeltaR(b2), 1., h_1d, "bbar dR for 20 < ApT < 40", 100, 0, 5.);
        plot1D("h_bbar_dR_A_pT_20_40", b3.DeltaR(b4), 1., h_1d, "bbar dR for 20 < ApT < 40", 100, 0, 5.);
      }
      if (((b1+b2).Pt()>40000. && (b1+b2).Pt()<60000.) || ((b3+b4).Pt()>40000. && (b3+b4).Pt()<60000.)){
        plot1D("h_bbar_dR_A_pT_40_60", b1.DeltaR(b2), 1., h_1d, "bbar dR for 40 < ApT < 60", 100, 0, 5.);
        plot1D("h_bbar_dR_A_pT_40_60", b3.DeltaR(b4), 1., h_1d, "bbar dR for 40 < ApT < 60", 100, 0, 5.);
      }
      if (((b1+b2).Pt()>60000. && (b1+b2).Pt()<80000.) || ((b3+b4).Pt()>60000. && (b3+b4).Pt()<80000.)){
        plot1D("h_bbar_dR_A_pT_60_80", b1.DeltaR(b2), 1., h_1d, "bbar dR for 60 < ApT < 80", 100, 0, 5.);
        plot1D("h_bbar_dR_A_pT_60_80", b3.DeltaR(b4), 1., h_1d, "bbar dR for 60 < ApT < 80", 100, 0, 5.);
      }
      if (((b1+b2).Pt()>80000. && (b1+b2).Pt()<100000.) || ((b3+b4).Pt()>80000. && (b3+b4).Pt()<100000.)){
        plot1D("h_bbar_dR_A_pT_80_100", b1.DeltaR(b2), 1., h_1d, "bbar dR for 80 < ApT < 100", 100, 0, 5.);
        plot1D("h_bbar_dR_A_pT_80_100", b3.DeltaR(b4), 1., h_1d, "bbar dR for 80 < ApT < 100", 100, 0, 5.);
      }
      if (((b1+b2).Pt()>100000.) || ((b3+b4).Pt()>100000.)){
        plot1D("h_bbar_dR_A_pT_100", b1.DeltaR(b2), 1., h_1d, "bbar dR for ApT < 100", 100, 0, 5.);
        plot1D("h_bbar_dR_A_pT_100", b3.DeltaR(b4), 1., h_1d, "bbar dR for ApT < 100", 100, 0, 5.);
      }
    }

    // cluster the small jets
    fastjet::ClusterSequence sub_clust_seq(input_particles_sub, sub_jet_def_matched);    
    vector<fastjet::PseudoJet> inclusive_jets_sub = sorted_by_pt(sub_clust_seq.inclusive_jets(sub_jet_ptmin));

    // small jet reconstruction
    for (auto jet : inclusive_jets_sub) {
      TLorentzVector jetvec(jet.px(), jet.py(), jet.pz(), jet.e());
      if (fabs(jetvec.Eta()) > jet_etamax) continue;
      particleJet partJet;
      partJet.jet = jetvec;
      partJet.isBjet = isBjet(jet);
      partJet.nBpartons = nBpartonInBjet(jet);
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
      selected_jets_sub.push_back(partJet);
    } // end small jet reconstruction


    // cluster the fat jets
    fastjet::ClusterSequence clust_seq(input_particles_fat, fat_jet_def);    
    vector<fastjet::PseudoJet> inclusive_jets_fat = sorted_by_pt(clust_seq.inclusive_jets(jet_ptmin));
    
    // define grooming algorithms
    fastjet::MassDropTagger md_tagger(mu_thr, y_thr);
    fastjet::Pruner pruner(fastjet::cambridge_algorithm, zcut, rcut_factor);
    fastjet::Filter trimmer(fastjet::JetDefinition(fastjet::kt_algorithm, R_trim), fastjet::SelectorPtFractionMin(ptfrac_trim));

    // fat jet reconstruction
    for (auto jet : inclusive_jets_fat) {
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

      //--- Fill md tagger jets
      partJet.hasMDSubstructure = false;
      fastjet::PseudoJet mdtagged = md_tagger(jet);
      if (!(mdtagged == 0)) {
        if (mdtagged.perp()>jet_ptmin && fabs(mdtagged.eta())<jet_etamax){
          partJet.mdtagged_pseudoJet = mdtagged; // copy cleaned jet

          partJet.hasMDSubstructure = true;
          fastjet::ClusterSequence mdtagged_clust_seq(partJet.mdtagged_pseudoJet.constituents(), sub_jet_def_mdtagged); // recluster constituents
          partJet.mdtagged_subPseudoJets = sorted_by_pt(mdtagged_clust_seq.inclusive_jets(sub_jet_ptmin)); // sort them and storesubjets
          for (auto j : partJet.mdtagged_subPseudoJets){
            partJet.mdtagged_subPseudoJets_bflag.push_back((int) isBjet(j)); // flag as b-subjets
          }
        }
      }

      //--- Fill pruned jets
      partJet.hasPruningSubstructure = false;
      fastjet::PseudoJet pruned_jet;
      pruned_jet= pruner(jet);
      if (!(pruned_jet == 0)) {
        if (pruned_jet.perp()>jet_ptmin && fabs(pruned_jet.eta())<jet_etamax){
          partJet.pruned_pseudoJet = pruned_jet;

          partJet.hasPruningSubstructure = true;
          fastjet::ClusterSequence Pruning_clust_seq(partJet.pruned_pseudoJet.constituents(), sub_jet_def_pruned); // recluster constituents
          partJet.pruned_subPseudoJets = sorted_by_pt(Pruning_clust_seq.inclusive_jets(sub_jet_ptmin)); // sort them and storesubjets
          for (auto j : partJet.pruned_subPseudoJets){
            partJet.pruned_subPseudoJets_bflag.push_back(isBjet(j));
          }
        }
      }

      //--- Fill BDRS jets
      partJet.hasBDRSSubstructure = false;
      fastjet::PseudoJet mdtagged_BDRS = md_tagger(jet);
      if (!(mdtagged_BDRS == 0)) {
        if (mdtagged_BDRS.perp()>jet_ptmin && fabs(mdtagged_BDRS.eta())<jet_etamax){
          std::vector<fastjet::PseudoJet> mdtagged_BDRS_pieces = mdtagged_BDRS.pieces();
          double Rfilt = min(0.3, 0.5*mdtagged_BDRS_pieces.at(0).delta_R(mdtagged_BDRS_pieces.at(1)));
          fastjet::PseudoJet filtered = fastjet::Filter(Rfilt, fastjet::SelectorNHardest(3))(mdtagged_BDRS);
          if (filtered.perp()>jet_ptmin && fabs(filtered.eta())<jet_etamax){
            partJet.filtered_pseudoJet = filtered; 
            partJet.filtered_pseudoJet.set_user_index(jet.user_index());

            partJet.hasBDRSSubstructure = true;
            fastjet::ClusterSequence BDRS_clust_seq(partJet.filtered_pseudoJet.constituents(), sub_jet_def_BDRS); // recluster constituents
            partJet.filtered_subPseudoJets = sorted_by_pt(BDRS_clust_seq.inclusive_jets(sub_jet_ptmin)); // sort them and storesubjets
            for (auto j : partJet.filtered_subPseudoJets){
              partJet.filtered_subPseudoJets_bflag.push_back((int) isBjet(j)); // flag as b-subjets
            }
          }
        }
      }

      //--- Fill trimmed jets
      partJet.hasTrimmingSubstructure = false;
      fastjet::PseudoJet trimmed_jet;
      trimmed_jet= trimmer(jet);
      if (!(trimmed_jet == 0)) {
        if (trimmed_jet.perp()>jet_ptmin && fabs(trimmed_jet.eta())<jet_etamax){
          partJet.trimmed_pseudoJet = trimmed_jet;

          partJet.hasTrimmingSubstructure = true;
          fastjet::ClusterSequence trimmed_clust_seq(partJet.trimmed_pseudoJet.constituents(), sub_jet_def_trimmed); // recluster constituents
          partJet.trimmed_subPseudoJets = sorted_by_pt(trimmed_clust_seq.inclusive_jets(sub_jet_ptmin)); // sort them and storesubjets
          for (auto j : partJet.trimmed_subPseudoJets){
            partJet.trimmed_subPseudoJets_bflag.push_back(isBjet(j));
          }
        }
      }

      //--- Fill matched jets
      partJet.hasMatchedSubstructure = false;
      for (auto subjet : selected_jets_sub){
          if (!partJet.hasMDSubstructure) continue;
          TLorentzVector sub_tlv(subjet.pseudoJet.px(), subjet.pseudoJet.py(), subjet.pseudoJet.pz(), subjet.pseudoJet.e());
          TLorentzVector fat_tlv(partJet.mdtagged_pseudoJet.px(), partJet.mdtagged_pseudoJet.py(), partJet.mdtagged_pseudoJet.pz(), partJet.mdtagged_pseudoJet.e());
          if (sub_tlv.DeltaR(fat_tlv) < fat_R){
            partJet.matched_subPseudoJets.push_back(subjet.pseudoJet);
            partJet.hasMatchedSubstructure = true;
          }
      }
      for (auto j : partJet.matched_subPseudoJets){
        partJet.matched_subPseudoJets_bflag.push_back(isBjet(j));
      }

      selected_jets_fat.push_back(partJet);
    } // end fat jet reconstruction









//    // Jet reconstruction
//    for (auto jet : inclusive_jets) {
//      TLorentzVector jetvec(jet.px(), jet.py(), jet.pz(), jet.e());
//      if (fabs(jetvec.Eta()) > jet_etamax) continue;
//      particleJet partJet;
//      partJet.jet = jetvec;
//      partJet.isBjet = isBjet(jet);
//      partJet.pseudoJet = jet;
//
//      if (isSignal) {
//        if (isBpartonJet(jet,1)) {
//          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[0][0]]));
//        } else if (isBpartonJet(jet,2)) {
//          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[0][1]]));
//        } else if (isBpartonJet(jet,3)) {
//          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[1][0]]));
//        } else if (isBpartonJet(jet,4)) {
//          partJet.parton.push_back(&(event->m_genParticles[theBHdecays[1][1]]));
//        }
//      }
//
//      fastjet::PseudoJet tagged = md_tagger(jet);
//      partJet.hasMDSubstructure = false;
//      if (!(tagged == 0)) {
//        partJet.hasMDSubstructure = true;
//
//        std::vector<fastjet::PseudoJet> charged_input_particles;
//        fastjet::PseudoJet pruned_jet;
//        if (doPrunning)
//          pruned_jet= pruner(jet);
//        else
//          pruned_jet = jet;
//        partJet.pruned_pseudoJet = pruned_jet;
//
//        if (use_only_charged) {
//          for (auto& ipart : pruned_jet.constituents()) {
//            if (ipart.user_index() > 0) { 
//              if ((db->GetParticle(event->m_genParticles[ipart.user_index()].m_pdgId))->Charge() != 0)
//                charged_input_particles.push_back(ipart);
//            } else {
//              charged_input_particles.push_back(ipart); // keep the ghosts
//            }
//          }
//        } else {
//          charged_input_particles = pruned_jet.constituents();
//        }
//
//        fastjet::ClusterSequence sub_clust_seq(charged_input_particles, sub_jet_def);    
//        vector<fastjet::PseudoJet> sub_inclusive_jets = sorted_by_pt(sub_clust_seq.inclusive_jets(sub_jet_ptmin));
//
//        vector<int> sub_jets_btag;
//
//        for (auto subjet : sub_inclusive_jets)
//          sub_jets_btag.push_back((int) isBjet(subjet));
//
//        partJet.subjets = sub_inclusive_jets;
//        partJet.subjets_btag = sub_jets_btag;
//        partJet.pieces = tagged.pieces();
//      }
//      selected_jets.push_back(partJet);
//    }

    // Lepton reconstruction
    for (auto lepton : pre_selected_lepton){
      // Compute isolations
      lepton.compute_miniIsolation(event->m_genParticles);
      lepton.compute_isolation(selected_jets_fat, 0.01, 0.2);

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

    // Count the number of subjets
    vector<int> cf_jetHasSubstructure;
    vector<int> cf_numberSubjets;
    vector<int> cf_numberBsubjets;

    // Define the b-jets (fat) and subjets
    int ijet=0;
    for (auto jet : selected_jets_fat) {
      if (jet.isBjet) {
        selected_bjets_fat.push_back(jet);
      }

      for (int ijet=0; ijet < selected_jets_fat.size(); ijet++) {
        cf_jetHasSubstructure.push_back(0);
        cf_numberSubjets.push_back(0);
        cf_numberBsubjets.push_back(0);
      }

      if (doMDtagger){
        if (jet.hasMDSubstructure) {
          cf_jetHasSubstructure.at(ijet) = 1;
          cf_numberSubjets.at(ijet) = jet.pruned_subPseudoJets.size();

          for (unsigned int isub = 0; isub < jet.mdtagged_subPseudoJets_bflag.size(); isub++) {
            if (jet.mdtagged_subPseudoJets_bflag.at(isub)) {
              cf_numberBsubjets.at(ijet)++;
            }
          }
        }
        ijet++;
      }
      else if (doPrunning){
        if (jet.hasPruningSubstructure) {
          cf_jetHasSubstructure.at(ijet) = 1;
          cf_numberSubjets.at(ijet) = jet.pruned_subPseudoJets.size();

          for (unsigned int isub = 0; isub < jet.pruned_subPseudoJets_bflag.size(); isub++) {
            if (jet.pruned_subPseudoJets_bflag.at(isub)) {
              cf_numberBsubjets.at(ijet)++;
            }
          }
        }
        ijet++;
      } 
      else if (doBDRS){
        if (jet.hasBDRSSubstructure) {
          cf_jetHasSubstructure.at(ijet) = 1;
          cf_numberSubjets.at(ijet) = jet.filtered_subPseudoJets.size();

          for (unsigned int isub = 0; isub < jet.filtered_subPseudoJets_bflag.size(); isub++) {
            if (jet.filtered_subPseudoJets_bflag.at(isub)) {
              cf_numberBsubjets.at(ijet)++;
            }
          }
        }
        ijet++;
      }
      else if (doTrimming){
        if (jet.hasTrimmingSubstructure) {
          cf_jetHasSubstructure.at(ijet) = 1;
          cf_numberSubjets.at(ijet) = jet.trimmed_subPseudoJets.size();

          for (unsigned int isub = 0; isub < jet.trimmed_subPseudoJets_bflag.size(); isub++) {
            if (jet.trimmed_subPseudoJets_bflag.at(isub)) {
              cf_numberBsubjets.at(ijet)++;
            }
          }
        }
        ijet++;
      } 
      else if (doFatMatching){
        if (jet.hasMatchedSubstructure) {
          cf_jetHasSubstructure.at(ijet) = 1;
          cf_numberSubjets.at(ijet) = jet.matched_subPseudoJets.size();

          for (unsigned int isub = 0; isub < jet.matched_subPseudoJets_bflag.size(); isub++) {
            if (jet.matched_subPseudoJets_bflag.at(isub)) {
              cf_numberBsubjets.at(ijet)++;
            }
          }
        }
        ijet++;
      } 
    }


    // P L O T S
    std::string pass;
    //--- Pass lepton requirement
    selected_lepton.size()>0 ? pass="passLepton" : pass="failLepton";
    if (pass.find(std::string("passLepton")) != std::string::npos){
      plot1D_cutflow("cutflow", 4, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(1, pass, h_1d, h_2d, selected_jets_fat, selected_bjets_fat, selected_lepton, mc_weight*xsec/nentries);

    //--- Pass lepton + fat jets requirements
    int nfatjets = selected_jets_fat.size();
    if (doMDtagger) nfatjets = number_fatjets(selected_jets_fat, "mdtagged", jet_ptmin, jet_etamax);
    if (doPrunning) nfatjets = number_fatjets(selected_jets_fat, "pruned", jet_ptmin, jet_etamax);
    if (doBDRS) nfatjets = number_fatjets(selected_jets_fat, "filtered", jet_ptmin, jet_etamax);
    if (doTrimming) nfatjets = number_fatjets(selected_jets_fat, "trimmed", jet_ptmin, jet_etamax);
    nfatjets>=2  ? pass=Form("%s-passJets", pass.c_str()) : pass=Form("%s-failJets", pass.c_str());
    if (pass.find(std::string("passLepton-passJets")) != std::string::npos){
      plot1D_cutflow("cutflow", 5, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(1, pass, h_1d, h_2d, selected_jets_fat, selected_bjets_fat, selected_lepton, mc_weight*xsec/nentries);

    //--- Pass lepton + fat jets + hasSubstructure requirements
    count_if(cf_jetHasSubstructure.begin(), cf_jetHasSubstructure.end(), [&](int x){return (x==1);})>=2 ? pass=Form("%s-passSubstr", pass.c_str()) : pass=Form("%s-failSubstr", pass.c_str());
    if (pass.find(std::string("passLepton-passJets-passSubstr")) != std::string::npos){
      plot1D_cutflow("cutflow", 6, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(1, pass, h_1d, h_2d, selected_jets_fat, selected_bjets_fat, selected_lepton, mc_weight*xsec/nentries);

    //--- Pass lepton + fat jets + hasSubstructure + subjet requirements
    count_if(cf_numberSubjets.begin(), cf_numberSubjets.end(), [&](int x){return (x>=2);})>=2 ? pass=Form("%s-passSubJet", pass.c_str()) : pass=Form("%s-failSubJet", pass.c_str());
    if (pass.find(std::string("passLepton-passJets-passSubstr-passSubJet")) != std::string::npos){
      plot1D_cutflow("cutflow", 7, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(1, pass, h_1d, h_2d, selected_jets_fat, selected_bjets_fat, selected_lepton, mc_weight*xsec/nentries);

    //--- Pass lepton + fat jets + hasSubstructure + subjet + b-subjet requirements
    count_if(cf_numberBsubjets.begin(), cf_numberBsubjets.end(), [&](int x){return (x>=2);})>=2 ? pass=Form("%s-passSubBJet", pass.c_str()) : pass=Form("%s-failSubBJet", pass.c_str());
    if (pass.find(std::string("passLepton-passJets-passSubstr-passSubJet-passSubBJet")) != std::string::npos){
      plot1D_cutflow("cutflow", 8, h_1d, "Cut flow", cutflow_bin_title);
    }
    doAllPlots(1, pass, h_1d, h_2d, selected_jets_fat, selected_bjets_fat, selected_lepton, mc_weight*xsec/nentries);

  } // end loop events

//  for (int ibin=1; ibin<=9; ibin++) cutflow_h->SetBinContent(ibin,cutflow[ibin-1]);
//  if (isSignal) {
//    fatjet_mass->Fit("gaus", "", "", hypMass-15., hypMass+5.);
//    fatjet_pruned_mass->Fit("gaus", "", "", hypMass-15., hypMass+5.);
//    subjet_mass->Fit("gaus", "", "", hypMass-15., hypMass+5.);
//    subjet_mass_mdpieces->Fit("gaus", "", "", hypMass-15., hypMass+5.);
//    bsubjet_mass->Fit("gaus", "", "", hypMass-15., hypMass+5.);
//    bsubjet_mass_mdpieces->Fit("gaus", "", "", hypMass-15., hypMass+5.);
//  }
//  
  cout << "---- REPORT ----" << endl;
  cout << "Acceptance: " << endl << "   " << 100*h_1d["cutflow"]->GetBinContent(9)/h_1d["cutflow"]->GetBinContent(1) << "%" << endl;
//
//  if (isSignal) {
//    cout << "Mass resolution: " << endl;
//    cout << "   fat jet mass: " << 100.*((TF1*) fatjet_mass->GetListOfFunctions()->At(0))->GetParameter(2)/hypMass << "%" << endl;
//    cout << "   pruned fat jet mass: " << 100.*((TF1*) fatjet_pruned_mass->GetListOfFunctions()->At(0))->GetParameter(2)/hypMass << "%" << endl;
//    cout << "   AK04 subjet mass: " << 100.*((TF1*) subjet_mass->GetListOfFunctions()->At(0))->GetParameter(2)/hypMass << "%" << endl;
//    cout << "   MD subjet mass: " << 100.*((TF1*) subjet_mass_mdpieces->GetListOfFunctions()->At(0))->GetParameter(2)/hypMass << "%" << endl;
//    cout << "   btagged AK04 subjet mass: " << 100.*((TF1*) bsubjet_mass->GetListOfFunctions()->At(0))->GetParameter(2)/hypMass << "%" << endl;
//    cout << "   btagged MD subjet mass: " << 100.*((TF1*) bsubjet_mass_mdpieces->GetListOfFunctions()->At(0))->GetParameter(2)/hypMass << "%" << endl;
//  }
  
  TFile* resultFile = TFile::Open(Form("results/%s", outputFileName.c_str()), "RECREATE");
  cout << Form("\nresults/%s has been created.", outputFileName.c_str()) << endl;

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


    
