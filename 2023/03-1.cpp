#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <string>
using namespace std;

//Asuming no number has more than three digits

class number{
    public:
    unsigned int value;
    unsigned int index_x;
    unsigned int index_y;
    char adjacent_symbol ='.';
    unsigned int len = 1;

    number(unsigned int y,unsigned int x){
        this->index_x = x;
        this->index_y = y;
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
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"LT"<<endl;}}
            }
                checkc = inputvector[index_y][index_x-1];
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"L"<<endl;}}
            if(index_y != (inputvector.size()-1)){
                checkc = inputvector[index_y+1][index_x-1];
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"LB"<<endl;}}
            }
        }
        if(index_x+len != inputvector[index_y].size()-1){//Check Right
            if(index_y != 0){
                checkc = inputvector[index_y-1][index_x+len];
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"RT"<<endl;}}
            }
                checkc = inputvector[index_y][index_x+len];
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"R"<<endl;}}
            if(index_y != (inputvector.size()-1)){
                checkc = inputvector[index_y+1][index_x+len];
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"RB"<<endl;}}
            }
        }
        for(int i = 0;i!= len;i++){
            if(index_y != 0){//Above
                checkc = inputvector[index_y-1][index_x+i];
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"T"<<endl;}}
            }
            if(index_y != (inputvector.size()-1)){
                checkc = inputvector[index_y+1][index_x+i];
                if(ispunct(checkc) && checkc != '.'){this->adjacent_symbol = checkc;if(visuals){cout<<"B"<<endl;}}
            }
        }
    return;
    }
};

void D_3_1(){
    static  vector<string> inputvector;
    string line;
    ifstream inputread("03-1.txt");
    while (getline(inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<number> numbers;
    for(int y_i =0;y_i != inputvector.size();y_i++){
        for(int x_i =0;x_i != inputvector[y_i].size();x_i++){
            //cout<<"("<<y_i<<"-"<<x_i<<")"<<endl;
            if(x_i ==0){
                if(isdigit(inputvector[y_i][x_i])){
                    number newnumber(y_i,x_i);
                    numbers.push_back(newnumber);
                }
            }else if (isdigit(inputvector[y_i][x_i]) &&  !isdigit(inputvector[y_i][x_i-1])){
                number newnumber(y_i,x_i);
                numbers.push_back(newnumber);
            }
        }
    }

    int sum = 0;
    for(auto x:numbers){
        x.calc_value(inputvector);
        x.calc_adjacent_symbol(inputvector);
        //cout << x.value<< ": "<< x.adjacent_symbol <<endl;
        if(x.adjacent_symbol != '.'){sum+=x.value;}
    }
    cout <<"Sum is "<<sum<< endl;




    return;
}
