#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <utility> //For pairs
#include <unordered_map>//For the memory of conjunction modules
using namespace std;

struct module{
char type;
bool state = false;
string id;
vector<string>children_id;//Helper vector
vector<module*>children;
unordered_map<string,bool> memory;
};


void push_button(vector<module>&modules,module*broadcast,long long&low,long long&high,bool visuals=false){
    pair <module*,bool> start_signal{broadcast,false};
    vector<pair<pair<module*,bool>,string>> signal_queue;
    pair<pair<module*,bool>,string> first_msg;
    first_msg.first = start_signal;
    first_msg.second = "button";
    signal_queue.push_back(first_msg);
    low++;

    while(signal_queue.size()!=0){
        module&cur_module=*signal_queue.front().first.first;
        bool signal_type =signal_queue.front().first.second;
        string source=signal_queue.front().second;
        int child_nr =1;//at what position to inser the msg
        //cout<<"Q:";
        for(auto&msg:signal_queue){//Increment child_nr based on how many Conjunctions are at the start
            //cout<<msg.first.first->type<<" ";
            if(msg.first.first->type == '&'){child_nr++;}else{break;}
        }
        if(child_nr>1){child_nr--;}//Remove overcounted 1
        if(visuals){cout<<source<<"->"<<cur_module.id<<endl;}
        switch(cur_module.type){
        case 'b':
            child_nr =1;
            if(signal_type){high+=cur_module.children_id.size();}else{low+=cur_module.children_id.size();}//This way it also count unknown modules
            for(auto&ichild:cur_module.children){
                //if(visuals){cout<<ichild->id<<signal_type<<", ";}
                pair <module*,bool> signal{ichild,signal_type};//Copies signal
                pair<pair<module*,bool>,string> msg;
                msg.first=signal;
                msg.second=cur_module.id;
                signal_queue.insert(signal_queue.begin()+child_nr,msg);
                child_nr++;
            }
        break;
        case '%':
            if(!signal_type){//Signal is low pulse
                cur_module.state=!cur_module.state;//Switches state
                child_nr =1;
                if(cur_module.state){high+=cur_module.children_id.size();}else{low+=cur_module.children_id.size();}//This way it also count unknown modules
                for(auto&ichild:cur_module.children){
                    //if(visuals){cout<<ichild->id<<cur_module.state<<", ";}
                    pair <module*,bool> signal{ichild,cur_module.state};//Uses state(after flip)
                    pair<pair<module*,bool>,string> msg;
                    msg.first=signal;
                    msg.second=cur_module.id;
                    signal_queue.insert(signal_queue.begin()+child_nr,msg);
                    child_nr++;
                }
            }
        break;
        case '&':
            cur_module.memory[source]=signal_type;//Switches state to input
            bool all_high=true;
            for (auto& it: cur_module.memory) {//Test if all are set to high
                if(!it.second){all_high=false;break;}
            }
            cur_module.state=!all_high;//Signal to send it is the inversion of all being high
            if(cur_module.state){high+=cur_module.children_id.size();}else{low+=cur_module.children_id.size();}//This way it also count unknown modules
            for(auto&ichild:cur_module.children){
                //if(visuals){cout<<ichild->id<<cur_module.state<<",";}
                pair <module*,bool> signal{ichild,cur_module.state};//Uses inverted state
                pair<pair<module*,bool>,string> msg;
                msg.first=signal;
                msg.second=cur_module.id;
                if(msg.first.first->type =='&'){//Give priority if it is as Conjunction
                    //signal_queue.insert(signal_queue.begin(),msg);
                }//else{
                    signal_queue.insert(signal_queue.begin()+child_nr,msg);
                //}
                child_nr++;
            }
        break;
        }
        //if(visuals){cout<<endl;}
        signal_queue.erase(signal_queue.begin());
    }
    if(visuals){cout<<endl;}
}

void D_20_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("20-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<module> modules;
    for(auto&line:inputvector){
        module newmodule;
        vector<string> split_line = split_string(line,'>');
        split_line[0].erase(split_line[0].end()-2,split_line[0].end());
        //cout<<split_line[0]<<endl;
        newmodule.type =split_line[0][0];
        if(split_line[0][0]!='%'&&split_line[0][0]!='&'){
            newmodule.id = split_line[0];
        }else{
            newmodule.id = split_line[0].substr(1);
        }
        vector<string> split_children = split_string(split_line[1],',');
        for(auto&child:split_children){
            newmodule.children_id.push_back(child.substr(1));
        }
        modules.push_back(newmodule);
    }

    module*broadcast;

    for(auto&imodule:modules){//Iterate over each modulue and connect it up
        if(imodule.id=="broadcaster"){//Finds the broadcaster
            broadcast=&imodule;
        }
        for(auto&ichild:imodule.children_id){
            for(auto&test:modules){
                if(ichild==test.id){
                    if(test.type=='&'){//ichild is input for this tested conjunction
                        test.memory[imodule.id]=false;
                    }
                    imodule.children.push_back(&test);//Turns the String of Children into Pointers
                    break;
                }
            }
        }
        //cout<<imodule.id<<":";
        //for(auto&ichild:imodule.children){cout<<ichild->id<<" ";}cout<<endl;
    }
    cout<<"All Modules connected"<<endl<<endl;

    long long low=0;
    long long high=0;
    for(int i=0;i!=1000;i++){
        push_button(modules,broadcast,low,high);
    }
    cout<<"High "<<high<<endl;
    cout<<"Low  "<<low<<endl;
    cout<<"Mult= "<<high*low<<endl;
    return;
}
