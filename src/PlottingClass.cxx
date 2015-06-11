#include "ExoticHiggs/PlottingClass.h"

#include <sstream>

//--- process class

process::process()
{
}

void process::setDistribution(std::string fileName, std::string distribution)
{
  m_distribution = distribution;

  std::cout<<"process::setDistribution: Opening "<<fileName<<"..."<<std::endl;
  TFile *file = new TFile(fileName.c_str());
  if (file->GetListOfKeys()->Contains(m_distribution.c_str())){
    m_histoExists = true;
    m_histogram = (TH1F*) file->Get(m_distribution.c_str())->Clone();
    m_histogram->SetDirectory(0);
  }
  else{
    m_histoExists = false;
    m_histogram = new TH1F(m_distribution.c_str(), m_distribution.c_str(), 1, 0, 1);
    m_histogram->SetDirectory(0);
  }
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

bool process::getHasHisto()
{
  return m_histoExists;
}



// --- plotClass

plotClass::plotClass(std::string distribution)
{
  m_distribution = distribution;
  std::cout<<"plotClass::plotClass: Initializing distribution "<<m_distribution<<std::endl;
}

plotClass::~plotClass()
{
  m_sbkg->Delete();
  m_ssig->Delete();
  m_hbkg->Delete();
  m_hsig->Delete();
  m_hratio->Delete();

}

void plotClass::setGlobalProperties(float luminosity, bool doLogScale, bool savePlot)
{
  m_luminosity = luminosity;
  m_doLogScale = doLogScale;
  m_savePlot = savePlot;
}

void plotClass::setSampleNames(std::vector<std::string> bkgName, std::vector<std::string> sigName)
{
  m_bkgName = bkgName;
  m_sigName= sigName;

  for (auto bkg : m_bkgName){
    m_sample_map[bkg] = process(); 
  }

  for (auto sig : m_sigName){
    m_sample_map[sig] = process(); 
  }
}

void plotClass::setSampleProperties(std::map<std::string, int> index_map, std::map<std::string,std::string> address_map, std::map<std::string,bool> isSignal_map, std::map<std::string,float> scale_map, std::map<std::string,int> lineColor, std::map<std::string,int> fillColor)
{
  m_index_map   =  index_map;
  m_address_map =  address_map;
  m_isSignal_map = isSignal_map;
  m_scale_map =    scale_map;
  m_lineColor =    lineColor;
  m_fillColor =    fillColor;
}

void plotClass::read()
{
  for (auto bkg : m_bkgName){
    m_sample_map[bkg].setDistribution(m_address_map[bkg], m_distribution);
    m_sample_map[bkg].setLuminosity(m_luminosity);
    m_sample_map[bkg].setIsSignal(m_isSignal_map[bkg]);
    m_sample_map[bkg].setProperties(m_scale_map[bkg], m_lineColor[bkg], m_fillColor[bkg]);
  }
  for (auto sig : m_sigName){
    m_sample_map[sig].setDistribution(m_address_map[sig], m_distribution);
    m_sample_map[sig].setLuminosity(m_luminosity);
    m_sample_map[sig].setIsSignal(m_isSignal_map[sig]);
    m_sample_map[sig].setProperties(m_scale_map[sig], m_lineColor[sig], m_fillColor[sig]);
  }

}

void plotClass::prepareCanvas()
{
  m_canvas = new TCanvas(m_distribution.c_str(), m_distribution.c_str(), 900, 600); 

  m_pad1 = new TPad(Form("pad1_%s", m_distribution.c_str()), Form("pad1_%s", m_distribution.c_str()), 0, 0.305, .75, 1);
  m_pad1->SetBottomMargin(0);
  m_pad1->Draw("same");
  
  m_pad2 = new TPad(Form("pad2_%s", m_distribution.c_str()), Form("pad2_%s", m_distribution.c_str()), 0, 0.01, .75, 0.295);
  m_pad2->SetTopMargin(0);
  m_pad2->SetBottomMargin(0.3);
  m_pad2->SetGrid(0,1);
  m_pad2->Draw("same");

  m_pad3 = new TPad(Form("pad3_%s", m_distribution.c_str()), Form("pad3_%s", m_distribution.c_str()), 0.76, 0.01, .99, 0.99);
  m_pad3->Draw("same");
}

void plotClass::plotUpperPad()
{
  m_pad1->cd();

  //--- Define m_hbkg and m_hsig
  for (auto bkg : m_bkgName){
    if (!m_sample_map[bkg.c_str()].getHasHisto()) continue;
    TH1F *htemp = (TH1F*) m_sample_map[bkg.c_str()].getHistogram()->Clone();
    m_hbkg = new TH1F(Form("h_bkg_%s", m_distribution.c_str()), "", htemp->GetSize()-2, htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax());
    m_hsig = new TH1F(Form("h_sig_%s", m_distribution.c_str()), "", htemp->GetSize()-2, htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax());
    htemp->Delete();
    break;
  }

  //--- Fill the map and histo of total background and signal
  float n_totalbkg = 0.;
  float n_totalsig = 0.;

  m_sbkg = new THStack(Form("sbkg_%s", m_distribution.c_str()), "");
  m_ssig = new THStack(Form("ssig_%s", m_distribution.c_str()), "");



  for (auto bkg : m_bkgName){
    if (!m_sample_map[bkg.c_str()].getHasHisto()) continue;
    m_sbkg->Add(m_sample_map[bkg.c_str()].getHistogram());
    m_hbkg->Add(m_sample_map[bkg.c_str()].getHistogram());
    n_totalbkg += m_sample_map[bkg.c_str()].getHistogram()->Integral();
  }

  for (auto sig : m_sigName){
    if (!m_sample_map[sig.c_str()].getHasHisto()) continue;
    m_ssig->Add(m_sample_map[sig.c_str()].getHistogram());
    m_hsig->Add(m_sample_map[sig.c_str()].getHistogram());
    n_totalsig += m_sample_map[sig.c_str()].getHistogram()->Integral();
  }

  //--- Set Log scale, maximum and minimum for the plot
  float ymax = 0.;
  for (int ibin=1; ibin<m_hbkg->GetSize()-1; ibin++){
    if (m_hbkg->GetBinContent(ibin)>ymax) ymax = m_hbkg->GetBinContent(ibin);
  }
  for (int ibin=1; ibin<m_hsig->GetSize()-1; ibin++){
    if (m_hsig->GetBinContent(ibin)>ymax) ymax = m_hsig->GetBinContent(ibin);
  }
  m_sbkg->SetMaximum(ymax*1.2);
  m_sbkg->SetMinimum(0.);
  if (m_doLogScale){
    m_sbkg->SetMinimum(fmin(n_totalbkg, n_totalsig)/100.);
    gPad->SetLogy();
  }

  //--- Draw THStacks and histograms
  m_sbkg->Draw("histo");
  m_ssig->Draw("histosame");

  //--- Set cosmetics for THStack
  m_sbkg->GetHistogram()->GetYaxis()->SetLabelSize(0.04);
  m_sbkg->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
  m_sbkg->GetHistogram()->GetYaxis()->SetTitleOffset(0.7);
  m_sbkg->GetHistogram()->SetYTitle("Events");
}

void plotClass::plotLowerPad()
{
  m_pad2->cd();

  TH1F *hbkg_lower = (TH1F*) m_hbkg->Clone();
  TH1F *hsig_lower = (TH1F*) m_hsig->Clone();

  if (hbkg_lower->Integral()>0.) hbkg_lower->Scale(1./hbkg_lower->Integral());
  if (hsig_lower->Integral()>0.) hsig_lower->Scale(1./hsig_lower->Integral());

  m_hratio = (TH1F*) hsig_lower->Clone();
  m_hratio->Divide(hbkg_lower);

  //--- Set cosmetics for ratio plot
  m_hratio->GetXaxis()->SetTitleSize(0.145);
  m_hratio->GetXaxis()->SetTitleOffset(0.93);
  m_hratio->GetXaxis()->SetLabelSize(0.1);
  m_hratio->GetXaxis()->SetTitle(m_distribution.c_str());

  m_hratio->GetYaxis()->SetNdivisions(3, 5, 0, true);
  m_hratio->GetYaxis()->SetTitle("S/B (norm.)");
  m_hratio->GetYaxis()->SetTitleSize(0.15);
  m_hratio->GetYaxis()->SetTitleOffset(0.24);
  m_hratio->GetYaxis()->SetLabelSize(0.1);
  m_hratio->SetMaximum(1.7);
  m_hratio->SetMinimum(0.5);

  //--- Draw ratio plot
  m_hratio->Draw();
}

void plotClass::plotSidePad()
{
  m_pad3->cd();

  m_legend = new TLegend(0., 0.1, 1., 1.);
  m_legend->SetTextSize(0.045);
  m_legend->SetBorderSize(1);
  m_legend->SetLineColor(kWhite);

  for (auto bkg : m_bkgName){
    m_legend->AddEntry(m_sample_map[bkg.c_str()].getHistogram(), Form("%s: %.1f", bkg.c_str(), m_sample_map[bkg.c_str()].getHistogram()->Integral()), "lf");
  }

  for (auto sig : m_sigName){
    m_legend->AddEntry(m_sample_map[sig.c_str()].getHistogram(), Form("%s: %.1f", sig.c_str(), m_sample_map[sig.c_str()].getHistogram()->Integral()), "l");
  }

  m_legend->Draw();

}

void plotClass::finalize()
{
  m_canvas->Update();
  if (m_savePlot){
    m_canvas->Print(Form("plots/%s.gif", m_distribution.c_str()));
    m_canvas->Print(Form("plots/%s.eps", m_distribution.c_str()));
  }
}

void plotClass::printSummary()
{
  std::cout<<"\n\n"<<std::endl;
  std::cout<<"=== S U M A R Y ==="<<std::endl;
  std::cout<<"Total Background : "<<m_hbkg->Integral()<<std::endl;
  std::cout<<"Signal           : "<<m_hsig->Integral()<<std::endl;
  std::cout<<"S / B            : "<<m_hsig->Integral()/m_hbkg->Integral()<<std::endl;
  std::cout<<"S / sqrt(B)      : "<<m_hsig->Integral()/sqrt(m_hbkg->Integral())<<std::endl;
  std::cout<<"==================="<<std::endl;
  std::cout<<"\n\n"<<std::endl;
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
