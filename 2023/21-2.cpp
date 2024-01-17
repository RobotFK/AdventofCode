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
	unsigned int x;
	unsigned int y;
	unsigned int steps;
	pos(unsigned int ix,unsigned int iy,unsigned int isteps){
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
	key.append(to_string(cur_pos.x)).append(" ").append(to_string(cur_pos.y));
	return key;
}

long count_big_garden(const int&start_x,const int&start_y,vector<vector<bool>>&big_garden,int steps_to_take,bool visualsise=false){
	unordered_map<string,int> closed_list;
	queue<pos> openlist;

	pos start(start_x,start_y,0);
	openlist.push(start);
    const int garden_max_x=big_garden[0].size()-1;
    const int garden_max_y=big_garden.size()-1;
    cout<<garden_max_x<<endl;
	while(openlist.size()!=0){
        if(openlist.front().steps>steps_to_take){goto counting;};
        pos here=openlist.front();
        //cout<<here.x<<" "<<here.y<<endl;
		//Spread open list
		if(!(big_garden[(here.y!=0)?here.y-1:garden_max_y][here.x])){//Move North
            pos newpos(here.x,(here.y!=0)?here.y-1:garden_max_y,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
		}
		if(!(big_garden[here.y][(here.x!=garden_max_x)?here.x+1:0])){//Move East
		    pos newpos((here.x!=garden_max_x)?here.x+1:0,here.y,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
		}
		if(!(big_garden[(here.y!=garden_max_y)?here.y+1:0][here.x])){//Move South
		    pos newpos(here.x,(here.y!=garden_max_y)?here.y+1:0,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
		}
        if(!(big_garden[here.y][(here.x!=0)?here.x-1:garden_max_x])){//Move West
		    pos newpos((here.x!=0)?here.x-1:garden_max_x,here.y,here.steps+1);
            if(closed_list.count(G_pos_hash2(newpos))==0){openlist.push(newpos);closed_list[G_pos_hash2(newpos)]=newpos.steps;}
        }

		//Add current Position to the closed list
		closed_list[G_pos_hash2(here)]=here.steps;
		//Remove current Position from the open list
		openlist.pop();
	}
	counting:;
	cout<<"Starting new count Positions:"<<closed_list.size() <<endl;
	//cout<<"Max:"<<big_garden.size()<<","<<big_garden[0].size()<<endl;
	vector<vector<int>> result={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
	long sum =0;
	for (auto& it: closed_list) {
        if(it.second%2 !=(steps_to_take%2)){continue;} //THIS LINE COST ME OVER A WEEK
        string key = it.first;
        vector<string> coord = split_string(key,' ');
        int x = stoi(coord[0]);
        int y = stoi(coord[1]);
        int section_x =floor((x/((big_garden.size())/5)));
        int section_y =floor((y/((big_garden[0].size())/5)));
        if(section_x > 4 || section_y > 4){cout <<x<<","<<y<< " -> "<<section_x<<","<<section_y<<endl;}
        result[section_y][section_x]++;
        sum++;
    }
    cout<<steps_to_take<<" steps : "<<sum<<endl;
	return sum;
}

void D_21_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("21-1.txt");
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
            newrow.push_back((plot=='#'));
            if(plot=='S'){start_found=true;}
            if(!start_found){start_x++;}
		}
		if(!start_found){start_y++;start_x=0;}
		city.push_back(newrow);
	}
    constexpr int steps_to_take = 26501365;

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

    /*
    for(auto&line:big_garden){
        for(auto plot:line){
                if(plot){cout<<'#';}else{cout<<'.';}
        } cout<<endl;
    }*/

    cout<<"Prep Complete"<<endl;
    vector<vector<int>> garden_plot_choices;
    int steps_to_fill;
    if(steps_to_take>big_garden.size()*2){
        steps_to_fill = steps_to_take%big_garden.size() + (big_garden.size()/5)*2;
    }else{steps_to_fill=steps_to_take;}
    cout<<"Plottig for "<<steps_to_fill<<" Steps"<<endl;
    //garden_plot_choices = count_big_garden(big_start_x,big_start_y,big_garden,64);
    long f0 = count_big_garden(big_start_x,big_start_y,big_garden,(65+ 131*0));
    long f1 = count_big_garden(big_start_x,big_start_y,big_garden,(65+ 131*1));
    long f2 = count_big_garden(big_start_x,big_start_y,big_garden,(65+ 131*2));
    //garden_plot_choices = count_big_garden(big_start_x,big_start_y,big_garden,steps_to_fill);

    long long c = f0;
    long long a = (f2-f1-f1+f0)/2;
    long long b = (f1-f0-a);

    cout<<"a: "<<a<<endl;
    cout<<"b: "<<b<<endl;
    cout<<"c: "<<c<<endl;

    long long steps_start_to_edge = ((city.size()-1)/2);
    long long repeats = (steps_to_take-steps_start_to_edge)/(city.size());
    cout<<"repeats: "<<repeats<<endl;
    long long result = (repeats*repeats*a)+(repeats*b)+c;
    cout<< "Result: "<<result<<endl;
    //cout<< "Result is : ("<<repeats<<"*"<<repeats<<"*"<<a<<") + ("<<repeats<<"*"<<b<<") + "<< c<<endl;
    /*
    cout<<endl<<"Options:"<<endl;
    for(auto l:garden_plot_choices){
        for(auto r:l){cout<<r<<"\t";}
        cout<<endl;
    }cout<<endl;

    long long choices =0;
    int steps_start_to_edge = ((city.size()-1)/2);
    //The expansion only works if the start is in the center, and there are uninterupted highways leading to the edges

    int repeats = (steps_to_take-steps_start_to_edge)/(city.size());//How many Gardens we just completly envelop by traveling in one distance (not counting the center )

    if(steps_to_take>steps_start_to_edge){
        cout<<"Found "<<repeats<<" Repeats"<<endl;

        long long tips =garden_plot_choices[0][2]+garden_plot_choices[4][2]+garden_plot_choices[2][0]+garden_plot_choices[2][4];
        choices+= tips;
        cout<<"Tips: "<<tips<<endl;

        long long Outer_Edges = (garden_plot_choices[0][1]+garden_plot_choices[3][0]+garden_plot_choices[4][3]+garden_plot_choices[1][4])*repeats;
        choices+= Outer_Edges;
        cout<<"Outer_Edges:\t"<<Outer_Edges<<endl;

        long long inner_Edges = (garden_plot_choices[1][1]+garden_plot_choices[3][1]+garden_plot_choices[1][3]+garden_plot_choices[3][3])*(repeats-1);
        choices+= inner_Edges;
        cout<<"inner_Edges:\t"<<inner_Edges<<endl;

        long long even_Tiles = garden_plot_choices[2][2]*(repeats*repeats);
        choices+= even_Tiles;
        cout<<"e_T:\t"<<garden_plot_choices[2][2]<<"*"<<repeats<<" ^2"<<endl;
        cout<<"even_Tiles:\t"<<even_Tiles<<endl;

        long long odd_Tiles = garden_plot_choices[2][1]*((repeats-1)*(repeats-1));
        choices+= odd_Tiles;
        cout<<"odd_Tiles:\t"<<odd_Tiles<<endl;

        cout<<"Old:"<<choices<< " choices"<<endl;
    }


	choices =0;
	choices +=tips;
	choices +=(garden_plot_choices[0][1]+garden_plot_choices[3][0]+garden_plot_choices[4][3]+garden_plot_choices[1][4]);
    choices += garden_plot_choices[2][2]*(repeats*repeats);
    choices += garden_plot_choices[2][1]*(repeats*repeats);
    */
    return;
}
//1915700023 Too low
//636387219948343 Too low
//636391417883143 Too low
//636391442968281 Wrong
//636391442896839 Wrong
//655544335793105 Wrong
//636391426712747 Is correct

