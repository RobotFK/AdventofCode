#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
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
string G_pos_hash(pos&cur_pos){
	string key;
	key.append(to_string(cur_pos.x)).append(" ");
	key.append(to_string(cur_pos.y)).append(" ");
	return key;
}

int garden_d_algo(int&start_x,int&start_y,vector<vector<bool>>&city,int total_steps,bool visualsise=false){

	unordered_map<string,int> closed_list;
	queue<pos> openlist;

	//Initalise the Starting position
	pos start(start_x,start_y,0);
	openlist.push(start);
	int display=0;
	while(openlist.size()!=0){
		//Sort by lowest Cost;
        if(openlist.front().steps>total_steps){goto counting;};

		if(openlist.front().steps>display){//A little glass window to see progress we do
            display++;
			cout<<"at "<<openlist.front().steps <<" Steps, backlog: "<<openlist.size()<<endl;
			/*queue<pos> tmp_openlist = openlist;
			while(!tmp_openlist.empty()){
                cout<<tmp_openlist.front().x<<" "<<tmp_openlist.front().y<< " :"<<tmp_openlist.front().steps<<endl;
                tmp_openlist.pop();
			}*/
		}

        pos here=openlist.front();
		//Spread open list
		if(here.y != 0 && !(city[here.y-1][here.x])){//Move North
            pos newpos(here.x,here.y-1,here.steps+1);
            if(closed_list.count(G_pos_hash(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash(newpos)]=newpos.steps;}
		}
		if(here.x != city[0].size()-1 && !(city[here.y][here.x+1])){//Move East
            pos newpos(here.x+1,here.y,here.steps+1);
            if(closed_list.count(G_pos_hash(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash(newpos)]=newpos.steps;}
		}
		if(here.y != city.size()-1 && !(city[here.y+1][here.x])){//Move South
			pos newpos(here.x,here.y+1,here.steps+1);
            if(closed_list.count(G_pos_hash(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash(newpos)]=newpos.steps;}
		}
		if(here.x != 0 && !(city[here.y][here.x-1])){//Move West
			pos newpos(here.x-1,here.y,here.steps+1);
            if(closed_list.count(G_pos_hash(newpos))==0){
                    openlist.push(newpos);
                    closed_list[G_pos_hash(newpos)]=newpos.steps;
            }

		}
		//Add current Position to the closed list
		closed_list[G_pos_hash(here)]=here.steps;
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

void D_21_1(){
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
    int steps_take = 100;

    /*
    for(auto&line:city){
        for(auto plot:line){
                if(plot){cout<<'#';}else{cout<<'.';}
        } cout<<endl;
    }*/
    cout<<"Starting spread from "<<start_x<<", "<<start_y<<endl;
	int choices = garden_d_algo(start_x,start_y,city,steps_take,true);
	cout<<choices<< " choices"<<endl;

    return;
}
//1100 to high
//Testcase /w 717 dead
