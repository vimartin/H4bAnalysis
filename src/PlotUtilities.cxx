#include "ExoticHiggs/PlotUtilities.h"

#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TH2.h"
#include "TKey.h"
#include "TRegexp.h"
#include <TIterator.h>
#include "TClass.h"
#include "TLegend.h"

#include <iostream>
#include <cmath>

H cumulate (const H &in, bool increasing)
{
  H h_out(in.GetName() + TString("tmp"), in.GetTitle(), in.GetNbinsX(),
      in.GetBinLowEdge(1), in.GetBinLowEdge(in.GetNbinsX() + 1));
  h_out.Sumw2();
  h_out.SetFillColor(in.GetFillColor());
  h_out.SetFillStyle(in.GetFillStyle());
  h_out.SetLineStyle(in.GetLineStyle());
  h_out.SetLineColor(in.GetLineColor());
  h_out.SetLineWidth(in.GetLineWidth());
  double sum = 0;
  double err2 = 0;
  if (increasing) {
    for (int j = 0; j <= in.GetNbinsX() + 1; ++j) {
      sum += in.GetBinContent(j);
      err2 += in.GetBinError(j)*in.GetBinError(j);
      h_out.SetBinContent(j, sum);
      h_out.SetBinError(j, sqrt(err2));
    }
  } else {
    for (int j = in.GetNbinsX() + 1; j >= 0; --j) {
      sum += in.GetBinContent(j);
      err2 += in.GetBinError(j)*in.GetBinError(j);
      h_out.SetBinContent(j, sum);
      h_out.SetBinError(j, sqrt(err2));
    }
  }
  return h_out;
}

TGraph eff_rej (const H &signal, H &background, bool normalize, bool increasing)
{
  H sig = *(TH1D*)signal.Clone("h_tmp_s");
  if (normalize)
    sig.Scale(1 / sig.Integral(0, sig.GetNbinsX() + 1));
  H bg = *(TH1D*)background.Clone("h_tmp_bg");
  if (normalize)
    bg.Scale(1 / bg.Integral(0, bg.GetNbinsX() + 1));
  H sig_cum = cumulate(sig, increasing);
  H bg_cum = cumulate(bg, increasing);
  TGraph ret(signal.GetNbinsX());
  for (int i = 1; i <= signal.GetNbinsX(); ++i) {
    const double x = sig_cum.GetBinCenter(i);
    const double s = sig_cum.GetBinContent(i);
    const double b = bg_cum.GetBinContent(bg_cum.FindBin(x));
    ret.SetPoint(i - 1, b, s); // gotta love offsets
    //        printf("point %d: %f s, %f b\n", i, s, b);
  }
  return ret;
}

void deleteHistos() {
  // Delete all existing histograms in memory
  TObject* obj;
  TList* list = gDirectory->GetList() ;
  TIterator* iter = list->MakeIterator();
  while ( (obj = (iter->Next())) ) {
    if (obj->IsA()->InheritsFrom(TH1::Class()) ||
        obj->IsA()->InheritsFrom(TH2::Class()) ) {
      delete obj;
    }
  }
}

void saveHist(const char* filename, const char* pat)
{

  TList* list = gDirectory->GetList() ;
  TIterator* iter = list->MakeIterator();
  TRegexp re(pat,kTRUE) ;

  TFile outf(filename,"RECREATE") ;
  printf("[PlotUtilities::saveHist] Saving histograms to %s\n", filename);

  int counter = 0;
  while(TObject* obj=iter->Next()) {

    // don't save TH1Keys objects
    // this is a bug fudge
    if (TString(obj->GetName()).Contains("histokeys")) continue;

    // save other stuff
    if (TString(obj->GetName()).Index(re)>=0) {
      obj->Write() ;
      ++counter;
    }

  }

  printf("[PlotUtilities::saveHist] Saved %i histograms\n", counter);
  outf.Close() ;
  delete iter ;
}

