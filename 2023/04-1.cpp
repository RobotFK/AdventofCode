#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

void D_4_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("04-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    int sum = 0;
    for(auto &line:inputvector){

        stringstream sg(line);
        string card, wnumb, hnumb; //Cardnumber, Winning numbers and numbers had
        vector<int> winners,owned;

        getline(sg,card,':');
        int cardid;
        cardid = stoi(card.substr(4));
        getline(sg,card,':'); // Move to the next Segement
        stringstream sc (card);//Stream card
        getline(sc,card,'|');
        stringstream sw(card);//Stream winners
        while(getline(sw,wnumb,' ')){
            if(wnumb.size() == 0){continue;}
            winners.push_back(stoi(wnumb));

        }
        getline(sc,card,'|');
        stringstream sh(card);//Stream haves
        while(getline(sh,hnumb,' ')){
            if(hnumb.size() == 0){continue;}
            owned.push_back(stoi(hnumb));

        }

        double exponent = 0;
        double base = 2;
        for(auto &hav:owned){
                for(auto &win:winners){
                    if(hav == win){exponent++;}
                }
        }
        double worth = exponent>=1 ?pow (base, exponent-1):0;
        sum+=worth;
    }
    cout<<"Sum is: "<<sum<<endl;
    return;
}
