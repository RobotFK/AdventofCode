#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

struct workflow;

struct condition{
int lindex;//We drop the Symbol and just use the index of [x,m,a,s]
bool test = true; //Weather this condition is a check or just a redirect
bool is_bigger;
int rvalue;
string next_id;// Helper value
workflow* next;
};

struct workflow{
string id;
vector<condition> conditions;
};

struct part{
vector<int> xmas;
workflow* state;
};

bool process_part(part&ipart,vector<workflow>&workflows,bool visuals=false){
    while(ipart.state->id != "R" && ipart.state->id != "A"){
        if(visuals){cout<<"->"<<ipart.state->id;}
        for(auto&icondition:ipart.state->conditions){
            if(!icondition.test){
                ipart.state = icondition.next;
                goto next_workfow;
            }
            if(icondition.is_bigger){//'>' Check
                if(ipart.xmas[icondition.lindex]>icondition.rvalue){
                    ipart.state = icondition.next;
                    goto next_workfow;
                }
            }else{//'<' Check
                if(ipart.xmas[icondition.lindex]<icondition.rvalue){
                    ipart.state = icondition.next;
                    goto next_workfow;
                }
            }

        }
        next_workfow:;
    }
    if(visuals){cout<<"->"<<ipart.state->id<<endl;}
    return (ipart.state->id=="A");
}


void D_19_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("19-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<workflow> workflows;
    vector<part> parts;

    workflow rejected;
    rejected.id="R";
    workflows.push_back(rejected);

    workflow accepted;
    accepted.id="A";
    workflows.push_back(accepted);

    for(auto&line:inputvector){
        if(line.size()==0){continue;}
        if(line[0]!='{'){//workflow
            workflow newworkflow;
            vector<string> ssplit = split_string(line,'{');
            newworkflow.id = ssplit[0];
            ssplit[1].pop_back();
            vector<string> sconditions = split_string(ssplit[1],',');
            for(auto&scon:sconditions){
                condition newcondition;
                if(scon.size()==1||(scon[1]!='>'&&scon[1]!='<')){
                    newcondition.next_id= scon;
                    newcondition.test = false;
                    newworkflow.conditions.push_back(newcondition);
                    continue;
                }
                if(scon[1]=='>'){
                    newcondition.is_bigger=true;
                }else if(scon[1]=='<'){
                    newcondition.is_bigger=false;
                }else{cout<<"Comparison error";}
                switch(scon[0]){
                    case 'x':
                    newcondition.lindex = 0;
                    break;
                    case 'm':
                    newcondition.lindex = 1;
                    break;
                    case 'a':
                    newcondition.lindex = 2;
                    break;
                    case 's':
                    newcondition.lindex = 3;
                    break;
                }
                string info=scon.substr(2,scon.size()-1);
                vector<string> sinfo = split_string(info,':');
                newcondition.rvalue=stoi(sinfo[0]);
                newcondition.next_id=sinfo[1];
                newworkflow.conditions.push_back(newcondition);
            }
            workflows.push_back(newworkflow);
        }else {//part
            part newpart;
            vector<string> ssplit = split_string(line,',');
            for(int i=0;i!=4;i++){
                if(i==0){
                    ssplit[i] = ssplit[i].erase(0,3);
                }else if(i==3){
                    ssplit[i] = ssplit[i].erase(0,2);
                    ssplit[i].pop_back();
                }else{
                    ssplit[i] = ssplit[i].erase(0,2);
                }
                newpart.xmas.push_back(stoi(ssplit[i]));
            }
            parts.push_back(newpart);
        }
    }

    cout<<"Processing "<<parts.size()<<" parts through "<<workflows.size()-2<<" Workflows"<<endl;

    for(auto&iworkflow:workflows){
        for(auto&icondition:iworkflow.conditions){
            for(auto&testworkflow:workflows){
                if(testworkflow.id==icondition.next_id){
                    icondition.next = &testworkflow;
                    break;
                }
            }
        }
    }

    workflow* start;
    for(auto&iworkflow:workflows){
        //cout<<iworkflow.id<<endl;
        if(iworkflow.id == "in"){
            start=&iworkflow;
            break;
        }
    }

    for(auto&ipart:parts){
        ipart.state=start;
    }
    cout<<"Prep Complete"<<endl;
    int sum = 0;
    for(auto&ipart:parts){
        if(process_part(ipart,workflows)){
            int partsum=0;
            for(auto&value:ipart.xmas){
                partsum+=value;
            }
            sum+=partsum;
            //cout<<"Hit:"<<partsum<<endl;
        }
    }
    cout<<"Total sum is "<<sum<<endl;
    return;
}
