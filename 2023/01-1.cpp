#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_1_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("01.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    int sum = 0;

    for(int i =0;i != inputvector.size();i++){
            int ldigit = 0;
            int rdigit = 0;
            for(int j=0;j != inputvector[i].length();j++){
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
                }
            }
            //cout<<"Done: "<<inputvector[i]<< endl;
            //cout<< ldigit <<"*10 +"<<rdigit << endl;
            sum +=((ldigit*10)+rdigit);

    }
    cout <<"Sum is "<< sum << endl;
    return;
}
