#include <string>
#include <iostream>
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TH1.h"
#include "TList.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TPaveStats.h"
#include "TLatex.h"

void combineAnodeTimingDistributions (std::string fname, float kLumi = 10.3, bool no_legend = false)
{
    TFile file(fname.c_str());
    TDirectoryFile *dir = (TDirectoryFile*)file.Get("recHits");

    TList *hlist = dir->GetListOfKeys();
    TH1F *h1;
    bool foundHist = false;
    for (auto hist : *hlist)    
    {
        TString name = hist->GetName();
        if (!name.Contains("hAnodeTiming")) continue;

        TH1F *h0 = (TH1F*)dir->Get(name.Data());        
        if (!foundHist)
        {
            h1 = (TH1F*)h0->Clone();
            h1->Sumw2();
            foundHist = true;
        }
        else
        {
            h1->Add(h0);
        }        
    }

    bool modified = false;
    do {
      double mean = h1->GetMean();
      double maxDiff = -1;
      int maxBin = -1;
      for (int i = 1; i <= h1->GetNbinsX(); ++i) {
        if (h1->GetBinContent(i) == 0) continue;
        double anodeTime = h1->GetBinCenter(i);
        double diff = fabs(anodeTime - mean);
        if (diff > maxDiff) {
          maxDiff = diff;
          maxBin = i;
        }
      }
      cout << __LINE__ << ": maxDiff= " << maxDiff << ", maxBin= " << maxBin << endl;
      if (maxDiff > 26) {
        h1->SetBinContent(maxBin, 0);
        modified = true;
      }
    } while (modified);

    TH1F *h2 = new TH1F("h2", "h2", 25, -31.25, 31.25);
    h2->GetXaxis()->SetTitle("anode time (ns)");
    h2->GetYaxis()->SetTitle("Fraction of Hits/ns");
    h2->GetYaxis()->SetTitleOffset(1.1);
    h2->SetTitle("CSC Anode Time");
    h2->SetTitleFont(42);
    h2->SetTitleSize(0.052);    
 
    h2->Sumw2();
    h2->SetLineColor(kCyan+3);
    h2->SetFillColor(kCyan+3);

    for (int ibin = 1; ibin <= 200; ibin++)
    {
        // h2->SetBinContent(ibin, h1->GetBinContent(ibin));
        // h2->SetBinError(ibin, h1->GetBinError(ibin));
        h2->Fill(h1->GetBinCenter(ibin), h1->GetBinContent(ibin));
        if (ibin == 200)
            h2->SetEntries(h1->GetEntries());
    }
    
    TCanvas c1("c1", "c1", 600, 400);
    gStyle->SetOptStat("");

    double rms = h2->GetRMS();
    double avg = h2->GetMean();
    if (avg < 0 && fabs(avg) < 0.05) avg = 0;

    TLatex cms(0.17, 0.83, "CMS");
    cms.SetNDC();
    cms.SetTextFont(61);
    cms.SetTextSize(0.06);

    TLatex prelim(0.17, 0.81, "Preliminary");
    prelim.SetNDC();
    prelim.SetTextAlign(13);
    prelim.SetTextFont(52);
    prelim.SetTextSize(0.0456);

    TLatex data(0.17, 0.76, "Data 2018");
    data.SetNDC();
    data.SetTextAlign(13);
    data.SetTextFont(52);
    data.SetTextSize(0.0456);

    TLatex lumi(0.9, 0.93, Form("%.1f fb^{-1} (13 TeV)", kLumi));
    lumi.SetNDC();
    lumi.SetTextAlign(31);
    lumi.SetTextFont(42);
    lumi.SetTextSize(0.052);    

    TLatex mean(0.66, 0.81, Form("Mean  %2.1f ns", avg));
    mean.SetNDC();
    mean.SetTextAlign(11);
    mean.SetTextFont(61);
    mean.SetTextSize(0.06);

    TLatex stdev(0.66, 0.76, Form("RMS   %s%2.1f ns", (avg<0)? " ":"", rms));
    stdev.SetNDC();
    stdev.SetTextAlign(11);
    stdev.SetTextFont(61);
    stdev.SetTextSize(0.06);

    h2->GetYaxis()->SetRangeUser(0, 1.2*h2->GetMaximum());
    h2->GetXaxis()->SetRangeUser(-6*rms,6*rms);   
    TH1F* h2norm = (TH1F*)h2->DrawNormalized("hist");
    // h2->Draw("hist");
    gPad->Update();
    if (!no_legend) {
      cms.Draw();
      prelim.Draw();
      data.Draw();
      lumi.Draw();
    }
    mean.Draw();
    stdev.Draw();
    
    TPaveText *title = (TPaveText*)gPad->GetPrimitive("title");
    title->SetBorderSize(0);
    title->SetFillColor(0);
    title->SetFillStyle(0);    
    title->SetX1NDC(0.13);
    title->SetY1NDC(0.88);
    title->SetX2NDC(0.9);
    title->SetY2NDC(0.98);
    title->SetTextFont(42);
    title->SetTextSize(0.052);    
    title->SetTextAlign(11);
    
    c1.Print("plots/anode_time_all.pdf");
    c1.Print("plots/anode_time_all.png");
    c1.Print("plots/anode_time_all.root");
}
