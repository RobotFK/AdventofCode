#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

void D_2_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("02-1.txt");
    while (getline(inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    vector<int> legal_games;
    int sum = 0;
    for (auto &game : inputvector){
            //cout <<endl<< game << endl;
            string sgame,draw,color;
            stringstream sg(game);
            getline(sg,sgame,':');
            int gameid = 0;
            int red=0,blue=0,green=0;
            gameid = stoi(sgame.substr(4));
            //cout << "Game is "<<gameid<<endl;

            getline(sg,sgame,':'); // Move to the next Segement
            stringstream sd(sgame);//Stream Draw
            while(getline(sd,draw,';')){
                //cout << "Draws is"<<draw<<endl;
                stringstream sc(draw);//Stream Colors
                while(getline(sc,color,',')){
                    //cout << color << " ";
                    int amount = stoi(color.substr(color.find(" ")));
                    if (color.find("red") != std::string::npos) {
                            if (amount > red){red = amount;}
                    }
                    if (color.find("green") != std::string::npos) {
                            if (amount > green){green = amount;}
                    }
                    if (color.find("blue") != std::string::npos) {
                            if (amount > blue){blue = amount;}
                    }
                }
                //cout << endl;
            }

            legal_games.push_back((red*blue*green));
    }
    cout <<endl;
    for(auto& x:legal_games){
        cout << x << " ";
        sum+= x;
    }
    cout <<endl<<"Sum is: "<< sum << endl;
    return;
}

