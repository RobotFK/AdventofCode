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
};

string pos_hash(pos&cur_pos){
	string key;
	key.append(to_string(cur_pos.x)).append(" ");
	key.append(to_string(cur_pos.y)).append(" ");
	key+=cur_pos.last;
	key.append(":").append(to_string(cur_pos.moved));
	return key;
}


vector<pos> remove_duplicates(vector<pos>&testlist,bool visuals=false){
	if(testlist.size()<2){return testlist;}
	int removed=0;
	vector<bool> duplicate(testlist.size(),false);
	for(int i=0;i!=testlist.size()-2;i++){
		for(int j=i+1;j!=testlist.size()-1;j++){
			if(testlist[i].x == testlist[j].x &&testlist[i].y == testlist[j].y&&testlist[i].last == testlist[j].last&&testlist[i].cost <= testlist[j].cost){
				duplicate[j]=true;
				removed++;
			}
		}
	}
	vector<pos> outlist;
	for(int i=0;i!=testlist.size();i++){
		if(!duplicate[i]){
			outlist.push_back(testlist[i]);
		}
	}
	if(visuals){cout<< "Removed "<<removed<<" Dupes"<<endl;}
	return outlist;
}

bool update_list(vector<pos>&testlist,pos&testpos,bool visuals=false){
	//This needs a full function, because it updates the testlist instead, if the testpos is just an improvement
	bool updated=false;
	if(testpos.x!= 12 ||testpos.y!= 12){visuals=false;}
	for(auto&listpos:testlist){
		if(listpos.x != testpos.x ||listpos.y != testpos.y){continue;}//Coordiante does not match
		if(listpos.last != testpos.last){continue;}//Entry path does not match
		if(visuals){
			cout<< "Moved: "<<listpos.moved<<" vs "<<testpos.moved<<endl;
			cout<< "cost: "<<listpos.cost<<" vs "<<testpos.cost<<endl;
		}
		if(listpos.moved >= testpos.moved && listpos.cost >= testpos.cost){//Testpos has an equal or better path And is Cheaper or equal
			if(visuals){cout<< "UPD: "<<testpos.moved<<", "<<testpos.cost<<endl;}
			listpos.cost=testpos.cost;
			listpos.moved=testpos.moved;
			updated=true;
		}
	}
	if(!updated){//Testpos was no imporovement, but also not accounted for yet inserting
		if(visuals){
			cout<< "Inserted: "<<testpos.moved<<", "<<testpos.cost<<endl;
		}
		testlist.push_back(testpos);
		return true;
	}else{//We have updated an Entry
		return false;
	}
}

bool pos_in_list(vector<pos>&testlist,pos&testpos){
	//This needs a full function, because I only consider the position to be in the list already if it has either a higher cost or has worse n,e,s,w values
	for(auto&listpos:testlist){
		if(listpos.x != testpos.x ||listpos.y != testpos.y){continue;}
		if(listpos.last != testpos.last){continue;}//Entry path does not match
		if(listpos.moved <= testpos.moved && listpos.cost <= testpos.cost){//Testpos worse
			return true;
		}
	}
	return false;
}



