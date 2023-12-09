#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_9_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("09-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

	struct sequence{
		vector<int> numbers;
		vector<vector<int>> differentials;//of the n'th order
	};
	vector<sequence>sequences;

	for(auto&line:inputvector){
		sequence newsequence;
		vector<string> values = split_string(line,' ');
		for(auto&value:values){
			newsequence.numbers.push_back(stoi(value));
		}
		sequences.push_back(newsequence);
	}

	for(auto&seq:sequences){//For each Sequence
		//cout<<endl<<"Next seq"<<endl;
		for(int i=0;i!=(seq.numbers.size()-1);i++){//For each Number (allowing differntials of an additional height)
			int change= seq.numbers[i+1]-seq.numbers[i];
			//cout<< "Basechange:"<<change<<endl;
			for(int j=0;j!=seq.differentials.size();j++){//go through each differential and append the value
				//cout<<"Subtracting"<<seq.differentials[j].back()<<endl;
				seq.differentials[j].push_back(change);
				change-=seq.differentials[j][seq.differentials[j].size()-2];
				//cout<< "Remaining Change:"<<change<<endl;
			}

			//cout<<"Adding "<<change<<endl;
			vector<int> diff(1,change);
			seq.differentials.push_back(diff);
		}
		next_sequence:;
		for(int i=0;i!=(seq.differentials.size());i++){
			for(auto idiff:seq.differentials[i]){
			//cout<<idiff<<",";
			}
			//cout<<endl;
		}
		//cout<<endl;
	}

	int sum = 0;
	for(auto&seq:sequences){//For each Sequence
			int nxt=0;
		for(auto idiff:seq.differentials){
			nxt+=idiff.back();
		}
		nxt+=seq.numbers.back();
		cout<< "Next: "<<nxt<<endl;
		sum+=nxt;
	}


    cout <<"Sum is "<< sum << endl;
    return;
}
