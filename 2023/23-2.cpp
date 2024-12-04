#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>
using namespace std;

struct pos{
    unsigned int x;
    unsigned int y;
    unsigned int px;//previous x
    unsigned int py;//previous y
    unsigned int cost=1;
    pos(unsigned int ix, unsigned int iy,unsigned int ipx, unsigned int ipy) : x(ix), y(iy), px(ipx), py(ipy){}
    void _move(int ix,int iy){
        px = x;
        py = y;
        x+=ix;
        y+=iy;
        cost++;
    }
};

struct knot{ // Knot is a location where the path splits
    unsigned int x;
    unsigned int y;
    string name;
    int id;
    knot(unsigned int ix, unsigned int iy,int iid,string iname) : x(ix), y(iy),id(iid), name(iname){}
    vector<pair<knot*,int>> connected; // Pair of Connected Knots and their Cost to reach it
    int start_dist = -1; // Distance of knots to reach the start
};

struct path{
    unsigned int knot_set=1;//32 Bit state of Visited Knots
    short int cur=0; // Is used as a small integer to record current position
    int sum=0;//Sum the current path
    path(int iknot_set,short int icur,int isum): knot_set(iknot_set), cur(icur), sum(isum){}
    bool contains(int test){return (knot_set>>test)%1;}
};

bool is_knot(unsigned int&x,unsigned int&y,vector<knot> knots){
    for(auto&iknot:knots){
        if(iknot.x == x && iknot.y == y){return true;}
    }
    return false;

};

void map_start_distvector(vector<knot>&knots,bool visuals=false){
    bool all_dist = false;
    int spread_dist = -1;
    while(!all_dist){
        all_dist=true;
        spread_dist++;
        for(auto&iknot:knots){
            if(iknot.start_dist != spread_dist){continue;}
            for(auto&ipair:iknot.connected){
                if(ipair.first->start_dist != -1){continue;}
                all_dist = false;
                ipair.first->start_dist=iknot.start_dist+1;
            }
        }
    }
}

map<int,vector<int>> map_edges(vector<knot>&knots,bool visuals=false){
    map<int,vector<int>> edges;
    for(auto&iknot:knots){
        vector<int> directions;
        for(auto&ipair:iknot.connected){
           if(iknot.connected.size()==3){
                //if(visuals){cout<<endl<<iknot.name<<"->"<< ipair.first->name<<"(E): ";}
                if(ipair.first->start_dist == iknot.start_dist-1 && ipair.first->connected.size()==3){
                    //Hitting a Knot connected to 3 others means that we are on the Edge of the grid and going to a other Edge node that is closer to the start isn't usefull as it will trap us
                    continue;
                }
            }
            directions.push_back(ipair.first->id);
        }
        if(visuals){cout<<iknot.id<<":";for(auto&idirection:directions){cout<<" "<<idirection;}cout<<endl;}
        edges[iknot.id]=directions;
    }
    return edges;
}

map<pair<int,int>,int> map_cost(vector<knot>&knots,bool visuals=false){
    map<pair<int,int>,int> map_cost;
    for(auto&iknot:knots){
        for(auto&ipair:iknot.connected){
            map_cost[{iknot.id,ipair.first->id}]=ipair.second;
            if(visuals){
                cout<<iknot.id<<":"<<ipair.first<<" -> "<<ipair.second<<endl;}
        }
    }
    return map_cost;
}

int dsf_recursive(int knot_id,vector<bool> visited,int current_length,int dest_id,int highest,map<int,vector<int>> &edges,map<pair<int,int>,int> &cost){
    if(knot_id==dest_id){
        if(current_length>highest){highest=current_length;
        cout<<"New Highscore: "<<highest<<endl;}
        return highest;
    }
    visited[knot_id]=true;
    for(auto&neighbor_id:edges[knot_id]){
        if(!visited[neighbor_id]){
            highest = dsf_recursive(neighbor_id, visited, current_length + cost[{knot_id,neighbor_id}],dest_id,highest,edges,cost);
        }
    }
    visited[knot_id]=false;
    return highest;
}

