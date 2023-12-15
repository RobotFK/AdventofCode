#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

int h_hash2(string const&input){
	int value =0;
	for(int i=0;i!=input.size();i++){
		value+=input[i];
		value*=17;
		value%=256;
	}
	return value;
}

void D_15_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("15-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

	vector<string> steps = split_string(inputvector[0],',');

	cout << steps.size()<<" steps"<<endl;


	struct lens{
		string id;
		int focal_length;
	};
	//vector<map<string,int>>boxes(256); Does not allow for order of elements based on insertion
	vector<vector<lens>>boxes(256);



	for(auto&step:steps){
		//Because the biggest value after an = is 9 we can use a trick here:
		if(step[step.size()-1]=='-'){
			string key = step.substr(0,step.size()-1);
			//auto erased = erase_if(boxes[h_hash2(key)],[](lens s,string key){return s.id ==key;})//My compiler is not new enough for it
			//boxes[h_hash2(key)].erase(remove_if(boxes[h_hash2(key)].begin(), boxes[h_hash2(key)].end(), [](lens s,string key){return s.id ==key;}), boxes[h_hash2(key)].end());
			int pos = -1;
			for(int i=0;i!=boxes[h_hash2(key)].size();i++){
				if(boxes[h_hash2(key)][i].id ==key){pos=i;break;}
			}
			if(pos!=-1){
				boxes[h_hash2(key)].erase(boxes[h_hash2(key)].begin()+pos);
			}

		}else{
			string key = step.substr(0,step.size()-2);
			int value = stoi(step.substr(step.size()-1,1));
			bool updated =false;
			for(auto&pos:boxes[h_hash2(key)]){
				if(pos.id ==key){pos.focal_length=value;updated=true;break;}
			}
			if(!updated){
				lens newlens;
				newlens.id =key;
				newlens.focal_length =value;
				boxes[h_hash2(key)].push_back(newlens);
			}
		}

		/*
		cout<<endl<<"After "<<step<<endl;
		int box_id=0;
		for(auto&box:boxes){
			if(box.size()==0){box_id++;continue;}
			cout<<"Box"<< box_id<<": ";
			for(auto&ilens:box){
				cout<<ilens.id <<" "<< ilens.focal_length << ", ";
			}
			cout<<endl;
		}*/
	}

	int sum=0;
	for(int box_id=0;box_id!=boxes.size();box_id++){
		if(boxes[box_id].size()==0){continue;}
		for(int lens_id=0;lens_id!=boxes[box_id].size();lens_id++){
		sum+= (box_id+1)*(lens_id+1)*(boxes[box_id][lens_id].focal_length);
		}
	}
    cout <<"Sum is "<< sum << endl;
    return;
}
