#include "ExoticHiggs/PlottingClass.h"
#include "ExoticHiggs/ComparisonClass.h"



// --- comparisonClass

comparisonClass::comparisonClass(std::string distribution)
{
  m_distribution = distribution;
  std::cout<<"comparisonClass::comparisonClass: Initializing distribution "<<m_distribution<<std::endl;
}

comparisonClass::~comparisonClass()
{
  for (unsigned int i=0; i<m_hsamples.size(); ++i){
    m_hsamples.at(i)->Delete();
  }
}

void comparisonClass::setGlobalProperties(float luminosity, bool doLogScale, bool savePlot)
{
  m_luminosity = luminosity;
  m_doLogScale = doLogScale;
  m_savePlot = savePlot;
}

void comparisonClass::setSampleNames(std::vector<std::string> sampleNames)
{
  m_sampleNames = sampleNames;
  for (auto sam : m_sampleNames){
    m_sample_map[sam] = process(); 
  }
}

void comparisonClass::setSampleProperties(std::map<std::string, int> index_map, std::map<std::string,std::string> address_map, std::map<std::string,bool> isSignal_map, std::map<std::string,float> scale_map, std::map<std::string,int> lineColor, std::map<std::string,int> fillColor)
{
  m_index_map   =  index_map;
  m_address_map =  address_map;
  m_isSignal_map = isSignal_map;
  m_scale_map =    scale_map;
  m_lineColor =    lineColor;
  m_fillColor =    fillColor;
}

void comparisonClass::setSamplesAndHistoTitles(std::string histoXTitle, std::string histoYTitle, std::vector<std::string> sampleTitles)
{
  m_histoXTitle = histoXTitle;
  m_histoYTitle = histoYTitle;
  m_sampleTitles = sampleTitles;
}

void comparisonClass::read()
{
  for (auto sam : m_sampleNames){
    std::cout<<"comparisonClass::read : reading "<<sam<<"..."<<std::endl;
    m_sample_map[sam].setDistribution(m_address_map[sam], m_distribution);
    m_sample_map[sam].setLuminosity(m_luminosity);
    m_sample_map[sam].setIsSignal(m_isSignal_map[sam]);
    m_sample_map[sam].setProperties(m_scale_map[sam], m_lineColor[sam], m_fillColor[sam]);
  }
}

void comparisonClass::prepareCanvas()
{
  std::cout<<"comparisonClass::prepareCanvas : preparing canvas..."<<std::endl;
  m_canvas = new TCanvas(m_distribution.c_str(), m_distribution.c_str(), 700, 500); 

  m_pad1 = new TPad(Form("pad1_%s", m_distribution.c_str()), Form("pad1_%s", m_distribution.c_str()), 0, 0., 1., 1);
  m_pad1->Draw("same");
}

void comparisonClass::plotUpperPad()
{
  std::cout<<"comparisonClass::plotUpperPad : plotting..."<<std::endl;
  m_pad1->cd();

  //--- Define m_hsamples
  int counter = 0;
  for (auto sam : m_sampleNames){
    if (!m_sample_map[sam.c_str()].getHasHisto()) continue;
    TH1F *htemp = (TH1F*) m_sample_map[sam.c_str()].getHistogram()->Clone();
    m_hsamples.push_back( new TH1F(Form("h_sam%d_%s", counter, m_distribution.c_str()), "", htemp->GetSize()-2, htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax()) );
    counter ++;
    htemp->Delete();
  }


  //--- Fill the map and histo of total background and signal

  for (unsigned int i=0; i<m_sampleNames.size(); ++i){
    if (!m_sample_map[m_sampleNames.at(i).c_str()].getHasHisto()) continue;
    m_hsamples.at(i)->Add(m_sample_map[m_sampleNames.at(i).c_str()].getHistogram());
    m_hsamples.at(i)->SetFillColor(m_fillColor[m_sampleNames.at(i)]);
    m_hsamples.at(i)->SetLineColor(m_lineColor[m_sampleNames.at(i)]);
    m_hsamples.at(i)->SetMarkerColor(m_lineColor[m_sampleNames.at(i)]);
//    if (m_hsamples.at(i)->Integral()>0.){
//      m_hsamples.at(i)->Scale(1./m_hsamples.at(i)->Integral());
////      m_hsamples.at(i)->Scale(1./9.760000e+03);
//    }
  }


  //--- Set Log scale, maximum and minimum for the plot
  float ymax = 0.;
  for (unsigned int i=0; i<m_sampleNames.size(); ++i){
    for (int ibin=1; ibin<m_hsamples.at(i)->GetSize()-1; ibin++){
      if (m_hsamples.at(i)->GetBinContent(ibin)>ymax) ymax = m_hsamples.at(i)->GetBinContent(ibin);
    }
  }

  m_hsamples.at(0)->SetMaximum(ymax*1.4);
  m_hsamples.at(0)->SetMinimum(0.);

  if (m_doLogScale){
    m_hsamples.at(0)->SetMinimum(1e-5);
    gPad->SetLogy();
  }


  //--- Draw THStacks and histograms
  m_hsamples.at(0)->Draw("e");
  for (unsigned int i=1; i<m_sampleNames.size(); ++i){
    m_hsamples.at(i)->Draw("esame");
  }
//  m_hsamples.at(0)->Draw("histo");
//  for (unsigned int i=1; i<m_sampleNames.size(); ++i){
//    m_hsamples.at(i)->Draw("histosame");
//  }

  //--- Set cosmetics for THStack
  m_hsamples.at(0)->GetXaxis()->SetLabelSize(0.04);
  m_hsamples.at(0)->GetXaxis()->SetTitleSize(0.05);
  m_hsamples.at(0)->GetXaxis()->SetTitleOffset(0.9);
  m_hsamples.at(0)->GetXaxis()->SetTitle(m_histoXTitle.c_str());
  m_hsamples.at(0)->GetYaxis()->SetLabelSize(0.04);
  m_hsamples.at(0)->GetYaxis()->SetTitleSize(0.05);
  m_hsamples.at(0)->GetYaxis()->SetTitleOffset(0.9);
  m_hsamples.at(0)->GetYaxis()->SetTitle(m_histoYTitle.c_str());

  gPad->Modified(); gPad->Update();


  //--- Legend
  m_legend = new TLegend(0.5, 0.6, 0.90, 0.90);
  m_legend->SetBorderSize(1);
  m_legend->SetLineColor(kWhite);
  for (unsigned int i=0; i<m_sampleNames.size(); i++){
    m_legend->AddEntry(m_sample_map[m_sampleNames.at(i).c_str()].getHistogram(), Form("%s", m_sampleTitles.at(i).c_str()), "l");
  }

  m_legend->Draw();

  TLatex *l = new TLatex();
  l->SetNDC();
  l->SetTextFont(72);
  l->SetTextSize(0.045);
//  l->DrawLatex(0.41,0.5,"WH, H#rightarrow aa, a#rightarrow b#bar{b}, m_{a} = 20 GeV");
  l->DrawLatex(0.41,0.5,"ggF, H#rightarrow aa, a#rightarrow #mu^{+}#mu^{-}");

  //--- ATLAS label
  ATLASLabel(0.25,0.86,"", m_luminosity, 1);

  m_canvas->Update();

}


void comparisonClass::finalize()
{
  m_canvas->Update();
  if (m_savePlot){
    m_canvas->Print(Form("plots/%s.gif", m_distribution.c_str()));
    m_canvas->Print(Form("plots/%s.eps", m_distribution.c_str()));
    m_canvas->Print(Form("plots/%s.pdf", m_distribution.c_str()));
  }
}
