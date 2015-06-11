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

#ifndef PLOTTINGCLASS_H
#define PLOTTINGCLASS_H

class process {

  public:

    process();
    void setDistribution(std::string fileName, std::string distribution);
    void setLuminosity(float luminosity);
    void setIsSignal(bool isSignal);
    void setProperties(float scale, int lineColor, int fillColor);

    TH1F* getHistogram();
    bool getHasHisto();


  private:
    bool m_histoExists;
    TH1F *m_histogram;
    bool m_isSignal;
    float m_luminosity;
    std::string m_distribution;
};



class plotClass {

  public:
    plotClass(std::string);
    ~plotClass();
    void setSampleNames(std::vector<std::string>, std::vector<std::string>);
    void setGlobalProperties(float luminosity, bool doLogScale, bool savePlot);
    void setSampleProperties(std::map<std::string, int> index_map, std::map<std::string,std::string> address_map, std::map<std::string,bool> isSignal_map, std::map<std::string,float> scale_map, std::map<std::string,int> lineColor, std::map<std::string,int> fillColor);
    void read();
    void prepareCanvas();
    void plotUpperPad();
    void plotLowerPad();
    void plotSidePad();
    void finalize();

  private:
    std::string m_distribution;

    std::map<std::string, process> m_sample_map;
    std::vector<std::string> m_bkgName;
    std::vector<std::string> m_sigName;

    TCanvas *m_canvas;
    TPad *m_pad1;
    TPad *m_pad2;
    TPad *m_pad3;

    THStack *m_sbkg;
    THStack *m_ssig;
    TH1F *m_hbkg;
    TH1F *m_hsig;
    TH1F *m_hratio;
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
};



std::vector<std::string> convertStringToVector(std::string);
void plotStyle();

#endif
