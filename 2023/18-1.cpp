#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_18_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("18-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;
    struct dig{
        char direction;
        int distance;
        string color;
    };
    vector<dig> digs;

    for(auto&line:inputvector){
        dig newdig;
        vector<string> vs = split_string(line,' ');
        newdig.direction = vs[0][0];
        newdig.distance = stoi(vs[1]);
        newdig.color = vs[2];
        digs.push_back(newdig);
    }

    int max_x=0;
    int max_y=0;
    int min_x=0;
    int min_y=0;

    int test_pos_x =0;
    int test_pos_y =0;
    for(auto&idig:digs){
        switch(idig.direction){
        case 'R':
            test_pos_x+=idig.distance;
        break;
        case 'L':
            test_pos_x-=idig.distance;
        break;
        case 'U':
            test_pos_y-=idig.distance;
        break;
        case 'D':
            test_pos_y+=idig.distance;
        break;
        }
        //cout<<idig.direction<<" "<<idig.distance<<endl;
        if(test_pos_x>max_x){max_x=test_pos_x;}
        if(test_pos_x<min_x){min_x=test_pos_x;}
        if(test_pos_y>max_y){max_y=test_pos_y;}
        if(test_pos_y<min_y){min_y=test_pos_y;}
    }
    cout<<"X:"<<min_x<< " - "<<max_x<<endl;
    cout<<"Y:"<<min_y<< " - "<<max_y<<endl;

    int start_x= -1*min_x;
    int start_y= -1*min_y;
    cout<<"Start: "<<start_x<< " - "<<start_y<<endl;
    vector<vector<int>> terrain;
    for(int i=min_y;i<=max_y+1;i++){
        vector<int> newrow(max_x-min_x+1,0);
        terrain.push_back(newrow);
    }

    int pos_x=start_x;
    int pos_y=start_y;
    for(auto&idig:digs){
        switch(idig.direction){
        case 'R':
            for(int i=0;i<=idig.distance;i++){
                terrain[pos_y][pos_x+i]+=1;
            }
            pos_x+=idig.distance;
        break;
        case 'L':
            for(int i=0;i<=idig.distance;i++){
                terrain[pos_y][pos_x-i]+=2;
            }
            pos_x-=idig.distance;
        break;
        case 'U':
            for(int i=0;i<=idig.distance;i++){
                terrain[pos_y-i][pos_x]+=4;
            }
            pos_y-=idig.distance;
        break;
        case 'D':
            for(int i=0;i<=idig.distance;i++){
                terrain[pos_y+i][pos_x]+=8;
            }
            pos_y+=idig.distance;
        break;
        }
    }
    if(true){//Filling in
    for(auto&irow:terrain){
        bool inside=false;
        bool wall=false;
        for(auto&coord:irow){
            //Like with the pipes, we measure the bottom right corner
            if(coord==4||coord==8){
                inside=!inside;
            }
            if(coord==5||coord==10){// '/'
                if(!wall){inside=!inside;wall=true;}
                else{wall=false;}
            }
            if(coord==6||coord==9){// '\'
                if(wall){inside=!inside;wall=false;}
                else{wall=true;}
            }
            if(inside && coord==0){coord=99;}
            //if(inside){coord=99;}

        }
    }
    }
    int sum =0;
    for(auto&irow:terrain){
        for(auto&coord:irow){
            if(coord!=0){sum++;}
        }
    }
    cout<<"Sum of "<<sum<<" Cubic meters"<<endl;

    if(false){//Drawing of a map
        for(auto&irow:terrain){
            for(auto&coord:irow){
                //if(coord!=0){cout<<coord;}
                if(coord==99){cout<<'X';}
                else if(coord==90){cout<<'W';}
                else if(coord==9||coord==6){cout<<'\\';}
                else if(coord==5||coord==10){cout<<'/';}
                else if(coord==1||coord==2){cout<<'-';}
                else if(coord==4||coord==8){cout<<'I';}
                else if(coord!=0){cout<<coord;}
                else{cout<<'.';}
            }
            cout<<endl;
        }
    }
    return;
}
