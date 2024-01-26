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

void makeAnodeTimingPlot (std::string fname, bool byStation, std::string outputdir, float kLumi = 0.0, bool no_legend = false)
{    
    TFile file(fname.c_str());
    TDirectoryFile *dir;
    if (byStation)
        dir = (TDirectoryFile*)file.Get("recHits");
    else
        dir = (TDirectoryFile*)file.Get("recHitsByChamber");
    
    TH1F *h1 = new TH1F("h1", "h1", 18, 0, 18.);
    h1->GetXaxis()->SetTitle("Ring");
    h1->GetYaxis()->SetTitle("mean anode hit time (ns)");
    h1->SetTitle("Mean and RMS of anode hit time for each ring of CSC chambers");
    h1->SetTitleFont(42);
    h1->SetTitleSize(0.052);    
    //h1->GetYaxis()->SetRangeUser(-25,25);
    h1->GetYaxis()->SetRangeUser(-300,300);
    h1->SetMarkerStyle(4);
    h1->GetXaxis()->SetTitleOffset(0.85);
    
    h1->Sumw2();
    h1->SetLineColor(kCyan+3);
    h1->SetFillColor(kCyan+3);
    h1->SetMarkerColor(kCyan+3);
    h1->SetLineWidth(2);
    
    std::map<std::string, TH1F*> mhist;    
    
    float means[5][4];
    float mean_ME1X(0);
    float mean_MEX2(0);
    float mean_MEX1(0);

    TList *hlist = dir->GetListOfKeys();
    for (auto hist : *hlist)
    {
        TString name = hist->GetName();
        if (!name.Contains("hAnodeTiming")) continue;
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

            auto h0 = (TH1*) obj;
            double mean = h0->GetMean();
            double rms  = h0->GetRMS();
            bool modified = false;
            do {
              double maxDiff = -1;
              int maxBin = -1;
              for (int i = 1; i <= h0->GetNbinsX(); ++i) {
                if (h0->GetBinContent(i) == 0) continue;
                double anodeTime = h0->GetBinCenter(i);
                double diff = fabs(anodeTime - mean);
                if (diff > maxDiff) {
                  maxDiff = diff;
                  maxBin = i;
                }
              }
              if (maxDiff > 26) {
                h0->SetBinContent(maxBin, 0);
                mean = h0->GetMean();
                rms  = h0->GetRMS();
                modified = true;
              }
            } while (modified);

            int bin = GetBin(endcap, station, ring);
            std::string label = "ME"+std::string(delim.Data())+std::string(s.Data())+"/"+std::string(r.Data());
            std::cout << "bin " << std::setw(2) << bin << " for " << label << " has mean: " << mean <<  std::endl;
            
            h1->SetBinContent(bin, mean);
            h1->SetBinError(bin, rms);
            h1->GetXaxis()->SetBinLabel(bin, label.c_str());
            
            if (station == 1) mean_ME1X += mean;
            else if (ring == 1) mean_MEX1 += mean;
            else if (ring == 2) mean_MEX2 += mean;
            else std::cout << "Unidentified ring?!!" << endl;

            if (endcap == 1) means[station][ring] = mean;
            else if (endcap == 2) means[station][ring] += mean;

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
                mhist[slabel]->GetYaxis()->SetTitle("Mean Anode Time (ns)");
                mhist[slabel]->SetTitle(Form("Mean and RMS of Anode Time for %s", label.c_str()));
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

    TLatex data(0.17, 0.76, "Data 2022");
    data.SetNDC();
    data.SetTextAlign(13);
    data.SetTextFont(52);
    data.SetTextSize(0.0456);

    //TLatex lumi(0.85, 0.82, Form("%.1f fb^{-1} (13 TeV)", kLumi));
    TLatex lumi(0.9, 0.93, Form("%.1i (13.6TeV)", 2022));
    lumi.SetNDC();
    lumi.SetTextAlign(31);
    lumi.SetTextFont(42);
    lumi.SetTextSize(0.052);    
        
    mean_ME1X /= 6;
    mean_MEX1 /= 6;
    mean_MEX2 /= 6;

    if (byStation)
    {
        h1->Draw("e1x0");

        TF1 line0("line0", "0", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
        line0.SetLineColor(13);
        line0.SetLineStyle(5);
        line0.Draw("same");

        // TF1 line1x("line1x", "-2", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
        // line1x.SetLineColor(13);
        // line1x.SetLineStyle(2);
        // line1x.Draw("same");

        // TF1 linex1("linex1", "-1.25", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
        // linex1.SetLineColor(13);
        // linex1.SetLineStyle(2);
        // linex1.Draw("same");

        // TF1 linex2("linex2", "-1.75", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
        // linex2.SetLineColor(13);
        // linex2.SetLineStyle(2);
        // linex2.Draw("same");

        h1->Draw("e1x0same");

        gPad->Update();
        if (!no_legend) {
          cms.Draw();
          prelim.Draw();
          data.Draw();
          lumi.Draw();
        }

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

        //gSystem->Exec(Form("mkdir -p plots/all_plots/Run357900/%s",outputdir.c_str()));
        //
        //c1.Print(Form("plots/all_plots/Run357900/%s/mean_anodetime_Run357900_byStation.pdf",outputdir.c_str()));
        //c1.Print(Form("plots/all_plots/Run357900/%s/mean_anodetime_Run357900_byStation.png",outputdir.c_str()));
        //c1.Print(Form("plots/all_plots/Run357900/%s/mean_anodetime_Run357900_byStation.root",outputdir.c_str()));

	TCanvas c2("c2", "c2", 600, 400);
	//TH1F *h2 = new TH1F("h2","h2",40,-10.,10.);
	TH1F *h2 = new TH1F("h2","h2",40,-300.,300.);
	h2->GetXaxis()->SetTitle("Mean anode hit time per ring [ns]");
        h2->GetYaxis()->SetTitle("Number of rings");
	h2->SetTitle("Mean anode time per ring");
	
	int bins = (h1->GetNbinsX()+1);

        for (int k = 1; k < bins; k++){
	     float value = h1->GetBinContent(k);
	     cout << "value for bin " << k << " = " << value << endl;
	     h2->Fill(value);
	}

        cout << "mean_ME1X = " << mean_ME1X << endl;
        cout << "mean_MEX1 = " << mean_MEX1 << endl;
        cout << "mean_MEX2 = " << mean_MEX2 << endl;

        for (int i : {1,2,3,4}) {
          for (int j : {1,2}){
            cout << "Avg mean for ME " << i << "/" << j << " is: " << means[i][j] / 2 << ", const to be shifted by: " << means[i][j] * 2 << endl;
	    //h2->Fill(means[i][j]);
	  }
          if (i == 1) cout << "Avg mean for ME " << 1 << "/" << 3 << " is: " << means[1][3] / 2 << ", const to be shifted by: " << means[1][3] * 2 << endl;
        }

	double mean_h2 = h2->GetMean();
	double rms_h2 = h2->GetRMS();

        auto legend = new TLegend(0.60,0.60,0.90,0.90);
	legend->AddEntry((TObject*)0, TString::Format("Mean = %g", mean_h2), "");
	legend->AddEntry((TObject*)0, TString::Format("RMS = %g", rms_h2), "");

	c2.cd();
	h2->Draw("hist same");
	legend->Draw();

        //c2.Print(Form("plots/all_plots/Run359812/%s/mean_anodetimes_allstations_Run359812.pdf",outputdir.c_str()));
	//c2.Print(Form("plots/all_plots/Run359812/%s/mean_anodetimes_allstations_Run359812.png",outputdir.c_str()));
	//c2.Print(Form("plots/all_plots/Run359812/%s/mean_anodetimes_allstations_Run359812.root",outputdir.c_str()));
	c2.Print("mean_anodetimes_allstations.root");

    }
    else
    {
        for (const auto& item : mhist)
        {
            item.second->Draw("e1x0");
            gPad->Update();
            if (!no_legend) {
              cms.Draw();
              prelim.Draw();
              data.Draw();
              lumi.Draw();
            }
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
            
            //c1.Print(Form("plots/all_plots/Run357900_testAnodes/mean_anodetime_Run357900_%s.pdf", item.first.c_str()));
            //c1.Print(Form("plots/all_plots/Run357900_testAnodes/mean_anodetime_Run357900_%s.png", item.first.c_str()));
            c1.Print(Form("mean_anodetime_%s.root", item.first.c_str()));
        }
    }
}
