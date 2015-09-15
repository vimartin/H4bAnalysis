#ifndef PLOTUTILITIES_H
#define PLOTUTILITIES_H

#include "TH1.h" 
#include "TH2.h" 
#include "TH3.h" 
#include "TGraph.h"
#include "TCanvas.h" 
#include "TFile.h"

#include<map>
#include<string>

#include "ExoticHiggs/JetUtils.h"
#include "ExoticHiggs/LeptonUtils.h"

using namespace std;

typedef TH1D H;

H cumulate (const H &in, bool increasing);
TGraph eff_rej (const H &signal, H &background, bool normalize, bool increasing);
void saveHist(const char* filename, const char* pat="*");
void deleteHistos();
TCanvas *ComparePlots(TFile *f, const char *hist1, const char *hist2, const char *label1, const char *label2, 
                  unsigned int rebin, bool norm, bool log, unsigned int opt);
TGraph GetROC(TFile *f, const char *hist1, const char *hist2, bool increasing);
TGraph GetEff(TFile *f, const char *hist1, bool increasing);

void plot1D(string name, float xval, double weight, std::map<string, TH1*> &allhistos, 
            string title, int numbinsx, float xmin, float xmax);
void plot1D(string name, float xval, double weight, std::map<string, TH1*> &allhistos, 
            string title, int numbinsx, const float * xbins);
void plot1D_cutflow(string name, int ibin, std::map<string, TH1*> &allhistos, string title, std::vector<std::string> bin_title_vec);
TH1D* getHist1D(string title, std::map<string, TH1*> &allhistos, 
            int numbinsx, float xmin, float xmax);
void insertHist1D(TH1D* hist, std::map<string, TH1*> &allhistos);
void plot2D(string name, float xval, float yval, double weight, std::map<string, TH1*> &allhistos, 
            string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax);
void plot2D(string name, float xval, float yval, double weight, std::map<string, TH1*> &allhistos, 
            string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins);
// Old plot2D, with TH2D map
void plot2D(string name, float xval, float yval, double weight, std::map<string, TH2D*> &allhistos, 
            string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax);
void plot2D(string name, float xval, float yval, double weight, std::map<string, TH2D*> &allhistos, 
            string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins);
void plot3D(string name, float xval, float yval, float zval, double weight, std::map<string, TH1*> &allhistos, 
            string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax,
                    int numbinsz, float zmin, float zmax);
void plot3D(string name, float xval, float yval, float zval, double weight, std::map<string, TH1*> &allhistos, 
            string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins,
                    int numbinsz, const float * zbins);

void plot1DUnderOverFlow(string title, double xval, double weight, std::map<string, TH1*> &allhistos, 
            int numbinsx, double xmin, double xmax);
void plot2DUnderOverFlow(string name, double xval, double yval, double weight, std::map<string, TH1*> &allhistos, 
            string title, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax);

void savePlots(std::map<string, TH1*>&, const char* );
void savePlots2(std::map<string, TH2D*>&, const char* );
void savePlotsDir(std::map<string, TH1*>& h_1d, TFile* outfile, const char* outdir = "");
void savePlots2Dir(std::map<string, TH2D*>& h_2d, TFile* outfile, const char* outdir = "");
void savePlots12(std::map<string, TH1D*>&, std::map<string, TH2D*>&, const char* );

void doAllPlots(int, std::string, std::map<string, TH1*> &, std::map<string, TH2D*> &, std::vector<particleJet>, std::vector<particleJet>, std::vector<particleJet>, std::vector<particleLepton>, double);
void doAllPlots(int, std::string, std::map<string, TH1*> &, std::map<string, TH2D*> &, std::vector<particleJet>, std::vector<particleJet>, std::vector<particleJet>, std::vector<particleJet>, std::vector<particleLepton>, double);

#endif

