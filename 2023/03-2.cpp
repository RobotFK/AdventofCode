#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
using namespace std;

//Asuming no number has more than three digits
class gear;

class number_2{
    public:
    unsigned int value;
    unsigned int index_x;
    unsigned int index_y;
    char adjacent_symbol ='.';
    unsigned int symbol_index_x;
    unsigned int symbol_index_y;
    unsigned int len = 1;

    number_2(unsigned int y,unsigned int x){
        this->index_x = x;
        this->index_y = y;
    }
    void connect_gear(int index_y,int index_x){
        symbol_index_x = index_x;
        symbol_index_y = index_y;
    }

    void calc_value(vector<string> inputvector){
        string str;
        str+=inputvector[index_y][index_x];
        int ilen = 1;
        while(isdigit(inputvector[index_y][index_x+ilen])){
            str+=inputvector[index_y][index_x+ilen];
            ilen++;
        }
        this->len = ilen;
        this->value= stoi(str);
        return;
    }
    void calc_adjacent_symbol(vector<string> inputvector, bool visuals =false){
    // 6 manual Checks + 2*len checks
    char checkc; //Checkstring
        if(index_x != 0){//Check left
            if(index_y != 0){
                checkc = inputvector[index_y-1][index_x-1];
                if(ispunct(checkc) && checkc != '.'){
                    this->adjacent_symbol = checkc;
                    if(visuals){cout<<"LT"<<endl;}
                    if(checkc == '*'){connect_gear(index_y-1,index_x-1);}
                    }
            }
                checkc = inputvector[index_y][index_x-1];
                if(ispunct(checkc) && checkc != '.'){
                    this->adjacent_symbol = checkc;
                    if(visuals){cout<<"L"<<endl;}
                    if(checkc == '*'){connect_gear(index_y,index_x-1);}
                    }
            if(index_y != (inputvector.size()-1)){
                checkc = inputvector[index_y+1][index_x-1];
                if(ispunct(checkc) && checkc != '.'){
                    this->adjacent_symbol = checkc;if(visuals){cout<<"LB"<<endl;}
                    if(checkc == '*'){connect_gear(index_y+1,index_x-1);}
                }

            }
        }
        if(index_x+len != inputvector[index_y].size()-1){//Check Right
            if(index_y != 0){
                checkc = inputvector[index_y-1][index_x+len];
                if(ispunct(checkc) && checkc != '.'){
                    this->adjacent_symbol = checkc;if(visuals){cout<<"RT"<<endl;}
                    if(checkc == '*'){connect_gear(index_y-1,index_x+len);}
                }
            }
                checkc = inputvector[index_y][index_x+len];
                if(ispunct(checkc) && checkc != '.'){
                    this->adjacent_symbol = checkc;if(visuals){cout<<"R"<<endl;}
                    if(checkc == '*'){connect_gear(index_y,index_x+len);}
                }

            if(index_y != (inputvector.size()-1)){
                checkc = inputvector[index_y+1][index_x+len];
                if(ispunct(checkc) && checkc != '.'){
                        this->adjacent_symbol = checkc;
                        if(visuals){cout<<"RB"<<endl;}
                        if(checkc == '*'){connect_gear(index_y+1,index_x+len);}
                }
            }
        }
        for(int i = 0;i!= len;i++){
            if(index_y != 0){//Above
                checkc = inputvector[index_y-1][index_x+i];
                if(ispunct(checkc) && checkc != '.'){
                    this->adjacent_symbol = checkc;
                    if(visuals){cout<<"T"<<endl;}
                    if(checkc == '*'){connect_gear(index_y-1,index_x+i);}
                }
            }
            if(index_y != (inputvector.size()-1)){//Below
                checkc = inputvector[index_y+1][index_x+i];
                if(ispunct(checkc) && checkc != '.'){
                    this->adjacent_symbol = checkc;
                    if(visuals){cout<<"B"<<endl;}
                    if(checkc == '*'){connect_gear(index_y+1,index_x+i);}
                }
            }
        }
    return;
    }
};

class gear{
    public:
    unsigned int value = 1;
    unsigned int index_x;
    unsigned int index_y;
    int parts = 0;

    gear(unsigned int y,unsigned int x){
        this->index_x = x;
        this->index_y = y;
    }
};

void D_3_2(){
    static  vector<string> inputvector;
    string line;
    ifstream inputread("03-1.txt");
    while (getline(inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;
    vector<number_2> numbers;
    vector<gear> gears;
    for(int y_i =0;y_i != inputvector.size();y_i++){
        for(int x_i =0;x_i != inputvector[y_i].size();x_i++){
            //cout<<"("<<y_i<<"-"<<x_i<<")"<<endl;
            if(inputvector[y_i][x_i] == '*'){
                    gear newgear(y_i,x_i);
                    gears.push_back(newgear);
            }
            if(x_i ==0){
                if(isdigit(inputvector[y_i][x_i])){
                    number_2 newnumber(y_i,x_i);
                    numbers.push_back(newnumber);
                }
            }else if (isdigit(inputvector[y_i][x_i]) &&  !isdigit(inputvector[y_i][x_i-1])){
                number_2 newnumber(y_i,x_i);
                numbers.push_back(newnumber);
            }
        }
    }

    int sum1 = 0;//Left in as a Sanity Check
    int sum2 = 0;
    for(auto &x:numbers){
        x.calc_value(inputvector);
        x.calc_adjacent_symbol(inputvector);
        //cout << x.value<< ": "<< x.adjacent_symbol <<endl;
        if(x.adjacent_symbol != '.'){sum1+=x.value;}
        if(x.adjacent_symbol == '*'){
            //cout <<"Looking for "<< x.symbol_index_x <<"-"<< x.symbol_index_y<<endl;
            for (auto &igear:gears){//Ugly, I know
                //cout<<igear.index_x <<"-"<<igear.index_y<<endl;
                if(x.symbol_index_x == igear.index_x && x.symbol_index_y == igear.index_y){
                    igear.value *= x.value;
                    igear.parts++;
                    //cout << "Multiplied by" <<x.value<<endl;
                }
            }
        }
    }

    for (const auto &igear:gears){
        if(igear.parts==2){
            sum2+=igear.value;
        }
    }
    cout <<"Sum1 is "<<sum1<< endl;
    cout <<"Sum2 is "<<sum2<< endl;



    return;
}
