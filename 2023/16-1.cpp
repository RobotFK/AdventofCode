#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_16_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("16-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    //if we use the input vector as a static map, we could make a second map of where light is

    vector<vector<int>>light_field;
    // 2= Up
    // 3= Right
    // 5= Down
    // 7= Left
    for(auto&line:inputvector){
		vector<int> new_light_light(line.size(),1);
		light_field.push_back(new_light_light);
    }
	//light_field[0][0] = 2;
	int max_x =light_field[0].size()-1;
	int max_y =light_field.size()-1;

	struct beam_head{
		int x;
		int y;
		int direction;
	};
	vector<beam_head> heads;

	beam_head start;
	start.x=0;
	start.y=0;
	start.direction=5;//Would be 3 Normally, but my input starts with a \ so it has to be immediatly rotated
	heads.push_back(start);
/*
	beam_head start;
	start.x=1;
	start.y=8;
	start.direction=2;
	heads.push_back(start);

	start.x=1;
	start.y=7;
	start.direction=7;
	heads.push_back(start);
*/
	while(heads.size()!=0){
		vector<bool> dead_heads(line.size(),false);
		//cout<<endl<<"Moving " <<heads.size()<<" Heads"<<endl;
		int amount = heads.size();
		for(int head_i=0;head_i!=amount;head_i++){
			beam_head&head = heads[head_i];
			//cout<<"Head "<< head_i <<" ( "<<head.x<<" - "<<head.y<<" ): "<<head.direction <<endl;
			if(light_field[head.y][head.x]%head.direction!=0){light_field[head.y][head.x]*=head.direction;}
			else{//Head is a duplicate
				dead_heads[head_i]=true;
				goto next_head;
			}

			switch(head.direction){
			case 2://up
				if(head.y ==0){
						dead_heads[head_i]=true;
						goto next_head;
				}
				head.y--;
				switch(inputvector[head.y][head.x]){
					case '/':
						head.direction=3;
					break;
					case '\\':
						head.direction=7;
					break;
					case '-':
						head.direction=3;
						beam_head new_head;
						new_head.x = head.x;
						new_head.y = head.y;
						new_head.direction=7;
						heads.push_back(new_head);
					break;
				}
				break;

			case 3://Right
				if(head.x ==max_x){
						dead_heads[head_i]=true;
						goto next_head;
				}
				head.x++;
				switch(inputvector[head.y][head.x]){
					case '/':
						head.direction=2;
					break;
					case '\\':
						head.direction=5;
					break;
					case '|':
						head.direction=5;
						beam_head new_head;
						new_head.x = head.x;
						new_head.y = head.y;
						new_head.direction=2;
						heads.push_back(new_head);
					break;
				}
				break;
			case 5://Down
				if(head.y ==max_y){
						dead_heads[head_i]=true;
						goto next_head;
				}
				head.y++;
				switch(inputvector[head.y][head.x]){
					case '/':
						head.direction=7;
					break;
					case '\\':
						head.direction=3;
					break;
					case '-':
						head.direction=3;
						beam_head new_head;
						new_head.x = head.x;
						new_head.y = head.y;
						new_head.direction=7;
						heads.push_back(new_head);
					break;
				}
				break;
			case 7://Left
				if(head.x ==0){
						dead_heads[head_i]=true;
						goto next_head;
				}
				head.x--;
				switch(inputvector[head.y][head.x]){
					case '/':
						head.direction=5;
					break;
					case '\\':
						head.direction=2;
					break;
					case '|':
						head.direction=2;
						beam_head new_head;
						new_head.x = head.x;
						new_head.y = head.y;
						new_head.direction=5;
						heads.push_back(new_head);
					break;
				}
				break;
			}



			next_head:;
		}
		int removed=0;
		for(int head_i=0;head_i<dead_heads.size();head_i++){
			if(dead_heads[head_i]){
				//cout<<"Stopped Head "<< head_i <<" ( "<<heads[head_i].x<<" - "<<heads[head_i].y<<" ): "<<heads[head_i].direction <<endl;
				heads.erase(heads.begin()+head_i-removed);
				removed++;
			}
		}
		//break;
	}


    if(true){
	for(auto&line:light_field){
		for(auto&light:line){
			if(light == 1){
				cout<< '.';
			}else{
				if(light>9){cout<<'#';}
				else{
					cout<< light;
				}
			}

		}cout<<endl;
    }
    }
    int sum = 0;
	for(auto&line:light_field){
		for(auto&light:line){
			if(light != 1){sum++;}
		}
	}
    cout <<"Sum is "<< sum << endl;
    return;
}
