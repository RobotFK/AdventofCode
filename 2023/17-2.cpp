#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

struct pos{
	int x;
	int y;
	short n=0;//How many tiles we moved North in a row (0-3)
	short e=0;//How many tiles we moved East in a row (0-3)
	short s=0;//How many tiles we moved South in a row (0-3)
	short w=0;//How many tiles we moved West in a row (0-3)
	int cost=0;//amount of heat lost
};

bool update_list2(vector<pos>&testlist,pos&testpos){
	//This needs a full function, because I only consider the position to be in the list already if it has either a higher cost or has worse n,e,s,w values
	//Additionally this function updates the testlist instead, if the testpos is just an improvement
	bool updated=false;
	for(auto&listpos:testlist){
		if(listpos.x != testpos.x ||listpos.y != testpos.y){continue;}//Coordiante does not match
		if(listpos.n == testpos.n && listpos.e == testpos.e && listpos.s == testpos.s && listpos.w == testpos.w){//Entry Path is exact
			if(listpos.cost > testpos.cost){
				listpos.cost=testpos.cost;
				updated=true;
			}else{return false;}//Exact match, but worse cost
		}
		if((listpos.n > testpos.n &&testpos.n!=0) || (listpos.e > testpos.e&&testpos.e!=0) || (listpos.s > testpos.s&&testpos.s!=0) || (listpos.w > testpos.w&&testpos.w!=0)){//Testpos has a better path
			if(listpos.cost > testpos.cost){//And is Cheaper
				listpos.cost=testpos.cost;
				listpos.n=testpos.n;
				listpos.e=testpos.e;
				listpos.s=testpos.s;
				listpos.w=testpos.w;
				updated=true;
			}
		}
	}
	if(!updated){//Testpos was no imporovement, but also not accounted for yet inserting
		testlist.push_back(testpos);
		return false;
	}else{
		return false;
	}

}

bool pos_in_list2(vector<pos>&testlist,pos&testpos){
	//This needs a full function, because I only consider the position to be in the list already if it has either a higher cost or has worse n,e,s,w values
	for(auto&listpos:testlist){
		if(listpos.x != testpos.x ||listpos.y != testpos.y){continue;}
		if(listpos.cost > testpos.cost){return false;}
		if(listpos.n == testpos.n && listpos.e == testpos.e && listpos.s == testpos.s && listpos.w == testpos.w){return true;}//Entry is exact
		if(listpos.n >= testpos.n && listpos.e >= testpos.e && listpos.s >= testpos.s && listpos.w >= testpos.w){return false;}//Entry is in some form worse
	}
	return false;
}

int min_heat_A_star2(int&start_x,int&start_y,int&end_x,int&end_y,vector<vector<int>>&city,bool visualsise=false){

	vector<pos> openlist;
	vector<pos> closedlist;

	pos start;
	start.x=start_x;
	start.y=start_y;
	openlist.push_back(start);

	while(openlist.size()!=0){
		int positions=openlist.size();
		if(visualsise){
				cout<<"Expanding "<<openlist.size()<<" positions"<<endl;
				cout<<closedlist.size()<<" Dead"<<endl;
				if(openlist.size()>=100000){
					for(auto&ipos:openlist){
						if(ipos.x!=4 ||ipos.y!=4){continue;}
						cout<< ipos.x<<","<<ipos.y<< " :"<<ipos.n<<" "<<ipos.e<<" "<<ipos.s<<" "<<ipos.w<<" c:"<<ipos.cost<<endl;
					}

					return 0;
				}
		}

		vector<pos> nextlist;
		for(auto&cur_pos:openlist){
			if(cur_pos.y != 0 && cur_pos.n != 3 && cur_pos.s == 0){//Move North
				pos newpos;
				newpos.x = cur_pos.x;
				newpos.y = cur_pos.y-1;
				newpos.n = cur_pos.n+1;
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list2(closedlist,newpos)){
					update_list2(nextlist,newpos);
					//nextlist.push_back(newpos);
				}
			}
			if(cur_pos.x != city.size()-1 && cur_pos.e != 3 && cur_pos.w == 0){//Move East
				pos newpos;
				newpos.x = cur_pos.x+1;
				newpos.y = cur_pos.y;
				newpos.e = cur_pos.e+1;
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list2(closedlist,newpos)){
					update_list2(nextlist,newpos);
				}
			}
			if(cur_pos.y != city.size()-1 && cur_pos.s != 3 && cur_pos.n == 0){//Move South
				pos newpos;
				newpos.x = cur_pos.x;
				newpos.y = cur_pos.y+1;
				newpos.s = cur_pos.s+1;
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list2(closedlist,newpos)){
					update_list2(nextlist,newpos);
				}
			}
			if(cur_pos.x != 0 && cur_pos.w != 3 && cur_pos.e == 0){//Move West
				pos newpos;
				newpos.x = cur_pos.x-1;
				newpos.y = cur_pos.y;
				newpos.w = cur_pos.w+1;
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list2(closedlist,newpos)){
					update_list2(nextlist,newpos);
				}
			}
			update_list2(closedlist,cur_pos);
		}
		openlist = nextlist;
	}
	int min_heat=city.size()*city[0].size()*10;
	for(auto&ipos:closedlist){
		if(ipos.x !=end_x ||ipos.y !=end_y){continue;}
		if(visualsise){cout<< ipos.x<<","<<ipos.y<< " :"<<ipos.n<<" "<<ipos.e<<" "<<ipos.s<<" "<<ipos.w<<" c:"<<ipos.cost<<endl;}
		if(min_heat>=ipos.cost){min_heat=ipos.cost;}
	}
	return min_heat;
}

void D_17_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("17-1-2.txt");
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

	int min_heat = min_heat_A_star2(start_x,start_y,end_x,end_y,city);
	cout<< "Min_heat loss is "<<min_heat<<endl;

    return;
}
