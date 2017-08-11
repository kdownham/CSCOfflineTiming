#include <string>
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TString.h"
#include <map>
#include "TList.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include "TLatex.h"
#include "TPaveText.h"
#include "TPad.h"

const int NENDCAPS = 2;
const int NSTATIONS = 4;
const int NRINGS[4] = {3, 2, 2, 2};

int GetBin (int e, int s, int r)
{
    int bin = 9*(e-1);
    for (unsigned int is = 0; is < s-1; is++)
        bin += NRINGS[is];

    bin += r;
    return bin;
}

int GetNumChambers (int s, int r)
{
    if (s != 1 && r == 1) return 18;
    return 36;
}

void makeSegmentMeanTimingPlot (std::string fname, bool byStation)
{    
    TFile file(fname.c_str());
    TDirectoryFile *dir;
    if (byStation)
        dir = (TDirectoryFile*)file.Get("Segments");
    else
        dir = (TDirectoryFile*)file.Get("SegmentsByChamber");
    
    TH1F *h1 = new TH1F("h1", "h1", 18, 0, 18.);
    h1->GetXaxis()->SetTitle("Ring");
    h1->GetYaxis()->SetTitle("Mean Segment Time (ns)");
    h1->SetTitle("Mean and RMS of Segment Time for each ring of CSC chambers");
    h1->GetYaxis()->SetRangeUser(-15,15);
    h1->SetMarkerStyle(4);
    h1->SetTitleFont(42);
    h1->SetTitleSize(0.052);    
    h1->GetXaxis()->SetTitleOffset(0.85);

    h1->Sumw2();
    h1->SetLineColor(kCyan+3);
    h1->SetFillColor(kCyan+3);
    h1->SetMarkerColor(kCyan+3);
    h1->SetLineWidth(2);
    
    std::map<std::string, TH1F*> mhist;    
    
    TList *hlist = dir->GetListOfKeys();
    for (auto hist : *hlist)
    {
        TString name = hist->GetName();
        if (!name.Contains("hNewSegTime")) continue;
        TObject *obj = dir->Get(name.Data());
        if (!obj->InheritsFrom(TH1::Class())) continue;

        if (byStation)
        {
            TString delim;
            int endcap, station, ring;
            if (name.Contains("+"))
            {
                delim = "+";
                endcap = 1;
            }
            else if (name.Contains("-"))
            {
                delim = "-";
                endcap = 2;
            }
            else continue;

            TObjArray *objarray = name.Tokenize(delim);
            if (objarray->GetEntries() != 2) continue;
            TString station_ring = ((TObjString*)objarray->Last())->GetString();
            if (station_ring.Sizeof() != 3) continue;
            TString s = station_ring[0];
            if (s.IsDigit()) station = s.Atoi();
            TString r = station_ring[1];
            if (r.IsDigit()) ring = r.Atoi();            

            double mean = ((TH1*)obj)->GetMean();
            double rms = ((TH1*)obj)->GetRMS();
            int bin = GetBin(endcap, station, ring);
            std::string label = "ME"+std::string(delim.Data())+std::string(s.Data())+"/"+std::string(r.Data());
            std::cout << "bin = " << bin << " from " << endcap << ", " << station << ", " << ring << std::endl;
            
            h1->SetBinContent(bin, mean);
            h1->SetBinError(bin, rms);
            h1->GetXaxis()->SetBinLabel(bin, label.c_str());
            
            objarray->Delete();            
        }
        else
        {
            TString delim = "_";
            int endcap, station, ring, chamber;
            if (name.Contains("+"))
                endcap = 1;
            else if (name.Contains("-"))
                endcap = 2;
            else continue;

            TObjArray *objarray = name.Tokenize(delim);
            if (objarray->GetEntries() != 4) continue;
            TString endcap_station = ((TObjString*)objarray->At(1))->GetString();
            if (endcap_station.Sizeof() != 3) continue;
            TString s = endcap_station[1];
            if (s.IsDigit()) station = s.Atoi();
            TString ring_ = ((TObjString*)objarray->At(2))->GetString();
            if (ring_.Sizeof() != 2) continue;
            if (ring_.IsDigit()) ring = ring_.Atoi();
            TString chamber_ = ((TObjString*)objarray->At(3))->GetString();
            if (chamber_.Sizeof() < 2 || chamber_.Sizeof() > 3) continue;
            if (chamber_.IsDigit()) chamber = chamber_.Atoi();

            double mean = ((TH1*)obj)->GetMean();
            double rms = ((TH1*)obj)->GetRMS();
            int bin = GetBin(endcap, station, ring);            
            std::string label = "ME"+std::string(endcap==1 ? "+" : "-")+std::string(s.Data())+"/"+std::string(ring_.Data());
            std::string slabel = "ME"+std::string(endcap==1 ? "+" : "-")+std::string(s.Data())+"_"+std::string(ring_.Data());            

            if (mhist.find(slabel) == mhist.end())
            {
                mhist[slabel] = new TH1F(slabel.c_str(), slabel.c_str(), GetNumChambers(station, ring), 0, GetNumChambers(station, ring));
                mhist[slabel]->GetXaxis()->SetTitle("Chamber");
                mhist[slabel]->GetYaxis()->SetTitle("Mean Segment Time (ns)");
                mhist[slabel]->SetTitle(Form("Mean and RMS of Segment Time for %s", label.c_str()));
                mhist[slabel]->SetMarkerStyle(4);
                mhist[slabel]->GetYaxis()->SetRangeUser(-15,15);
            }
                
            mhist[slabel]->SetBinContent(chamber, mean);
            mhist[slabel]->SetBinError(chamber, rms);
            mhist[slabel]->GetXaxis()->SetBinLabel(chamber, chamber_.Data());      
        }
    }

    TCanvas c1("c1", "c1", 600, 400);
    gStyle->SetOptStat("");

    TLatex cms(0.17, 0.83, "CMS");
    cms.SetNDC();
    cms.SetTextFont(61);
    cms.SetTextSize(0.06);

    TLatex prelim(0.17, 0.81, "Preliminary");
    prelim.SetNDC();
    prelim.SetTextAlign(13);
    prelim.SetTextFont(52);
    prelim.SetTextSize(0.0456);

    TLatex data(0.17, 0.76, "Data 2017");
    data.SetNDC();
    data.SetTextAlign(13);
    data.SetTextFont(52);
    data.SetTextSize(0.0456);

    TLatex lumi(0.85, 0.82, "2.89 fb^{-1} (13 TeV)");
    lumi.SetNDC();
    lumi.SetTextAlign(31);
    lumi.SetTextFont(42);
    lumi.SetTextSize(0.052);    
    
    if (byStation)
    {
        h1->Draw("e1x0");
        gPad->Update();
        cms.Draw();
        prelim.Draw();
        data.Draw();
        lumi.Draw();

        TPaveText *title = (TPaveText*)gPad->GetPrimitive("title");
        title->SetBorderSize(0);
        title->SetFillColor(0);
        title->SetFillStyle(0);    
        title->SetX1NDC(0.07);
        title->SetY1NDC(0.88);
        title->SetX2NDC(0.8);
        title->SetY2NDC(0.98);
        title->SetTextFont(42);
        title->SetTextSize(0.046);    
        title->SetTextAlign(11);
        
        // c1.Print("plots/mean_segtime.pdf");
        c1.Print("plots/mean_segtime.png");
        // c1.Print("plots/mean_segtime.root");
    }
    else
    {
        for (const auto& item : mhist)
        {
            item.second->Draw("e1x0");
            gPad->Update();
            cms.Draw();
            prelim.Draw();
            data.Draw();
            lumi.Draw();

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

            // c1.Print(Form("plots/mean_segtime_%s.pdf", item.first.c_str()));
            c1.Print(Form("plots/mean_segtime_%s.png", item.first.c_str()));
        }
    }
}
