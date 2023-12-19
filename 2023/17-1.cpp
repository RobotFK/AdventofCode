#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

struct pos{
	int x;
	int y;
	char last = 'O';
	//Last direction the the position moved
	//0 = No Previous direction
	short moved=1;
	int cost=0;//amount of heat lost
	int estimated_cost;//amount of cost we estimate to remain to reach the goal
	bool operator < (const pos& b) const {
        return (cost+estimated_cost) < (b.cost+b.estimated_cost);
    }
};

string pos_hash(pos&cur_pos){
	string key;
	key.append(to_string(cur_pos.x)).append(" ");
	key.append(to_string(cur_pos.y)).append(" ");
	key+=cur_pos.last;
	key.append(":").append(to_string(cur_pos.moved));
	return key;
}

string pos_only_hash(pos&cur_pos){
	string key;
	key.append(to_string(cur_pos.x)).append(" ");
	key.append(to_string(cur_pos.y)).append(" ");
	//key+=cur_pos.last;
	//key.append(":").append(to_string(cur_pos.moved));
	return key;
}

string pos_and_dir_hash(pos&cur_pos){
	string key;
	key.append(to_string(cur_pos.x)).append(" ");
	key.append(to_string(cur_pos.y)).append(" ");
	key+=cur_pos.last;
	//key.append(":").append(to_string(cur_pos.moved));
	return key;
}

