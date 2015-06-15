#include "fastjet/ClusterSequence.hh"
#include "generator/McEventCollection_p5.h"
#include "generator/GenParticle_p5.h"

#include <vector>
#include <utility>
#include <iostream>

#include "TLorentzVector.h"

#ifndef JETUTILS_H
#define JETUTILS_H

std::vector<int> findPartonHadrons(int parton, std::vector<GenParticle_p5>& partList);
std::vector< std::vector<int> > findBHdecays(std::vector<GenParticle_p5>& partList);
bool isBhadron(int pdgId);
bool isBjet(const fastjet::PseudoJet& jet);
int nBpartonInBjet(const fastjet::PseudoJet& jet);
bool isBpartonJet(const fastjet::PseudoJet& jet, int parton);
bool isJetMatchedToB(fastjet::PseudoJet jet, std::vector<GenParticle_p5>& partList);

class particleJet {

 public:

  particleJet();

  TLorentzVector jet;
  fastjet::PseudoJet pseudoJet;
  bool isBjet;
  int nBpartons;
  
  // Subjet information
  bool hasMDSubstructure;
  bool hasPruningSubstructure;
  bool hasBDRSSubstructure;
  bool hasTrimmingSubstructure;
  bool hasMatchedSubstructure;

  fastjet::PseudoJet mdtagged_pseudoJet;
  fastjet::PseudoJet pruned_pseudoJet;
  fastjet::PseudoJet filtered_pseudoJet;
  fastjet::PseudoJet trimmed_pseudoJet;

  std::vector<fastjet::PseudoJet> mdtagged_subPseudoJets;
  std::vector<fastjet::PseudoJet> pruned_subPseudoJets;
  std::vector<fastjet::PseudoJet> filtered_subPseudoJets;
  std::vector<fastjet::PseudoJet> trimmed_subPseudoJets;
  std::vector<fastjet::PseudoJet> matched_subPseudoJets;

  std::vector<int> mdtagged_subPseudoJets_bflag;
  std::vector<int> pruned_subPseudoJets_bflag;
  std::vector<int> filtered_subPseudoJets_bflag;
  std::vector<int> trimmed_subPseudoJets_bflag;
  std::vector<int> matched_subPseudoJets_bflag;
  
  std::vector<GenParticle_p5*> parton;
};

// bool isBJet(std::vector<GenParticle_p5>& partList, fastjet::PseudoJet& jet);
// bool isBHdecayJet(int BHdecayidx, std::vector<GenParticle_p5>& partList, fastjet::PseudoJet& jet);

double findMinDeltaR(std::vector<particleJet>);
double findResolvedRecoHmass(std::vector<particleJet>);
double findBoostedRecoHmass(std::vector<particleJet>);
double findResolvedRecoAmass(std::vector<particleJet>);

std::ostream& operator<<(std::ostream& os, const particleJet jet);
std::ostream& operator<<(std::ostream& os, const GenParticle_p5 parton);
std::ostream& operator<<(std::ostream& ostr, const fastjet::PseudoJet& jet);

int number_fatjets(std::vector<particleJet> selected_jets_fat, std::string type, float pt_thres, float eta_thres);

#endif /*JETUTILS_H*/
