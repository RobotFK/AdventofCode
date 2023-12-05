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
    ifstream inputread("05-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    int locationsamount = 8;
    struct range{
    long long upper;
    long long lower;
    };
    struct seed{
        vector<vector<range>> values;
    };
    vector<seed> seeds;
    struct stl{ //seed to location;
        string type;
        long long lstart;//location Start
        long long lower;//Lower limit to take input from
        long long upper;//Upper limit to take input from
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
                newrange.upper = stoll(seedstrings[i+1])+newrange.lower-1;//-1 as the value only is the range and not the addition
                newseed.values.push_back({newrange});
                seeds.push_back(newseed);
            }
        }else{
            if(!isdigit(x[0])){continue;}
            stl newstl;
            newstl.type = locations[stage+1];
            vector<string> stlstrings = split_string(x,' ');
            newstl.lstart = stoll(stlstrings[0]);
            newstl.lower = stoll(stlstrings[1]);
            newstl.range = stoll(stlstrings[2]);
            newstl.upper = newstl.lower+newstl.range-1;
            stls.push_back(newstl);
        }
    }

    cout <<"Seedgroups: "<<seeds.size() <<endl;
    cout <<"Mapperamount: "<<stls.size() <<endl;
    cout << endl;
    //for(auto &iseed:seeds){cout<< iseed.values[0][0].lower <<" - "<< iseed.values[0][0].upper << endl;}

    for(auto &iseed:seeds){//Each Seed
        for(int stage = 1;stage != locationsamount;stage++){//Each Stage,skipping inital value
            cout<<endl<<endl<<"Stage: to "<<locations[stage] <<endl<<endl;
            if(iseed.values.size()<= stage){
                    iseed.values.push_back(vector<range>());
            }
            cout << "Entering with "<< iseed.values[stage-1].size()<<" ranges "<<endl;
            for(auto &irange:iseed.values[stage-1]){//Each range
                cout << "Range: " <<irange.lower<<" x "<<irange.upper<< endl;
            }
            for(auto &irange:iseed.values[stage-1]){//Each range
                bool fully_utilised = false;
                cout << irange.lower<<" - " << irange.upper<< " :"<<endl;
                for(auto &istl:stls){//Each Transfomration
                    if (istl.type != locations[stage]){continue;}//Wrong stage
                    long long offset = istl.lstart-istl.lower ;
                    //cout << "Offset: "<<offset<<endl;
                    if(max(irange.lower,istl.lower) < min(irange.upper,istl.upper)){//Has overlap
                        /*
                        The Order of Checks here is if the Transformation segment is:
                        Enclosing full
                        or  Enclosing the End
                        or  Enclosing the Start
                        or  Enclosing Start and End
                        */
                        if(istl.upper >= irange.upper && istl.lower <= irange.lower){// Transformation fully encompasses Range
                            cout <<"Transformation fully encompasses Range +"<< offset <<endl;
                            range newrange;
                            newrange.lower = irange.lower+offset;
                            newrange.upper = irange.upper+offset;
                            iseed.values[stage].push_back(newrange);

                            fully_utilised = true;
                            continue;
                        }else if(irange.upper <= istl.upper){//Range (partially)covered for Transformation, limited by Upper Range of the input (stretches from Lower Transformation to upper range limit)
                            cout <<"Section stretches from Lower Transformation to upper range limit+"<< offset <<endl;
                            range newrange;
                            newrange.lower = istl.lower+offset;
                            newrange.upper = irange.upper+offset;
                            iseed.values[stage].push_back(newrange);

                            irange.upper = istl.lower-1;//remove Used Segment

                        }else if(irange.lower >= istl.upper){//Range (partially)covered for Transformation, limited by lower Range of the input (stretches from Lower range limit to  upper limit Transformation)
                            cout <<"Section stretches from Lower range limit to  upper limit Transformation+"<< offset <<endl;
                            range newrange;
                            newrange.lower = irange.lower+offset;
                            newrange.upper = istl.upper+offset;
                            iseed.values[stage].push_back(newrange);

                            irange.lower = istl.upper-1;//remove Used Segment

                        }else if(irange.upper >= istl.upper && irange.lower <= istl.lower){//Range fully encompasses Transformation
                            cout <<"Range fully encompasses Transformation+"<< offset <<endl;
                            range newrange;
                            newrange.lower = istl.lower+offset;
                            newrange.upper = istl.upper+offset;
                            iseed.values[stage].push_back(newrange);

                            continue;// Transformation fully Utilised
                        }else if (false){
                            //cout << "A Terrible Error has occured"<<endl;
                            cout << irange.lower<< " - "<< irange.upper << " Failed to fit into "<< istl.lower << " - "<<istl.upper<<endl;
                            }
                    }
                }
                long long remainingrange = irange.upper -irange.lower;

                if(remainingrange >= 0 and !fully_utilised){
                    cout << "Could not Transform "<< remainingrange << " states"<<endl;
                    range newrange;
                    newrange.lower = irange.lower;
                    newrange.upper = irange.upper;
                    iseed.values[stage].push_back(newrange);
                }
            }
            //cout<< "Final location Value: "<< iseed.values[stage]<<endl;
            //break;
        }
        cout<<"Seed Done"<<endl;
        //break;
    }

    long long minvalue = -1;
    for(auto &iseed:seeds){//Each Seed
        for(auto &irange:iseed.values[locationsamount-1]){//Each range
            if(irange.lower < minvalue || minvalue == -1){minvalue = irange.lower; }
        }
    }
    cout <<endl<< "Minvalue: " << minvalue<< endl;

    return;
}
