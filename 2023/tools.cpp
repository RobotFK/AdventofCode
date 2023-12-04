#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

vector<string> split_string(string& input,char delimiter){
    vector<string> output;
    stringstream ss(input);
    string slice;
    while(getline(ss,slice,delimiter)){
            output.push_back(slice);
    }
    return output;
}
