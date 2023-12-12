#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

bool is_valid_placement2(string&record,vector<int>&placement,vector<int>&groups,bool visuals=false){
    int group_index=0;//Next or Current group that is evaluated
    for(int i=0;i!=record.size();i++){
        if(group_index==placement.size()){//damaged Spring after final group
            if(record[i]=='#'){
                if(visuals){cout<<"4";}
                return false;
            }
            continue;
        }
        if(i<placement[group_index]){
            if(record[i]=='#'){
                if(visuals){cout<<"1";}
                return false;
            }//damaged Spring before group
            continue;
        }
        if(i<=placement[group_index]+groups[group_index]-1){
            if(record[i]=='.'){
                if(visuals){cout<<"2:"<<i<<" is less than "<<placement[group_index]+groups[group_index]-1;}
                return false;
            }//operational  Spring in group
            continue;
        }
        if(i==placement[group_index]+groups[group_index]){
            if(record[i]=='#'){
                if(visuals){cout<<"3";}
                return false;
            }//damaged spring immediatly after in group
            group_index++;
            continue;
        }

        cout<<"Error";
    }
    return true;
}

int arrangements(string&record,vector<int>&groups,bool visuals=false){
    int arrangements = 0;

    /*
    cout<<endl<<endl;
    cout<<record<< "<-";
    for(auto&x:groups){cout<<x<<" ";}
    cout<<endl<<endl;
    */

    vector<int>placement;
    for(int i=0;i!=groups.size();i++){// Inital layout, does not have to be valid
        if(placement.size()==0){placement.push_back(0);continue;}
        int offset =placement.back()+groups[i-1]+1;
        placement.push_back(offset);
    }
    int exhausted_index = groups.size();//Index of the First group we have tested all positions (most importantly as late placed as possible)
    //cout<<"New Row"<<endl;
    arrangements += is_valid_placement2(record,placement,groups);//In case the first arrangement is valid
    //cout<<"New Row"<<endl;
    while(exhausted_index!=0){
        //cout<<"E_x:"<<exhausted_index<<endl;
        if(exhausted_index == groups.size()){//Test Final Group
            //cout<<"Final Group moved"<<endl;
            //For day 2 we only allow solutions that have a Final Group that is valid
            for(int i=placement[exhausted_index-1]+1;i<=record.size()-groups[exhausted_index-1];i++){
                bool fits= true;
                for(int j=0;j!=groups[exhausted_index-1];j++){
                    if(record[i+j]=='.'){fits=false;}//Attempted Placement contains a op spring
                }
                //if(i+irow.groups[exhausted_index-1]<=irow.record.size()-1){//Stops Springtest if Group is up to the edge
                if(record[i+groups[exhausted_index-1]]=='#'){fits=false;}//Attempted Placement contains ends in broken spring
                //}
                if(fits){
                    placement[exhausted_index-1]=i;
                    goto final_placed;
                }
            }
            //cout <<"No final config can fit"<<endl;
            exhausted_index--;
            continue;
            //cout<< "Final Placed"<<endl;
            final_placed:;
        }else{//Test Last Group
            //For day 2 we only allow solutions that have a Group that is valid
            for(int i=placement[exhausted_index-1]+1;i<=placement[exhausted_index]-groups[exhausted_index-1]-1;i++){
                bool fits= true;
                for(int j=0;j!=groups[exhausted_index-1];j++){
                    if(record[i+j]=='.'){fits=false;}//Attempted Placement contains a op spring
                }
                if(record[i+groups[exhausted_index-1]]=='#'){fits=false;}//Attempted Placement contains ends in broken spring
                if(fits){
                    placement[exhausted_index-1]=i;
                    //The following is rough, but could be improved for the reseting lower stages
                    for(int reset_i=exhausted_index;reset_i!=groups.size();reset_i++){//Move all later groups to the earliest options
                    placement[reset_i] = placement[reset_i-1]+ groups[reset_i-1]+1;
                    }
                    exhausted_index = groups.size();//Reset exhausted index;
                    goto last_placed;
                }
            }
            exhausted_index--;
            continue;
            last_placed:;
            //cout<< "Last Placed"<<endl;
        }
        arrangements += is_valid_placement2(record,placement,groups);
        //if(is_valid_placement2(irow.record,placement,irow.groups)){cout<<"V ";}
        //cout<<"Testing:"<<endl;
        //for(auto&x:placement){cout<<x<<",";}cout<<endl;
        }
    if(visuals){cout<< arrangements << " Combinations"<<endl;}
    return arrangements;
}

