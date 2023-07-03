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

void dumpPlots (std::string fname, std::string run, std::string outputdir) {
  std::string fname_short = fname;
  fname_short.erase(fname_short.end()-5,fname_short.end());
  if (fname_short.find("/") != string::npos) fname_short.erase(fname_short.begin(), fname_short.begin()+fname_short.find("hist"));
  TCanvas c1("c1","c1",600,400);
  gStyle->SetOptStat("emrou");

  std::string recHits = "recHitsByChamber";
  std::string no_hist = "hRHTiming";
  std::string no_hist_1 = "hAnodeTiming_+1_1";
  std::string no_hist_2 = "hAnodeTiming_+1_3";
  std::string no_hist_3 = "hAnodeTiming_+2_1";
  std::string no_hist_4 = "hAnodeTiming_+2_2";
  std::string no_hist_5 = "hAnodeTiming_+3_1";
  std::string no_hist_6 = "hAnodeTiming_+3_2";
  std::string no_hist_7 = "hAnodeTiming_+4_1";
  std::string no_hist_8 = "hAnodeTiming_+4_2";
  std::string no_hist_9 = "hAnodeTiming_-";

  TFile file(fname.c_str());
  file.cd();
  TList *flist = file.GetListOfKeys();
  for (auto item : *flist) {
    TObject *obj = file.Get(item->GetName());
    if (obj->InheritsFrom(TDirectory::Class())) {
      TDirectory *dir = (TDirectory*)obj;
      TList *dlist = dir->GetListOfKeys();
      bool is_first = true;

      cout << "item = " << item->GetName() << endl;
      //cout << "fname_short = " << fname_short.c_str() << endl;
      

      //if ( item->GetName() == recHits || item->GetName() == Segments ){
	//   continue;
      //}
      if ( item->GetName() != recHits ) continue;

      cout << Form("plots/all_plots/%s/%s/%s/%s", run.c_str(), outputdir.c_str(), fname_short.c_str(), dir->GetName()) << endl;
      gSystem->Exec(Form("mkdir -p plots/all_plots/%s/%s/%s/%s", run.c_str(), outputdir.c_str(), fname_short.c_str(), dir->GetName()));

      for (auto ditem : *dlist) {
        TObject *dobj = dir->Get(ditem->GetName());
        if (dobj->InheritsFrom(TH1::Class())) {
	  std::string name = dobj->GetName();
	  if (name.find(no_hist) != string::npos) continue; 
	  if (name.find(no_hist_1) != string::npos) continue;
	  if (name.find(no_hist_2) != string::npos) continue;
	  if (name.find(no_hist_3) != string::npos) continue;
	  if (name.find(no_hist_4) != string::npos) continue;
	  if (name.find(no_hist_5) != string::npos) continue;
	  if (name.find(no_hist_6) != string::npos) continue;
	  if (name.find(no_hist_7) != string::npos) continue;
	  if (name.find(no_hist_8) != string::npos) continue;
	  if (name.find(no_hist_9) != string::npos) continue;
          dobj->Draw();
          // c1.Print(Form("plots/%s/%s/%s.pdf", fname_short.c_str(), dir->GetName(), dobj->GetName()));
          c1.Print(Form("plots/all_plots/%s/%s/%s/%s/%s.png", run.c_str(), outputdir.c_str(), fname_short.c_str(), dir->GetName(), dobj->GetName()));
        }
      }
    }
  }
}
