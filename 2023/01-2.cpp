#include <iostream>+=
#include "days.h"
#include <fstream>
#include <vector>
#include <map>
using namespace std;

void D_1_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("01.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    int sum = 0;
    map<string, int> numbers;
    numbers["one"] = 1;
    numbers["two"] = 2;
    numbers["three"] = 3;
    numbers["four"] = 4;
    numbers["five"] = 5;
    numbers["six"] = 6;
    numbers["seve"] = 7;
    numbers["eight"] = 8;
    numbers["nine"] = 9;

    for(int i =0;i != inputvector.size();i++){//Each Word
            int ldigit = 0;
            int rdigit = 0;
            for(int j=0;j != inputvector[i].length();j++){//Each Digit
                //cout<<"Test: "<<inputvector[i][j]<< endl;
                if(inputvector[i].at(j) >= '0' && inputvector[i].at(j) <= '9'){
                    if(ldigit == 0){
                        //cout<<"FoundF: "<<inputvector[i][j]<< endl;
                        ldigit = (int)inputvector[i].at(j) -48;
                        rdigit = (int)inputvector[i].at(j) -48;
                    }
                    else{
                        //cout<<"Found: "<<inputvector[i][j]<< endl;
                        rdigit = (int)inputvector[i].at(j)-48;}
                }else{
                    string seg = inputvector[i].substr(j);
                    for (auto const& x : numbers){//Compare to written Numbers
                        if(seg.substr(0, x.first.length()) == x.first){
                            if(ldigit == 0){
                                ldigit = x.second;
                                rdigit = x.second;
                            }
                            else{
                                rdigit = x.second;}
                            }
                    }
                }
            }
            //cout<<"Done: "<<inputvector[i]<< endl;
            //cout<< ldigit <<"*10 +"<<rdigit << endl;
            sum +=((ldigit*10)+rdigit);

    }
    cout <<"Sum is "<< sum << endl;
    return;
}
