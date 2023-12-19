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

struct range{
int l=1;
int h=4000;
};

struct part{
vector<range> xmas;
vector<string>visited;
workflow* state;
};

vector<part> process_range(part ipart,vector<workflow>&workflows,bool visuals=false){
    vector<part> valid_parts;
    if(visuals){cout<<endl<<"New Function";}
    while(ipart.state->id != "R" && ipart.state->id != "A"){
        if(visuals){cout<<"->"<<ipart.state->id;}
        for(auto&icondition:ipart.state->conditions){
            if(!icondition.test){//Everything is transfered to the next
                ipart.state = icondition.next;
                vector<part> childpaths = process_range(ipart,workflows);
                if(childpaths.size()>0){
                    for(auto&jpart:childpaths){
                        valid_parts.push_back(jpart);
                    }
                }
            }
            if(icondition.is_bigger){//'>' Check
                part split_part = ipart;//Split where the condition is true
                split_part.xmas[icondition.lindex].l =icondition.rvalue+1;
                ipart.xmas[icondition.lindex].h=icondition.rvalue;
                vector<part> childpaths = process_range(split_part,workflows);
                if(childpaths.size()>0){
                    for(auto&jpart:childpaths){
                        valid_parts.push_back(jpart);
                    }
                }
                if(ipart.xmas[icondition.lindex].l>ipart.xmas[icondition.lindex].h){return valid_parts;}//No more possisbilies
                goto next_workfow;
            }else{//'<' Check
                part split_part = ipart;//Split where the condition is true
                split_part.xmas[icondition.lindex].h =icondition.rvalue-1;
                ipart.xmas[icondition.lindex].l=icondition.rvalue;
                vector<part> childpaths = process_range(split_part,workflows);
                if(childpaths.size()>0){
                    for(auto&jpart:childpaths){
                        valid_parts.push_back(jpart);
                    }
                }
                if(ipart.xmas[icondition.lindex].l>ipart.xmas[icondition.lindex].h){return valid_parts;}//No more possisbilies
                goto next_workfow;
            }

        }
        next_workfow:;
    }
    if(ipart.state->id=="A"){valid_parts.push_back(ipart);}
    if(visuals){cout<<"->"<<ipart.state->id<<endl;}
    return valid_parts;
}


void D_19_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("19-1-1.txt");
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

    part startpart;
    startpart.state=start;
    for(int i=0;i!=4;i++){
        range newrange;
        startpart.xmas.push_back(newrange);
    }
    cout<<"Prep Complete"<<endl;
    vector <part> valid_parts = process_range(startpart,workflows);
    int sum = 0;

    cout<<"Total sum is "<<sum<<endl;
    return;
}
