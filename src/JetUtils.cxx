#include "ExoticHiggs/JetUtils.h"

#include <algorithm>
#include <iostream>
#include <sstream>  // needed for internal io
#include <iomanip>  

using namespace std;

particleJet::particleJet()
{}

bool isBhadron(int pdgId)
{

  int pdg=abs(pdgId);

  if((pdg>=511 && pdg<=545) || (pdg>=10511 && pdg<=10545) || (pdg>=20511 && pdg<=20545) || (pdg>=5112 && pdg<=5554)) { 
    return true;
  } else return false;
  
}

bool isBjet(const fastjet::PseudoJet& jet)
{

  vector<fastjet::PseudoJet> constituents = jet.constituents();
  for (auto constituent : constituents) 
    if (constituent.user_index() == -5) return true;
  return false;
  
}

int nBpartonInBjet(const fastjet::PseudoJet& jet)
{
  int nb = 0;
  vector<fastjet::PseudoJet> constituents = jet.constituents();
  for (auto constituent : constituents) 
    if (constituent.user_index() == -5) nb++;
  return nb;
  
}

bool isBpartonJet(const fastjet::PseudoJet& jet, int parton)
{

  vector<fastjet::PseudoJet> constituents = jet.constituents();
  for (auto constituent : constituents) 
    if (constituent.user_index() == (-50000-parton)) return true;
  return false;
  
}

bool isJetMatchedToB(fastjet::PseudoJet jet, std::vector<GenParticle_p5>& partList)
{
  TLorentzVector jet_tlv;
  jet_tlv.SetXYZM(jet.px(), jet.py(), jet.pz(), jet.m());
  for (auto part : partList){
    if (!(part.m_status==71)) continue; // According to pythia, "copied partons to collect into contiguous colour singlet"
    TLorentzVector part_tlv;
    part_tlv.SetXYZM(part.m_px, part.m_py, part.m_pz, part.m_m);
    if (part_tlv.Pt()<1000) continue;
    if (fabs(part.m_pdgId)==5 && jet_tlv.DeltaR(part_tlv)<0.1){
      return true;
    }
  }
  return false;
}

std::vector<int> findPartonHadrons(int parton, std::vector<GenParticle_p5>& partList)
{

  vector<int> retval;
  
  vector<int> prodVtx;
  prodVtx.push_back(partList[parton].m_endVtx);

  for (int index = parton; index < partList.size(); index++) {
    if (find(prodVtx.begin(), prodVtx.end(), partList[index].m_prodVtx) != prodVtx.end()) {
      if (partList[index].m_status == 1)
	retval.push_back(index);
      else
	prodVtx.push_back(partList[index].m_endVtx);
    }
  }
  return retval;
}


std::vector< std::vector<int> > findBHdecays(std::vector<GenParticle_p5>& partList)
{

  int index = -1;
  int prodVtx1 = 1;
  int prodVtx2 = 1;
  std::vector<int> vec1;
  std::vector<int> vec2;
  std::vector<std::vector<int>> retval;
  for (auto part : partList) {
    index++;
    if ((abs(part.m_pdgId) == 36) && part.m_status == 22) {
      if (prodVtx1 > 0)
        prodVtx1 = part.m_endVtx;
      else
        prodVtx2 = part.m_endVtx;
    }
    if ((abs(part.m_pdgId) == 36) && part.m_prodVtx == prodVtx1) {
      prodVtx1 = part.m_endVtx;
    }
    if ((abs(part.m_pdgId) == 36) && part.m_prodVtx == prodVtx2) {
      prodVtx2 = part.m_endVtx;
    }
    if (abs(part.m_pdgId) == 5 && part.m_prodVtx == prodVtx1) {
      vec1.push_back(index);
    }
    if (abs(part.m_pdgId) == 5 && part.m_prodVtx == prodVtx2) {
      vec2.push_back(index);
    }
  }
  retval.push_back(vec1);
  retval.push_back(vec2);
  return retval;
}    

double findMinDeltaR(std::vector<particleJet>){
  double minDR = 1000.;

  return minDR;
}
  
