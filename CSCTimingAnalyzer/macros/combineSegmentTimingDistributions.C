#include <string>
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TH1.h"
#include "TList.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"

void combineSegmentTimingDistributions(std::string fname, float kLumi = 0.0, bool no_legend = false)
{
    TFile file(fname.c_str());
    TDirectoryFile *dir = (TDirectoryFile*)file.Get("Segments");

    TList *hlist = dir->GetListOfKeys();
    TH1F *h1;
    bool foundHist = false;
    for (auto hist : *hlist)    
    {
        TString name = hist->GetName();
        if (!name.Contains("hNewSegTime")) continue;

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

    TH1F *h2 = new TH1F("h2", "h2", 200, -100, 100);
    h2->GetXaxis()->SetTitle("segment time (ns)");
    h2->GetYaxis()->SetTitle("Fraction of Segments/ns");
    h2->GetYaxis()->SetTitleOffset(1.1);
    h2->GetXaxis()->SetTitleOffset(0.8);        
    h2->SetTitle("CSC Segment Time");
    h2->SetTitleFont(42);
    h2->SetTitleSize(0.052);    

    h2->Sumw2();
    h2->SetLineColor(kCyan+3);
    h2->SetFillColor(kCyan+3);
    
    for (int ibin = 1; ibin <= 200; ibin++)
    {
        h2->SetBinContent(ibin, h1->GetBinContent(ibin));
        h2->SetBinError(ibin, h1->GetBinError(ibin));
        if (ibin == 200)
            h2->SetEntries(h1->GetEntries());
    }
    
    TCanvas c1("c1", "c1", 600, 400);
    gStyle->SetOptStat("");

    double rms = h2->GetRMS();
    double avg = h2->GetMean();
    
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

    double rmserr = h2->GetRMSError();
    double avgerr = h2->GetMeanError();
    cout << "Mean: " << avg << " +- " << avgerr << endl;
    cout << "RMS: " << rms << " +- " << rmserr << endl;

    h2->GetXaxis()->SetRangeUser(-6*rms,6*rms);   
    TH1F* h2norm = (TH1F*)h2->DrawNormalized("hist");
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

    c1.Print("plots/segment_time_all.pdf");
    c1.Print("plots/segment_time_all.png");
    c1.Print("plots/segment_time_all.root");    
}
