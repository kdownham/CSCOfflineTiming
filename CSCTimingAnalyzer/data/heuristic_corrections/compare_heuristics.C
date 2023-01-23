#include "Riostream.h"


std::vector<std::tuple<int,int,int,int,float>> get_corr(TString file);

void compare_heuristics(TString file1, TString file2){

      std::vector<std::tuple<int,int,int,int,float>> corr1 = get_corr(file1);
      std::vector<std::tuple<int,int,int,int,float>> corr2 = get_corr(file2);

      std::vector<std::tuple<int,int,int,int,float>> large_diff;

      TH1* h1 = new TH1F("h1", "difference in heuristic corrections per chamber (Run 357900 vs Run 362437)", 100, -10., 10.);

      //float corr_diff = corr1 - corr2; 
      //std::cout << " Difference in corrections = " << corr_diff << endl;    
      for (int i = 0; i < corr1.size(); i++){
	   std::tuple<int,int,int,int,float> bad_chambers;
	   float diff = std::get<4>(corr1[i]) - std::get<4>(corr2[i]);
	   float pct_diff = diff/std::get<4>(corr1[i]);
	   std::cout << "endcap = " << std::get<0>(corr1[i]) << " , station = " << std::get<1>(corr1[i]) << " , ring = " << std::get<2>(corr1[i]) << " , chamber = " << std::get<3>(corr1[i]) << " , diff (ns) = " << diff << " , pct difference = " << pct_diff*100. << "%" << endl;

	   if ( !(std::get<0>(corr1[i]) == std::get<0>(corr2[i]) && std::get<1>(corr1[i]) == std::get<1>(corr2[i]) && std::get<2>(corr1[i]) == std::get<2>(corr2[i]) && std::get<3>(corr1[i]) == std::get<3>(corr2[i]) ) ){
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	        std::cout << "endcap 1 = " << std::get<0>(corr1[i]) << " , endcap 2 = " << std::get<0>(corr2[i]) << endl;
	 	std::cout << "station 1 = " << std::get<1>(corr1[i]) << " , station 2 = " << std::get<1>(corr2[i]) << endl;
		std::cout << "ring 1 = " << std::get<2>(corr1[i]) << " , ring 2 = " << std::get<2>(corr2[i]) << endl;
		std::cout << "chamber 1 = " << std::get<3>(corr1[i]) << " , chamber 2 = " << std::get<3>(corr2[i]) << endl;
		break;
	   }

	   if ( std::abs(diff) > 2.0 ){
		std::get<0>(bad_chambers) = std::get<0>(corr1[i]);
		std::get<1>(bad_chambers) = std::get<1>(corr1[i]);
		std::get<2>(bad_chambers) = std::get<2>(corr1[i]);
		std::get<3>(bad_chambers) = std::get<3>(corr1[i]);
		//std::get<4>(bad_chambers) = pct_diff*100.;
		std::get<4>(bad_chambers) = std::abs(diff);
		large_diff.push_back(bad_chambers);
	   }

	   h1->Fill(diff);	  

      }

      std::cout << "//////////////////////////////////////////////////////////////////////////" << endl;

      for (int j = 0; j < large_diff.size(); j++){

	   std::cout << "Chambers with at least 2 ns difference = " << std::get<0>(large_diff[j]) << "/" << std::get<1>(large_diff[j]) << "/" << std::get<2>(large_diff[j]) << "/" << std::get<3>(large_diff[j]) << " , absolute difference (ns) = " << std::get<4>(large_diff[j]) << endl;

      }

      h1->GetXaxis()->SetTitle("correction difference [ns]");
      h1->GetYaxis()->SetTitle("Number of chambers");
      h1->Draw("hist");

}

std::vector<std::tuple<int,int,int,int,float>> get_corr(TString file){

      std::vector<std::tuple<int,int,int,int,float>> chambers_vector;
      std::ifstream f(file);
      std::string line;
      while(std::getline(f, line)){
	int col1;
	int col2;
	int col3;
	int col4;
	float col5;
	std::tuple<int,int,int,int,float> chamber_values;
	std::istringstream ss(line);
	ss >> col1 >> col2 >> col3 >> col4 >> col5;
	//std::cout << "endcap = " << col1 << endl;
	//std::cout << "station = " << col2 << endl;
	//std::cout << "ring = " << col3 << endl;
	//std::cout << "chamber = " << col4 << endl;
	//std::cout << "corr = " << col5 << endl;
	//if ( col1 == 1 && col2 == 1 && col3 == 1 && col4 == 1 ){
	  //   corr = col5;
	//}
	std::get<0>(chamber_values) = col1;
	std::get<1>(chamber_values) = col2;
	std::get<2>(chamber_values) = col3;
        std::get<3>(chamber_values) = col4;
	std::get<4>(chamber_values) = col5;

	chambers_vector.push_back(chamber_values);	

      }

      return chambers_vector;

}
