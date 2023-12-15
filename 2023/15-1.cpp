#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

int h_hash(string const&input){
	int value =0;
	for(int i=0;i!=input.size();i++){
		value+=input[i];
		value*=17;
		value%=256;
	}
	return value;
}

void D_15_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("15-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

	vector<string> steps = split_string(inputvector[0],',');

	int sum=0;
	for(auto&step:steps){
		sum+=h_hash(step);
	}
    cout <<"Sum is "<< sum << endl;
    return;
}
