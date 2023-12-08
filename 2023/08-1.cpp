#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void D_8_1(){
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
    node*here = nullptr;
    node*there =nullptr;
    for(auto&source:nodes){
        if(source.id=="AAA"){
            here=&source;
        }
    }

    int steps=0;
    while((*here).id!="ZZZ"){
        int index = steps%directions.size();
        //cout <<directions[index]<<endl;
        //cout <<(*here).id<<"+ ";
        if(directions[index] == 'L'){
            there=(*here).left;
            here=there;
        }else{
            there=(*here).right;
            here=there;
        }
        steps++;
    }
    cout<<endl<<"Taken "<<steps<< " steps to ZZZ";

    return;
}
