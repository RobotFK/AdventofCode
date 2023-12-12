#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

bool is_valid_placement(string&record,vector<int>&placement,vector<int>&groups,bool visuals=false){
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

//void display_placement(string&record,vector<int>placement,vector<int> groups){

void D_12_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("12-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    //Essentially just Nonogramms

    struct row{
    string record;
    vector<int> groups;
    int arrangements = 0;
    };
    vector<row> rows;

    for(int i =0;i != inputvector.size();i++){
            vector<string> ssplit = split_string(inputvector[i],' ');
            vector<string> sgroups = split_string(ssplit[1],',');
            row newrow;
            newrow.record = ssplit[0];
            for(auto&group:sgroups){
                newrow.groups.push_back(stoi(group));
            }
            rows.push_back(newrow);
    }

    for(auto&irow:rows){
        vector<int>placement;
        for(int i=0;i!=irow.groups.size();i++){// Inital layout, does not have to be valid
            if(placement.size()==0){placement.push_back(0);continue;}
            int offset =placement.back()+irow.groups[i-1]+1;
            placement.push_back(offset);
        }

        int exhausted_index = irow.groups.size();//Index of the First group we have tested all positions (most importantly as late placed as possible)
        //cout<<"New Row"<<endl;
        irow.arrangements += is_valid_placement(irow.record,placement,irow.groups);//In case the first arrangement is valid
        //cout<<"New Row"<<endl;
        while(exhausted_index!=0){


            if(exhausted_index == irow.groups.size()){//Test Final Group
                if(placement[exhausted_index-1]+irow.groups[exhausted_index-1]<irow.record.size()){//Test if we can move the Final Group one further
                    //cout<< "Final move ";
                    placement[exhausted_index-1]++;
                }else{//Final Group can't be moved anymore
                    exhausted_index--;
                    continue;
                }
            }else{//Test Last Group
                if(placement[exhausted_index-1]+irow.groups[exhausted_index-1]+1 < placement[exhausted_index]){//Test if we can move the last not exhausted one further
                    //cout<< "last move ";
                    placement[exhausted_index-1]++;
                    for(int i=exhausted_index;i!=irow.groups.size();i++){//Move all later groups to the earliest options
                        placement[i] = placement[i-1]+ irow.groups[i-1]+1;
                    }
                    exhausted_index = irow.groups.size();//Reset exhausted index;
                }else{
                    exhausted_index--;
                    continue;
                }
            }
            irow.arrangements += is_valid_placement(irow.record,placement,irow.groups);
        }
        //cout<< irow.arrangements << " Combinations"<<endl;
    }

    int sum = 0;
    for(auto&irow:rows){
        sum+=irow.arrangements;
    }
    cout<< "Sum of all Rows is: "<<sum<<endl;
    return;
}
//Too low 6785











