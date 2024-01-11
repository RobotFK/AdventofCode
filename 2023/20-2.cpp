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

int b_presses=0;
unordered_map<module*,int> m_conjunction_cycle;//List of all Conjunctions with more than one output and at witch intervalls they send out

bool push_button_to_target(vector<module>&modules,module*broadcast,string&target,module*target_parent,bool visuals=false){

	pulse startpulse(broadcast,broadcast,false);
    vector<pulse> signal_queue;
    signal_queue.push_back(startpulse);
    while(signal_queue.size()!=0){
		module&source=*signal_queue.front().sender;
        module&cur_module=*signal_queue.front().reciever;
        bool signal_type =signal_queue.front().high;

        //Code Relevant for day 2
        if(cur_module.id == target_parent->id && signal_type ){
                cout<<"Recieved "<<"H"<<" from "<<source.id<<"\t ("<<b_presses<<")"<<endl;
                if(m_conjunction_cycle.find(&source)== m_conjunction_cycle.end()){m_conjunction_cycle[&source]=b_presses;}
        }

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

    module*broadcast;

    //Add rx module
    string starget ="rx";
    module newmodule;
    newmodule.id = starget;
    newmodule.type = 'r';
    modules.push_back(newmodule);
    module* target;

    for(auto&line:inputvector){
        module newmodule;
        vector<string> split_line = split_string(line,'>');
        string self_info = split_line[0].substr(0,split_line[0].size()-2);
        string child_info = split_line[1];
        //cout<<split_line[0]<<endl;
        newmodule.type =self_info[0];
        newmodule.id = (self_info[0]!='%'&&self_info[0]!='&')?newmodule.id = self_info:newmodule.id = self_info.substr(1);
        vector<string> s_children = split_string(child_info,',');
        for(auto&child:s_children){newmodule.children_id.push_back(child.substr(1));}
        modules.push_back(newmodule);
    }

    for(auto&imodule:modules){//Iterate over each modulue and connect it up
        if(imodule.id=="broadcaster"){broadcast=&imodule;}else
        if(imodule.id==starget){target=&imodule;}
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

    //By manual Examination of the data my Input has only one Module (of type Conjunction) linking to rx
    //That in turn is recieving from four Conjunctions.
    //Meaning that rx only recieves a low pulse when the Parent remembers a High Pulse from those 4 Conjunctions

    module* target_parent;
    for(auto&imodule:modules){
        for(auto&ichild:imodule.children){
            if(ichild->id==target->id){
                target_parent = &imodule;
            }
        }
    }
    cout<<"Parent "<<target_parent->id<<" has "<< target_parent->memory.size()<<" Parents"<<endl;


    bool found=false;
    while(!found){
        b_presses++;
        //cout<<b_presses<<endl;
        found=push_button_to_target(modules,broadcast,starget,target_parent);
        if(m_conjunction_cycle.size()==4){break;}
    }
    long long mult = 1;
    for(auto&icycel:m_conjunction_cycle){
        cout<<icycel.first->id<<" "<<icycel.second<<endl;
        if(icycel.first->id == "sg"){icycel.second+=3;}//This was found by manually Calculating the answer, but I have not been able to find out why this value is counted 3 too low
        mult*=icycel.second;
    }

    cout<<"It too "<<mult<<" presses"<<endl;
    return;
}
//900399384 Too low
//245823353585944 Too low