int min_heat_A_starv2(int&start_x,int&start_y,int&end_x,int&end_y,vector<vector<int>>&city,bool visualsise=false){

	unordered_map<string,int> closed_list;
	unordered_map<string,int> min_traveld;
	vector<pos> openlist;

	int max_travel = 3;

	//Initalise the Starting position
	pos start;
	start.x=start_x;
	start.y=start_y;
	start.moved=0;
	start.estimated_cost=end_x-start.x + end_y-start.y; //Really we should take the absolutes here instead
	openlist.push_back(start);

	bool worse_aproach;

	int display=0;
	while(openlist.size()!=0){
		//Sort by lowest Cost;
		sort(openlist.begin(),openlist.end());

		if(display++%1000==0){//A little glass window to see progress we do
			cout<<openlist[0].estimated_cost <<" "<<openlist[0].cost<<" Open "<<openlist.size()<<endl;
		}

		//Spread open list
		if(openlist[0].y != 0 && !(openlist[0].last == 'n' && openlist[0].moved == max_travel)&& openlist[0].last != 's'){//Move North
			pos newpos;
			newpos.x = openlist[0].x;
			newpos.y = openlist[0].y-1;
			newpos.last = 'n';
			if(openlist[0].last == 'n'){newpos.moved=openlist[0].moved+1;}//Increment consecutive moves North by one if the move before was north
			newpos.cost = openlist[0].cost + city[newpos.y][newpos.x];
			newpos.estimated_cost=end_x-newpos.x + end_y-newpos.y;//We estimate that each H or V movement cost 1 (we know that it is more)
			//Test to see if we are at the end
			if(newpos.x==end_x &&newpos.y==end_y){return newpos.cost;}

			worse_aproach=false;
			if(min_traveld.count(pos_and_dir_hash(newpos))>0){//
				if(min_traveld[pos_and_dir_hash(newpos)]>newpos.moved){//We have been here already with less moves from this direction
				worse_aproach=true;
				}
			}
			if(!worse_aproach){
				//Lastly we only need to check if the position we try to visit has been not visited before
				//No check for being better needed, as our advance always finds the lowest, because we are starting from the lowest point
				if(closed_list.count(pos_only_hash(newpos))==0){openlist.push_back(newpos);min_traveld[pos_and_dir_hash(newpos)]=newpos.moved;}
			}
		}
		if(openlist[0].x != city[0].size()-1 && !(openlist[0].last == 'e' && openlist[0].moved == max_travel)&& openlist[0].last != 'w'){//Move East
			pos newpos;
			newpos.x = openlist[0].x+1;
			newpos.y = openlist[0].y;
			newpos.last = 'e';
			if(openlist[0].last == 'e'){newpos.moved=openlist[0].moved+1;}
			newpos.cost = openlist[0].cost + city[newpos.y][newpos.x];
			newpos.estimated_cost=end_x-newpos.x + end_y-newpos.y;
			if(newpos.x==end_x &&newpos.y==end_y){return newpos.cost;}
			worse_aproach=false;
			if(min_traveld.count(pos_and_dir_hash(newpos))>0){//
				if(min_traveld[pos_and_dir_hash(newpos)]>newpos.moved){//We have been here already with less moves from this direction
				worse_aproach=true;
				}
			}
			if(!worse_aproach){
			if(closed_list.count(pos_only_hash(newpos))==0){openlist.push_back(newpos);min_traveld[pos_and_dir_hash(newpos)]=newpos.moved;}
			}
		}
		if(openlist[0].y != city.size()-1 && !(openlist[0].last == 's' && openlist[0].moved == max_travel)&& openlist[0].last != 'n'){//Move South
			pos newpos;
			newpos.x = openlist[0].x;
			newpos.y = openlist[0].y+1;
			newpos.last = 's';
			if(openlist[0].last == 's'){newpos.moved=openlist[0].moved+1;}
			newpos.cost = openlist[0].cost + city[newpos.y][newpos.x];
			newpos.estimated_cost=end_x-newpos.x + end_y-newpos.y;
			if(newpos.x==end_x &&newpos.y==end_y){return newpos.cost;}
			worse_aproach=false;
			if(min_traveld.count(pos_and_dir_hash(newpos))>0){//
				if(min_traveld[pos_and_dir_hash(newpos)]>newpos.moved){//We have been here already with less moves from this direction
				worse_aproach=true;
				}
			}
			if(!worse_aproach){
			if(closed_list.count(pos_only_hash(newpos))==0){openlist.push_back(newpos);min_traveld[pos_and_dir_hash(newpos)]=newpos.moved;}
			}
		}
		if(openlist[0].x != 0 && !(openlist[0].last == 'w' && openlist[0].moved == max_travel)&& openlist[0].last != 'e'){//Move West
			pos newpos;
			newpos.x = openlist[0].x-1;
			newpos.y = openlist[0].y;
			newpos.last = 'w';
			if(openlist[0].last == 'w'){newpos.moved=openlist[0].moved+1;}
			newpos.cost = openlist[0].cost + city[newpos.y][newpos.x];
			newpos.estimated_cost=end_x-newpos.x + end_y-newpos.y;
			if(newpos.x==end_x &&newpos.y==end_y){return newpos.cost;}
			worse_aproach=false;
			if(min_traveld.count(pos_and_dir_hash(newpos))>0){//
				if(min_traveld[pos_and_dir_hash(newpos)]>newpos.moved){//We have been here already with less moves from this direction
				worse_aproach=true;
				}
			}
			if(!worse_aproach){
			if(closed_list.count(pos_only_hash(newpos))==0){openlist.push_back(newpos);min_traveld[pos_and_dir_hash(newpos)]=newpos.moved;}
			}
		}

		//Add current Position to the closed list
		closed_list[pos_only_hash(openlist[0])]=openlist[0].cost;
		//Remove current Position from the open list
		openlist.erase(openlist.begin());

	}
	cout<<"No path Found"<<endl;
	return -1;
}

void D_17_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("17-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

	vector<vector<int>> city;
	for(auto&line:inputvector){
		vector<int> newrow;
		for(auto&block:line){
			newrow.push_back(block -'0');
		}
		city.push_back(newrow);
	}
	int start_x=0;
	int start_y=0;
	int end_x=city[0].size()-1;
	int end_y=city.size()-1;
	int min_heat = min_heat_A_starv2(start_x,start_y,end_x,end_y,city,true);
	cout<< "Min_heat loss is "<<min_heat<<endl;

    return;
}
//1100 to high
//Testcase /w 717 dead
