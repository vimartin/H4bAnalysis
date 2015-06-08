#include <vector>
#include <utility>
#include <iostream>

#include "TROOT.h"
#include "TStyle.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1F.h"

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


  private:
    TH1F *m_histogram;
    bool m_isSignal;
    float m_luminosity;
    std::string m_distribution;
};

std::vector<std::string> convertStringToVector(std::string);
void plotStyle();

#endif
