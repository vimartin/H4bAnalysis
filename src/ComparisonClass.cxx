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

void comparisonClass::setSamplesAndHistoTitles(std::string histoXTitle, std::vector<std::string> sampleTitles)
{
  m_histoXTitle = histoXTitle;
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
    if (m_hsamples.at(i)->Integral()>0.){
      m_hsamples.at(i)->Scale(1./m_hsamples.at(i)->Integral());
    }
  }


  //--- Set Log scale, maximum and minimum for the plot
  float ymax = 0.;
  for (int ibin=1; ibin<m_hsamples.at(0)->GetSize()-1; ibin++){
    if (m_hsamples.at(0)->GetBinContent(ibin)>ymax) ymax = m_hsamples.at(0)->GetBinContent(ibin);
  }

  m_hsamples.at(0)->SetMaximum(ymax*1.2);
  m_hsamples.at(0)->SetMinimum(0.);

  if (m_doLogScale){
    m_hsamples.at(0)->SetMinimum(1e-5);
    gPad->SetLogy();
  }


  //--- Draw THStacks and histograms
  m_hsamples.at(0)->Draw("histo");
  for (unsigned int i=1; i<m_sampleNames.size(); ++i){
    m_hsamples.at(i)->Draw("histosame");
  }

  //--- Set cosmetics for THStack
  m_hsamples.at(0)->GetXaxis()->SetLabelSize(0.04);
  m_hsamples.at(0)->GetXaxis()->SetTitleSize(0.05);
  m_hsamples.at(0)->GetXaxis()->SetTitleOffset(0.9);
  m_hsamples.at(0)->GetXaxis()->SetTitle(m_histoXTitle.c_str());
  m_hsamples.at(0)->GetYaxis()->SetLabelSize(0.04);
  m_hsamples.at(0)->GetYaxis()->SetTitleSize(0.05);
  m_hsamples.at(0)->GetYaxis()->SetTitleOffset(0.9);
  m_hsamples.at(0)->GetYaxis()->SetTitle("Entries (a.u.)");

  gPad->Modified(); gPad->Update();


  //--- Legend
  m_legend = new TLegend(0.5, 0.7, 0.90, 0.90);
  m_legend->SetBorderSize(1);
  m_legend->SetLineColor(kWhite);
  for (unsigned int i=0; i<m_sampleNames.size(); i++){
    m_legend->AddEntry(m_sample_map[m_sampleNames.at(i).c_str()].getHistogram(), Form("%s", m_sampleTitles.at(i).c_str()), "l");
  }

  m_legend->Draw();

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
  }
}






////--- comparisonClass
//comparisonClass::comparisonClass(std::string distribution)
//{
//}
//
//comparisonClass::~comparisonClass()
//{}
//
//void comparisonClass::plotUpperPad(int a)
//{
//  m_pad1->cd();
//
//  //--- Define m_hbkg and m_hsig
//  for (auto bkg : m_bkgName){
//    if (!m_sample_map[bkg.c_str()].getHasHisto()) continue;
//    TH1F *htemp = (TH1F*) m_sample_map[bkg.c_str()].getHistogram()->Clone();
//    m_hbkg = new TH1F(Form("h_bkg_%s", m_distribution.c_str()), "", htemp->GetSize()-2, htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax());
//    m_hsig = new TH1F(Form("h_sig_%s", m_distribution.c_str()), "", htemp->GetSize()-2, htemp->GetXaxis()->GetXmin(), htemp->GetXaxis()->GetXmax());
//    htemp->Delete();
//    break;
//  }
//
//
//  //--- Fill the map and histo of total background and signal
//  float n_totalbkg = 0.;
//  float n_totalsig = 0.;
//
//  m_sbkg = new THStack(Form("sbkg_%s", m_distribution.c_str()), "");
//  m_ssig = new THStack(Form("ssig_%s", m_distribution.c_str()), "");
//
//
//
//  for (auto bkg : m_bkgName){
//    if (!m_sample_map[bkg.c_str()].getHasHisto()) continue;
//    m_sbkg->Add(m_sample_map[bkg.c_str()].getHistogram());
//    m_hbkg->Add(m_sample_map[bkg.c_str()].getHistogram());
//    n_totalbkg += m_sample_map[bkg.c_str()].getHistogram()->Integral();
//  }
//
//  for (auto sig : m_sigName){
//    if (!m_sample_map[sig.c_str()].getHasHisto()) continue;
//    m_ssig->Add(m_sample_map[sig.c_str()].getHistogram());
//    m_hsig->Add(m_sample_map[sig.c_str()].getHistogram());
//    n_totalsig += m_sample_map[sig.c_str()].getHistogram()->Integral();
//  }
//
//
//  //--- Set Log scale, maximum and minimum for the plot
//  float ymax = 0.;
//  for (int ibin=1; ibin<m_hbkg->GetSize()-1; ibin++){
//    if (m_hbkg->GetBinContent(ibin)>ymax) ymax = m_hbkg->GetBinContent(ibin);
//  }
//  for (int ibin=1; ibin<m_hsig->GetSize()-1; ibin++){
//    if (m_hsig->GetBinContent(ibin)>ymax) ymax = m_hsig->GetBinContent(ibin);
//  }
//
//  m_sbkg->SetMaximum(ymax*1.2);
//  m_sbkg->SetMinimum(0.);
//
//  if (m_doLogScale){
//    m_sbkg->SetMinimum(fmin(n_totalbkg, n_totalsig)/100.);
//    gPad->SetLogy();
//  }
//
//
//  //--- Draw THStacks and histograms
//  m_sbkg->Draw("histo");
//  m_ssig->Draw("histosame");
//
//  //--- Set cosmetics for THStack
//  m_sbkg->GetHistogram()->GetXaxis()->SetLabelSize(0.04);
//  m_sbkg->GetHistogram()->GetXaxis()->SetTitleSize(0.05);
//  m_sbkg->GetHistogram()->GetXaxis()->SetTitleOffset(0.9);
//  m_sbkg->GetHistogram()->GetXaxis()->SetTitle(m_histoXTitle.c_str());
//  m_sbkg->GetHistogram()->GetYaxis()->SetLabelSize(0.04);
//  m_sbkg->GetHistogram()->GetYaxis()->SetTitleSize(0.05);
//  m_sbkg->GetHistogram()->GetYaxis()->SetTitleOffset(0.9);
//  m_sbkg->GetHistogram()->GetYaxis()->SetTitle("Entries (a.u.)");
//
//  gPad->Modified(); gPad->Update();
//
//
//  //--- Legend
//  m_legend = new TLegend(0.5, 0.7, 0.90, 0.90);
//  m_legend->SetBorderSize(1);
//  m_legend->SetLineColor(kWhite);
//  for (auto bkg : m_bkgName){
//    m_legend->AddEntry(m_sample_map[bkg.c_str()].getHistogram(), Form("%s", m_sample1Title.c_str()), "l");
//  }
//
//  for (auto sig : m_sigName){
//    m_legend->AddEntry(m_sample_map[sig.c_str()].getHistogram(), Form("%s", m_sample2Title.c_str()), "l");
//  }
//  m_legend->Draw();
//
//  //--- ATLAS label
//  ATLASLabel(0.25,0.86,"", m_luminosity, 1);
//
//  m_canvas->Update();
//}
//
//void comparisonClass::setSamplesAndHistoTitles(std::string histoXTitle, std::string sample1Title, std::string sample2Title)
//{
//  m_histoXTitle = histoXTitle;
//  m_sample1Title = sample1Title;
//  m_sample2Title = sample2Title;
//}





