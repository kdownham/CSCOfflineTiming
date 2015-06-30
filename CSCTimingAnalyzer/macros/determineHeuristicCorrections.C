#include <string>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TString.h"
#include "TList.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TH1.h"
#include "TCanvas.h"

void determineHeuristicCorrections (std::string fname, bool byStation, bool byChamber, std::string ofname = "")
{
    TFile file(fname.c_str());
    TDirectoryFile *dir;
    if (byStation)
        dir = (TDirectoryFile*)file.Get("recHits");
    else if (byChamber)
        dir = (TDirectoryFile*)file.Get("recHitsByChamber");

    ofstream outfile;
    bool print_to_file = !ofname.empty();

    TCanvas c1("c1","c1",600,400);
    TH1F hnrhs("hnrhs","Number of rechits per chamber", 100, 0, 2500);

    if (byStation)
    {
        if (print_to_file)
        {
            outfile.open(ofname);
            outfile << "endcap\t" << "station\t" << "ring\t" << "corr" << std::endl;
        }
        else
        {
            printf("endcap\t station\t ring\t heuristic correction\n");
            printf("------------------------------------------------------------------\n");
        }
    }
    else if (byChamber)
    {
        if (print_to_file)
        {
            outfile.open(ofname);
            outfile << "endcap\t" << "station\t" << "ring\t" << "chamber\t" << "corr" << std::endl;
        }
        else
        {
            printf("endcap\t station\t ring\t chamber\t heuristic correction\n");
            printf("------------------------------------------------------------------------------\n");
        }
    }

    TList *hlist = dir->GetListOfKeys();
    for (auto hist : *hlist)
    {
        TString name = hist->GetName();
        if (!name.Contains("hRHTiming")) continue;
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
            double corr = ((TH1*)obj)->GetMean() * -50.;
            if (print_to_file)
                outfile << endcap << "\t" << station << "\t" << ring << "\t" << corr << std::endl;
            else
                printf("%d\t%d\t%d\t%4.2f\n", endcap, station, ring, corr);
            objarray->Delete();
        }
        else if (byChamber)
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
            double corr = ((TH1*)obj)->GetMean() * -50.;
            int nrhs = ((TH1*)obj)->GetEntries();
            hnrhs.Fill(std::min(nrhs,2499));
            if (print_to_file)
                outfile << endcap << "\t" << station << "\t" << ring << "\t" << chamber << "\t" << corr << std::endl;
            else
                printf("%d\t%d\t%d\t%d\t%4.2f\n", endcap, station, ring, chamber, corr);
            objarray->Delete();
        }
    }

    if (print_to_file)
        outfile.close();

    hnrhs.Draw();
    c1.Print("num_rechits_per_chamber.pdf");

    return;
}
