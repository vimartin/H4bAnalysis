#include "fastjet/ClusterSequence.hh"
#include "generator/McEventCollection_p5.h"
#include "generator/GenParticle_p5.h"

#include <vector>
#include "TLorentzVector.h"

#include "ExoticHiggs/JetUtils.h"

#ifndef LEPTONUTILS_H
#define LEPTONUTILS_H

class particleLepton {

 public:
  particleLepton();
  bool                isHL;
  double              m_isolation;
  double              m_miniIsolation;

  void compute_isolation(std::vector<particleJet>, double, double);
  void compute_miniIsolation(std::vector<GenParticle_p5>& partList);
  void setLeptonRaw(TLorentzVector);
  void setGenParticle(GenParticle_p5&);
  void dressLepton(std::vector<GenParticle_p5>& partList, bool isPythia6, double radius);
  void setIndexLepton(int);
  std::vector<int> getDressingIndex();
  TLorentzVector getLeptonDressed();
  TLorentzVector getLeptonRaw();
  GenParticle_p5& getLeptonGenPart();
  double getIsolation();
  double getMiniIsolation();

 private:
  TLorentzVector               m_lepton_raw;
  TLorentzVector               m_lepton_dressed;
  int                          m_index_lep;
  std::vector<int>             m_index_dressing;
  GenParticle_p5               m_parton;
};


std::vector<int> findHardScatterLeptons(std::vector<GenParticle_p5>& partList, bool isSherpa=false, bool isPythia6 = false);
TLorentzVector findDressedLepton(std::vector<GenParticle_p5>& partList, bool isPythia6, TLorentzVector Vlepton_vec, double radius);
double LeptonIsolation(fastjet::PseudoJet& lepton, std::vector<fastjet::PseudoJet>& jetList, double Rmin, double Rmax);
double LeptonMiniIsolation(GenParticle_p5& lepton, std::vector<GenParticle_p5>& partList);

#endif /*LEPTONUTILS_H*/
