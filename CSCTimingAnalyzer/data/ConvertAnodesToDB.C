#include <iostream>
#include <fstream>

void ConvertAnodesToDB(){
      
     std::string fname_new = "../data/anode_bx_offsets_385801_new_copy.txt";
     std::ifstream f(fname_new);
     std::string line;

     std::string fname_old = "../test/anode_bx_offset.txt";
     std::ifstream f1;
     std::string line1;

     if (f.is_open()){
     	while (std::getline(f,line)){
	   int endcap;
	   int station;
	   int ring;
	   int chamber;
	   int new_anode;
	   std::istringstream ss(line);
	   ss >> endcap >> station >> ring >> chamber >> new_anode;

	   f1.open(fname_old);
	   if (f1.is_open()){
	   	while (std::getline(f1,line1)){
	   		int endcap_old;
	   		int station_old;
	   		int ring_old;
	   		int chamber_old;
	   		int old_anode;
	   		int xf_anode;
	   		std::istringstream ss1(line1);
	   		ss1 >> endcap_old >> station_old >> ring_old >> chamber_old >> old_anode;
	   		if ( endcap==endcap_old && station==station_old && ring==ring_old && chamber==chamber_old ){
	   			xf_anode = ((2.0*old_anode) - new_anode);
				if (chamber < 10){
	   		        	std::cout << endcap << "       " << station << "       " << ring << "       " << chamber << "       " << xf_anode << std::endl;
				} else {
					std::cout << endcap << "       " << station << "       " << ring << "       " << chamber << "      " << xf_anode << std::endl;
				}
	   		} 
	   	}
		f1.close();
	   } 
	}
	f.close();
     } else {
     	std::cerr << "Error opening one or both files!" << std::endl;
     }


     //while(std::getline(f, line)){
     //   int endcap;
     //   int station;
     //   int ring;
     //   int chamber;
     //   double anode;
     //   double old_anode;
     //   double new_anode;
     //   std::istringstream ss(line);
     //   ss >> endcap >> station >> ring >> chamber >> anode;
     //   if ( station == 1 && ring == 1 ){
     //        //diff = ( anode - 822.0 );
     //        old_anode = 822.0;
     //        //new_anode = (anode + diff);
     //        new_anode = ((2.0*old_anode) - anode);
     //   }
     //   if ( station == 1 && ring > 1 ){
     //        //diff = ( anode - 820.0 );
     //        old_anode = 820.0;
     //        //new_anode = (anode + diff);
     //        new_anode = ((2.0*old_anode) - anode);
     //   }
     //   if ( station > 1 ){
     //        //diff = ( anode - 815.0 );
     //        old_anode = 815.0;
     //        //new_anode = (anode + diff);
     //        new_anode = ((2.0*old_anode) - anode);
     //   }

     //   std::cout << endcap << "       " << station << "       " << ring << "       " << chamber << "       " << new_anode << std::endl;
     //
     //}

}
