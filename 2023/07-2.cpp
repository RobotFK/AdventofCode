#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int calculate_value2(vector<int>&cards){
    vector<int>cardcount (15,0);
    int jokers=0;
    for(auto&x:cards){
        if(x==1){jokers++;}else{
        cardcount[x]+=1;
        }
    }
    vector<int>cardamount;

    for(int i=cardcount.size()-1;i!=0;i--){
        if(cardcount[i]!=0){
            //cout<< i<<": "<<cardcount[i]<< endl;
            cardamount.push_back(cardcount[i]);
        }
    }//cout<<endl;
    cardamount.push_back(0);//Empty amount in case of only Jokers
    sort(cardamount.begin(),cardamount.end(),greater<int>());
    cardamount[0]+=jokers;
    if(cardamount[0]>5){cardamount[0]=5;}//Stops groups of Jokers messing it up

    switch(cardamount[0]){//How many we have of the Most common card
        case 5:
            return 6;
        break;
        case 4:
            return 5;
        break;
        case 3:
            if(cardamount[1]==2){return 4;}//full house
            else{return 3;}
        break;
        case 2:
            if(cardamount[1]==2){return 2;}//two pairs house
            else{return 1;}
        break;
        default:
            return 0;//No value in Type

    }
return 0;
};

void D_7_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("07-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    map<char, int> cardvalue{{'A',14},{'K',13},{'Q',12},{'J',1},{'T',10}};
    struct hand{
        vector<int> cards;
        int bet;
        int value;//7 possible values

        bool operator < (const hand& comp) const
        {
            if(value!=comp.value){return (value < comp.value);};
            for(int i=0;i!=cards.size();i++){
                if(cards[i]==comp.cards[i]){continue;}
                else{return(cards[i]<comp.cards[i]);}
            }
            cout<<"Equal"<<endl;
            return false;
        }
    };
    vector<hand>hands;

    for(int i =0;i != inputvector.size();i++){
        //cout<<inputvector[i]<< endl;
        hand newhand;
        vector<string> strings = split_string(inputvector[i],' ');
        for(auto ichar:strings[0]){
            if(cardvalue.count(ichar)){newhand.cards.push_back(cardvalue[ichar]);}
            else{newhand.cards.push_back(int(ichar - '0'));}
        }
        //sort(newhand.cards.begin(),newhand.cards.end(),greater<int>());
        newhand.bet = stoi(strings[1]);
        hands.push_back(newhand);
    }

    for(auto&x:hands){
        //for(auto&y:x.cards){cout<<y<< " ";}cout<<endl;
        x.value = calculate_value2(x.cards);
        //cout <<"Value:"<< x.value<<endl;
    }
    sort(hands.begin(),hands.end());
    int sum = 0;
    for(int i=0;i!=hands.size();i++){
        sum+=hands[i].bet*(i+1);
        cout<<"("<<hands[i].value<<") "<<hands[i].bet<<" * "<< i+1<<endl;
    }
    cout <<"Sum "<<sum<<endl;
    return;
}
//Too low 249163737
//Too low 249709814
//Too high251633916
