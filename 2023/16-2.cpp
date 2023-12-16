#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

struct beam_head{
	int x;
	int y;
	int direction;
};

int energise(int&start_x,int&start_y,int&start_direction,vector<string>&field,bool visuals=false){
	int max_x =field[0].size()-1;
	int max_y =field.size()-1;

	vector<vector<int>>light_field;
    // 2= Up
    // 3= Right
    // 5= Down
    // 7= Left
    for(auto&line:field){
		vector<int> new_light_light(line.size(),1);
		light_field.push_back(new_light_light);
    }

	vector<beam_head> heads;
	beam_head start;
	start.x=start_x;
	start.y=start_y;
	start.direction=start_direction;

	//Because the main loop only checks the Tile state after moving we need one additional check here;:
	switch(start.direction){
		case 2://up
			switch(field[start.y][start.x]){
				case '/':
					start.direction=3;
					break;
				case '\\':
					start.direction=7;
					break;
					case '-':
					start.direction=3;
					beam_head new_head;
					new_head.x = start.x;
					new_head.y = start.y;
					new_head.direction=7;
					heads.push_back(new_head);
					break;
				}
				break;
			case 3://Right
				switch(field[start.y][start.x]){
					case '/':
						start.direction=2;
					break;
					case '\\':
						start.direction=5;
					break;
					case '|':
						start.direction=5;
						beam_head new_head;
						new_head.x = start.x;
						new_head.y = start.y;
						new_head.direction=2;
						heads.push_back(new_head);
					break;
				}
				break;
			case 5://Down
				switch(field[start.y][start.x]){
					case '/':
						start.direction=7;
					break;
					case '\\':
						start.direction=3;
					break;
					case '-':
						start.direction=3;
						beam_head new_head;
						new_head.x = start.x;
						new_head.y = start.y;
						new_head.direction=7;
						heads.push_back(new_head);
					break;
				}
				break;
			case 7://Left
				switch(field[start.y][start.x]){
					case '/':
						start.direction=5;
					break;
					case '\\':
						start.direction=2;
					break;
					case '|':
						start.direction=2;
						beam_head new_head;
						new_head.x = start.x;
						new_head.y = start.y;
						new_head.direction=5;
						heads.push_back(new_head);
					break;
				}
				break;
			}

	heads.push_back(start);

	//start of main loop
	while(heads.size()!=0){
		vector<bool> dead_heads(heads.size(),false);
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
				switch(field[head.y][head.x]){
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
				switch(field[head.y][head.x]){
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
				switch(field[head.y][head.x]){
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
				switch(field[head.y][head.x]){
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
	}

	if(visuals){
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
	return sum;
}

void D_16_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("16-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

	int best_sum=0;
	int d= -1;
	for(int y=0;y!=inputvector.size();y++){
		for(int x=0;x!=inputvector[y].size();x++){
			if(y!=0 && x!=0 && y!=inputvector.size()-1 && x!=inputvector[y].size()-1){continue;}//Position not on a corner
			//cout<< "("<<x<<","<<y<<")"<<endl;
			if(x==0){//Coming from the Left
				d = 3;
				int energise_sum = energise(x,y,d,inputvector);
				if(energise_sum>best_sum){best_sum=energise_sum;}
			}else if(x==inputvector[y].size()-1){//Coming from the Right
				d = 7;
				int energise_sum = energise(x,y,d,inputvector);
				if(energise_sum>best_sum){best_sum=energise_sum;}
			}
			if(y==0){//Coming from the Top
				d = 5;
				int energise_sum = energise(x,y,d,inputvector);
				if(energise_sum>best_sum){best_sum=energise_sum;}
			}else if(y==inputvector.size()-1){//Coming from the Bottom
				d = 2;
				int energise_sum = energise(x,y,d,inputvector);
				if(energise_sum>best_sum){best_sum=energise_sum;}
			}
		}
	}

	cout <<"Max Sum is "<< best_sum << endl;

    return;
}
