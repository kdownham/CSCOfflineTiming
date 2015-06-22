#include <iostream>
#include <string>
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TIter.h"
#include "TObject.h"

void dumpPlots (std::string fname)
{
    std::string fname_short = fname;
    fname_short.erase(fname_short.end()-5,fname_short.end());
    TCanvas c1("c1","c1",600,400);

    TFile file(fname.c_str());
    file.cd();
    TList *flist = file.GetListOfKeys();
    for (auto item : *flist)
    {
        TObject *obj = file.Get(item->GetName());
        if (obj->InheritsFrom(TDirectory::Class()))
        {
            TDirectory *dir = (TDirectory*)obj;
            TList *dlist = dir->GetListOfKeys();
            bool is_first = true;
            for (auto ditem : *dlist)
            {
                TObject *dobj = dir->Get(ditem->GetName());
                if (dobj->InheritsFrom(TH1::Class()))
                {
                    dobj->Draw();
                    c1.Print(Form("plots/%s/%s/%s.pdf", fname_short.c_str(), dir->GetName(), dobj->GetName()));
                    c1.Print(Form("plots/%s/%s/%s.png", fname_short.c_str(), dir->GetName(), dobj->GetName()));
                }
            }
        }
    }    
}
