#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_2_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("01.txt");
    while (getline(inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    //Task here
    return;
}

