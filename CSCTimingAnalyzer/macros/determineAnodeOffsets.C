#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TString.h"
#include "TList.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TH1.h"
#include "TCanvas.h"

struct CSCAnodeCorrDetId
{

    CSCAnodeCorrDetId () {endcap=0;station=0;ring=0;chamber=0;}
    CSCAnodeCorrDetId (int endcap_, int station_, int ring_, int chamber_) {endcap=endcap_; station=station_; ring=ring_; chamber=chamber_;}
    int endcap;
    int station;
    int ring;
    int chamber;
    bool operator<( const CSCAnodeCorrDetId & n ) const {
        if (this->endcap < n.endcap)
            return true;
        else if (this->endcap == n.endcap && this->station < n.station)
            return true;
        else if (this->endcap == n.endcap && this->station == n.station && this->ring < n.ring)
            return true;
        else if (this->endcap == n.endcap && this->station == n.station && this->ring == n.ring && this->chamber < n.chamber)
            return true;
        else
            return false;
    }

};

std::tuple<int,int,int,int,double> get_current_bx_offset(int endcap, int station, int ring, int chamber, std::string fname="../test/anode_bx_offset.txt");

void determineAnodeOffsets (std::string fname, std::string ofname){

	TFile file(fname.c_str());
	TDirectoryFile *dir;
	//Get the corresponding TDirectory that houses the anode_bx_offsets
	dir = (TDirectoryFile*)file.Get("recHitsByChamber");

	ofstream outfile;
	bool print_to_file = !ofname.empty();

	if (print_to_file){

	    outfile.open(ofname);
	    outfile << "endcap\t" << "station\t" << "ring\t" << "chamber\t" << "new anode_bx_offset" << std::endl;

	}
	else
	{
	
	    printf("endcap\t station\t ring\t chamber\t new anode bx offset\n");
	    printf("---------------------------------------------------------------------------------\n");
	
	}

	std::map<CSCAnodeCorrDetId, std::pair<unsigned long, double> > m;
	TList *hlist = dir->GetListOfKeys();

	for (auto hist : *hlist)
	{
	     TString name = hist->GetName();
	     if (!name.Contains("hAnodeTiming")) continue;
	     TObject *obj = dir->Get(name.Data());
	     if (!obj->InheritsFrom(TH1::Class())) continue;

	     TString delim;
	     int endcap, station, ring, chamber;
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
	     if (station_ring.Sizeof() != 7 && station_ring.Sizeof() != 6) continue;
	     TString s = station_ring[0];
	     if (s.IsDigit()) station = s.Atoi();
	     TString r = station_ring[2];
	     if (r.IsDigit()) ring = r.Atoi();
	     TString c; 
	     if (station_ring.Sizeof() == 6){
	     	 c = station_ring[4];
	     }
	     if (station_ring.Sizeof() == 7){
	         c = station_ring[4];
		 c += station_ring[5]; 
	     }
	     if (c.IsDigit()) chamber = c.Atoi();    
	     double anode_corr = ((TH1*)obj)->GetMean() / -25. * 100.;  // Convert from ns to number of bunch crossings (x100)
	     unsigned long nrhs = ((TH1*)obj)->GetEntries();

	     // Need to include a function here that gets the list of anode_bx_offsets for all chambers
	    
	     std::tuple<int,int,int,int,double> old_bx_offsets = get_current_bx_offset(endcap,station,ring,chamber);

             //std::cout << "Endcap = " << endcap << ", Station = " << station << ", Ring = " << ring << ", Chamber = " << chamber << std::endl;
	     //std::cout << "Anode correction for this chamber = " << anode_corr << std::endl;

	     // Need to combine ME11 A and B
	     if (ring == 4) ring = 1;
	     CSCAnodeCorrDetId id(endcap, station, ring, chamber);
	     if (m.find(id) != m.end()){
		 m[id].first += nrhs;
		 m[id].second += (double)nrhs*anode_corr;
	     }
	     else{
		 m[id] = std::make_pair(nrhs, (double)nrhs*anode_corr);
	     }
	 
             objarray->Delete();    

	     if (print_to_file){
		double offset = std::get<4>(old_bx_offsets);
	        double new_offset = offset + anode_corr;
	        outfile << endcap << "\t" << station << "\t" << ring << "\t" << chamber << "\t" << new_offset << std::endl;	
	     }
	     else{
		double offset = std::get<4>(old_bx_offsets);
		double new_offset = offset + anode_corr;
		//printf("%d\t%d\t%d\t%d\t%4.2f\n", endcap, station, ring, chamber, new_offset);
	        std::cout << "--------------------------------------------------------------------" << std::endl;
		printf("%d\t%d\t%d\t%d\t", endcap, station, ring, chamber);
		std::cout << "Old anode correction = " << offset << std::endl;
		std::cout << "Mean of anode timing distribution = " << ((TH1*)obj)->GetMean() << std::endl;
		std::cout << "Mean (in bx units x100) = " << anode_corr << std::endl;
		std::cout << "New offset = " << new_offset << std::endl;

	     }
	}

	if (print_to_file) outfile.close();
	return;

} 


std::tuple<int,int,int,int,double> get_current_bx_offset(int endcap, int station, int ring, int chamber, std::string fname="../test/anode_bx_offset.txt"){

	//std::vector<int> chambers;
	double bx_offset;
	int ec, st, rg, ch;

	std::ifstream f(fname);
	std::string line;
	while(std::getline(f, line)){
	  int col1;
	  int col2;
	  int col3;
	  int col4;
	  int col5;
	  std::istringstream ss(line);
	  ss >> col1 >> col2 >> col3 >> col4 >> col5;	  
	  // If we match the endcap, station, ring, then loop over the chambers, store the chambers
	  // and store the bx_offsets in double form
	  if ( col1 == endcap && col2 == station && col3 == ring && col4 == chamber ){
	       ec = col1;
	       st = col2;
	       rg = col3; 
	       //chambers.push_back(col4);
	       ch = col4;
	       bx_offset = col5*1.0;
	  }
	  else continue;
	}

	// Now we have a collection of the useful quantities, let us define the tuple and return it
	std::tuple<int,int,int,int,double> offset_tuple;
	std::get<0>(offset_tuple) = ec;
	std::get<1>(offset_tuple) = st;
	std::get<2>(offset_tuple) = rg;
	//std::get<3>(offset_tuple) = chambers;
	std::get<3>(offset_tuple) = ch;
	std::get<4>(offset_tuple) = bx_offset;

	return offset_tuple;

}
