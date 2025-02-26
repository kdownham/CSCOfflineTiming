void AverageCorrections(){

     std::string fname = "anode_bx_offsets_362654.txt";
     std::ifstream f(fname);
     std::string line;

     double ME_11 = 0.;
     double N_11 = 0;
     double ME_X1 = 0.;
     double N_X1 = 0;
     double ME_XY = 0.;
     double N_XY = 0;

     while(std::getline(f, line)){
        int col_ecap;
        int col_station;
        int col_ring;
        int col_chamber;
        double col_anode;
        std::istringstream ss(line);
        ss >> col_ecap >> col_station >> col_ring >> col_chamber >> col_anode;
        if ( col_station == 1 && col_ring == 1 ){
	     ME_11 += col_anode;
	     N_11 += 1.0;
        }
	if ( col_station > 1 && col_ring == 1 ){
             ME_X1 += col_anode;
             N_X1 += 1.0;
        }
	if ( col_station > 1 && col_ring > 1 ){
             ME_XY += col_anode;
             N_XY += 1.0;
        }
      }

      std::cout << "ME 1/1 correction = " << ME_11/N_11 << std::endl;
      std::cout << "ME X/1 correction = " << ME_X1/N_X1 << std::endl;
      std::cout << "ME X/Y correction = " << ME_XY/N_XY << std::endl;

}
