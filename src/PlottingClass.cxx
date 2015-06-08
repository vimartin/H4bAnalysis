#include "ExoticHiggs/PlottingClass.h"

#include <sstream>

process::process()
{
}

void process::setDistribution(std::string fileName, std::string distribution)
{
  m_distribution = distribution;

  std::cout<<"process::setDistribution: Opening "<<fileName<<"..."<<std::endl;
  TFile *file = new TFile(fileName.c_str());
  m_histogram = (TH1F*) file->Get(m_distribution.c_str())->Clone();
  m_histogram->SetDirectory(0);
  file->Close();
}

void process::setLuminosity(float luminosity)
{
  m_luminosity = luminosity;
  m_histogram->Scale(m_luminosity);
}

void process::setIsSignal(bool isSignal)
{
  m_isSignal = isSignal;
}

void process::setProperties(float scale, int lineColor, int fillColor)
{
  m_histogram->Scale(scale);
  m_histogram->SetLineColor(lineColor);
  m_histogram->SetFillColor(fillColor);
}

TH1F* process::getHistogram()
{
  return m_histogram;
}







//--- Other functions

std::vector<std::string> convertStringToVector(std::string s)
{
  std::stringstream ss(s);
  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;
  std::vector<std::string> vstrings(begin, end);
  return vstrings;
}

//--- Define style
void plotStyle()
{
  TStyle *atlasStyle= new TStyle("ATLAS","Atlas style");

  atlasStyle->SetPalette(1);

  // use plain black on white colors
  Int_t icol=0;
  atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetCanvasColor(icol);
  atlasStyle->SetStatColor(icol);
  //atlasStyle->SetFillColor(icol);

  // set the paper & margin sizes
  atlasStyle->SetPaperSize(20,26);
  atlasStyle->SetPadTopMargin(0.05);
  atlasStyle->SetPadRightMargin(0.05);
  atlasStyle->SetPadBottomMargin(0.16);
  atlasStyle->SetPadLeftMargin(0.12);

  // use large fonts
  //Int_t font=72;
  Int_t font=42;
  Double_t tsize=0.05;
  atlasStyle->SetTextFont(font);


  atlasStyle->SetTextSize(tsize);
  atlasStyle->SetLabelFont(font,"x");
  atlasStyle->SetTitleFont(font,"x");
  atlasStyle->SetLabelFont(font,"y");
  atlasStyle->SetTitleFont(font,"y");
  atlasStyle->SetLabelFont(font,"z");
  atlasStyle->SetTitleFont(font,"z");

  atlasStyle->SetLabelSize(tsize,"x");
  atlasStyle->SetTitleSize(tsize,"x");
  atlasStyle->SetLabelSize(tsize,"y");
  atlasStyle->SetTitleSize(tsize,"y");
  atlasStyle->SetLabelSize(tsize,"z");
  atlasStyle->SetTitleSize(tsize,"z");


  //use bold lines and markers
  atlasStyle->SetMarkerStyle(20);
  atlasStyle->SetMarkerSize(1.2);
  atlasStyle->SetHistLineWidth(2.);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  //get rid of X error bars and y error bar caps
  //atlasStyle->SetErrorX(0.001);

  //do not display any of the standard histogram decorations
  atlasStyle->SetOptTitle(0);
  //atlasStyle->SetOptStat(1111);
  atlasStyle->SetOptStat(0);
  //atlasStyle->SetOptFit(1111);
  atlasStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  atlasStyle->SetPadTickX(1);
  atlasStyle->SetPadTickY(1);

  gROOT->SetStyle("Plain");
}
