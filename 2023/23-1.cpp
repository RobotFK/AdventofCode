#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

struct hike{
    unsigned int x;
    unsigned int y;
    hike(unsigned int ix, unsigned int iy) : x(ix),y(iy){}
};

struct knot{
    unsigned int x;
    unsigned int y;
    string id;
    knot(unsigned int ix, unsigned int iy) : x(ix),y(iy){}
};

void D_23_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("23-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<vector<char>> type;
    vector<vector<int>> distance;

    for(auto&iline:inputvector){
        vector<char> new_type_line;
        type.push_back(new_type_line);
        vector<int> new_distance_line;
        distance.push_back(new_distance_line);
        for(auto&ichar:iline){
            type.back().push_back(ichar);
            distance.back().push_back((ichar=='#')?-3:-2);
        }
    }

    // This stuff can be exportet to a function later
    queue<hike> hikes;
    hike starthike(1,0);
    distance[0][1]=0;
    hikes.push(starthike);

    int x;//Shorthands due to repeatet usage
    int y;
    for (; !hikes.empty(); hikes.pop()){
        x = hikes.front().x;
        y = hikes.front().y;
        //cout<<"("<<x<<" ,"<<y<<" ): ";
        if(x!=0){//Left
            if(type[y][x-1]=='.'||type[y][x-1]=='<'){//Valid direction
                if(distance[y][x-1]!= distance[y][x]-1){//Not Where we came from
                    if(distance[y][x]==-1||distance[y][x-1]<(distance[y][x]-1)){//Not Where we came from and either unvisited or reached in the past in less steps
                        distance[y][x-1]=distance[y][x]+1;
                        hike newpos(x-1,y);
                        hikes.push(newpos);
                        //cout<<"L";
                    }
                }
            }
        }
        if(x!=type[0].size()-1){//Right
            if(type[y][x+1]=='.'||type[y][x+1]=='>'){//Valid direction
                if(distance[y][x+1]!= distance[y][x]-1){//Not Where we came from
                    if(distance[y][x]==-1||distance[y][x+1]<(distance[y][x]-1)){//Not Where we came from and either unvisited or reached in the past in less steps
                        distance[y][x+1]=distance[y][x]+1;
                        hike newpos(x+1,y);
                        hikes.push(newpos);
                        //cout<<"R";
                    }
                }
            }
        }
        if(y!=0){//Up
            if(type[y-1][x]=='.'||type[y-1][x]=='^'){//Valid direction
                if(distance[y-1][x]!= distance[y][x]-1){//Not Where we came from
                    if(distance[y][x]==-1||distance[y-1][x]<(distance[y][x]-1)){//Not Where we came from and either unvisited or reached in the past in less steps
                        distance[y-1][x]=distance[y][x]+1;
                        hike newpos(x,y-1);
                        hikes.push(newpos);
                    }
                }
            }
        }
        if(y!=type.size()-1){//Down
            if(type[y+1][x]=='.'||type[y+1][x]=='v'){//Valid direction
                if(distance[y+1][x]!= distance[y][x]-1){//Not Where we came from
                    //cout<<"BP";
                    if(distance[y][x]==-1||distance[y+1][x]<(distance[y][x]-1)){//Not Where we came from and either unvisited or reached in the past in less steps
                        distance[y+1][x]=distance[y][x]+1;
                        hike newpos(x,y+1);
                        hikes.push(newpos);
                        //cout<<"D";
                    }
                }
            }
        }
        //cout<<endl;
    }

    if(false){
        for(auto&iline:distance){
            for(auto&value:iline){
                switch(value){
                case -3:
                    cout<<"#";
                    break;
                case -2:
                    cout<<".";
                    break;
                default:
                    cout<<value%9;
                }
            }
            cout<<endl;
        }
    }


    cout<<"Reached end with "<< distance[distance.size()-1][distance[0].size()-2] <<" steps"<<endl;
    return;
}
