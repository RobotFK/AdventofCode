#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
using namespace std;

void D_10_1(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("10-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

	class pipeseg{
		public:
		char tile;
		bool north	=false;
		bool east	=false;
		bool south	=false;
		bool west	=false;
		vector<pipeseg*>neighbors;
		int distance=-1;

		pipeseg(char tile){
			this->tile = tile;
			switch(tile) {
				case 'S':
					this->north = true;
					this->east = true;
					this->south = true;
					this->west = true;
					this->distance = 0;
					break;
				case '.':
					break;
				case '|':
					this->north = true;
					this->south = true;
					break;
				case '-':
					this->east = true;
					this->west = true;
					break;
				case 'L':
					this->north = true;
					this->east = true;
					break;
				case 'J':
					this->north = true;
					this->west = true;
					break;
				case '7':
					this->south = true;
					this->west = true;
					break;
				case 'F':
					this->south = true;
					this->east = true;
					break;
				default:
					cout<<"Unknown Tile"<<endl;
			}
		}
	};
	vector<vector<pipeseg>>pipefield;


	for(auto&line:inputvector){//Create
		vector<pipeseg> vline;
		for(int i=0;i!=line.size();i++){
		pipeseg newpipeseg(line[i]);
		vline.push_back(newpipeseg);
		}
		pipefield.push_back(vline);
	}

	int starti_x=0,starti_y=0;//Indexes of the starting position
	for(auto&line:pipefield){//Find start
		starti_x=0;
		for(auto&pipe:line){
			if(pipe.tile=='S'){goto Startfound;}
			starti_x++;
		}
		starti_y++;
	}
	Startfound:;
	cout<< "Starting at ["<<starti_y<<"]["<<starti_x<<"]"<<endl;

	for(int i=0;i!=pipefield.size();i++){//Connect Pipes (Vertical loop)
		for(int j=0;j!=pipefield[i].size();j++){// (Horizontal loop)
			pipeseg*here =&pipefield[i][j];
			if((*here).tile == '.'){continue;}
			if(i!=0){//Test North
				if((*here).north && pipefield[i-1][j].south){
					(*here).neighbors.push_back(&pipefield[i-1][j]);
				}
			}
			if(i!=pipefield.size()-1){//Test South
				if((*here).south && pipefield[i+1][j].north){
					(*here).neighbors.push_back(&pipefield[i+1][j]);
				}
			}
			if(j!=pipefield[i].size()-1){//Test East
				if((*here).east && pipefield[i][j+1].west){
					(*here).neighbors.push_back(&pipefield[i][j+1]);
				}
			}
			if(j!=0){//Test West
				if((*here).west && pipefield[i][j-1].east){
					(*here).neighbors.push_back(&pipefield[i][j-1]);
				}
			}
			//if((*here).neighbors.size()>=2){cout<<"#";}else{cout<<".";}//Could be made much to replace
		}
		//cout<<endl;
	}

	pipeseg*start =&pipefield[starti_y][starti_x];
	pipeseg*here =start;//Current location of Animal
	pipeseg*prev =start;//Previous location of Animal
	cout<<"Start has "<<(*start).neighbors.size()<<" neighbors"<<endl;
	for(auto&travel_direcition:(*start).neighbors){
		prev = travel_direcition;//Move in Specific direction from the start by pretending we came from the other option
		goto movementloop;//Allows movement from the Start
		while(here != start){
			movementloop:;
			for(auto&neighbor:(*here).neighbors){//Moving forward
				if(neighbor != prev){
					prev = here;
					here = neighbor;
					//cout<<"Moving to "<<(*here).tile<<endl;
					goto moved;
				}
			}
			moved:;

			if(here == start){cout<<"Finished First Route"<<endl;goto routcomplete;}//Route Complete
			if((*here).distance == -1){//If never visited
				//cout<<"Incementing"<<(*prev).distance+1;<<"by 1"<<endl;
				(*here).distance = (*prev).distance+1;
			}else{
				//cout<<"Found Footprints"<<(*here).distance <<" am at "<<(*prev).distance<<endl;
				if((*here).distance == (*prev).distance+1 || (*here).distance == (*prev).distance){//Reached middle
					cout<< "Reached middle";
					goto routcomplete;
				}else{(*here).distance = (*prev).distance+1;}
			}
		}
		routcomplete:;
	}


	if(false){
	cout<<endl;
	for(auto&line:pipefield){//Visualise Tile
		for(auto&pipe:line){
			cout<<pipe.tile;
		}
		cout<<endl;
	}
	}

	if(false){
	cout<<endl;
	for(auto&line:pipefield){//Visualise Dist
		for(auto&pipe:line){
			if(pipe.distance == -1){cout<<'.';}else{cout<<pipe.distance;}
		}
		cout<<endl;
	}
	}
	int max_dist=0;
	for(auto&line:pipefield){//Visualise Dist
		for(auto&pipe:line){
			if(pipe.distance >= max_dist){max_dist=pipe.distance;}
		}
	}
	cout<<endl<<"Max distance is "<<max_dist<<endl;
    return;
}
