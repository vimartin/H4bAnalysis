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
  file->Close();
}

void process::setLuminosity(float luminosity)
{
  m_luminosity = luminosity;
}

void process::setIsSignal(bool isSignal)
{
  m_isSignal = isSignal;
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
