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
unordered_map<module*,bool> memory;
bool sent_high = false;//Helper part 2
};

struct pulse{
	module* sender	= nullptr;
	bool high;
	module* reciever= nullptr;
	pulse(module* s,module* r,bool h){
		sender=s;
		reciever=r;
		high=h;
	};
};

unordered_map<module*,int> m_conjunction_cycle;//List of all Conjunctions with more than one output and at witch intervalls they send out

bool push_button_to_target(vector<module>&modules,module*broadcast,string&target,module*target_parent,bool visuals=false){

	pulse startpulse(nullptr,broadcast,false);
    vector<pulse> signal_queue;
    signal_queue.push_back(startpulse);

    while(signal_queue.size()!=0){
		module&source=*signal_queue.front().reciever;
        module&cur_module=*signal_queue.front().reciever;
        bool signal_type =signal_queue.front().high;

        if(cur_module.id == target&& !signal_type){return true;}
        int batch_offset =1;//at what position to inser the msg
        //cout<<"Q:";
        for(auto&ipulse:signal_queue){//Increment batch_offset based on how many Conjunctions are at the start
            //cout<<msg.first.first->type<<" ";
            if(ipulse.reciever->type == '&'){batch_offset++;}else{break;}
        }
        if(batch_offset>1){batch_offset--;}//Remove overcounted 1

        if(visuals){cout<<source.id<<"->"<<cur_module.id<<endl;}
        switch(cur_module.type){
        case 'b':
            batch_offset =1;
            for(auto&ichild:cur_module.children){
                if(visuals){cout<<ichild->id<<signal_type<<", ";}
                pulse newpulse(&cur_module,ichild,signal_type);
                signal_queue.insert(signal_queue.begin()+batch_offset++,newpulse);
            }
        break;
        case '%':
            if(!signal_type){//Signal is low pulse
                cur_module.state=!cur_module.state;//Switches state
                batch_offset =1;
                for(auto&ichild:cur_module.children){
                    //if(visuals){cout<<ichild->id<<cur_module.state<<", ";}
                    pulse newpulse(&cur_module,ichild,cur_module.state);
					signal_queue.insert(signal_queue.begin()+batch_offset++,newpulse);
                }
            }
        break;
        case '&':
            cur_module.memory[&source]=signal_type;//Switches state to input
            bool all_high=true;
            for (auto& it: cur_module.memory) {//Test if all are set to high
                if(!it.second){all_high=false;}
            }
            cur_module.state=!all_high;//Signal to send it is the inversion of all being high
            if(cur_module.id==target_parent->id && all_high){//Partent of our target gets fired
                cout<<cur_module.id<<" all high";
            }
            for(auto&ichild:cur_module.children){//Add all children
                //if(visuals){cout<<ichild->id<<cur_module.state<<",";}
                pulse newpulse(&cur_module,ichild,cur_module.state);
				signal_queue.insert(signal_queue.begin()+batch_offset++,newpulse);
            }
        break;
        }
        //if(visuals){cout<<endl;}
        signal_queue.erase(signal_queue.begin());
    }
    if(visuals){cout<<"Out of signals"<<endl;}
    return false;
}

void D_20_2(){
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
        newmodule.sent_high=false;
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

    //Add rx module
    string starget ="rx";
    module newmodule;
    newmodule.id = starget;
    modules.push_back(newmodule);
    module* target;

    for(auto&imodule:modules){//Iterate over each modulue and connect it up
        if(imodule.id=="broadcaster"){//Finds the broadcaster
            broadcast=&imodule;
        }else if(imodule.id==starget){//Finds the target
            target=&imodule;
        }
        ;
        for(auto&ichild:imodule.children_id){//Iterate over each of the Children
            for(auto&test:modules){//Iterate over each modulue to find the module that matches the Childs id
                if(ichild==test.id){
                    if(test.type=='&'){//ichild is input for this tested conjunction
                        test.memory[&imodule]=false;
                    }
                    imodule.children.push_back(&test);//Turns the String of Children into Pointers
                    break;
                }
            }
        }
        //cout<<imodule.id<<" "<<imodule.type<<":";
        //for(auto&ichild:imodule.children){cout<<ichild->id<<" ";}cout<<endl;
    }

    cout<<"All Modules connected"<<endl<<endl;
    //this might crash if in this line the parent at one point might be a flipflop (but could be easily implemented)
    module* target_parent;
    for(auto&imodule:modules){
        for(auto&ichild:imodule.children){
            if(ichild->id==target->id){
                target_parent = &imodule;
            }
        }
    }
    cout<<"Found "<< target_parent->memory.size()<<" Target grandparents:"<<endl;

    long long presses=0;
    bool found=false;
    while(!found){
        presses++;
        //cout<<presses<<" ";
        found=push_button_to_target(modules,broadcast,starget,target_parent);
        if(m_conjunction_cycle.size()==4){break;}
    }

    for(auto&icycel:m_conjunction_cycle){
        cout<<icycel.first<<" "<<icycel.second<<endl;
    }

    cout<<"It too "<<presses<<" presses"<<endl;
    return;
}

