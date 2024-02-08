#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include<algorithm>
using namespace std;

struct position{
    int x;
    int y;
    int z;
    position(int ix, int iy, int iz){
    x=ix;
    y=iy;
    z=iz;
    }
};

struct brick{
    string id = "0";
    position l;
    position h;
    brick(int ilx,int ily,int ilz,int ihx,int ihy,int ihz, string iid = "0") : id(iid), l(ilx, ily, ilz), h(ihx, ihy, ihz) {}
    void display(){
        cout<<id<<":\t";
        if(l.x == h.x){cout<<l.x;}else{cout<<l.x<<"-"<<h.x;}cout<<" \t";
        if(l.y == h.y){cout<<l.y;}else{cout<<l.y<<"-"<<h.y;}cout<<" \t";
        if(l.z == h.z){cout<<l.z;}else{cout<<l.z<<"-"<<h.z;}cout<<endl;
    }
    void fall(unsigned const int&fall_amount){
        l.z -=fall_amount;
        h.z -=fall_amount;
    }
    vector<brick*> children; //Bricks that Rest Upon this Brick
    vector<brick*> parents; //Bricks that this Brick Rest Upon
};

int settel_bricks2(vector<brick>&ibricks,bool visuals=false){
    int dropped= 0;

    //Sort all falling bricks by their lowes Points (ASC):
    sort(ibricks.begin(),ibricks.end(), [](const brick&lhs, const brick&rhs){return lhs.l.z<rhs.l.z;});

    //Falling of each Brick:
    for(int i=1;i!=ibricks.size();i++){//Skips the Floorslab
        sort(ibricks.begin(),ibricks.begin()+i, [](const brick&lhs, const brick&rhs){return lhs.h.z<rhs.h.z;});
        int h_o = 0; //Highest obstruction
        for(int j=i-1;j>=0;j--){ // We can reduce the runtime by finding the highest collision when searching from the Top
            if(ibricks[j].l.x <= ibricks[i].h.x &&
               ibricks[j].h.x >= ibricks[i].l.x &&
               ibricks[j].l.y <= ibricks[i].h.y &&
               ibricks[j].h.y >= ibricks[i].l.y
               ){//Has x and y overlap
                if(ibricks[j].h.z >= ibricks[i].l.z){continue;}//ignores all bricks that that it could not land on
                h_o= ibricks[j].h.z;
                break;
            }
        }
        if(visuals){cout<<"Brick "<<ibricks[i].id<<" V: "<< h_o+1-ibricks[i].l.z<<" "<<endl;}
        if(ibricks[i].l.z != h_o+1){dropped++;}
        ibricks[i].fall(ibricks[i].l.z-(h_o+1));
    }
    return dropped;
}

int cascade_bricks(map<string,vector<string>> childern,map<string,vector<string>> parents,string&d_brick_id){

    set<string> affected_id;
    queue<string> cascade_id;cascade_id.push(d_brick_id);
    while(cascade_id.size()!=0){
        affected_id.insert(cascade_id.front());//Add Current Brick to Affected Bricks
        if(childern[cascade_id.front()].size()!=0){
            for(auto&child:childern[cascade_id.front()]){//For each Child of the brick
                bool no_support = true;
                for(auto&parent:parents[child]){//Test each Parent of the Child
                    if(affected_id.count(parent)==0){no_support = false;break;}
                }
                if(no_support){cascade_id.push(child);}
            }
        }
        cascade_id.pop();
    }
    return affected_id.size()-1;
}

void D_22_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("22-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<brick> bricks; //Set does not work due to not having non-constant references
    //Needed for determining the Size of the Floorslab
    int id_counter = 0;
    int id_digit_counter = 0;
    unsigned int max_x=0;
    unsigned int max_y=0;

    for(auto&line:inputvector){
        vector<string> positions = split_string(line,'~');
        vector<string> pos1 = split_string(positions[0],',');
        vector<string> pos2 = split_string(positions[1],',');
        //Ensuring that the first position always is the lower is possible, but not needed for these inputs
        string sname = "";

        int name_id=id_counter;
        sname+=(name_id%26)+'a';name_id=(name_id-name_id%26)/26;
        while(name_id){sname+=(name_id%26)+'a';name_id=(name_id-name_id%26)/26;}
        id_counter++;

        brick newbrick(stoi(pos1[0]),stoi(pos1[1]),stoi(pos1[2]),stoi(pos2[0]),stoi(pos2[1]),stoi(pos2[2]),sname);
        newbrick.l = position(stoi(pos1[0]),stoi(pos1[1]),stoi(pos1[2]));
        newbrick.h = position(stoi(pos2[0]),stoi(pos2[1]),stoi(pos2[2]));

        bricks.push_back(newbrick);
        max_x = (newbrick.h.x) > max_x?newbrick.h.x:max_x;
        max_y = (newbrick.h.y) > max_y?newbrick.h.y:max_y;
    }

    brick floorslab{0,0,0,max_x,max_y,0,"Floor"};
    bricks.push_back(floorslab);

    cout<<"Base Slab: (0,0)-("<<max_x<<","<<max_y<<")"<<endl;

    int dropped = settel_bricks2(bricks);

    //Sort All bricks by their highest Points (ASC):
    sort(bricks.begin(),bricks.end(), [](const brick&lhs, const brick&rhs){return lhs.h.z<rhs.h.z;});

    //Marking Children
    for(int i=1;i!=bricks.size();i++){//Skips the Floorslab
        //cout<<endl<<bricks[i].id<<" rest on:";
        for(int j=i-1;j>=0;j--){ //J is the Brickindex that might be under it
            if(bricks[j].h.z+1 != bricks[i].l.z){continue;}
            //if(bricks[j].h.z+1 > bricks[i].l.z){cout<<"-l ";continue;}
            if(bricks[j].l.x <= bricks[i].h.x &&
               bricks[j].h.x >= bricks[i].l.x &&
               bricks[j].l.y <= bricks[i].h.y &&
               bricks[j].h.y >= bricks[i].l.y
               ){//Has overlap
                //cout<<bricks[j].id<<", ";
                bricks[j].children.push_back(&bricks[i]);
                bricks[i].parents.push_back(&bricks[j]);
            }
        }
    }

    //additional compounting into a maps for easier Calcualtion of Cascading bricks
    map<string,vector<string>> childern;
    map<string,vector<string>> parents;
    for(auto&ibrick:bricks){
        if(ibrick.l.z == 0){continue;}//Floor Skipping
        if(ibrick.children.size()!=0){
            vector<string> children_id;
            for(auto&ibrickchildren:ibrick.children){
                children_id.push_back(ibrickchildren->id);
            }
            childern.insert(pair<string,vector<string>>(ibrick.id,children_id));
        }
        vector<string> parent_id;
        for(auto&ibrickchildren:ibrick.parents){
            parent_id.push_back(ibrickchildren->id);
        }
        parents.insert(pair<string,vector<string>>(ibrick.id,parent_id));
    }

    int fallsum =0;
    for(auto&ibrick:bricks){
        if(ibrick.l.z==0){continue;}//Skip Floor
        int brick_fall = cascade_bricks(childern,parents,ibrick.id);
        cout<<ibrick.id<<":\t"<< brick_fall<<endl;
        fallsum+=brick_fall;
    }

    cout<<fallsum<<" Bricks Will Fall in sum"<<endl;
    return;
}
