#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

void D_4_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("04-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

     struct card {
      int cardid;
      vector<int> winners;
    };
    vector<card> cards;

    int sum = 0;
    for(auto &line:inputvector){
        struct card newcard;

        stringstream sg(line);
        string card, wnumb, hnumb; //Cardnumber, Winning numbers and numbers had
        vector<int> winners,owned;

        getline(sg,card,':');
        newcard.cardid = stoi(card.substr(4));
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
        for(auto &hav:owned){
                for(auto &win:winners){
                    if(hav == win){newcard.winners.push_back(win);}
                }
        }
        cards.push_back(newcard);
    }

    vector<int> cardamount(inputvector.size(),1);
    for(int i=0;i!=cardamount.size()-1;i++){
        if(cards[i].winners.size()==0){continue;}
        //cout<<"Card "<<i<<": ";
        for(int wins=0;wins !=cards[i].winners.size();wins++){
            //cout<<"+"<<cardamount[i]<<",";
            if(wins+i+1 >= cardamount.size()) {continue;}
            cardamount[i+wins+1]+=cardamount[i];
        }
        //cout<<endl;
    }
    for(auto &card:cardamount){
            //cout<<card<<",";
            sum+=card;
    }

    cout <<endl<< "Sum:"<<sum<<endl;
    return;
}