TCanvas *ComparePlots(TFile *f, const char *hist1, const char *hist2, const char *label1, const char *label2, unsigned int rebin, bool norm, bool log, unsigned int opt)
{

  // get hists
  TH1D *h1 = (TH1D*)f->Get(hist1)->Clone(hist1);
  TH1D *h2 = (TH1D*)f->Get(hist2)->Clone(hist2);

  // overflow
  h1->SetBinContent(h1->GetNbinsX(), h1->GetBinContent(h1->GetNbinsX()) + h1->GetBinContent(h1->GetNbinsX()+1));
  h1->SetBinContent(h1->GetNbinsX()+1, 0.0);
  h2->SetBinContent(h2->GetNbinsX(), h2->GetBinContent(h2->GetNbinsX()) + h2->GetBinContent(h2->GetNbinsX()+1));
  h2->SetBinContent(h2->GetNbinsX()+1, 0.0);

  // rebin
  h1->Rebin(rebin);
  h2->Rebin(rebin);

  // normalize
  if (norm) h1->Scale(h2->Integral(0, h2->GetNbinsX() + 1) / h1->Integral(0, h1->GetNbinsX() + 1));

  // format
  if (opt == 1) {
    h1->SetLineWidth(2);
    h1->SetLineColor(kRed);
    h2->SetLineColor(kBlue);
    h2->SetFillColor(kBlue);
    h2->SetFillStyle(0);
    h2->SetLineWidth(2);
    h2->SetMarkerStyle(20);
    h2->SetMarkerColor(kBlue);
  } 
  if (opt == 2) {
    h1->SetLineWidth(2);
    h1->SetLineColor(kRed);
    h1->SetFillColor(kRed);
    h1->SetFillStyle(3004);
    h2->SetFillStyle(0);
    h2->SetLineWidth(2);
    h2->SetMarkerStyle(20);
    h2->SetMarkerColor(kBlack);
    h2->SetLineColor(kBlack);
  }

  // legend
  TLegend *l1 = new TLegend(0.70, 0.85, 0.98, 0.98);
  l1->SetLineColor(kWhite);
  l1->SetFillColor(kWhite);
  l1->SetShadowColor(kWhite);
  l1->AddEntry(h1, label1, "f");
  l1->AddEntry(h2, label2, "f");

  // draw
  TCanvas *c1 = new TCanvas();
  if (log) c1->SetLogy();
  c1->cd();
  h1->Draw("HIST");
  h2->Draw("SAME E1");
  double yMax = TMath::Max(h1->GetMaximum(), h2->GetMaximum());
  h1->GetYaxis()->SetRangeUser(0.0, TMath::Max(yMax + 2*sqrt(yMax), yMax + 0.3*yMax));
  if (log) h1->GetYaxis()->SetRangeUser(0.1, yMax * 1000);
  l1->Draw();

  return c1;

}

TGraph GetROC(TFile *f, const char *hist1, const char *hist2, bool increasing)
{

  // get hists
  TH1D *sig = (TH1D*)f->Get(hist1)->Clone(hist1);
  TH1D *bkg = (TH1D*)f->Get(hist2)->Clone(hist2);

  // overflow
  sig->SetBinContent(sig->GetNbinsX(), sig->GetBinContent(sig->GetNbinsX()) + sig->GetBinContent(sig->GetNbinsX()+1));
  sig->SetBinContent(sig->GetNbinsX()+1, 0.0);
  bkg->SetBinContent(bkg->GetNbinsX(), bkg->GetBinContent(bkg->GetNbinsX()) + bkg->GetBinContent(bkg->GetNbinsX()+1));
  bkg->SetBinContent(bkg->GetNbinsX()+1, 0.0);

  TGraph gr_roc = eff_rej(*sig, *bkg, true, increasing);
  return gr_roc;
}


TGraph GetEff(TFile *f, const char *hist1, bool increasing)
{

  // get hists
  TH1D *sig = (TH1D*)f->Get(hist1)->Clone(hist1);

  // overflow
  sig->SetBinContent(sig->GetNbinsX(), sig->GetBinContent(sig->GetNbinsX()) + sig->GetBinContent(sig->GetNbinsX()+1));
  sig->SetBinContent(sig->GetNbinsX()+1, 0.0);

  TGraph ret(sig->GetNbinsX());
  for (int i = 1; i <= sig->GetNbinsX(); ++i) {
    float val = sig->GetBinLowEdge(i);
    float ntotal = sig->Integral(1, sig->GetNbinsX());
    float npass = sig->Integral(i, sig->GetNbinsX());
    if (increasing) npass = ntotal - npass;
    ret.SetPoint(i - 1, val, npass/ntotal);
  }

  return ret;
}


void plot1D(string name, float xval, double weight, std::map<string, TH1*> &allhistos, 
    string title, int numbinsx, float xmin, float xmax)  
{
  if (title=="") title=name; 
  std::map<string, TH1*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH1D* currentHisto= new TH1D(name.c_str(), title.c_str(), numbinsx, xmin, xmax);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, weight);
    allhistos.insert(std::pair<string, TH1D*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    (*iter).second->Fill(xval, weight);
  }

}

void plot1D(string name, float xval, double weight, std::map<string, TH1*> &allhistos, 
    string title, int numbinsx, const float * xbins)  
{
  if (title=="") title=name; 
  std::map<string, TH1*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH1D* currentHisto= new TH1D(name.c_str(), title.c_str(), numbinsx, xbins);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, weight);
    allhistos.insert(std::pair<string, TH1D*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    (*iter).second->Fill(xval, weight);
  }

}


void plot1D_cutflow(std::string name, int ibin, std::map<string, TH1*> &allhistos, string title, std::vector<std::string> bin_title_vec)
{
  if (title=="") title=name; 
  std::map<string, TH1*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH1D* currentHisto= new TH1D(name.c_str(), title.c_str(), bin_title_vec.size(), 0, bin_title_vec.size());
    for (unsigned int bin = 1; bin<=bin_title_vec.size(); bin++){
      currentHisto->GetXaxis()->SetBinLabel(bin, bin_title_vec.at(bin-1).c_str());
    }

    currentHisto->SetBinContent(ibin+1, currentHisto->GetBinContent(ibin+1)+1);
    allhistos.insert(std::pair<string, TH1D*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    (*iter).second->SetBinContent(ibin+1, (*iter).second->GetBinContent(ibin+1)+1);
  }
}