int min_heat_A_starv1_1(int&start_x,int&start_y,int&end_x,int&end_y,vector<vector<int>>&city,bool visualsise=false){
	vector<pos> openlist;
	vector<vector<vector<pos>>> closedlist_city;

	int max_heat = city.size()*city[0].size()*10;
	int max_travel = 3;

	for(auto&i:city){
		vector<vector<pos>>new_closedlistrow;
		for(auto&j:city){
			vector<pos> closedlist;
			new_closedlistrow.push_back(closedlist);
		}
		closedlist_city.push_back(new_closedlistrow);
	}

	pos start;
	start.x=start_x;
	start.y=start_y;
	start.moved=0;
	openlist.push_back(start);

	int min_heat=city.size()*city[0].size()*10;
	bool path_found = false;
	int rem_dist= end_x-start_x +end_y-start_y;
	int heat_lead=rem_dist*10;
	while(openlist.size()!=0){
		int positions=openlist.size();
		if(visualsise){
			cout<<endl<<"Expanding "<<openlist.size()<<" positions"<<endl;
			//cout<<closedlist.size()<<" Dead"<<endl;
			if(rem_dist!=0){cout<<rem_dist<<" to go ("<< heat_lead<<")"<<endl;}
			if(rem_dist<1){
				cout<<"Closed points:"<<endl;
				for(auto&i:closedlist_city){
					for(auto&j:i){
						cout<<j.size();
					}cout<<endl;
				}cout<<endl;
			}
		}

		vector<pos> nextlist;

		for(int i=0;i!=openlist.size();i++){
			bool duplicate =false;
			pos&cur_pos=openlist[i];
			for(int j=i+1;j!=openlist.size();j++){
				if(cur_pos.x==openlist[j].x && cur_pos.y==openlist[j].y && cur_pos.last==openlist[j].last && cur_pos.moved>=openlist[j].moved && cur_pos.cost>=openlist[j].cost){
					duplicate=true;
				}
			}
			if(duplicate){
				//cout<<"Duplicate"<<endl;
				continue;
			}


			if(path_found){
				if(min_heat<cur_pos.cost){continue;}//Skip path if it is worse than a known solution
				if(min_heat<cur_pos.cost){continue;}
			}else{
				int temperature=10;//How much detour we allow
				if(heat_lead+temperature<cur_pos.cost){cout<<"Too slow";continue;}
			}
			if(end_x==cur_pos.x&&end_y==cur_pos.y){
				cout<<"Reached end"<<endl;
				if(min_heat>cur_pos.cost){
					heat_lead=cur_pos.cost;
					path_found=true;
				}
			}

			if(end_x-cur_pos.x+end_y-cur_pos.y < rem_dist){
					rem_dist=end_x-cur_pos.x+end_y-cur_pos.y;//Update shortest dist
					heat_lead=cur_pos.cost;
			}

			//cout<<"O "<< cur_pos.x<<","<<cur_pos.y<<" "<<cur_pos.last<<":"<<cur_pos.moved<<" c:"<<cur_pos.cost<<endl;
			if(cur_pos.y != 0 && !(cur_pos.last == 'n' && cur_pos.moved == 3)&& cur_pos.last != 's'){//Move North
				pos newpos;
				newpos.x = cur_pos.x;
				newpos.y = cur_pos.y-1;
				newpos.last = 'n';
				if(cur_pos.last == 'n'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)&&!pos_in_list(nextlist,newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"N,";
					}
				}
			}
			if(cur_pos.x != city[0].size()-1 && !(cur_pos.last == 'e' && cur_pos.moved == 3)&& cur_pos.last != 'w'){//Move East
				pos newpos;
				newpos.x = cur_pos.x+1;
				newpos.y = cur_pos.y;
				newpos.last = 'e';
				if(cur_pos.last == 'e'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)&&!pos_in_list(nextlist,newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"E,";
					}
				}
			}
			if(cur_pos.y != city.size()-1 && !(cur_pos.last == 's' && cur_pos.moved == 3)&& cur_pos.last != 'n'){//Move South
				pos newpos;
				newpos.x = cur_pos.x;
				newpos.y = cur_pos.y+1;
				newpos.last = 's';
				if(cur_pos.last == 's'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)&&!pos_in_list(nextlist,newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"S,";
					}
				}
			}
			if(cur_pos.x != 0 && !(cur_pos.last == 'w' && cur_pos.moved == 3)&& cur_pos.last != 'e'){//Move West
				pos newpos;
				newpos.x = cur_pos.x-1;
				newpos.y = cur_pos.y;
				newpos.last = 'w';
				if(cur_pos.last == 'w'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"W,";
					}
				}
			}
			if(!pos_in_list(closedlist_city[cur_pos.x][cur_pos.y],cur_pos)){
				update_list(closedlist_city[cur_pos.x][cur_pos.y],cur_pos);
			}

		}
		openlist = nextlist;
		//cout<<"Pre rem";
		//closedlist_city[cur_pos.x][cur_pos.y] = remove_duplicates(closedlist_city[cur_pos.x][cur_pos.y]);
		//cout<<"Post rem";

	}
	//int min_heat=city.size()*city[0].size()*10;
	for(auto&ipos:closedlist_city[end_y][end_x]){
		if(ipos.x <end_x-1 ||ipos.y <end_y-1){continue;}
		cout<<" "<< ipos.x<<","<<ipos.y<< " "<<ipos.last<<": "<<ipos.moved<<" c:"<<ipos.cost<<endl;
		if(ipos.x !=end_x ||ipos.y !=end_y){continue;}
		if(min_heat>=ipos.cost){
			if(visualsise){cout<<"G: "<< ipos.x<<","<<ipos.y<< " "<<ipos.last<<": "<<ipos.moved<<" c:"<<ipos.cost<<endl;}
			min_heat=ipos.cost;
		}
	}
	return min_heat;
}

