#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <numeric>
using namespace std;

long long gcd(long long a, long long b){
    while(a!=0||b!=0){
        if (a == 0) return b;
            b %= a;
        if (b == 0) return a;
            a %= b;
    }
}

long long lcm(long long a,long long b){
    return (a*b)/gcd(a,b);
}
void D_8_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("08-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    string directions;
    struct node{
        string id;
        string sleft;
        string sright;
        node* left;
        node* right;
    };
    vector<node> nodes;

    for(auto&line:inputvector){
        if(line.size()==0){continue;}
        if(line.size()!=16){directions=line;
        }else{
            node newnode;
            newnode.id = line.substr(0,3);
            newnode.sleft = line.substr(7,3);
            newnode.sright = line.substr(12,3);
            nodes.push_back(newnode);
        }
    }

    for(auto&source:nodes){
        for(auto&destination:nodes){
                if (destination.id == source.sleft){
                    source.left = &destination;
                }
                if (destination.id == source.sright){
                    source.right = &destination;
                }
        }
        //cout << source.id << "->"<<source.left->id <<" or "<<source.right->id << endl;
    }
    node*there =nullptr;
    vector<node*>heres;
    for(auto&source:nodes){
        if(source.id.substr(2,1)=="A"){
            heres.push_back(&source);
        }
    }
    cout<<"Calculating "<<heres.size() <<" positions"<<endl;
    vector <int> offset(heres.size(),0);//Time from the starting pos until the first Z pos
    vector <int> cycle(heres.size(),0);//Time to reach the same Z pos + direction index again

    //for(auto&x:offset){cout << x<<","<<endl;}

    int steps=0;
    int cycels_found =0;
    while(cycels_found != heres.size()){
        int index = steps%directions.size();
        int hindex = 0;
        for(auto&here:heres){
            if(directions[index] == 'L'){
                //cout<<(*here).id<<"-L->"<<(*here).left->id<<endl;
                there=(*here).left;
                here=there;
            }else{
                //cout<<(*here).id<<"-R->"<<(*here).left->id<<endl;
                there=(*here).right;
                here=there;
            }
            if((*here).id.substr(2,1)=="Z"){
                    if(offset[hindex] == 0){
                        offset[hindex] = steps+1;
                    }else if(cycle[hindex] == 0){
                        cycle[hindex] = steps+1-offset[hindex];
                        cycels_found++;
                    }
            }
            hindex++;
        }
        steps++;//More than 350000000 steps

    }

    long long all_lcm=1;
    for(int i=0;i!=heres.size();i++){
            cout <<"Offset "<< offset[i]<<" then at Cycle "<< cycle[i]<<endl;
            all_lcm = lcm(all_lcm,cycle[i]);
    }


    cout<<endl<<"Takes "<<all_lcm<< "steps to reach all Heres to __Z";

    return;
}
