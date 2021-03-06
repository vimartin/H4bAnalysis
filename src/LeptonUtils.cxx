#include "TLorentzVector.h"

#include "ExoticHiggs/LeptonUtils.h"

#include <cmath>
#include <iostream>

using namespace std;

particleLepton::particleLepton()
{}

void particleLepton::setLeptonRaw(TLorentzVector lepton_raw)
{
  m_lepton_raw = lepton_raw;
}

void particleLepton::setGenParticle(GenParticle_p5& parton)
{
  m_parton = parton;
}

void particleLepton::setIndexLepton(int index)
{
  m_index_lep = index;
}

std::vector<int> particleLepton::getDressingIndex()
{
  return m_index_dressing;
}

TLorentzVector particleLepton::getLeptonDressed()
{
  return m_lepton_dressed;
}

TLorentzVector particleLepton::getLeptonRaw()
{
  return m_lepton_raw;
}

GenParticle_p5& particleLepton::getLeptonGenPart()
{
  return m_parton;
}

double particleLepton::getMiniIsolation()
{
  return m_miniIsolation;
}

double particleLepton::getIsolation()
{
  return m_isolation;
}

void particleLepton::dressLepton(std::vector<GenParticle_p5>& partList, bool isPythia6, double radius)
{
  // Photons
  TLorentzVector ph; 
  std::vector<TLorentzVector> radiation;

  int index = 0;
  for (auto part : partList) {
    if (abs(part.m_pdgId) == 22 && part.m_status == 1) {
      ph.SetXYZM(part.m_px,part.m_py,part.m_pz,part.m_m);
      float dR = m_lepton_raw.DeltaR(ph);
      if (dR<radius){
        radiation.push_back(ph);
        m_index_dressing.push_back(index);
      }
    }
    index++;
  }

  // Dressed lepton
  m_lepton_dressed = m_lepton_raw;
  for (auto part : radiation){
    m_lepton_dressed = m_lepton_dressed + part;
  }
}

void particleLepton::compute_isolation(std::vector<particleJet> jetList, double Rmin, double Rmax)
{
  double iso = 0.;
  for (auto jet : jetList) {
    TLorentzVector jet_tlv = jet.jet;
    if (jet_tlv.DeltaR(m_lepton_dressed) < Rmax && jet_tlv.DeltaR(m_lepton_dressed) > Rmin)
      iso += jet_tlv.E();
  }
  iso /= m_lepton_dressed.E();

  m_isolation = iso;
}

void particleLepton::compute_miniIsolation(std::vector<GenParticle_p5>& partList)
{
  TLorentzVector lep_vec;

  lep_vec = m_lepton_dressed;
  double dr = 0.2;
  if (lep_vec.Pt() > 50000)  dr = 10000./lep_vec.Pt();
  if (lep_vec.Pt() > 200000) dr = 0.05;

  double iso=0.;
  int index=-1;
  for (auto part : partList) {
    index++;
    if (part.m_status != 1) continue;
    if (abs(part.m_pdgId) == 12) continue;
    if (abs(part.m_pdgId) == 14) continue;
    if (abs(part.m_pdgId) == 16) continue;

    TLorentzVector part_vec;
    part_vec.SetXYZM(part.m_px,part.m_py,part.m_pz,part.m_m);
    if (m_index_lep == index) continue; // remove the lepton
    if (std::find(m_index_dressing.begin(), m_index_dressing.end(), index)!=m_index_dressing.end()) continue; // remove dressing
    if (part_vec.Pt() < 500.) continue;
    if (lep_vec.DeltaR(part_vec) < dr){
      iso += part_vec.Pt();
//      std::cout<<"Lepton #"<<m_index_lep<<"  Pt(lep): "<<lep_vec.Pt()<<"  Particle #"<<index<<"   Pt: "<<part_vec.Pt()<<"  dR: "<<lep_vec.DeltaR(part_vec)<<"  pdgId: "<<part.m_pdgId<<std::endl;
    }
  }
  iso /= lep_vec.Pt();

  m_miniIsolation = iso;
}



