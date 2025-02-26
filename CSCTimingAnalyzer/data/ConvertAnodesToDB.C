#include <iostream>
#include <fstream>

void ConvertAnodesToDB(){
      
     std::string fname ="../data/anode_bx_offsets_367229.txt";
     std::ifstream f(fname);
     std::string line;


     while(std::getline(f, line)){
        int endcap;
	int station;
	int ring;
	int chamber;
	double anode;
	double old_anode;
	double new_anode;
	std::istringstream ss(line);
	ss >> endcap >> station >> ring >> chamber >> anode;
	if ( station == 1 && ring == 1 ){
	     //diff = ( anode - 822.0 );
	     old_anode = 822.0;
	     //new_anode = (anode + diff);
	     new_anode = ((2.0*old_anode) - anode);
	}
	if ( station == 1 && ring > 1 ){
	     //diff = ( anode - 820.0 );
	     old_anode = 820.0;
	     //new_anode = (anode + diff);
	     new_anode = ((2.0*old_anode) - anode);
	}
	if ( station > 1 ){
	     //diff = ( anode - 815.0 );
	     old_anode = 815.0;
	     //new_anode = (anode + diff);
	     new_anode = ((2.0*old_anode) - anode);
	}

	std::cout << endcap << "       " << station << "       " << ring << "       " << chamber << "       " << new_anode << std::endl;
     
     }

}
