#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>
using namespace std;

long long race_combinations2(long long time, long long distance){
    long long hold = floor(time/2);//optimal time to hold for the given time to get the maximum distance
    long long range = sqrt(pow(time, 2) - 4 * distance);

    return range;
}

void D_6_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("06-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<string> time;
    vector<string> distance;
        {
        stringstream ss(inputvector[0]);
        string slice;
        while(getline(ss,slice,' ')){
            if(slice.size()== 0){continue;}
            if(!isdigit(slice[0])){continue;}
            time.push_back(slice);
        }
        }
        {
        stringstream ss(inputvector[1]);
        string slice;
        while(getline(ss,slice,' ')){
            if(slice.size()== 0){continue;}
            if(!isdigit(slice[0])){continue;}
            distance.push_back(slice);
        }}

    string stime= "";
    for(auto&itime:time){
        stime+=itime;
    }

    string sdistance= "";
    for(auto&idistance:distance){
        sdistance+=idistance;
    }
    cout << stime << " : "<< sdistance<< endl;
    long long product = race_combinations2(stoll(stime),stoll(sdistance));
    cout << "Final Product: "<< product<<endl;
    return;
}
//20537782 To low
