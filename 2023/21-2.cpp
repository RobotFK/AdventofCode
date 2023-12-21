#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <math.h>
using namespace std;

struct pos{
	int x;
	int y;
	int steps;
	pos(int ix,int iy,int isteps){
        x=ix;
        y=iy;
        steps=isteps;
	}
	bool operator < (const pos& b) const {
        return steps < b.steps;
    }
};
string G_pos_hash2(pos&cur_pos){
	string key;
	key.append(to_string(cur_pos.x)).append(" ");
	key.append(to_string(cur_pos.y)).append(" ");
	return key;
}

vector<vector<int>> garden_plot_choices={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

int garden_d_algo2(const int&start_x,const int&start_y,vector<vector<bool>>&city,int total_steps,bool visualsise=false){

	unordered_map<string,int> closed_list;
	queue<pos> openlist;

	//Initalise the Starting position
	pos start(start_x,start_y,0);
	openlist.push(start);
	int max_steps=0;
	int city_max_x=city[0].size()-1;
    int city_max_y=city.size()-1;
    cout<<city_max_x<<endl;
	while(openlist.size()!=0){

        if(openlist.front().steps>total_steps){goto counting;};

		if(openlist.front().steps>max_steps){//A little glass window to see progress we do
            max_steps++;
			//cout<<"at "<<openlist.front().steps <<" Steps, backlog: "<<openlist.size()<<endl;
			/*
			queue<pos> tmp_openlist = openlist;
			while(!tmp_openlist.empty()){
                cout<<tmp_openlist.front().x<<" "<<tmp_openlist.front().y<< " :"<<tmp_openlist.front().steps<<endl;
                tmp_openlist.pop();
			}*/
		}

        pos here=openlist.front();
        //cout<<here.x<<" "<<here.y<<endl;
		//Spread open list
		if(!(city[(here.y!=0)?here.y-1:city_max_y][here.x])){//Move North
            pos newpos(here.x,(here.y!=0)?here.y-1:city_max_y,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
		}
		if(!(city[here.y][(here.x!=city_max_x)?here.x+1:0])){//Move East
		    pos newpos((here.x!=city_max_x)?here.x+1:0,here.y,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
		}
		if(!(city[(here.y!=city_max_y)?here.y+1:0][here.x])){//Move South
		    pos newpos(here.x,(here.y!=city_max_y)?here.y+1:0,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
		}
        if(!(city[here.y][(here.x!=0)?here.x-1:city_max_x])){//Move West
		    pos newpos((here.x!=0)?here.x-1:city_max_x,here.y,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
        }

		//Add current Position to the closed list
		closed_list[G_pos_hash2(here)]=here.steps;
		//Remove current Position from the open list
		openlist.pop();

	}
	counting:;
	int options =0;
	for (auto& it: closed_list) {
        //cout<<it.first<<" : "<<it.second<<endl;
        if(it.second%2==total_steps%2){options++;}
    }
    if(openlist.size()==0){cout<<"Visited all options"<<endl;}
	return options;
}

void D_21_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("21-1-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    int start_x=0;
	int start_y=0;
	bool start_found=false;
	vector<vector<bool>> city;
	for(auto&line:inputvector){
		vector<bool> newrow;
		for(auto&plot:line){
            if(plot=='#'){
                newrow.push_back(true);
            }else{
                newrow.push_back(false);
            }
            if(plot=='S'){start_found=true;}
            if(!start_found){start_x++;}
		}
		if(!start_found){start_y++;start_x=0;}
		city.push_back(newrow);
	}
    int steps_to_take = 26501365;

    /*
    for(auto&line:city){
        for(auto plot:line){
                if(plot){cout<<'#';}else{cout<<'.';}
        } cout<<endl;
    }*/
    //cout<<"Starting spread from "<<start_x<<", "<<start_y<<endl;

    vector<vector<bool>> big_garden;//To collect all variations we need a 5*5 grid of the small garden
    for (int big_row : {1,2,3,4,5}){
        for(auto row:city){
                vector<bool> newrow_big;
                newrow_big.insert(newrow_big.end(), row.begin(), row.end());
                newrow_big.insert(newrow_big.end(), row.begin(), row.end());
                newrow_big.insert(newrow_big.end(), row.begin(), row.end());
                newrow_big.insert(newrow_big.end(), row.begin(), row.end());
                newrow_big.insert(newrow_big.end(), row.begin(), row.end());
                big_garden.push_back(newrow_big);
        }
    }

    int big_start_x=(big_garden[0].size()-1)/2;
	int big_start_y=(big_garden.size()-1)/2;

    for(auto&line:big_garden){
        for(auto plot:line){
                if(plot){cout<<'#';}else{cout<<'.';}
        } cout<<endl;
    }

    return;
    long long choices =0;
    int steps_start_to_edge = ((city.size()-1)/2);
    //The expansion only works if the start is in the center, and there are uninterupted highways leading to the edges
    choices += garden_d_algo2(start_x,start_y,city,steps_to_take,true);
    if(steps_to_take>steps_start_to_edge){
        int repeats = steps_to_take-steps_start_to_edge/(city.size()/2);//How many Gardens we just completly envelop by traveling in one distance
        int sny = 0;//Y pos where you start if you only moved south on the previous start tile
        long long center_1 = garden_d_algo2(start_x,sny,city,steps_to_take,true);
        long long center_2 = garden_d_algo2(start_x,sny,city,steps_to_take-1,true);

        choices += 4*pow(center_1,repeats/2);//Formula determined by napkin math
        choices += 4*pow(center_1,repeats/2);
    }

	cout<<choices<< " choices"<<endl;

    return;
}
//1100 to high
//Testcase /w 717 dead