void D_23_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("23-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<vector<char>> type;
    vector<knot> knots;

    for(auto&iline:inputvector){
        vector<char> new_type_line;
        type.push_back(new_type_line);
        for(auto&ichar:iline){
            type.back().push_back(ichar);
        }
    }

    int id_counter = 0;
    string name = "";
    for(unsigned iy=1;iy!=type.size()-1;iy++){
        for(unsigned ix=1;ix!=type[0].size()-1;ix++){
            if(type[iy][ix]!='.'){continue;}
            if(type[iy+1][ix]=='.' ||
               type[iy-1][ix]=='.' ||
               type[iy][ix+1]=='.' ||
               type[iy][ix-1]=='.'
               ){continue;}

            name+=(id_counter<26)?id_counter+'A':id_counter+'A'+6;
            id_counter++;
            //cout<<"Created Knot "<<name<<endl;
            knot newknow(ix,iy,id_counter,name);
            knots.push_back(newknow);
            name ="";
        }
    }

    //cout<<endl;
    //Adding start and end Coordinates:
    knot start(1,0,++id_counter,"start");
    start.start_dist=0;
    knots.push_back(start);

    knot end_k(type[0].size()-2,type.size()-1,++id_counter,"end");
    knots.push_back(end_k);

    cout<<"Id Counter at "<<id_counter<<endl;

    //cout<<"There are "<<knots.size()<<" knots"<<endl; //36
    //We can bring this down by ignoring Start,end and the first position and asuming the final (pre end) knot has not been visited

    for(auto&iknot:knots){//Connecting the Knots to each other knot they can reach
        queue<pos>positions; //Running list of positions that might be a Knot to connect to
        if(iknot.y !=0){//Up
            if(type[iknot.y-1][iknot.x]!='#'){positions.emplace(pos(iknot.x,iknot.y-1,iknot.x,iknot.y));}
        }
        if(iknot.y !=type.size()-1){//Down
            if(type[iknot.y+1][iknot.x]!='#'){positions.emplace(pos(iknot.x,iknot.y+1,iknot.x,iknot.y));}
        }
        if(iknot.x !=0){//Left
            if(type[iknot.y][iknot.x-1]!='#'){positions.emplace(pos(iknot.x-1,iknot.y,iknot.x,iknot.y));}
        }
        if(iknot.x !=type[0].size()-1){//Right
            if(type[iknot.y][iknot.x+1]!='#'){positions.emplace(pos(iknot.x+1,iknot.y,iknot.x,iknot.y));}
        }
        for (; !positions.empty(); positions.pop()){//For each Path from the Knot we have found
            //cout<<"("<<positions.front().px<<","<<positions.front().py<<")->("<<positions.front().x<<","<<positions.front().y<<")"<<endl;
            while(!is_knot(positions.front().x,positions.front().y,knots)){
                //if(iknot.name == "D"){cout<<positions.front().x<<" - "<<positions.front().y<<endl;}
                if(type[positions.front().y-1][positions.front().x]!='#' &&
                   positions.front().y-1 != positions.front().py){
                    positions.front()._move(0,-1);
                }else if(type[positions.front().y+1][positions.front().x]!='#' &&
                   positions.front().y+1 != positions.front().py){
                    positions.front()._move(0,1);
                }else if(type[positions.front().y][positions.front().x-1]!='#' &&
                   positions.front().x-1 != positions.front().px){
                    positions.front()._move(-1,0);
                }else if(type[positions.front().y][positions.front().x+1]!='#' &&
                   positions.front().x+1 != positions.front().px){
                    positions.front()._move(1,0);
                }
            }
            //cout<<"Knot ready to connected"<<endl;
            for(auto&targetknot:knots){
                if(targetknot.x==positions.front().x &&targetknot.y==positions.front().y){
                    iknot.connected.push_back(make_pair(&targetknot,positions.front().cost));
                    break;
                }
            }
        }//cout<<endl;
    }
    cout << "Knots linked" << endl;

    //Now, this modification only works because start and end are both only linked to a single knot
    int basecost;//Path from 'start' to the start_knot_id
    int start_knot_id;//Not 'start',but the first knot we have a choice from
    int endcost;//Path from 'end' to the only knot connected to the end
    int end_knot_id;//Not 'end',but the only knot connected to the end

    for(auto&iknot:knots){ //Find the nodes to exclude
        if(iknot.name=="start"){
            cout<<"Internal start at: "<<iknot.connected[0].first->name<<endl;
            basecost=iknot.connected[0].second;
            start_knot_id=iknot.connected[0].first->id;
        }
        else if(iknot.name=="end"){
            cout<<"Internal End at: "<<iknot.connected[0].first->name<<endl;
            endcost=iknot.connected[0].second;
            end_knot_id=iknot.connected[0].first->id;
        }
    }

    for(auto&iknot:knots){//Push the exliminated knots to the highest id's (they can't be eliminated as the Id's will serve in the map of connections later)
        if(iknot.id==start_knot_id){iknot.id=knots.size()-1;continue;}
        if(iknot.id==end_knot_id){iknot.id=knots.size();continue;}
        int lshif = 0;
        if(iknot.id>start_knot_id){lshif++;}
        if(iknot.id>end_knot_id){lshif++;}
        iknot.id-=lshif;
    }
    start_knot_id=knots.size()-1;
    end_knot_id=knots.size();


    if(true){
        for(auto&iknot:knots){
            cout<< iknot.name << " : "<< iknot.id<<endl;
        }
    }

    map<int,vector<int>> edges = map_edges(knots);
    map<pair<int,int>,int> cost = map_cost(knots);

    vector<bool> visited;
    for(auto&iknot:knots){visited.push_back(false);}

    int  highest=0;
    highest = dsf_recursive(start_knot_id,visited,basecost,end_knot_id,highest,edges,cost);//After Taking Advice this is what allow this Programm to finish in Human runtime

    cout<<"Reached end with "<< highest+ endcost<<" steps"<<endl;//6030<highest<9811 Is needed !6391
    return;
}