//void display_placement(string&record,vector<int>placement,vector<int> groups){

void D_12_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("12-1-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    //Essentially just Nonogramms
    int folds = 2;
    bool optimising = true;
    struct row{
    vector<string>records;
    string given_record;
    vector<vector<int>> groups;
    int arrangements = 1;
    };
    vector<row> rows;

    for(int i =0;i != inputvector.size();i++){
            vector<string> ssplit = split_string(inputvector[i],' ');
            vector<string> sgroups = split_string(ssplit[1],',');
            row newrow;
            string initalstring;
            vector<int> initalgroups;
            for(int j=0;j!=folds-1;j++){//Additional folds
                initalstring.append(ssplit[0]).append("?");
                for(auto&group:sgroups){
                    initalgroups.push_back(stoi(group));
                }
            }
            initalstring.append(ssplit[0]);
            for(auto&group:sgroups){
                initalgroups.push_back(stoi(group));
            }
            newrow.given_record = initalstring;
            newrow.records.push_back(initalstring);
            newrow.groups.push_back(initalgroups);
            rows.push_back(newrow);
    }

    if(optimising){
    cout<< "Reducing"<<endl;
    for(auto&irow:rows){//For each row

        //Our first step is trying to split the Full Record into sections that have a set border group (the group needs to be place at the specific index)
        for(int section=0;section!=irow.records.size();section++){//For each section
            vector<int> spaces;
            vector<int> groups =irow.groups[section];
            sort(groups.begin(),groups.end(),greater<int>());

            int unbroken_len = 0;
            for(int i=0;i!=irow.records[section].size();i++){
                if(irow.records[section][i]=='.'){
                    if(unbroken_len!=0){spaces.push_back(unbroken_len);}
                    unbroken_len = 0;
                }else{unbroken_len++;}
            }
            if(unbroken_len!=0){spaces.push_back(unbroken_len);}

            sort(spaces.begin(),spaces.end(),greater<int>());
            //for(auto&space:spaces){cout<<space<<",";}cout<<endl;
            //for(auto&group:groups){cout<<group<<",";}cout<<endl;
            //for(auto&section:irow.records){cout<<section<<" ";}cout<<" | ";
            //for(int i=0;i!=irow.groups[section].size();i++){ cout<< irow.groups[section][i]<<",";}
            //cout<<endl;
            //cout<<"Fitting "<<largest_group<<" into "<<largest_space<<endl;
            vector<int> filling;
            bool fill_overflow;
            for(int space_i=0;space_i!=spaces.size();space_i++){//
                if(space_i==folds){break;}
                if(spaces[space_i]==groups[space_i]){
                    filling.push_back(groups[space_i]);
                }
                if(spaces[space_i]>=(groups[space_i]+1)*2-1){
                    fill_overflow=true;break;
                    //No time to Implement.
                }
            }
            if(!fill_overflow){
                vector<int> filling_indexes;
                for(auto&x:filling){
                    unbroken_len = 0;
                    for(int i=0;i!=irow.records[section].size();i++){
                        if(irow.records[section][i]=='.'){
                            if(unbroken_len==x){filling_indexes.push_back(i-1);}
                            unbroken_len = 0;
                        }else{unbroken_len++;}
                    }
                }

                //Now we find the space that exactly matches
                cout<<fill_overflow;
            }
            cout<<"Filling "<<filling.size()<<" spaces"<<endl;
        }
    }
    }
    //return;

    for(auto&irow:rows){
        for(int section=0;section!=irow.records.size();section++){
            //for(auto&sect:irow.records[section]){cout<<sect;}cout<<endl;
            irow.arrangements*=arrangements(irow.records[section],irow.groups[section],true);
        }
    }

    int sum = 0;
    for(auto&irow:rows){
        sum+=irow.arrangements;
    }
    cout<<"For "<<folds << " folds the Sum of all Rows is: "<<sum<<endl;
    return;
}
//Too low 6785











