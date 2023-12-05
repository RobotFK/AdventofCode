#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

void D_5_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("05-1-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    int locationsamount = 8;
    struct range{
    long long lower;
    long long upper;
    };
    struct seed{
        vector<vector<range>> values;
    };
    vector<seed> seeds;
    struct stl{ //seed to location;
        string type;
        long long lstart;//location Start
        long long sstart;//Seed Start
        long long range;
    };
    vector<stl> stls;

    string locations[locationsamount]{"seed","soil","fertilizer","water","light","temperature","humidity","location"};


    int stage = -1;  //Type of data we read in
    for(auto &x:inputvector){
        if(x.size() == 0){stage++;continue;cout << "Nxt Stage"<<endl;}
        if(stage == -1){
            vector<string> seedstrings = split_string(x,' ');
            for(int i=1;i != seedstrings.size();i+=2){
                seed newseed;
                range newrange;
                newrange.lower = stoll(seedstrings[i]);
                newrange.upper = stoll(seedstrings[i+1]);
                newseed.values.push_back({newrange});
                seeds.push_back(newseed);
            }
        }else{
            if(!isdigit(x[0])){continue;}
            stl newstl;
            newstl.type = locations[stage+1];
            vector<string> stlstrings = split_string(x,' ');
            newstl.lstart = stoll(stlstrings[0]);
            newstl.sstart = stoll(stlstrings[1]);
            newstl.range = stoll(stlstrings[2]);
            stls.push_back(newstl);
        }
    }
    cout <<"Seedgroups: "<<seeds.size() <<endl;
    cout <<"Mapperamount: "<<stls.size() <<endl;
    cout << endl;
    //for(auto &istl:stls){cout<< istl.type << ": " << istl.sstart << " , "<< istl.range << endl;}
    //for(auto &iseed:seeds){cout<< iseed.values[0] << endl;}

    for(auto &iseed:seeds){//Each Seed
        for(int stage = 1;stage != locationsamount;stage++){//Each Stage,skipping inital value
            long long value = -1;
            //cout << "Entering with "<< iseed.values[stage-1]<<endl;
            for(auto &istl:stls){//Each Tranfomration
                    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                if (istl.type != locations[stage]){continue;}
                long long startoffset = iseed.values[stage-1] -istl.sstart;
                //cout<<locations[stage]<<": "<< startoffset<< endl;
                //cout<<iseed.values[stage-1] <<" -"<<istl.sstart<<endl;
                if( startoffset >= 0 && startoffset < istl.range){
                    //cout <<istl.type <<" Inside " << istl.sstart << "->" << istl.sstart+(istl.range-1)<< endl;
                    //cout <<istl.type<<": "<<istl.lstart+startoffset<<endl;
                    value = istl.lstart+startoffset;
                }
            }
            if(value == -1){
                //cout<< "No mapping found, defaulting" << endl;
                value = iseed.values[stage-1];
            }
            iseed.values.push_back(value);
            //cout<< "Final location Value: "<< iseed.values[stage]<<endl;
            //break;
        }
        cout<<"Seed Done"<<endl;
    }

    long long minvalue  = -1;
    for(auto &iseed:seeds){
        if(iseed.values.back() < minvalue || minvalue == -1){minvalue =iseed.values.back();}
    }
    cout <<endl<< "Minvalue: " << minvalue<< endl;

    return;
}
