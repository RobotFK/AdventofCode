#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

//Coordinates measured from the Top left with positive running towardes the bottom right
struct rock{
    int x;
    int y;
    bool can_move;
};

bool sort_most_n(rock const&l,rock const&r){
    return (l.y<r.y);
}

void tilt_n(vector<rock>&rocks,int const&max_x,int const&max_y){
    sort(rocks.begin(), rocks.end(), &sort_most_n);
    vector<int>floor(max_x+1,-1);
    for(auto&irock:rocks){
        if(!irock.can_move){
            floor[irock.x]=irock.y;
            continue;
        }
        irock.y = ++floor[irock.x];
    }
}

int load(vector<rock>&rocks,int const&max_x,int const&max_y){
    int sum = 0;
    for(auto&irock:rocks){
        if(irock.can_move){
            sum+=max_y-irock.y+1;
        }
    }
    return sum;
}

void D_14_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("14-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<rock> rocks;
    int const max_x = inputvector[0].size()-1;//is the max index
    int const max_y = inputvector.size()-1;//is the max index

    for(int i_y =0;i_y != inputvector.size();i_y++){
        for(int i_x =0;i_x != inputvector[i_y].size();i_x++){
            if(inputvector[i_y][i_x]=='.'){continue;}
            rock newrock;
            newrock.x = i_x;
            newrock.y = i_y;
            newrock.can_move = (inputvector[i_y][i_x]=='O');
            rocks.push_back(newrock);
        }
    }

    tilt_n(rocks,max_x,max_y);
    cout<<"Load "<< load(rocks,max_x,max_y)<<endl;
    return;
}