std::vector<int> findHardScatterLeptons(std::vector<GenParticle_p5>& partList, bool isSherpa, bool isPythia6)
{
  int index = -1;
  std::vector<int> index_vec;

  if (!isSherpa){
    if (!isPythia6) {
      std::vector<int> prodVtx;
      for (auto part : partList) {
        index++;
        if ((abs(part.m_pdgId) == 24 || abs(part.m_pdgId) == 23) && part.m_status == 22) {
          prodVtx.push_back(part.m_endVtx);
        }
        if ((abs(part.m_pdgId) == 24 || abs(part.m_pdgId) == 23) && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
          std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
        }
        if (abs(part.m_pdgId) == 15 && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
          std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
        }
        if ((abs(part.m_pdgId) == 11 || abs(part.m_pdgId) == 13) && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
          if (part.m_status == 1) {
            index_vec.push_back(index);
          } else {
            std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
          }
        }
      }
    } 
    else{ // Pythia 6
      std::vector<int> prodVtx;
      std::vector<TLorentzVector> status3_lep;
      for (auto part : partList) {
        index++;
        if ((abs(part.m_pdgId) == 24 || abs(part.m_pdgId) == 23) && part.m_status == 3) { // Find all W/Z (they come first)
          prodVtx.push_back(part.m_endVtx);
        }
        if (abs(part.m_pdgId) == 15 && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
          std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
        }
        if ((abs(part.m_pdgId) == 11 || abs(part.m_pdgId) == 13) && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
          if (part.m_status == 3) {
            TLorentzVector lvec;
            lvec.SetXYZM(part.m_px,part.m_py,part.m_pz,part.m_m);
            status3_lep.push_back(lvec);
          } else {
            std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
          }
        }
        if ((abs(part.m_pdgId) == 11 || abs(part.m_pdgId) == 13) && part.m_status == 1) {
          TLorentzVector v1; 
          v1.SetXYZM(part.m_px,part.m_py,part.m_pz,part.m_m);
          for (auto lep : status3_lep){
            if (lep.DeltaR(v1) < 0.1) {
              index_vec.push_back(index);
            }
          }
        }
      }
    }
  }
  else{ // So it's Sherpa
    std::vector<int> prodVtx;
//    cout<<"\nIm Sherpa!"<<endl;
    for (auto part : partList) {
      index++;
      if ((abs(part.m_pdgId) == 11 || abs(part.m_pdgId) == 13 || abs(part.m_pdgId) == 15) && part.m_status == 11) {
//        cout<<"Found lepton status 11"<<endl;
        prodVtx.push_back(part.m_endVtx);
      }
        if (abs(part.m_pdgId) == 15 && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
          std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
//        cout<<"Found tau status 11"<<endl;
        }
      if (abs(part.m_pdgId) == 15 && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
//        cout<<"Found tau"<<endl;
        std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
      }
      if ((abs(part.m_pdgId) == 11 || abs(part.m_pdgId) == 13) && find(prodVtx.begin(), prodVtx.end(), part.m_prodVtx) != prodVtx.end()) {
        if (part.m_status == 1) {
//          cout<<"Found lepton status 1"<<endl;
          index_vec.push_back(index);
        } else {
//          cout<<"Found lepton other status"<<endl;
          std::replace(prodVtx.begin(), prodVtx.end(), part.m_prodVtx, part.m_endVtx);
        }
      }
    }
  }
  return index_vec;
}


std::vector<int> findIndexPdgidStatus(std::vector<GenParticle_p5>& partList, int pdgId, int status, bool isSherpa, bool isPythia6)
{
  int index = 0;
  std::vector<int> index_vec;

  if (!isSherpa){
    if (!isPythia6) {
      for (auto part : partList) {
        if (fabs(part.m_pdgId)==pdgId && part.m_status==status){
          index_vec.push_back(index);
        }
        index++;
      }
    }
  }
  return index_vec;
}

TLorentzVector findDressedLepton(std::vector<GenParticle_p5>& partList, bool isPythia6, TLorentzVector Vlepton_vec, double radius)
{
  // Lepton TLorentzVector
  TLorentzVector lepton = Vlepton_vec;

  // Photons
  TLorentzVector ph; 
  std::vector<TLorentzVector> radiation;

  for (auto part : partList) {
    if (abs(part.m_pdgId) == 22 && part.m_status == 1) {
        ph.SetXYZM(part.m_px,part.m_py,part.m_pz,part.m_m);
        float dR = lepton.DeltaR(ph);
        if (dR<radius){
          radiation.push_back(ph);
        }
    }
  }

  // Dressed lepton
  TLorentzVector dressed_lepton = lepton;
  for (auto part : radiation){
    dressed_lepton = dressed_lepton + part;
  }
  return dressed_lepton;
}

std::vector<int> finalStateMuons(std::vector<GenParticle_p5>& partList, bool isSherpa, bool isPythia6)
{
  int index = -1;
  std::vector<int> index_vec;

  for (auto part : partList) {
    index ++;
    if (abs(part.m_pdgId)==13 && part.m_status==1){
      index_vec.push_back(index);
    }
  }
  return index_vec;
}


double LeptonIsolation(fastjet::PseudoJet& lepton, std::vector<fastjet::PseudoJet>& jetList, double Rmin, double Rmax)
{
  double iso = 0.;
  for (auto jet : jetList) {
    if (jet.delta_R(lepton) < Rmax && jet.delta_R(lepton) > Rmin)
      iso += jet.e();
  }
  iso /= lepton.e();
  return iso;
}

double LeptonMiniIsolation(GenParticle_p5& lepton, std::vector<GenParticle_p5>& partList)
{
  TLorentzVector lep_vec;
  lep_vec.SetXYZM(lepton.m_px,lepton.m_py,lepton.m_pz,lepton.m_m);
  double dr = 0.2;
  if (lep_vec.Pt() > 50)  dr = 10./lep_vec.Pt();
  if (lep_vec.Pt() > 200) dr = 0.05;

  double iso=0.;
  for (auto part : partList) {
    if (abs(part.m_pdgId) == 12) continue;
    if (abs(part.m_pdgId) == 14) continue;
    if (abs(part.m_pdgId) == 16) continue;

    TLorentzVector part_vec;
    part_vec.SetXYZM(part.m_px,part.m_py,part.m_pz,part.m_m);
    if (part_vec == lep_vec) continue;
    if (part_vec.Pt() < 500.) continue;
    if (lep_vec.DeltaR(part_vec) < dr)
      iso += part_vec.Pt();
  }
  iso /= lep_vec.Pt();
  return iso;

}


