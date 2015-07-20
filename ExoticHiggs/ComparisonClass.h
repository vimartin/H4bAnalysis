#include <vector>
#include <map>
#include <utility>
#include <iostream>

#include "TCanvas.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLatex.h"

#ifndef COMPARISONCLASS_H
#define COMPARISONCLASS_H


class comparisonClass {

  public:
    comparisonClass(std::string);
    ~comparisonClass();
    void setSampleNames(std::vector<std::string>); //--- vector with the strings of all the samples to compare
    void setGlobalProperties(float luminosity, bool doLogScale, bool savePlot);
    void setSampleProperties(std::map<std::string, int> index_map, std::map<std::string,std::string> address_map, std::map<std::string,bool> isSignal_map, std::map<std::string,float> scale_map, std::map<std::string,int> lineColor, std::map<std::string,int> fillColor);
    void setSamplesAndHistoTitles(std::string histoXTitle, std::string histoYTitle, std::vector<std::string> sampleTitles);
    void read();
    void prepareCanvas();
    void plotUpperPad();
    void finalize();
    void printSummary();

  protected:
    std::string m_distribution;

    std::map<std::string, process> m_sample_map;
    std::vector<std::string> m_sampleNames;

    TCanvas *m_canvas;
    TPad *m_pad1;

    std::vector< TH1F* > m_hsamples;
    TLegend *m_legend;

    float m_luminosity;
    bool m_doLogScale;
    bool m_savePlot;
    std::map<std::string, int>         m_index_map;
    std::map<std::string, std::string> m_address_map;
    std::map<std::string, bool>        m_isSignal_map;
    std::map<std::string, float>       m_scale_map;
    std::map<std::string, int>         m_lineColor;
    std::map<std::string, int>         m_fillColor;

    std::string m_histoXTitle;
    std::string m_histoYTitle;
    std::vector<std::string> m_sampleTitles;
};

#endif
