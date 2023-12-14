#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>
using namespace std;

//It worked before, lets do it again
unordered_map<string,long>known_platforms;

//Coordinates measured from the Top left with positive running towardes the bottom right
struct rock{
    int x;
    int y;
    bool can_move;
};


string platform_hash(vector<rock>&rocks,int const&max_y){
	vector<int>rocks_pos;
	for(auto&irock:rocks){
        if(irock.can_move){
            //cout<< irock.y<<" + "<<irock.x<<" = "<<irock.x+irock.y*(max_y+1)<<endl;
            rocks_pos.push_back(irock.x+irock.y*(max_y+1));
        }
	}
	string output;
	sort(rocks_pos.begin(),rocks_pos.end(),greater<int>());
	for(auto&rock:rocks_pos){output.append(to_string(rock)).append(" ");}
	return output;
}

bool sort_most_n2(rock const&l,rock const&r){
    return (l.y<r.y);}

bool sort_most_e(rock const&l,rock const&r){
    return (l.x>r.x);}

bool sort_most_s(rock const&l,rock const&r){
    return (l.y>r.y);}

bool sort_most_w(rock const&l,rock const&r){
    return (l.x<r.x);}


void tilt_n2(vector<rock>&rocks,int const&max_x,int const&max_y){
    sort(rocks.begin(), rocks.end(), &sort_most_n2);
    vector<int>floor(max_x+1,-1);
    for(auto&irock:rocks){
        if(!irock.can_move){
            floor[irock.x]=irock.y;
            continue;
        }
        irock.y = ++floor[irock.x];
    }
}

void tilt_e(vector<rock>&rocks,int const&max_x,int const&max_y){
    sort(rocks.begin(), rocks.end(), &sort_most_e);
    vector<int>floor(max_y+1,max_x+1);
    for(auto&irock:rocks){
        if(!irock.can_move){
            floor[irock.y]=irock.x;
            continue;
        }
        irock.x = --floor[irock.y];
    }
}

void tilt_s(vector<rock>&rocks,int const&max_x,int const&max_y){
    sort(rocks.begin(), rocks.end(), &sort_most_s);
    vector<int>floor(max_x+1,max_y+1);
    for(auto&irock:rocks){
        if(!irock.can_move){
            floor[irock.x]=irock.y;
            continue;
        }
        irock.y = --floor[irock.x];
    }
}

void tilt_w(vector<rock>&rocks,int const&max_x,int const&max_y){
    sort(rocks.begin(), rocks.end(), &sort_most_w);
    vector<int>floor(max_y+1,-1);
    for(auto&irock:rocks){
        if(!irock.can_move){
            floor[irock.y]=irock.x;
            continue;
        }
        irock.x = ++floor[irock.y];
    }
}

int load2(vector<rock>&rocks,int const&max_x,int const&max_y){
    int sum = 0;
    for(auto&irock:rocks){
        if(irock.can_move){
            sum+=max_y-irock.y+1;
        }
    }
    return sum;
}

void display_platform(vector<rock>&rocks,int const&max_x,int const&max_y){
    vector<string> platform;
    for(int i=0;i!=max_y+1;i++){//Each line
        string newstring(max_x+1,'.');
        platform.push_back(newstring);
    }
    for(auto&irock:rocks){
     if(irock.can_move){
         platform[irock.y][irock.x]='O';
     }else{
        platform[irock.y][irock.x]='#';
     }
    }
    for(auto&line:platform){cout<<line<<endl;}cout<<endl;
    return;
}

void D_14_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("14-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<rock> rocks;
    int const max_x = inputvector[0].size()-1;//is the maximum possible index
    int const max_y = inputvector.size()-1;//is the maximum possible index

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

    long cycels = 1000000000;
    long cycle_start=-1;
    bool repetition=false;
    string cycle_first;
    for(long i=0;i<cycels;i++){
        tilt_n2(rocks,max_x,max_y);
        tilt_w(rocks,max_x,max_y);
        tilt_s(rocks,max_x,max_y);
        tilt_e(rocks,max_x,max_y);
        long cur_load =load2(rocks,max_x,max_y);
        //cout<<"After Cycel "<<i+1<<" Load =\t"<<cur_load<<endl;
        //display_platform(rocks,max_x,max_y);
        string cur_hash = platform_hash(rocks,max_y);
        //cout<<"i="<<i<<": "<<cur_hash<<endl;
        if(known_platforms.count(cur_hash)){//Repetition found
            if(!repetition){//Start of repetition
                repetition=true;
                cout<<"First instance of repetition at i:"<<i<<endl;
                cycle_start=i;
                cycle_first=cur_hash;
            }
            else if (cycle_first==cur_hash){
                int cycle_len =i-cycle_start;
                cout<< "cycle found of len "<< cycle_len<<endl;
                if(cycels-i>cycle_len){//Enough time left to Skip
                    cout<< cycels-i << "/"<< cycle_len<<" ="<< (cycels-i)/(cycle_len) <<endl;
                    cout<<"Skipping "<< floor((cycels-i)/(cycle_len)) <<" Floors"<<endl;
                    i+=floor((cycels-i)/(cycle_len))*cycle_len;
                    continue;
                };
            }
        }else{
            long cur_load =load2(rocks,max_x,max_y);
            known_platforms[cur_hash]=cur_load;
        }
    }

    cout<<"Load "<< load2(rocks,max_x,max_y)<<endl;
    return;
}
