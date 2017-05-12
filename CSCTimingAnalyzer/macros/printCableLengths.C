#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

struct DetId
{
    DetId () {endcap=0;station=0;ring=0;chamber=0;}
    DetId (int endcap_, int station_, int ring_, int chamber_) {endcap=endcap_; station=station_; ring=ring_; chamber=chamber_;}
    int endcap;
    int station;
    int ring;
    int chamber;
    bool operator<( const DetId & n ) const {
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

void printCableLengths (std::string fname, std::string ofname = "")
{
    // std::map<DetId, int> mcl;
    // std::map<DetId, int> mcd;    
    // std::map<DetId, int> mncl;
    // std::map<DetId, int> mncd;    

    const unsigned int ppib_dcfeb_cable_length = 270;
    const double skewclear_revd_propagation_delay = 5.2; // [ns/m]
    const double skewclear_revo_propagation_delay = 5.05; // [ns/m]
    const double ppib_lvds_delay = 5; // [ns]
    
    
    TFile file(fname.c_str());
    TTree *tree = (TTree*)file.Get("TimingParameters");
    
    int endcap, station, ring, chamber, cfeb_cable_length, new_cfeb_cable_length, cfeb_skew_delay, new_cfeb_skew_delay;
    char cfeb_cable_rev, new_cfeb_cable_rev;
    tree->SetBranchAddress("endcap", &endcap);
    tree->SetBranchAddress("station", &station);
    tree->SetBranchAddress("ring", &ring);
    tree->SetBranchAddress("chamber", &chamber);
    tree->SetBranchAddress("cfeb_cable_length", &cfeb_cable_length);
    tree->SetBranchAddress("new_cfeb_cable_length", &new_cfeb_cable_length);
    tree->SetBranchAddress("cfeb_cable_rev", &cfeb_cable_rev);
    tree->SetBranchAddress("new_cfeb_cable_rev", &new_cfeb_cable_rev);
    tree->SetBranchAddress("cfeb_skew_delay", &cfeb_skew_delay);
    tree->SetBranchAddress("new_cfeb_skew_delay", &new_cfeb_skew_delay);    
    
    ofstream outfile;
    outfile.open(ofname);
    if (!outfile) return;

    outfile << "endcap\t station\t ring\t chamber\t new rev\t new length\t new delay\n";
    const unsigned int nentries = tree->GetEntries();
    for (unsigned int n = 0; n < nentries; n++)
    {        
        tree->GetEntry(n);
        if (station != 1 || ring != 1) continue;

        double total_delay = new_cfeb_cable_length * skewclear_revd_propagation_delay / 100. + ppib_dcfeb_cable_length * skewclear_revo_propagation_delay / 100. + ppib_lvds_delay;
        int new_cable_length = (int)(total_delay*100./5.2);
        outfile << endcap << "\t" << station << "\t" << ring << "\t" << chamber << "\t" << new_cfeb_cable_rev << "\t" << new_cable_length << "\t" << new_cfeb_skew_delay << std::endl;
        std::cout << endcap << "\t" << station << "\t" << ring << "\t" << chamber << "\t" << (new_cable_length - cfeb_cable_length) * skewclear_revd_propagation_delay / 100. << std::endl;
    }

    if (!outfile)
        outfile.close();
    
    return;
}
