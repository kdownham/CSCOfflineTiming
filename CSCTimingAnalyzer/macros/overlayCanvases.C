#include <string>
#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPad.h"
#include "TDirectoryFile.h"
#include "TPaveStats.h"
#include "TPad.h"
#include "TList.h"
#include "TCollection.h"
#include "TObject.h"

void overlayCanvases (std::string fname1, std::string fname2, std::string canvas1, std::string canvas2, std::string leg1, std::string leg2, std::string oname)
{
    TCanvas oc("oc", "oc", 600, 400);
    gStyle->SetOptStat("emrou");

    TFile file1(fname1.c_str());
    TH1F *h1 = 0;
    TCanvas *can1 = (TCanvas*)file1.Get(canvas1.c_str());
    TList *list1 = can1->GetListOfPrimitives();
    TIter iter1 = list1->begin();    
    while (TObject *obj1 = iter1())
    {
      if (!obj1->InheritsFrom(TH1::Class())) continue;
      h1 = (TH1F*)can1->GetPrimitive(obj1->GetName());
      h1->SetLineColor(kBlack);
      h1->SetMarkerColor(kBlack);      
    }
    
    TFile file2(fname2.c_str());
    TH1F *h2 = 0;    
    TCanvas *can2 = (TCanvas*)file2.Get(canvas2.c_str());
    TList *list2 = can2->GetListOfPrimitives();
    TIter iter2 = list2->begin();
    while (TObject *obj2 = iter2())
    {
      if (!obj2->InheritsFrom(TH1::Class())) continue;
      h2 = (TH1F*)can2->GetPrimitive(obj2->GetName());
      h2->SetLineColor(kRed);
      h2->SetMarkerColor(kRed);      
    }

    // oc.cd();
    bool h1_is_max = (h1->GetBinContent(h1->GetMaximumBin()) > h2->GetBinContent(h2->GetMaximumBin()));
    if (h1_is_max)
    {
        h1->Draw();
        h2->Draw("sames");
    }
    else
    {
        h2->Draw();
        h1->Draw("sames");
    }

    gPad->Update();
    TPaveStats *s1 = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
    TPaveStats *s2 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
    if (s1 != 0 && s2 != 0)
    {
      s2->SetTextColor(kRed);
      s2->SetX1NDC(s1->GetX1NDC());
      s2->SetX2NDC(s1->GetX2NDC());
      s2->SetY2NDC(s1->GetY1NDC()-0.02);
      s2->SetY1NDC(s2->GetY2NDC()-(s1->GetY2NDC()-s1->GetY1NDC()));
      gPad->Modified();
    }
    
    TLegend leg(0.2,0.6,0.4,0.8);
    leg.SetFillColor(0);
    leg.SetBorderSize(0);
    leg.AddEntry(h1,leg1.c_str());
    leg.AddEntry(h2,leg2.c_str());
    leg.Draw();
    
    oc.Print(Form("%s_comp.pdf",oname.c_str()));
    oc.Print(Form("%s_comp.png",oname.c_str()));
    oc.Print(Form("%s_comp.root",oname.c_str()));    

    if (h1_is_max)
    {
      double rat = h1->Integral() / h2->Integral();
      h2->Scale(rat);
      h1->Draw();
      h2->Draw("sames");
    }
    else
    {
      double rat = h2->Integral() / h1->Integral();
      h1->Scale(rat);
      h2->Draw();
      h1->Draw("sames");
    }

    gPad->Update();
    if (s1 != 0 && s2 != 0)
    {
      s2->SetTextColor(kRed);
      s2->SetX1NDC(s1->GetX1NDC());
      s2->SetX2NDC(s1->GetX2NDC());
      s2->SetY2NDC(s1->GetY1NDC()-0.02);
      s2->SetY1NDC(s2->GetY2NDC()-(s1->GetY2NDC()-s1->GetY1NDC()));
      gPad->Modified();
    }

    leg.Draw();
    oc.Print(Form("%s_comp_scaled.pdf",oname.c_str()));
    oc.Print(Form("%s_comp_scaled.png",oname.c_str()));
    oc.Print(Form("%s_comp_scaled.root",oname.c_str()));    
}
