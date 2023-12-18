#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

struct transition{
    int x;
    char type;
};

bool sortbyx(const transition &a, const transition &b)
{
    return a.x < b.x;
}

void D_18_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("18-1-2.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;
    struct dig{
        char direction;
        int distance;
    };
    vector<dig> digs;

    bool day_2 = false;
    if(day_2){
        for(auto&line:inputvector){
            dig newdig;
            vector<string> vs = split_string(line,' ');
            switch(vs[2][7]){
                case '0':
                newdig.direction = 'R';
                break;
                case '1':
                newdig.direction = 'D';
                break;
                case '2':
                newdig.direction = 'L';
                break;
                case '3':
                newdig.direction = 'U';
                break;
            }
            string dist =vs[2].substr(2,5);
            stringstream ss;
            ss << hex << dist;
            ss >> newdig.distance;
            digs.push_back(newdig);
        }
    }else{
        for(auto&line:inputvector){
        dig newdig;
        vector<string> vs = split_string(line,' ');
        newdig.direction = vs[0][0];
        newdig.distance = stoi(vs[1]);
        digs.push_back(newdig);
        }
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

    //return;


    struct corner{
    int x;
    int y;
    int type=0;
    };
    vector<corner> corners;

    struct V_Wall{
    int x;
    int y_start;//Lower position
    int y_end;
    int type=0;
    };
    vector<V_Wall> V_Walls;

    int pos_x=start_x;
    int pos_y=start_y;
    corner startcorner;
    startcorner.x = start_x;
    startcorner.y = start_y;
    corners.push_back(startcorner);

    //As we have learned from day 1 we only need to find Vertical boundaries and Corners
    for(auto&idig:digs){
        corner newcorner;
        //cout<<pos_x<<","<<pos_y<<" + "<<idig.direction<<endl;
        switch(idig.direction){
        case 'R':
            newcorner.x = pos_x+idig.distance;
            newcorner.y = pos_y;
            if(!(newcorner.x==start_x&&newcorner.y==start_y)){corners.push_back(newcorner);}
            for(auto&icorner:corners){
                if(icorner.x==pos_x &&icorner.y==pos_y){icorner.type+=1;}//Add Right to inital pos
                if(icorner.x==pos_x+idig.distance &&icorner.y==pos_y){icorner.type+=1;}//Add Right to New pos
            }
            pos_x+=idig.distance;
            break;
        case 'L':
            newcorner.x = pos_x-idig.distance;
            newcorner.y = pos_y;
            if(!(newcorner.x==start_x&&newcorner.y==start_y)){corners.push_back(newcorner);}
            for(auto&icorner:corners){
                if(icorner.x==pos_x &&icorner.y==pos_y){icorner.type+=2;}//Add Left to inital pos
                if(icorner.x==pos_x-idig.distance &&icorner.y==pos_y){icorner.type+=2;}//Add Left to New pos
            }
            pos_x-=idig.distance;
            break;
        case 'U':
            if(idig.distance>1){
                V_Wall new_wall;
                new_wall.x= pos_x;
                new_wall.y_start=pos_y-1;
                new_wall.y_end=pos_y-idig.distance+1;
                if(new_wall.y_start>new_wall.y_end){//Swap to make start smaller
                    int temp = new_wall.y_start;
                    new_wall.y_start= new_wall.y_end;
                    new_wall.y_end= temp;
                }
                V_Walls.push_back(new_wall);
            }

            newcorner.x = pos_x;
            newcorner.y = pos_y-idig.distance;
            if(!(newcorner.x==start_x&&newcorner.y==start_y)){corners.push_back(newcorner);}
            for(auto&icorner:corners){
                if(icorner.x==pos_x &&icorner.y==pos_y){icorner.type+=4;}//Add Up to inital pos
                if(icorner.y==pos_y-idig.distance &&icorner.x==pos_x){icorner.type+=4;}//Add Up to New pos
            }
            pos_y-=idig.distance;
            break;
        case 'D':
            if(idig.distance>1){
                V_Wall new_wall;
                new_wall.x= pos_x;
                new_wall.y_start=pos_y+1;
                new_wall.y_end=pos_y+idig.distance-1;
                if(new_wall.y_start>new_wall.y_end){//Swap to make start smaller
                    int temp = new_wall.y_start;
                    new_wall.y_start= new_wall.y_end;
                    new_wall.y_end= temp;
                }
                V_Walls.push_back(new_wall);
            }

            newcorner.x = pos_x;
            newcorner.y = pos_y+idig.distance;
            if(!(newcorner.x==start_x&&newcorner.y==start_y)){corners.push_back(newcorner);}
            for(auto&icorner:corners){
                if(icorner.x==pos_x &&icorner.y==pos_y){icorner.type+=8;}//Add Down to inital pos
                if(icorner.y==pos_y+idig.distance &&icorner.x==pos_x){icorner.type+=8;}//Add Down to New pos
            }
            pos_y+=idig.distance;
            break;
        }
    }
    if(pos_x!=start_x||pos_y!=start_y){cout<<"Paths dont meet"<<endl;return;}

    vector<int> row_with_corners;
    for(auto&icorner:corners){
        bool accounted = false;
        for(auto&y:row_with_corners){
            if(y==icorner.y){accounted=true;break;}
        }
        if(!accounted){
            row_with_corners.push_back(icorner.y);
        }
    }
    sort(row_with_corners.begin(),row_with_corners.end());
    cout<<row_with_corners.size()<<" Edge rows"<<endl;
    long long sum =0;

    for(int irow=0;irow!=row_with_corners.size();irow++){//We calculate the Volume for every row with corners and the unbroken lines following it
        vector<transition> transitions;
        int row = row_with_corners[irow];
        for(auto&iwall:V_Walls){//Adding all relevant wall transitions
            if(iwall.y_end >= row &&iwall.y_start <= row){
                transition newtransition;
                newtransition.x = iwall.x;
                newtransition.type = 'I';
                transitions.push_back(newtransition);
            }
        }
        for(auto&icorner:corners){//Adding all relevant Corner transitions
            if(icorner.y == row){
                transition newtransition;
                newtransition.x = icorner.x;
                if(icorner.type == 9||icorner.type == 6){
                    newtransition.type = '\\';
                }else if(icorner.type == 5||icorner.type == 10){
                    newtransition.type = '/';
                }else{
                    cout<<"Error, Value "<<icorner.type<<" at Coord:"<<icorner.x<<","<<icorner.y<<endl;
                }
                transitions.push_back(newtransition);
            }
        }
        sort(transitions.begin(),transitions.end(),sortbyx);
        bool inside=false;
        bool wall=false;
        long rowsum=0;
        for(int itran=0;itran!=transitions.size();itran++){
            cout<<transitions[itran].type<<" ";
            if(inside){
                cout<<transitions[itran-1].x<< "-" <<transitions[itran].x<<"  ";
                rowsum+=(transitions[itran].x-transitions[itran-1].x);
            }else if(wall){
                cout<<"W" <<transitions[itran-1].x<< "-" <<transitions[itran].x<<"  ";
                rowsum+=(transitions[itran].x-transitions[itran-1].x);
            }

            if(transitions[itran].type == 'I'){
                inside=!inside;
                //if(inside==true){rowsum++;}
                continue;
            }
            if(transitions[itran].type == '/'){
                if(!wall){inside=!inside;wall=true;}
                else{wall=false;}
            }
            if(transitions[itran].type == '\\'){
                if(wall){inside=!inside;wall=false;}
                else{wall=true;}
            }
        }
        cout<<endl<< "Row "<<row<<" ="<<rowsum<<endl;
        sum+=rowsum;
        if(irow==row_with_corners.size()-1){continue;}//Last row, does not have any rows after it
        if((row_with_corners[irow+1]-row_with_corners[irow])>1){
            //There are Rows between this row_w_c and the next one
            transitions.clear();
            int row = row_with_corners[irow]+1;
            for(auto&iwall:V_Walls){//Adding all relevant wall transitions
                if(iwall.y_end >= row &&iwall.y_start <= row){
                    transition newtransition;
                    newtransition.x = iwall.x;
                    newtransition.type = 'I';
                    transitions.push_back(newtransition);
                }
            }
            sort(transitions.begin(),transitions.end(),sortbyx);
            bool inside=false;
            long long rowsum=0;
            for(int itran=0;itran!=transitions.size();itran++){
                    if(inside){
                    cout<<transitions[itran].x<< "-" <<transitions[itran-1].x<<"+1  ";
                    rowsum+=(transitions[itran].x-transitions[itran-1].x)+1;
                    }
                    inside=!inside;
            }
            long long multiplier = row_with_corners[irow+1]-row_with_corners[irow]-1;
            cout<<endl<< "V Row "<<row<<" ="<<rowsum * multiplier<< "("<<rowsum<<"*"<<multiplier<<")"<<endl;
            sum+=rowsum*multiplier;

        }
    }

    cout<<"Total Sum of "<<sum<<" Cubic meters"<<endl;

    if(false){//Drawing
        for(auto&icorner:corners){
            cout<<"Corner "<< icorner.x << ","<<icorner.y << " T:"<<icorner.type<<endl;
        }cout<<endl;
    }
    return;
}
