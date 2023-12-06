#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>
using namespace std;

int race_combinations(int time, int distance){
    int hold = floor(time/2);//optimal time to hold for the given time
    int combinations = 0;
    for(int i = 0;i!=time;i++){//can be done smarter, but brute force will cut it here
        int idist = (time-i)*i;

        if(idist > distance){combinations++;}
    }
    return combinations;
}

void D_6_1(){
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
            time.push_back(slice);
        }}
        {
        stringstream ss(inputvector[1]);
        string slice;
        while(getline(ss,slice,' ')){
            if(slice.size()== 0){continue;}
            distance.push_back(slice);
        }}

    struct race{
        int time;
        int distance;
        int combinations;
    };
    vector<race> races;

    for(int i=1;i!=time.size();i++){
        race newrace;
        newrace.time = stoi(time[i]);
        newrace.distance = stoi(distance[i]);
        races.push_back(newrace);
    }

    for(auto&irace:races){
        cout<< irace.time << ": "<< irace.distance<< endl;
        irace.combinations = race_combinations(irace.time,irace.distance);
        cout<< irace.combinations<< " combinations"<<endl<<endl;
    }
    int product = 1;
    for(auto&irace:races){product *=irace.combinations;}
    cout << "Final Product: "<< product<<endl;
    return;
}