void plot1DUnderOverFlow(string title, double xval, double weight, std::map<string, TH1*> &allhistos, 
    int numbinsx, double xmin, double xmax)  
{
  if (xval >= xmax) xval = xmax - (xmax-xmin)/double(numbinsx)/1000.;
  if (xval <= xmin) xval = xmin + (xmax-xmin)/double(numbinsx)/1000.;

  std::map<string, TH1*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH1D* currentHisto= new TH1D(title.c_str(), title.c_str(), numbinsx, xmin, xmax);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, weight);
    allhistos.insert(std::pair<string, TH1D*> (title,currentHisto) );
  }
  else // exists already, so just fill it
  {
    (*iter).second->Fill(xval, weight);
  }

}

TH1D* getHist1D(string title, std::map<string, TH1*> &allhistos, 
    int numbinsx, float xmin, float xmax)  
{

  TH1D* currentHisto = 0;

  std::map<string, TH1*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    currentHisto= new TH1D(title.c_str(), title.c_str(), numbinsx, xmin, xmax);
    currentHisto->Sumw2();
    allhistos.insert(std::pair<string, TH1D*> (title,currentHisto) );
  }
  else // exists already, so just fill it
  {
    currentHisto = (TH1D*) (*iter).second;
  }

  return currentHisto;
}

void insertHist1D(TH1D* hist, std::map<string, TH1*> &allhistos)
{

  string title(hist->GetName());

  std::map<string, TH1*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, insert into map
  {
    allhistos.insert(std::pair<string, TH1D*> (title,hist) );
  }
  else // exists already, give warning and overwrite
  {
    printf("[PlotUtilities::insertHist1D] WARNING: overwriting histogram %s\n", title.c_str());
    (*iter).second = hist;
  }

  return;
}

void savePlots(std::map<string, TH1*> &h_1d, const char* outfilename){
  TFile outfile(outfilename,"RECREATE") ;

  printf("[PlotUtilities::savePlots] Saving histograms to %s\n", outfilename);

  std::map<std::string, TH1*>::iterator it1d;
  for(it1d=h_1d.begin(); it1d!=h_1d.end(); it1d++) {
    it1d->second->Write();
    delete it1d->second;
  }

  outfile.Write();
  outfile.Close();
}

void savePlots2(std::map<string, TH2D*> &h_1d, const char* outfilename){
  TFile outfile(outfilename,"RECREATE") ;

  printf("[PlotUtilities::savePlots] Saving histograms to %s\n", outfilename);

  std::map<std::string, TH2D*>::iterator it1d;
  for(it1d=h_1d.begin(); it1d!=h_1d.end(); it1d++) {
    it1d->second->Write();
    delete it1d->second;
  }

  outfile.Write();
  outfile.Close();
}


void savePlotsDir(std::map<string, TH1*> &h_1d, TFile* outfile, const char* dirname){

  printf("[PlotUtilities::savePlots] Saving 1d histograms to dir: %s\n", dirname);

  TDirectory* dir = 0;

  // base dir: just cd to output file
  if (strcmp(dirname,"") == 0) {
    outfile->cd();
  } else {
    // first check for directory, create if it doesn't exist
    dir = (TDirectory*)outfile->Get(dirname);
    if (dir == 0) {
      dir = outfile->mkdir(dirname);
    } 
    dir->cd();
  }

  std::map<std::string, TH1*>::iterator it1d;
  for(it1d=h_1d.begin(); it1d!=h_1d.end(); it1d++) {
    it1d->second->Write();
    delete it1d->second;
  }

}

void savePlots2Dir(std::map<string, TH2D*> &h_2d, TFile* outfile, const char* dirname){

  printf("[PlotUtilities::savePlots2Dir] Saving 2d histograms to dir: %s\n", dirname);

  TDirectory* dir = 0;

  // base dir: just cd to output file
  if (strcmp(dirname,"") == 0) {
    outfile->cd();
  } else {
    // first check for directory, create if it doesn't exist
    dir = (TDirectory*)outfile->Get(dirname);
    if (dir == 0) {
      dir = outfile->mkdir(dirname);
    } 
    dir->cd();
  }

  std::map<std::string, TH2D*>::iterator it2d;
  for(it2d=h_2d.begin(); it2d!=h_2d.end(); it2d++) {
    it2d->second->Write();
    delete it2d->second;
  }

}

void savePlots12(std::map<string, TH1D*> &h_1d, std::map<string, TH2D*> &h_2d, const char* outfilename){
  TFile outfile(outfilename,"RECREATE") ;

  printf("[PlotUtilities::savePlots12] Saving histograms to %s\n", outfilename);

  std::map<std::string, TH1D*>::iterator it1d;
  for(it1d=h_1d.begin(); it1d!=h_1d.end(); it1d++) {
    it1d->second->Write();
    delete it1d->second;
  }

  std::map<std::string, TH2D*>::iterator it2d;
  for(it2d=h_2d.begin(); it2d!=h_2d.end(); it2d++) {
    it2d->second->Write();
    delete it2d->second;
  }

  outfile.Write();
  outfile.Close();
}


