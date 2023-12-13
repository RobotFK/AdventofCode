#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_13_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("13-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    struct field{
    vector<string> rows;
    int v_mirror=-1;//First index of the second half
    int h_mirror=-1;//First index of the second half
    };
    vector<field> fields;

    field newfield;
    fields.push_back(newfield);
    for(int i =0;i != inputvector.size();i++){
            if(inputvector[i].size()==0){
                fields.push_back(newfield);
                field newfield;
                continue;
            }
            fields.back().rows.push_back(inputvector[i]);
            //cout<<inputvector[i]<<" | "<<inputvector[i].size() <<endl;
    }

    cout<<"Found "<< fields.size() <<" fields"<<endl;

    for(auto&ifield:fields){
        for(int i=0;i!=ifield.rows.size();i++){cout<<ifield.rows[i]<<endl;}
        //Horizontal split
        for(int i=1;i!=ifield.rows.size();i++){//Split location
            bool valid=true;
            for(int j=0;j!=ifield.rows.size()-i;j++){//Testing depth from split
                if(i-j-1 <0){break;}
                if(ifield.rows[i+j]!=ifield.rows[i-j-1]){valid=false;break;}
                //cout<<"("<<i-j-1<<"<>"<<i+j<<")";
            }
            if(valid){
                ifield.h_mirror = i;
                cout<<"Valid h mirror at index "<<i<<endl;
            }
            //cout<<endl;
        }
        //cout<<"V:"<<endl;
        //Vertical Split
        for(int i=1;i!=ifield.rows[0].size();i++){//Split location
            bool valid=true;
            for(int j=0;j!=ifield.rows[0].size()-i;j++){//Testing depth from split
                if(i-j-1 <0){break;}
                //cout<<"("<<i-j-1<<"<>"<<i+j<<")";
                for(int k=0;k!=ifield.rows.size();k++){
                    if(ifield.rows[k][i+j]!=ifield.rows[k][i-j-1]){valid=false;break;}

                }//cout<<endl;
            }
            if(valid){
                ifield.v_mirror = i;
                cout<<"Valid v mirror at index "<<i<<endl;
            }
            //cout<<endl;
        }
        //cout<<endl;
    }

    int sum = 0;
    for(auto&ifield:fields){
        if(ifield.v_mirror != -1){sum+=ifield.v_mirror;}
        if(ifield.h_mirror != -1){sum+=100*(ifield.h_mirror);}
    }
    cout<<"Sum "<<sum<<endl;
    return;
}
