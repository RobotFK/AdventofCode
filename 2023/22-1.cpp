#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
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

bool settel_bricks(vector<brick>&ibricks,bool visuals=false){
    bool dropped= false;

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
        if(ibricks[i].l.z != h_o+1){dropped=true;}
        ibricks[i].fall(ibricks[i].l.z-(h_o+1));
    }
    return dropped;
}

void D_22_1(){
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

    bool dropped = settel_bricks(bricks);
    while(dropped){dropped = settel_bricks(bricks);cout<<"Falling"<<endl;}

    //return ;

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
    cout<<endl;

    int safe_discards =0;
    for(auto&ibrick:bricks){
        if(ibrick.children.size()==0){
            safe_discards++;
            cout<<"No Children: "<<ibrick.id<<endl;
            continue;
        }
        bool critical = false;
        for(auto&ibrickchildren:ibrick.children){
            if(ibrickchildren->parents.size()==1){critical=true;}
        }
        if(!critical){
                safe_discards++;
                cout<<"Not critical: "<<ibrick.id<<endl;
        }
    }

    cout<<safe_discards<<" Bricks can be Safely extracted"<<endl;
    return;
}

//564 Too high