void plot2D(string name, float xval, float yval, double weight, std::map<string, TH1*> &allhistos, 
    string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax){

  if (title=="") title=name; 
  std::map<string, TH1*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH2D* currentHisto= new TH2D(name.c_str(), title.c_str(), numbinsx, xmin, xmax, numbinsy, ymin, ymax);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, weight);
    allhistos.insert(std::pair<string, TH1*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    ((TH2D*) (*iter).second)->Fill(xval, yval, weight);
  }

  return;

}

void plot2D(string name, float xval, float yval, double weight, std::map<string, TH1*> &allhistos, 
    string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins){

  if (title=="") title=name; 
  std::map<string, TH1*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH2D* currentHisto= new TH2D(name.c_str(), title.c_str(), numbinsx, xbins, numbinsy, ybins);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, weight);
    allhistos.insert(std::pair<string, TH1*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    ((TH2D*) (*iter).second)->Fill(xval, yval, weight);
  }

  return;

}
void plot2D(string name, float xval, float yval, double weight, std::map<string, TH2D*> &allhistos, 
    string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax){

  if (title=="") title=name; 
  std::map<string, TH2D*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH2D* currentHisto= new TH2D(name.c_str(), title.c_str(), numbinsx, xmin, xmax, numbinsy, ymin, ymax);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, weight);
    allhistos.insert(std::pair<string, TH2D*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    (*iter).second->Fill(xval, yval, weight);
  }

  return;

}

void plot2D(string name, float xval, float yval, double weight, std::map<string, TH2D*> &allhistos, 
    string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins){

  if (title=="") title=name; 
  std::map<string, TH2D*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH2D* currentHisto= new TH2D(name.c_str(), title.c_str(), numbinsx, xbins, numbinsy, ybins);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, weight);
    allhistos.insert(std::pair<string, TH2D*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    (*iter).second->Fill(xval, yval, weight);
  }

  return;

}

void plot2DUnderOverFlow(string name, double xval, double yval, double weight, std::map<string, TH1*> &allhistos, 
    string title, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax){

  if (title=="") title=name; 

  if (xval >= xmax) xval = xmax - (xmax-xmin)/double(numbinsx)/1000.;
  if (xval <= xmin) xval = xmin + (xmax-xmin)/double(numbinsx)/1000.;
  if (yval >= ymax) yval = ymax - (ymax-ymin)/double(numbinsy)/1000.;
  if (yval <= ymin) yval = ymin + (ymax-ymin)/double(numbinsy)/1000.;

  std::map<string, TH1*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH2D* currentHisto= new TH2D(title.c_str(), title.c_str(), numbinsx, xmin, xmax, numbinsy, ymin, ymax);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, weight);
    allhistos.insert(std::pair<string, TH1*> (title,currentHisto) );
  }
  else // exists already, so just fill it
  {
    ((TH2D*) (*iter).second)->Fill(xval, yval, weight);
  }

  return;

}

void plot3D(string name, float xval, float yval, float zval, double weight, std::map<string, TH1*> &allhistos, 
    string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax,
    int numbinsz, float zmin, float zmax){

  if (title=="") title=name; 
  std::map<string, TH1*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH3D* currentHisto= new TH3D(name.c_str(), title.c_str(), numbinsx, xmin, xmax, numbinsy, ymin, ymax, numbinsz, zmin, zmax);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, zval, weight);
    allhistos.insert(std::pair<string, TH1*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    ((TH3D*) (*iter).second)->Fill(xval, yval, zval, weight);
  }

  return;

}

void plot3D(string name, float xval, float yval, float zval, double weight, std::map<string, TH1*> &allhistos, 
    string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins, int numbinsz, const float * zbins){

  if (title=="") title=name; 
  std::map<string, TH1*>::iterator iter= allhistos.find(name);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
  {
    TH3D* currentHisto= new TH3D(name.c_str(), title.c_str(), numbinsx, xbins, numbinsy, ybins, numbinsz, zbins);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, zval, weight);
    allhistos.insert(std::pair<string, TH1*> (name,currentHisto) );
  }
  else // exists already, so just fill it
  {
    ((TH3D*) (*iter).second)->Fill(xval, yval, zval, weight);
  }

  return;

}