int min_heat_A_starv2(int&start_x,int&start_y,int&end_x,int&end_y,vector<vector<int>>&city,bool visualsise=false){
	vector<pos> openlist;
	unordered_map<string,int>closedlist;

	int max_heat = city.size()*city[0].size()*10;
	int max_travel = 3;

	for(auto&i:city){//Construct closed lists
		vector<vector<pos>>new_closedlistrow;
		for(auto&j:city){
			vector<pos> closedlist;
			new_closedlistrow.push_back(closedlist);
		}
		closedlist_city.push_back(new_closedlistrow);
	}

	pos start;
	start.x=start_x;
	start.y=start_y;
	start.moved=0;
	openlist.push_back(start);

	int min_heat=city.size()*city[0].size()*10;
	bool path_found = false;
	int rem_dist= end_x-start_x +end_y-start_y;
	int heat_lead=rem_dist*10;
	while(openlist.size()!=0){
		int positions=openlist.size();
		if(visualsise){
			cout<<endl<<"Expanding "<<openlist.size()<<" positions"<<endl;
			//cout<<closedlist.size()<<" Dead"<<endl;
			if(rem_dist!=0){cout<<rem_dist<<" to go ("<< heat_lead<<")"<<endl;}
			if(rem_dist<1){
				cout<<"Closed points:"<<endl;
				for(auto&i:closedlist_city){
					for(auto&j:i){
						cout<<j.size();
					}cout<<endl;
				}cout<<endl;
			}
		}

		vector<pos> nextlist;

		for(int i=0;i!=openlist.size();i++){
			bool duplicate =false;
			pos&cur_pos=openlist[i];
			for(int j=i+1;j!=openlist.size();j++){
				if(cur_pos.x==openlist[j].x && cur_pos.y==openlist[j].y && cur_pos.last==openlist[j].last && cur_pos.moved>=openlist[j].moved && cur_pos.cost>=openlist[j].cost){
					duplicate=true;
				}
			}
			if(duplicate){
				//cout<<"Duplicate"<<endl;
				continue;
			}


			if(path_found){
				if(min_heat<cur_pos.cost){continue;}//Skip path if it is worse than a known solution
				if(min_heat<cur_pos.cost){continue;}
			}else{
				int temperature=10;//How much detour we allow
				if(heat_lead+temperature<cur_pos.cost){cout<<"Too slow";continue;}
			}
			if(end_x==cur_pos.x&&end_y==cur_pos.y){
				cout<<"Reached end"<<endl;
				if(min_heat>cur_pos.cost){
					heat_lead=cur_pos.cost;
					path_found=true;
				}
			}

			if(end_x-cur_pos.x+end_y-cur_pos.y < rem_dist){
					rem_dist=end_x-cur_pos.x+end_y-cur_pos.y;//Update shortest dist
					heat_lead=cur_pos.cost;
			}

			//cout<<"O "<< cur_pos.x<<","<<cur_pos.y<<" "<<cur_pos.last<<":"<<cur_pos.moved<<" c:"<<cur_pos.cost<<endl;
			if(cur_pos.y != 0 && !(cur_pos.last == 'n' && cur_pos.moved == 3)&& cur_pos.last != 's'){//Move North
				pos newpos;
				newpos.x = cur_pos.x;
				newpos.y = cur_pos.y-1;
				newpos.last = 'n';
				if(cur_pos.last == 'n'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)&&!pos_in_list(nextlist,newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"N,";
					}
				}
			}
			if(cur_pos.x != city[0].size()-1 && !(cur_pos.last == 'e' && cur_pos.moved == 3)&& cur_pos.last != 'w'){//Move East
				pos newpos;
				newpos.x = cur_pos.x+1;
				newpos.y = cur_pos.y;
				newpos.last = 'e';
				if(cur_pos.last == 'e'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)&&!pos_in_list(nextlist,newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"E,";
					}
				}
			}
			if(cur_pos.y != city.size()-1 && !(cur_pos.last == 's' && cur_pos.moved == 3)&& cur_pos.last != 'n'){//Move South
				pos newpos;
				newpos.x = cur_pos.x;
				newpos.y = cur_pos.y+1;
				newpos.last = 's';
				if(cur_pos.last == 's'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)&&!pos_in_list(nextlist,newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"S,";
					}
				}
			}
			if(cur_pos.x != 0 && !(cur_pos.last == 'w' && cur_pos.moved == 3)&& cur_pos.last != 'e'){//Move West
				pos newpos;
				newpos.x = cur_pos.x-1;
				newpos.y = cur_pos.y;
				newpos.last = 'w';
				if(cur_pos.last == 'w'){newpos.moved=cur_pos.moved+1;}
				newpos.cost = cur_pos.cost + city[newpos.y][newpos.x];
				if(!pos_in_list(closedlist_city[newpos.y][newpos.x],newpos)){
					if(update_list(nextlist,newpos)){
						//cout<<"W,";
					}
				}
			}
			if(!pos_in_list(closedlist_city[cur_pos.x][cur_pos.y],cur_pos)){
				update_list(closedlist_city[cur_pos.x][cur_pos.y],cur_pos);
			}

		}
		openlist = nextlist;
		//cout<<"Pre rem";
		//closedlist_city[cur_pos.x][cur_pos.y] = remove_duplicates(closedlist_city[cur_pos.x][cur_pos.y]);
		//cout<<"Post rem";

	}
	//int min_heat=city.size()*city[0].size()*10;
	for(auto&ipos:closedlist_city[end_y][end_x]){
		if(ipos.x <end_x-1 ||ipos.y <end_y-1){continue;}
		cout<<" "<< ipos.x<<","<<ipos.y<< " "<<ipos.last<<": "<<ipos.moved<<" c:"<<ipos.cost<<endl;
		if(ipos.x !=end_x ||ipos.y !=end_y){continue;}
		if(min_heat>=ipos.cost){
			if(visualsise){cout<<"G: "<< ipos.x<<","<<ipos.y<< " "<<ipos.last<<": "<<ipos.moved<<" c:"<<ipos.cost<<endl;}
			min_heat=ipos.cost;
		}
	}
	return min_heat;
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
	cout<<city[0].size();
	int end_x=city[0].size()-1;
	int end_y=city.size()-1;
	int min_heat = min_heat_A_starv2(start_x,start_y,end_x,end_y,city,true);
	cout<< "Min_heat loss is "<<min_heat<<endl;

    return;
}
//Testcase /w 717 dead
