#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

void D_11_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("11-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

	class galaxy{
		public:
		long long x_pos;
		long long y_pos;

		galaxy(long long x_pos,long long y_pos){
			this->x_pos = x_pos;
			this->y_pos = y_pos;
		}
	};
	vector<galaxy> galaxies;

	for(int i=0;i!=inputvector.size();i++){
		for(int j=0;j!=inputvector[i].size();j++){
			if(inputvector[i][j]=='#'){
				galaxy newgalaxy(j,i);
				galaxies.push_back(newgalaxy);
			}
		}
	}
	cout<<galaxies.size()<<" Galaxies"<<endl;

	int expansion_muliplier = 1000000 -1;//Amount of Lines to add
	int expansion=0;
	for(long long i=0;i!=inputvector.size()+(expansion*expansion_muliplier);i++){//Horizontal Exansion
		bool empty_space = true;
		for(auto&igalaxy:galaxies){
			if(igalaxy.y_pos==i){empty_space=false;}
		}
		if(empty_space){
			//cout<< "Expanding at Y="<<i<<endl;
			for(auto&igalaxy:galaxies){
				if(igalaxy.y_pos>i){igalaxy.y_pos+=expansion_muliplier;}
			}
			i+=expansion_muliplier;
			expansion++;
		}
	}cout<<endl;
	cout<<expansion<<" Horizontal Exansions"<<endl;
	expansion=0;
	for(long long i=0;i!=inputvector[0].size()+(expansion*expansion_muliplier);i++){//Vertical Exansion
		bool empty_space = true;
		for(auto&igalaxy:galaxies){
			if(igalaxy.x_pos==i){empty_space=false;}
		}
		if(empty_space){
			//cout<< "Expanding at X="<<i<<endl;
			for(auto&igalaxy:galaxies){
				if(igalaxy.x_pos>i){igalaxy.x_pos+=expansion_muliplier;}
			}
			i+=expansion_muliplier;
			expansion++;
		}
	}
	cout<<expansion<<" Vertical Exansions"<<endl;

	long long sum=0;
	for(long long i=0;i!=galaxies.size()-1;i++){//starting point
		for(long long j=i+1;j!=galaxies.size();j++){//Ending point
			sum+=abs(galaxies[i].x_pos-galaxies[j].x_pos)+abs(galaxies[i].y_pos-galaxies[j].y_pos);
		}
	}

	cout<<endl<<"Sum is "<<sum<<" with an expansion multiplier of "<<expansion_muliplier+1<<endl;
    return;
}