void doAllPlots(int analysis_type, std::string pass, std::map<string, TH1*> &h_1d, std::map<string, TH2D*> &h_2d, std::vector<particleJet> selected_jets, std::vector<particleJet> selected_bjets, std::vector<particleLepton> selected_lepton, double weight)
{

  plot1D(Form("h_total_events-%s",pass.c_str()), 0.5, weight, h_1d, "", 1, 0., 1.);
  if (analysis_type==0){ //--- 0: resolved analysis
    for (auto lepton : selected_lepton){
      plot1D(Form("h_lepton_pt-%s",pass.c_str()), lepton.getLeptonDressed().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_lepton_eta-%s",pass.c_str()), lepton.getLeptonDressed().Eta(), weight, h_1d, "", 100, -5, -5);
      plot1D(Form("h_leptonraw_pt-%s",pass.c_str()), lepton.getLeptonRaw().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_lepton_iso-%s",pass.c_str()), lepton.getIsolation(), weight, h_1d, "", 100, 0., 0.5);
      plot1D(Form("h_lepton_miniiso-%s",pass.c_str()), lepton.getMiniIsolation(), weight, h_1d, "", 100, 0., 0.5);
    }
    plot1D(Form("h_njets-%s",pass.c_str()), selected_jets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    for (auto jet : selected_jets) {
      plot1D(Form("h_jets_pt-%s",pass.c_str()), jet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_jets_eta-%s",pass.c_str()), jet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
    }
    plot1D(Form("h_nbjets-%s",pass.c_str()), selected_bjets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    plot1D(Form("h_RecoHMass-%s",pass.c_str()), findResolvedRecoHmass(selected_bjets), weight, h_1d, "", 25, 0., 250.);
    plot1D(Form("h_RecoAMass-%s",pass.c_str()), findResolvedRecoAmass(selected_bjets), weight, h_1d, "", 25, 0., 250.);
    plot2D(Form("h_njets_vs_nbjets-%s",pass.c_str()), selected_jets.size(), selected_bjets.size(), weight, h_2d, "", 10, -0.5, 9.5, 10, -0.5, 9.5);
    for (auto bjet : selected_bjets) {
      plot1D(Form("h_bjets_pt-%s",pass.c_str()), bjet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_bjets_eta-%s",pass.c_str()), bjet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
      plot1D(Form("h_nbpartonjets-%s",pass.c_str()), bjet.nBpartons, weight, h_1d, "", 5, -0.5, 4.5);
    }
  }

  else if (analysis_type==1){ //--- 1: boosted analysis
    for (auto lepton : selected_lepton){
      plot1D(Form("h_lepton_pt-%s",pass.c_str()), lepton.getLeptonDressed().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_lepton_eta-%s",pass.c_str()), lepton.getLeptonDressed().Eta(), weight, h_1d, "", 100, -5, -5);
      plot1D(Form("h_leptonraw_pt-%s",pass.c_str()), lepton.getLeptonRaw().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_lepton_iso-%s",pass.c_str()), lepton.getIsolation(), weight, h_1d, "", 100, 0., 0.5);
      plot1D(Form("h_lepton_miniiso-%s",pass.c_str()), lepton.getMiniIsolation(), weight, h_1d, "", 100, 0., 0.5);
    }
    plot1D(Form("h_nfatjets-%s",pass.c_str()), selected_jets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    plot1D(Form("h_nfatbjets-%s",pass.c_str()), selected_bjets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    plot1D(Form("h_RecoHMass-%s",pass.c_str()), findBoostedRecoHmass(selected_bjets), weight, h_1d, "", 25, 0., 250.);
    if (selected_bjets.size()==2){
      if (findBoostedRecoHmass(selected_bjets)>60)
        plot1D(Form("h_fatjet_trimmed_mass_verena-%s",pass.c_str()), findBoostedRecoHmass(selected_bjets), weight, h_1d, "", 50, 0., 150.);
    }
    for (auto jet : selected_jets) {
      plot1D(Form("h_fatjet_pt-%s",pass.c_str()), jet.pseudoJet.perp()/1000., weight, h_1d, "", 30, 0., 300.);
      plot1D(Form("h_fatjet_eta-%s",pass.c_str()), jet.pseudoJet.eta(), weight, h_1d, "", 100, -5., 5.);
      plot1D(Form("h_fatjet_mass-%s",pass.c_str()), jet.pseudoJet.m()/1000., weight, h_1d, "", 100, 0., 150.);

      if (jet.mdtagged_pseudoJet.perp()>0.){
        plot1D(Form("h_fatjet_mdtagged_pt-%s",pass.c_str()), jet.mdtagged_pseudoJet.perp()/1000., weight, h_1d, "", 30, 0., 300.);
        plot1D(Form("h_fatjet_mdtagged_eta-%s",pass.c_str()), jet.mdtagged_pseudoJet.eta(), weight, h_1d, "", 100, -5., 5.);
        plot1D(Form("h_fatjet_mdtagged_mass-%s",pass.c_str()), jet.mdtagged_pseudoJet.m()/1000., weight, h_1d, "", 100, 0., 150.);
        plot1D(Form("h_nsubjet_mdtagged-%s",pass.c_str()), jet.mdtagged_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
        if (jet.hasMDSubstructure){
          int nbsub = 0;
          int bsub1 = -1;
          int bsub2 = -1;
          for (int isub = 0; isub < jet.mdtagged_subPseudoJets_bflag.size(); isub++) {
            plot1D(Form("h_subjet_mdtagged_pt-%s",pass.c_str()), jet.mdtagged_subPseudoJets.at(isub).perp()/1000., weight, h_1d, "", 30, 0., 300.);
            if (jet.mdtagged_subPseudoJets_bflag.at(isub)) {
              nbsub++;
              if (bsub1 < 0) {
                bsub1 = isub;
              } else if (bsub2 < 0) {
                bsub2 = isub;
              }
            }
          }
          plot1D(Form("h_nbsubjet_mdtagged-%s",pass.c_str()), nbsub, weight, h_1d, "", 10, -0.5, 9.5);
          if (jet.mdtagged_subPseudoJets.size() >= 2){
            plot1D(Form("h_subjet_pieces_mass_mdtagged-%s",pass.c_str()), (jet.mdtagged_subPseudoJets.at(0)+jet.mdtagged_subPseudoJets.at(1)).m()/1000., weight, h_1d, "", 100, 0., 150.);
          }
        }
      }

      if (jet.pruned_pseudoJet.perp()>0.){
        plot1D(Form("h_fatjet_pruned_pt-%s",pass.c_str()), jet.pruned_pseudoJet.perp()/1000., weight, h_1d, "", 30, 0., 300.);
        plot1D(Form("h_fatjet_pruned_eta-%s",pass.c_str()), jet.pruned_pseudoJet.eta(), weight, h_1d, "", 100, -5., 5.);
        plot1D(Form("h_fatjet_pruned_mass-%s",pass.c_str()), jet.pruned_pseudoJet.m()/1000., weight, h_1d, "", 100, 0., 150.);
        plot1D(Form("h_nsubjet_pruned-%s",pass.c_str()), jet.pruned_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
        if (jet.hasPruningSubstructure){
          int nbsub = 0;
          int bsub1 = -1;
          int bsub2 = -1;
          for (int isub = 0; isub < jet.pruned_subPseudoJets_bflag.size(); isub++) {
            plot1D(Form("h_subjet_pruned_pt-%s",pass.c_str()), jet.pruned_subPseudoJets.at(isub).perp()/1000., weight, h_1d, "", 30, 0., 300.);
            if (jet.pruned_subPseudoJets_bflag.at(isub)) {
              nbsub++;
              if (bsub1 < 0) {
                bsub1 = isub;
              } else if (bsub2 < 0) {
                bsub2 = isub;
              }
            }
          }
          plot1D(Form("h_nsubjet_pruned-%s",pass.c_str()), jet.pruned_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
          plot1D(Form("h_nbsubjet_pruned-%s",pass.c_str()), nbsub, weight, h_1d, "", 10, -0.5, 9.5);
          if (jet.pruned_subPseudoJets.size() >= 2){
            plot1D(Form("h_subjet_pieces_mass_pruned-%s",pass.c_str()), (jet.pruned_subPseudoJets.at(0)+jet.pruned_subPseudoJets.at(1)).m()/1000., weight, h_1d, "", 100, 0., 150.);
          }
        }
      }

      if (jet.filtered_pseudoJet.perp()>0.){
        plot1D(Form("h_fatjet_BDRS_pt-%s",pass.c_str()), jet.filtered_pseudoJet.perp()/1000., weight, h_1d, "", 30, 0., 300.);
        plot1D(Form("h_fatjet_BDRS_eta-%s",pass.c_str()), jet.filtered_pseudoJet.eta(), weight, h_1d, "", 100, -5., 5.);
        plot1D(Form("h_fatjet_BDRS_mass-%s",pass.c_str()), jet.filtered_pseudoJet.m()/1000., weight, h_1d, "", 100, 0., 150.);
        plot1D(Form("h_nsubjet_BDRS-%s",pass.c_str()), jet.filtered_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
        if (jet.hasBDRSSubstructure){
          int nbsub = 0;
          int bsub1 = -1;
          int bsub2 = -1;
          for (int isub = 0; isub < jet.filtered_subPseudoJets_bflag.size(); isub++) {
            plot1D(Form("h_subjet_filtered_pt-%s",pass.c_str()), jet.filtered_subPseudoJets.at(isub).perp()/1000., weight, h_1d, "", 30, 0., 300.);
            if (jet.filtered_subPseudoJets_bflag.at(isub)) {
              nbsub++;
              if (bsub1 < 0) {
                bsub1 = isub;
              } else if (bsub2 < 0) {
                bsub2 = isub;
              }
            }
          }
          plot1D(Form("h_nsubjet_BDRS-%s",pass.c_str()), jet.filtered_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
          plot1D(Form("h_nbsubjet_BDRS-%s",pass.c_str()), nbsub, weight, h_1d, "", 10, -0.5, 9.5);
          if (jet.filtered_subPseudoJets.size() >= 2){
            plot1D(Form("h_subjet_pieces_mass_BDRS-%s",pass.c_str()), (jet.filtered_subPseudoJets.at(0)+jet.filtered_subPseudoJets.at(1)).m()/1000., weight, h_1d, "", 100, 0., 150.);
            plot1D(Form("h_subjet_momentum_balance_BDRS-%s",pass.c_str()), min(jet.filtered_subPseudoJets.at(0).perp(), jet.filtered_subPseudoJets.at(1).perp())*jet.filtered_subPseudoJets.at(0).delta_R(jet.filtered_subPseudoJets.at(1))/jet.filtered_pseudoJet.m(), weight, h_1d, "", 50, 0., 1.);
          }
        }
      }

      if (jet.trimmed_pseudoJet.perp()>0.){
        plot1D(Form("h_fatjet_trimmed_pt-%s",pass.c_str()), jet.trimmed_pseudoJet.perp()/1000., weight, h_1d, "", 30, 0., 300.);
        plot1D(Form("h_fatjet_trimmed_eta-%s",pass.c_str()), jet.trimmed_pseudoJet.eta(), weight, h_1d, "", 100, -5., 5.);
        plot1D(Form("h_fatjet_trimmed_mass-%s",pass.c_str()), jet.trimmed_pseudoJet.m()/1000., weight, h_1d, "", 100, 0., 150.);
        plot1D(Form("h_nsubjet_trimmed-%s",pass.c_str()), jet.trimmed_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
//        plot1D(Form("h_fatjet_trimmed_mass_verena-%s",pass.c_str()), jet.trimmed_pseudoJet.m()/1000., weight, h_1d, "", 50, 0., 150.);
        if (jet.hasTrimmingSubstructure){
          int nbsub = 0;
          int bsub1 = -1;
          int bsub2 = -1;
          for (int isub = 0; isub < jet.trimmed_subPseudoJets_bflag.size(); isub++) {
            plot1D(Form("h_subjet_trimmed_pt-%s",pass.c_str()), jet.trimmed_subPseudoJets.at(isub).perp()/1000., weight, h_1d, "", 30, 0., 300.);
            if (jet.trimmed_subPseudoJets_bflag.at(isub)) {
              nbsub++;
              if (bsub1 < 0) {
                bsub1 = isub;
              } else if (bsub2 < 0) {
                bsub2 = isub;
              }
            }
          }
          plot1D(Form("h_nsubjet_trimmed-%s",pass.c_str()), jet.trimmed_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
          plot1D(Form("h_nbsubjet_trimmed-%s",pass.c_str()), nbsub, weight, h_1d, "", 10, -0.5, 9.5);
          if (jet.trimmed_subPseudoJets.size() >= 2){
            plot1D(Form("h_subjet_pieces_mass_trimmed-%s",pass.c_str()), (jet.trimmed_subPseudoJets.at(0)+jet.trimmed_subPseudoJets.at(1)).m()/1000., weight, h_1d, "", 100, 0., 150.);
          }
        }
      }
      if (jet.hasMatchedSubstructure){
        int nbsub = 0;
        int bsub1 = -1;
        int bsub2 = -1;
        for (int isub = 0; isub < jet.matched_subPseudoJets_bflag.size(); isub++) {
          plot1D(Form("h_subjet_matched_pt-%s",pass.c_str()), jet.matched_subPseudoJets.at(isub).perp()/1000., weight, h_1d, "", 30, 0., 300.);
          if (jet.matched_subPseudoJets_bflag.at(isub)) {
            nbsub++;
            if (bsub1 < 0) {
              bsub1 = isub;
            } else if (bsub2 < 0) {
              bsub2 = isub;
            }
          }
        }
        plot1D(Form("h_nsubjet_matched-%s",pass.c_str()), jet.matched_subPseudoJets.size(), weight, h_1d, "", 10, -0.5, 9.5);
        plot1D(Form("h_nbsubjet_matched-%s",pass.c_str()), nbsub, weight, h_1d, "", 10, -0.5, 9.5);
        if (jet.matched_subPseudoJets.size() >= 2){
          plot1D(Form("h_subjet_pieces_mass_matched-%s",pass.c_str()), (jet.matched_subPseudoJets.at(0)+jet.matched_subPseudoJets.at(1)).m()/1000., weight, h_1d, "", 100, 0., 150.);
        }
      }
    } // end jets
  } // end type 1
  else if (analysis_type==2){ //--- 2: bmuon analysis
    plot1D(Form("h_nmuons-%s",pass.c_str()), selected_lepton.size(), weight, h_1d, "", 10, -0.5, 9.5);
    for (auto lepton : selected_lepton){
      plot1D(Form("h_muon_pt-%s",pass.c_str()), lepton.getLeptonDressed().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_muon_eta-%s",pass.c_str()), lepton.getLeptonDressed().Eta(), weight, h_1d, "", 100, -5, -5);
      plot1D(Form("h_muonraw_pt-%s",pass.c_str()), lepton.getLeptonRaw().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_muon_iso-%s",pass.c_str()), lepton.getIsolation(), weight, h_1d, "", 100, 0., 0.5);
      plot1D(Form("h_muon_miniiso-%s",pass.c_str()), lepton.getMiniIsolation(), weight, h_1d, "", 100, 0., 0.5);
    }
    plot1D(Form("h_njets-%s",pass.c_str()), selected_jets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    for (auto jet : selected_jets) {
      plot1D(Form("h_jets_pt-%s",pass.c_str()), jet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_jets_eta-%s",pass.c_str()), jet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
    }
    plot1D(Form("h_nbjets-%s",pass.c_str()), selected_bjets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    plot1D(Form("h_RecoHMass-%s",pass.c_str()), findResolvedRecoHmass(selected_bjets), weight, h_1d, "", 25, 0., 250.);
    plot1D(Form("h_RecoAMass-%s",pass.c_str()), findResolvedRecoAmass(selected_bjets), weight, h_1d, "", 25, 0., 250.);
    plot2D(Form("h_njets_vs_nbjets-%s",pass.c_str()), selected_jets.size(), selected_bjets.size(), weight, h_2d, "", 10, -0.5, 9.5, 10, -0.5, 9.5);
    for (auto bjet : selected_bjets) {
      plot1D(Form("h_bjets_pt-%s",pass.c_str()), bjet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_bjets_eta-%s",pass.c_str()), bjet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
      plot1D(Form("h_nbpartonjets-%s",pass.c_str()), bjet.nBpartons, weight, h_1d, "", 5, -0.5, 4.5);
    }
    if (selected_lepton.size()>0 && selected_jets.size()){
      for (auto lepton : selected_lepton){
        float minDR_mu_jet = 100.;
        for (auto jet : selected_jets) {
          float dR = lepton.getLeptonDressed().DeltaR(jet.jet);
          if (dR < minDR_mu_jet){
            minDR_mu_jet = dR;
          }
        }
        plot1D(Form("h_dRmin_muons_jets-%s",pass.c_str()), minDR_mu_jet, weight, h_1d, "", 100, 0, 5);
      }
    }
    if (selected_lepton.size()>0 && selected_bjets.size()){
      for (auto lepton : selected_lepton){
        float minDR_mu_bjet = 100.;
        for (auto bjet : selected_bjets) {
          float dR = lepton.getLeptonDressed().DeltaR(bjet.jet);
          if (dR < minDR_mu_bjet){
            minDR_mu_bjet = dR;
          }
        }
        plot1D(Form("h_dRmin_muons_bjets-%s",pass.c_str()), minDR_mu_bjet, weight, h_1d, "", 100, 0, 5);
      }
    }
  } // end type 2

  if (analysis_type==3){ //--- 3: yellow book plots
    for (auto lepton : selected_lepton){
      plot1D(Form("h_lepton_pt-%s",pass.c_str()), lepton.getLeptonDressed().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_lepton_eta-%s",pass.c_str()), lepton.getLeptonDressed().Eta(), weight, h_1d, "", 100, -5, -5);
      plot1D(Form("h_leptonraw_pt-%s",pass.c_str()), lepton.getLeptonRaw().Pt()/1000., weight, h_1d, "", 100, 0., 150.);
      plot1D(Form("h_lepton_iso-%s",pass.c_str()), lepton.getIsolation(), weight, h_1d, "", 100, 0., 0.5);
      plot1D(Form("h_lepton_miniiso-%s",pass.c_str()), lepton.getMiniIsolation(), weight, h_1d, "", 100, 0., 0.5);
    }
    plot1D(Form("h_njets-%s",pass.c_str()), selected_jets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    int jet_counter = 0;
    for (auto jet : selected_jets) {
      plot1D(Form("h_jets_pt-%s",pass.c_str()), jet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_jets_eta-%s",pass.c_str()), jet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
      plot1D(Form("h_jet%d_pt-%s", jet_counter, pass.c_str()), jet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_jet%d_eta-%s", jet_counter, pass.c_str()), jet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
      jet_counter++;
    }
    plot1D(Form("h_nbjets-%s",pass.c_str()), selected_bjets.size(), weight, h_1d, "", 10, -0.5, 9.5);
    plot1D(Form("h_RecoHMass-%s",pass.c_str()), findResolvedRecoHmass(selected_bjets), weight, h_1d, "", 25, 0., 250.);
    plot1D(Form("h_RecoAMass-%s",pass.c_str()), findResolvedRecoAmass(selected_bjets), weight, h_1d, "", 25, 0., 250.);
    plot2D(Form("h_njets_vs_nbjets-%s",pass.c_str()), selected_jets.size(), selected_bjets.size(), weight, h_2d, "", 10, -0.5, 9.5, 10, -0.5, 9.5);
    int bjet_counter = 0;
    for (auto bjet : selected_bjets) {
      plot1D(Form("h_bjets_pt-%s",pass.c_str()), bjet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_bjets_eta-%s",pass.c_str()), bjet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
      plot1D(Form("h_nbpartonjets-%s",pass.c_str()), bjet.nBpartons, weight, h_1d, "", 5, -0.5, 4.5);
      plot1D(Form("h_bjet%d_pt-%s", bjet_counter, pass.c_str()), bjet.jet.Pt()/1000., weight, h_1d, "", 30, 0., 300);
      plot1D(Form("h_bjet%d_eta-%s", bjet_counter, pass.c_str()), bjet.jet.Eta(), weight, h_1d, "", 100, -5, 5);
      bjet_counter++;
    }
    if (selected_lepton.size()>0 && selected_bjets.size()>1){
      plot2D(Form("h_leptonpt_vs_bjetpt-%s",pass.c_str()), selected_lepton.at(0).getLeptonDressed().Pt()/1000., (selected_bjets.at(0).jet.Pt()+selected_bjets.at(1).jet.Pt())/1000., weight, h_2d, "", 100, 0., 150., 30, 0., 300.);
    }
  } // end type 3
}
