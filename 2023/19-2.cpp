#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
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
int l=1;//Lower bound
int h=4000;//Higher bound
};

struct part{
vector<range> xmas;
workflow* state;
};

vector<string> pos;//Debugging saves the day !

vector<part> process_range(part&ipart,vector<workflow>&workflows,bool visuals=false){
    vector<part> valid_parts;
    if(visuals){for(auto&seg:pos){cout<<seg<<" ";}cout<<endl;}
    pos.push_back(ipart.state->id);
    while(ipart.state->id != "R" && ipart.state->id != "A"){
        //if(visuals){cout<<"-i->"<<ipart.state->id;}
        for(auto&icondition:ipart.state->conditions){
            if(!icondition.test){//Everything left is transfered to the next
                ipart.state = icondition.next;
                vector<part> childpaths = process_range(ipart,workflows,visuals);
                if(visuals){cout<<" returned: "<<childpaths.size()<<endl;}
                for(auto&itpart:childpaths){
					valid_parts.push_back(itpart);

				}pos.pop_back();
            }
            if(icondition.is_bigger){//'>' Check
                part split_part = ipart;//Split where the condition is true
                split_part.xmas[icondition.lindex].l =icondition.rvalue+1;
                split_part.state = icondition.next;
                ipart.xmas[icondition.lindex].h=icondition.rvalue;
                if(split_part.xmas[icondition.lindex].l<=split_part.xmas[icondition.lindex].h && split_part.state->id!= "R"){ //Split doesn't have negative space or ends in immediate rejection
					vector<part> childpaths = process_range(split_part,workflows,visuals);
					if(childpaths.size()>0){
						if(visuals){
							cout<<" > returned: "<<childpaths.size()<<endl;
							for(auto&ipart:childpaths){
								for(auto&irange:ipart.xmas){
									cout<<" "<<irange.l<<" - "<<irange.h<<",";
								}cout<<endl;
							}
						}
						for(auto&jpart:childpaths){
							valid_parts.push_back(jpart);

						}pos.pop_back();
					}
                }
                if(ipart.xmas[icondition.lindex].l>ipart.xmas[icondition.lindex].h){return valid_parts;}//No more possisbilies
                goto next_condition;
            }else{//'<' Check
                part split_part = ipart;//Split where the condition is true
                split_part.xmas[icondition.lindex].h =icondition.rvalue-1;
                split_part.state = icondition.next;
                ipart.xmas[icondition.lindex].l=icondition.rvalue;
                if(split_part.xmas[icondition.lindex].l<=split_part.xmas[icondition.lindex].h && split_part.state->id!= "R"){ //Split doesn't have negative space or ends in immediate rejection
					vector<part> childpaths = process_range(split_part,workflows,visuals);
					if(childpaths.size()>0){
						if(visuals){
							cout<<" < returned: "<<childpaths.size()<<endl;
							for(auto&ipart:childpaths){
								for(auto&irange:ipart.xmas){
									cout<<" "<<irange.l<<" - "<<irange.h<<",";
								}cout<<endl;
							}
						}
						for(auto&jpart:childpaths){
							valid_parts.push_back(jpart);

						}pos.pop_back();
					}
                }
                if(ipart.xmas[icondition.lindex].l>ipart.xmas[icondition.lindex].h){return valid_parts;}//No more possisbilies
                goto next_condition;
            }
            next_condition:;

        }
        next_workfow:;
    }
    //if(ipart.state->id=="A"){valid_parts.push_back(ipart);}
    if(pos.back()=="A"){valid_parts.push_back(ipart);}
    if(visuals){cout<<"Exit from ";cout<<pos.back()<<" "<<endl;}
    return valid_parts;
}

void D_19_2(){
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

	part startpart;
    for(auto&iworkflow:workflows){//Set Start to start at in
        //cout<<iworkflow.id<<endl;
        if(iworkflow.id == "in"){
            startpart.state=&iworkflow;
			//cout<<"Found start";
            break;
        }
    }

    for(int i=0;i!=4;i++){ // Add inital ranges
        range newrange;
        startpart.xmas.push_back(newrange);
    }
    cout<<"Prep Complete"<<endl;
    vector <part> valid_parts = process_range(startpart,workflows);

	cout<<"Found "<<valid_parts.size()<<" raw segments"<<endl;
	long long sum = 0;
	for(auto&ipart:valid_parts){
		long long part_com=1;
		//cout<<ipart.visited.size()<<endl;
		//for(auto&seg:ipart.visited){cout<<" "<<seg;}cout<<endl;
		for(auto&irange:ipart.xmas){
			part_com*= (irange.h-irange.l+1);
			//cout<<" "<<irange.l<<" - "<<irange.h<<",";
		}
		sum+=part_com;
		//cout<<"= "<<part_com<<endl;
	}


    cout<<"Total sum is "<<sum<<endl;
    return;
}
