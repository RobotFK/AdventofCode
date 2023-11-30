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

    for(int i =0;i != inputvector.size();i++){
            cout<<inputvector[i]<< endl;
    }
    return;
}