double findResolvedRecoHmass(std::vector<particleJet> selected_bjets){
  double recoHmass = -9999;
  TLorentzVector H_tlv;
  if (selected_bjets.size()==4){
    for (auto bjet : selected_bjets){
     H_tlv += bjet.jet; 
    }
  }
  recoHmass = H_tlv.M()/1000.;
  return recoHmass;
}

double findBoostedRecoHmass(std::vector<particleJet> selected_jets){
  double recoHmass = -9999;
  TLorentzVector H_tlv;
  if (selected_jets.size()>1){
    for (auto jet : selected_jets){
      H_tlv += TLorentzVector(jet.pruned_pseudoJet.px(), jet.pruned_pseudoJet.py(), jet.pruned_pseudoJet.pz(), jet.pruned_pseudoJet.e());
    }
  }
  recoHmass = H_tlv.M()/1000.;
  return recoHmass;
}

double findResolvedRecoAmass(std::vector<particleJet> selected_bjets)
{
  double recoAmass = -9999;
  if (selected_bjets.size()<4) return recoAmass;

  TLorentzVector j1_tlv, j2_tlv, j3_tlv, j4_tlv;
  int counter = 0;
  for (auto jet : selected_bjets){
    if (counter==0) j1_tlv = jet.jet;
    if (counter==1) j2_tlv = jet.jet;
    if (counter==2) j3_tlv = jet.jet;
    if (counter==3) j4_tlv = jet.jet;
    counter ++;
  }

  std::vector<double> comb1;
  comb1.push_back((j1_tlv+j2_tlv).M());
  comb1.push_back((j3_tlv+j4_tlv).M());
  double diff1 = fabs(comb1.at(0) - comb1.at(1));

  std::vector<double> comb2;
  comb2.push_back((j1_tlv+j3_tlv).M());
  comb2.push_back((j2_tlv+j4_tlv).M());
  double diff2 = fabs(comb2.at(0) - comb2.at(1));

  std::vector<double> comb3;
  comb3.push_back((j1_tlv+j4_tlv).M());
  comb3.push_back((j2_tlv+j3_tlv).M());
  double diff3 = fabs(comb3.at(0) - comb3.at(1));

  if (min(diff1, min(diff2, diff3))==diff1)      recoAmass = (comb1.at(0)+comb1.at(1))/2.;
  else if (min(diff1, min(diff2, diff3))==diff2) recoAmass = (comb2.at(0)+comb2.at(1))/2.;
  else if (min(diff1, min(diff2, diff3))==diff3) recoAmass = (comb3.at(0)+comb3.at(1))/2.;

  return recoAmass/1000.;
}


 ostream& operator<<(std::ostream & os, const fastjet::PseudoJet& jet)
{
  os << jet.px() << " " << jet.py() << " " << jet.pz() << " " << jet.e() << " " << ::isBjet(jet) << " " << jet.eta() << " " << jet.phi() << " " << jet.pt()/1000. << endl;
  return os;
}


std::ostream& operator<<(std::ostream& os, const particleJet jet)
{

  os << jet.jet.Px() << " " << jet.jet.Py() << " " << jet.jet.Pz() << " " << jet.jet.E() << " " << jet.isBjet << " " << jet.jet.Eta() << " " << jet.jet.Phi() << " " << jet.jet.Pt()/1000. << endl;
  for (auto parton : jet.parton) {
    TLorentzVector _temp;
    _temp.SetXYZM(parton->m_px,parton->m_py,parton->m_pz,parton->m_m);
    os << "   matched parton: " << parton->m_px << " " << parton->m_py << " " << parton->m_pz << " " << parton->m_pdgId << " " << _temp.Eta() << " " << _temp.Phi() << " " << _temp.Pt()/1000. << endl;
  }
  return os;      
}

std::ostream& operator<<(std::ostream& os, const GenParticle_p5 parton)
{

  TLorentzVector _temp;
  _temp.SetXYZM(parton.m_px,parton.m_py,parton.m_pz,parton.m_m);
  os << parton.m_px << " " << parton.m_py << " " << parton.m_pz << " " << parton.m_pdgId << " " << _temp.Eta() << " " << _temp.Phi() << " " << _temp.Pt()/1000.;
  return os;      
}
