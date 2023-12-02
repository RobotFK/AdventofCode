#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_3_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("02-1.txt");
    while (getline(inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    return;
}
