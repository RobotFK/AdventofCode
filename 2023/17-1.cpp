#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

struct c_pos{
    unsigned int x;
    unsigned int y;
    unsigned int cost;
    bool facing_h; //If the position if facing Horizontal or Verticall (it can only move in the oposite axis next)
    c_pos(unsigned int ix,unsigned int iy,unsigned int icost,bool ifacing_h){
        x=ix;
        y=iy;
        cost=icost;
        facing_h =ifacing_h;
    };
};

string pos_to_string(const c_pos cur_pos){
	return to_string(cur_pos.x)+" "+to_string(cur_pos.y)+" "+(cur_pos.facing_h ? "H" : "V");
}

int min_heat(const int&start_x,const int&start_y,const int&end_x,const int&end_y,const vector<vector<int>>&city,const int most_straight,bool visualsise=false){

    vector<c_pos>open_list;
    unordered_map<string,int> closed_list;

    //Add the 2 Start nodes to open list;
    open_list.push_back(c_pos(start_x,start_y,0,true ));
    open_list.push_back(c_pos(start_x,start_y,0,false));

    //Create possible end nodes:
    string end_h_key = pos_to_string(c_pos(end_x,end_y,0,true ));
    string end_v_key = pos_to_string(c_pos(end_x,end_y,0,false));

    while(open_list.size()!=0){
        if(visualsise){cout<<endl<<open_list.size()<<" Open"<<endl;}
        vector<c_pos> nextlist;
        for(auto&cur_pos:open_list){

            //Test if cur_pos is in the closed list, and skip it if cur_pos.cost is worse or equal
            if(closed_list.find(pos_to_string(cur_pos)) != closed_list.end()){
                    if(closed_list[pos_to_string(cur_pos)]<=cur_pos.cost){continue;}
            }
            //if(visualsise){cout<< pos_to_string(cur_pos)<<" C:"<< cur_pos.cost<<endl;}

            if(cur_pos.facing_h){//Move North/South
                for(int i=(-1*most_straight);i<=most_straight;i++){//Y offset
                    if(i==0){continue;} //Skip not moving
                    if(cur_pos.y+i <0 || cur_pos.y+i >city.size()-1){continue;}//Out of bounds

                    unsigned short cost_increase=0;
                    for(int j=i;j!=0;j>0?j--:j++){cost_increase+=city[cur_pos.y+j][cur_pos.x];}//Adding up the cost of moving i steps to the side

                    nextlist.push_back(c_pos(cur_pos.x,cur_pos.y+i,cur_pos.cost+cost_increase,!cur_pos.facing_h));
                }
            }else{
                for(int i=(-1*most_straight);i<=most_straight;i++){//X offset
                    if(i==0){continue;} //Skip not moving
                    if(cur_pos.x+i <0 || cur_pos.x+i >city[0].size()-1){continue;}//Out of bounds

                    unsigned short cost_increase=0;
                    for(int j=i;j!=0;j>0?j--:j++){cost_increase+=city[cur_pos.y][cur_pos.x+j];}//Adding up the cost of moving i steps to the side

                    nextlist.push_back(c_pos(cur_pos.x+i,cur_pos.y,cur_pos.cost+cost_increase,!cur_pos.facing_h));
                }
            }
            closed_list[pos_to_string(cur_pos)]=cur_pos.cost;
        }
        open_list = nextlist;
        if(closed_list.find(end_h_key) != closed_list.end()){cout<<"Highscore H:"<<closed_list[end_h_key]<<endl;}
        if(closed_list.find(end_v_key) != closed_list.end()){cout<<"Highscore V:"<<closed_list[end_v_key]<<endl;}
    }
    return min(closed_list[end_h_key],closed_list[end_v_key]);
}

void D_17_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("17-1-1.txt");
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
	constexpr int start_x=0;
	constexpr int start_y=0;
	constexpr int most_straight=3;
	const unsigned int end_x=city[0].size()-1;
	const unsigned int end_y=city.size()-1;
    cout<<"("<<start_x<<","<<start_y<<") to ("<<end_x<<","<<end_y<<")"<<endl;

	int min_h = min_heat(start_x,start_y,end_x,end_y,city,most_straight,true);

	cout<< "Min_heat loss is "<<min_h<<endl;

    return;
}
//1100 to high
//Testcase /w 221 dead
//Reached Expanding 9155 positions with 291289 Dead
