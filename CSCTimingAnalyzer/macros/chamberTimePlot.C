#include <iostream>
#include <string>
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TCollection.h"
#include "TObject.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"

void chamberTimePlot (std::string fname) {
  std::string fname_short = fname;
  fname_short.erase(fname_short.end()-5,fname_short.end());
  if (fname_short.find("/") != string::npos) fname_short.erase(fname_short.begin(), fname_short.begin()+fname_short.find("hist"));
  TCanvas c1("c1","c1",600,400);
  TCanvas c2("c2","c2",600,400);
  gStyle->SetOptStat("emrou");

  std::string recHits = "recHitsByChamber";
  std::string no_hist_p_1 = "hAnodeTiming_-";
  std::string no_hist_p_2 = "hRHTiming_";
  std::string no_hist_n_1 = "hAnodeTiming_+";

  TH1D *h_p = new TH1D("Chamber Anode Time Mean (Positive Endcap)","Chamber Anode Time Mean (Positive Endcap)",270,1.,270.);
  TH1D *h_n = new TH1D("Chamber Anode Time Mean (Negative Endcap)","Chamber Anode Time Mean (Negative Endcap)",270,1.,270.);

  int i = 1;
  int j = 1;

  TFile file(fname.c_str());
  file.cd();
  TList *flist = file.GetListOfKeys();
  for (auto item : *flist) {
    TObject *obj = file.Get(item->GetName());
    if (obj->InheritsFrom(TDirectory::Class())) {
      TDirectory *dir = (TDirectory*)obj;
      TList *dlist = dir->GetListOfKeys();
      bool is_first = true;

      //cout << "item = " << item->GetName() << endl;
      //cout << "fname_short = " << fname_short.c_str() << endl;
      

      //if ( item->GetName() == recHits || item->GetName() == Segments ){
	//   continue;
      //}
      if ( item->GetName() != recHits ) continue;


      for (auto ditem : *dlist) {
        TObject *dobj = dir->Get(ditem->GetName());
        if (dobj->InheritsFrom(TH1::Class())) {
	  std::string name = dobj->GetName();
	  if (name.find(no_hist_p_1) != string::npos) continue;
	  if (name.find(no_hist_p_2) != string::npos) continue;
	  double avg = ((TH1D*)dobj)->GetMean();
	  double rms = ((TH1D*)dobj)->GetRMS();
	  h_p->SetBinContent(i,avg);
	  i++;
        }
      }

      for (auto ditem : *dlist) {
        TObject *dobj = dir->Get(ditem->GetName());
        if (dobj->InheritsFrom(TH1::Class())) {
          std::string name = dobj->GetName();
          if (name.find(no_hist_n_1) != string::npos) continue;
          if (name.find(no_hist_p_2) != string::npos) continue;
          double avg = ((TH1D*)dobj)->GetMean();
          double rms = ((TH1D*)dobj)->GetRMS();
          h_n->SetBinContent(j,avg);
          j++;
        }
      }

    }
  }

  TLatex cms(0.17, 0.83, "CMS");
  cms.SetNDC();
  cms.SetTextFont(61);
  cms.SetTextSize(0.06);

  TLatex prelim(0.17, 0.81, "Preliminary");
  prelim.SetNDC();
  prelim.SetTextAlign(13);
  prelim.SetTextFont(52);
  prelim.SetTextSize(0.0456);

  TLatex data(0.17, 0.76, "Data 2024");
  data.SetNDC();
  data.SetTextAlign(13);
  data.SetTextFont(52);
  data.SetTextSize(0.0456);

  //TLatex lumi(0.85, 0.82, Form("%.1f fb^{-1} (13 TeV)", kLumi));
  TLatex lumi(0.85, 0.82, Form("%.1i (13.6 TeV)", 2024));
  lumi.SetNDC();
  lumi.SetTextAlign(31);
  lumi.SetTextFont(42);
  lumi.SetTextSize(0.052);

  c1.cd();
  h_p->SetStats(0);
  h_p->SetMarkerStyle(107);
  h_p->SetMarkerSize(0.8);
  h_p->GetXaxis()->SetTitle("Chamber index");
  h_p->GetYaxis()->SetTitle("Mean anode time [ns]");
  h_p->GetYaxis()->SetRangeUser(-10.,10.);
  //h_p->Draw("P0");
  h_p->Draw("e1x0");
  gPad->Update();
  cms.Draw();
  prelim.Draw();
  data.Draw();
  //lumi.Draw();
  c1.Print("anode_mean_perChamber_positive.png");

  c2.cd();
  h_n->SetStats(0);
  h_n->SetMarkerStyle(107);
  h_n->SetMarkerSize(0.8);
  h_n->GetXaxis()->SetTitle("Chamber index");
  h_n->GetYaxis()->SetTitle("Mean anode time [ns]");
  h_n->GetYaxis()->SetRangeUser(-10.,10.);
  h_n->Draw("e1x0");
  gPad->Update();
  cms.Draw();
  prelim.Draw();
  data.Draw();
  c2.Print("anode_mean_perChamber_negative.png");

}
