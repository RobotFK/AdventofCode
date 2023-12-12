#include <iostream>
#include "days.h"
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
//FIX INPUT line 154
bool is_valid_placement2(string&record,vector<int>&placement,vector<int>&groups,bool visuals=false){
    int group_index=0;//Next or Current group that is evaluated
    for(int i=0;i!=record.size();i++){
        if(group_index==placement.size()){//damaged Spring after final group
            if(record[i]=='#'){
                if(visuals){cout<<"4";}
                return false;
            }
            continue;
        }
        if(i<placement[group_index]){
            if(record[i]=='#'){
                if(visuals){cout<<"1";}
                return false;
            }//damaged Spring before group
            continue;
        }
        if(i<=placement[group_index]+groups[group_index]-1){
            if(record[i]=='.'){
                if(visuals){cout<<"2:"<<i<<" is less than "<<placement[group_index]+groups[group_index]-1;}
                return false;
            }//operational  Spring in group
            continue;
        }
        if(i==placement[group_index]+groups[group_index]){
            if(record[i]=='#'){
                if(visuals){cout<<"3";}
                return false;
            }//damaged spring immediatly after in group
            group_index++;
            continue;
        }

        cout<<"Error";
    }
    return true;
}

int arrangements(string&record,vector<int>&groups,bool visuals=false){
    int arrangements = 0;

    /*
    cout<<endl<<endl;
    cout<<record<< "<-";
    for(auto&x:groups){cout<<x<<" ";}
    cout<<endl<<endl;
    */

    vector<int>placement;
    for(int i=0;i!=groups.size();i++){// Inital layout, does not have to be valid
        if(placement.size()==0){placement.push_back(0);continue;}
        int offset =placement.back()+groups[i-1]+1;
        placement.push_back(offset);
    }
    int exhausted_index = groups.size();//Index of the First group we have tested all positions (most importantly as late placed as possible)
    //cout<<"New Row"<<endl;
    arrangements += is_valid_placement2(record,placement,groups);//In case the first arrangement is valid
    //cout<<"New Row"<<endl;
    while(exhausted_index!=0){
        //cout<<"E_x:"<<exhausted_index<<endl;
        if(exhausted_index == groups.size()){//Test Final Group
            //cout<<"Final Group moved"<<endl;
            //For day 2 we only allow solutions that have a Final Group that is valid
            for(int i=placement[exhausted_index-1]+1;i<=record.size()-groups[exhausted_index-1];i++){
                bool fits= true;
                for(int j=0;j!=groups[exhausted_index-1];j++){
                    if(record[i+j]=='.'){fits=false;}//Attempted Placement contains a op spring
                }
                //if(i+irow.groups[exhausted_index-1]<=irow.record.size()-1){//Stops Springtest if Group is up to the edge
                if(record[i+groups[exhausted_index-1]]=='#'){fits=false;}//Attempted Placement contains ends in broken spring
                //}
                if(fits){
                    placement[exhausted_index-1]=i;
                    goto final_placed;
                }
            }
            //cout <<"No final config can fit"<<endl;
            exhausted_index--;
            continue;
            //cout<< "Final Placed"<<endl;
            final_placed:;
        }else{//Test Last Group
            //For day 2 we only allow solutions that have a Group that is valid
            for(int i=placement[exhausted_index-1]+1;i<=placement[exhausted_index]-groups[exhausted_index-1]-1;i++){
                bool fits= true;
                for(int j=0;j!=groups[exhausted_index-1];j++){
                    if(record[i+j]=='.'){fits=false;}//Attempted Placement contains a op spring
                }
                if(record[i+groups[exhausted_index-1]]=='#'){fits=false;}//Attempted Placement contains ends in broken spring
                if(fits){
                    placement[exhausted_index-1]=i;
                    //The following is rough, but could be improved for the reseting lower stages
                    for(int reset_i=exhausted_index;reset_i!=groups.size();reset_i++){//Move all later groups to the earliest options
                    placement[reset_i] = placement[reset_i-1]+ groups[reset_i-1]+1;
                    }
                    exhausted_index = groups.size();//Reset exhausted index;
                    goto last_placed;
                }
            }
            exhausted_index--;
            continue;
            last_placed:;
            //cout<< "Last Placed"<<endl;
        }
        arrangements += is_valid_placement2(record,placement,groups);
        //if(is_valid_placement2(record,placement,groups)){cout<<"V ";
		//	for(auto&x:placement){cout<<x<<",";}cout<<endl;
        //}
        //cout<<"Testing:"<<endl;
        //for(auto&x:placement){cout<<x<<",";}cout<<endl;
        }
    if(visuals){cout<< arrangements << " Combinations"<<endl;}
    return arrangements;
}

//void display_placement(string&record,vector<int>placement,vector<int> groups){

void D_12_2(){
    static vector<string> inputvector;
    string line;
    ifstream inputread("12-1-1.txt");
    while (getline (inputread, line)) {
    inputvector.push_back(line);
    }
    cout << "Read input of "<<inputvector.size() << " Lines" << endl;

    //Essentially just Nonogramms
    int folds = 5;
    bool optimising = true;

    struct section{
    	string record;
    	vector<int> groups;
    	int arrangements = 1;
    };
    struct row{
		vector<section> sections;
		int arrangements = 1;
    };
    vector<row> rows;
    for(int i =0;i != inputvector.size();i++){
            vector<string> ssplit = split_string(inputvector[i],' ');
            vector<string> sgroups = split_string(ssplit[1],',');
            row newrow;
            section newsection;
			for(int j=0;j!=folds-1;j++){//Additional folds
                newsection.record.append(ssplit[0]).append("?");
                for(auto&group:sgroups){
                    newsection.groups.push_back(stoi(group));
                }
            }
            newsection.record.append(ssplit[0]);
            for(auto&group:sgroups){
				newsection.groups.push_back(stoi(group));
			}
            newrow.sections.push_back(newsection);
            //cout<< newsection.record;
            rows.push_back(newrow);
    }

    if(optimising){
    cout<< "Reducing"<<endl;
    int optimised =0;
    int completed_rows=0;
    for(auto&irow:rows){//For each row
		cout<<"Optimising Row "<<++completed_rows<<endl;

        vector<vector<int>>F_placement;//List of Entries where Placements are forced;
		for(auto&isection:irow.sections){//For each section we create a dummy to write Forced Placements in
			vector<int> new_F_placement (isection.groups.size(),-1);//-1 means unset
			F_placement.push_back(new_F_placement);
		}
		optimise_1:;
		//Our first step is trying to split the Full Record into sections that have a set border group (the group needs to be place at the specific index)
        for(int section=0;section!=irow.sections.size();section++){//For each section
            vector<int> spaces;
            vector<int> groups =irow.sections[section].groups;
            sort(groups.begin(),groups.end(),greater<int>());

            int unbroken_len = 0;
            for(int i=0;i!=irow.sections[section].record.size();i++){

                if(irow.sections[section].record[i]=='.'){
                    if(unbroken_len!=0){spaces.push_back(unbroken_len);}
                    unbroken_len = 0;
                }else{unbroken_len++;}
            }
            if(unbroken_len!=0){spaces.push_back(unbroken_len);}

            sort(spaces.begin(),spaces.end(),greater<int>());
            //For it to handle groups that were one smaller than the space I would need a inplement an artifical cut mechanic....
            if(spaces[0]>groups[0]){
				//cout<<"Skip O_1,as "<<spaces[0]<<" > "<<groups[0]<<endl;
				goto optimise_2;
            }


            //for(auto&space:spaces){cout<<space<<",";}cout<<endl;
            //for(auto&group:groups){cout<<group<<",";}cout<<endl;
            /* Visuals
            //for(auto&isection:irow.sections[section].record){cout<<isection<<" ";}cout<<" | ";
            //for(int i=0;i!=irow.groups[section].size();i++){ cout<< irow.groups[section][i]<<",";}
            //cout<<endl;
            //cout<<"Fitting "<<largest_group<<" into "<<largest_space<<endl;
			*/

            vector<int> filling;
            int sobg = groups[0];//size_of_biggest_group
            for(int space_i=0;space_i!=spaces.size();space_i++){//
                if(space_i==folds){//Found one space for each Fold
						if(spaces[space_i]>=sobg){//There are more spaces than biggest groups, some will not be occupied
							//cout<<"decision_paralysis"<<endl;
							goto optimise_2;
						}
						break;}
                if(spaces[space_i]==sobg){
                    filling.push_back(sobg);
                }
                if(spaces[space_i]>=(sobg+1)*2-1){//Biggest space is large enough to fit the size_of_biggest_group twice inside, situation is not clea
					//cout<<"Fillout overflow"<<endl;
					goto optimise_2;
                    //No time to Implement.
                }
            }
            vector<int> filling_indexes;
            //cout<<"(1)Filling "<<filling.size()<<" spaces:"<<endl;
			//Only look for the Biggest filling,it should appear fold times at max;
			unbroken_len = 0;
			for(int i=0;i!=irow.sections[section].record.size();i++){
				if(irow.sections[section].record[i]=='.'){
					if(unbroken_len==filling[0]){filling_indexes.push_back(i-filling[0]);}
					unbroken_len = 0;
				}else{unbroken_len++;}
			}//cout<<endl;
			if(unbroken_len!=0){goto optimise_2;}//Group Touches the end, gets skipped as id doesnt reduce computation time by a lot
			if(unbroken_len!=0){filling_indexes.push_back(irow.sections[section].record.size()-filling[0]-1);}
            //cout<<"Finding Group indexes"<<endl;
            for(int ifilling_indexes=0;ifilling_indexes!=filling.size();ifilling_indexes++){//For each filling
				int Forced_index=filling_indexes[ifilling_indexes];//Ugly, i know
				cout<< "Forced_index="<<Forced_index<<" ";
				//cout<<"Size:"<<groups[0]<<endl;
				int found_folds=0;
				int g_i  = -1;
				for(int group_index=0;group_index!=irow.sections[section].groups.size();group_index++){//Test each group
					if(irow.sections[section].groups[group_index]==groups[0]){//group testet matches size of the largest group
						//cout<< "group_index match "<<group_index<<": "<< found_folds <<". vs "<<ifilling_indexes<<". "<<endl;
						if (found_folds==ifilling_indexes){//group is correct instance
							g_i =group_index;
							F_placement[section][group_index]=Forced_index;
							break;
						}else{found_folds++;}
					}
				}
				//cout<<g_i <<". (1)Group at index: "<<filling_indexes[ifilling_indexes]<<endl;
			}
            //cout<<endl;
        }

        optimise_2:;
        cout<<"optimisation type 2"<<endl;
        //Secondly we find it there is a space that forces a specific group by the amount of connected damaged springs
        for(int isection=0;isection!=irow.sections.size();isection++){//For each section
			vector<int> spaces;
			vector<int> groups =irow.sections[isection].groups;
			sort(groups.begin(),groups.end(),greater<int>());

			int unbroken_len = 0;
            for(int i=0;i!=irow.sections[isection].record.size();i++){
                if(irow.sections[isection].record[i]!='#'){
                    if(unbroken_len!=0){spaces.push_back(unbroken_len);}
                    unbroken_len = 0;
                }else{unbroken_len++;}
            }
            if(unbroken_len!=0){spaces.push_back(unbroken_len);}

            sort(spaces.begin(),spaces.end(),greater<int>());
            if(spaces.size()==0)goto optimise_3;
			if(spaces[0]<groups[0]){
				//cout<<"Skip O_2,as "<<spaces[0]<<" < "<<groups[0]<<endl;
				goto optimise_3;
			}

			//Bonus optimisation: if the largest group has a space that is fully fills,every space of that size gets walled of
			if(groups[0]==spaces[0]){
				cout<<"Bonus optimisation"<<endl;
				unbroken_len = 0;
				for(int i=0;i!=irow.sections[isection].record.size();i++){
					if(irow.sections[isection].record[i]!='#'){//End of Space
						if(unbroken_len==groups[0]){
						//cout<<"Found one at"<<i<<endl;
						irow.sections[isection].record[i]= '.';
						irow.sections[isection].record[i-groups[0]-1]= '.';
						unbroken_len = 0;
						}
					}else{unbroken_len++;}
				}
			}//End of Bonus optimisation

			for(auto&space:spaces){cout<<space<<",";}cout<<endl;
            for(auto&group:groups){cout<<group<<",";}cout<<endl;
			vector<int> filling;
            for(int space_i=0;space_i!=spaces.size();space_i++){
                if(space_i==folds){
						if(space_i+1<spaces.size()){//Not the last
							if(groups[space_i+1]==groups[space_i]){//Next group could fill the hole fully aswell
								//Todo: Add check to mitigate false alarms
								cout<<"decision_paralysis(2)"<<endl;
								goto optimise_3;
							}
						}
						break;}
                if(spaces[space_i]==groups[space_i]){
                    filling.push_back(groups[space_i]);
                }
            }
            //if(unbroken_len!=0){filling.push_back(unbroken_len);}//Group Touches the end
			vector<int> filling_indexes;
            //cout<<"(2)Filling "<<filling.size()<<" spaces:"<<endl;
            //Again, we only look for the Biggest filling,it should appear fold times maximaly;
			unbroken_len = 0;
			for(int i=0;i!=irow.sections[isection].record.size();i++){
				//cout<<irow.sections[isection].record[i];
				if(irow.sections[isection].record[i]!='#'){
					if(unbroken_len==filling[0]){filling_indexes.push_back(i-filling[0]);}
					unbroken_len = 0;
				}else{unbroken_len++;}
			}
			if(unbroken_len!=0){filling_indexes.push_back(irow.sections[isection].record.size()-filling[0]);}//Group Touches the end

			cout<<endl;
			for(auto&x:filling){cout<<x<<" ";}cout<<endl;
			for(auto&x:filling_indexes){cout<<x<<" ";}cout<<endl;

			for(int ifilling_indexes=0;ifilling_indexes!=filling.size();ifilling_indexes++){//For each filling
				int Forced_index=filling_indexes[ifilling_indexes];//Ugly, i know
				//cout<< "Forced_index="<<Forced_index<<" ";
				//cout<<"Size:"<<groups[0]<<endl;
				int found_folds=0;
				int g_i  = -1;
				for(int group_index=0;group_index!=irow.sections[isection].groups.size();group_index++){//Test each group
					if(irow.sections[isection].groups[group_index]==groups[0]){//group testet matches size of the largest group
						//cout<< "group_index match "<<group_index<<": "<< found_folds <<" vs "<<ifilling_indexes<<" "<<endl;
						if (found_folds==ifilling_indexes){//group is correct instance
							g_i =group_index;
							if(F_placement[isection][group_index]==-1){
							F_placement[isection][group_index]=Forced_index;
							}else if(F_placement[isection][group_index]!=Forced_index){cout<<"O_2 Force mismatch index  "<<F_placement[isection][group_index]<<" -> "<< Forced_index <<endl;}
							break;
						}else{found_folds++;}
					}
				}
				irow.sections[isection].record[filling_indexes[ifilling_indexes]-1]='.';//WARNING this is a extremly volatile operation and a sign of stress, this forces the spot before the first spring of the group to be '.'
				//cout<<g_i <<". (2)Group at index: "<<filling_indexes[ifilling_indexes]<<endl;
			}
            //cout<<endl;
        }

        optimise_3:;

		split_section:;
        //Finally, We count the Optimisations and Split the sections:
        vector<section> new_sections;
        bool row_improvements=false;
		for(int isection=0;isection!=irow.sections.size();isection++){
			bool section_improvements=false;
			for(auto&x:F_placement[isection]){
				//cout<<x<<" ^ ";
				if(x!=-1){
					section_improvements=true;}
			}//cout<<endl;
			if(section_improvements){
				row_improvements=true;
				optimised++;//For Statistics
			}else{//This section has not been Optimised and is just added to the vector of sections unchanged
				//cout<<"skipping section"<<endl;
				new_sections.push_back(irow.sections[isection]);
				continue;
			}

			//cout<<endl;
			//cout<< irow.sections[isection].record<<endl;
			//cout<<"Size:"<< irow.sections[isection].record.size()<<endl;
			int last_r_cut = 0;//last Cut of the Record that is gone
			int last_g_cut = 0;//last Cut of the Groups that is gone
			int index_F_placement=0;
			for(;index_F_placement!=F_placement[isection].size();index_F_placement++){
				//This loop only creates the New section BEFORE the cut
				int index= F_placement[isection][index_F_placement];//Index in the Record of where the Forced Group starts
				//cout<<"index="<<index<<endl;
				if(index ==-1){continue;}//No Forced start
				if(index == 0){//Group touches the left Edge, no Section created, only last_r_cut  and last_g_cut increased
					//cout<<"Skipped has len of "<<irow.sections[isection].groups[index_F_placement]<<endl;
					last_r_cut=index+irow.sections[isection].groups[index_F_placement];
					last_g_cut++;
					continue;
				}
				if(last_r_cut == irow.sections[isection].record.size()){ //Group touches the right Edge
					//cout<<"Clean R end"<<endl;
					break;
				}
				if(last_g_cut == irow.sections[isection].groups.size()){ //No groups left
					//cout<<"Clean G end"<<endl;
					break;
				}
				section new_section;
				for(int i=last_g_cut;i!=index_F_placement;i++){//Copy groups
					new_section.groups.push_back(irow.sections[isection].groups[i]);
				}
				last_g_cut = index_F_placement+1;
				//cout<<"Record section:"<<last_r_cut <<","<<index-last_r_cut<<endl;
				new_section.record = irow.sections[isection].record.substr(last_r_cut,index-last_r_cut);
				//cout<<new_section.record<<"|";
				//for(auto&x:new_section.groups){cout<<x<<" ";}cout<<endl;
				last_r_cut=index+irow.sections[isection].groups[index_F_placement]+1;
				new_sections.push_back(new_section);
				//cout<<"Added new section"<<endl;
			}

			//Fill the rest into another Section
			if(last_r_cut != irow.sections[isection].record.size()&&last_g_cut != irow.sections[isection].groups.size()){
				//cout<<"Leftover cut starting  "<< last_r_cut<<"/G:"<<last_g_cut <<endl;
				section new_section;
				for(int i=last_g_cut;i!=irow.sections[isection].groups.size();i++){//Copy groups
					new_section.groups.push_back(irow.sections[isection].groups[i]);
				}
				new_section.record = irow.sections[isection].record.substr(last_r_cut,irow.sections[isection].record.size()-last_r_cut);
				//for(auto&x:new_section.groups){cout<< x<< ",";}cout<<endl;
				new_sections.push_back(new_section);
			}
        }
        if(irow.sections.size()>new_sections.size()){cout<<irow.sections.size()-new_sections.size()<<" Sections lost error"<<endl;}
        //else{cout<<"Expanded to "<<new_sections.size()<<endl;}
		irow.sections = new_sections;
    }

    }
    //return;

	cout<<"Calculating arrangements:"<<endl;
    for(auto&irow:rows){
		//cout<<"New Row"<<endl;
        for(int section=0;section!=irow.sections.size();section++){
            for(auto&sect:irow.sections[section].record){cout<<sect;}cout<<"| ";
            for(auto&group:irow.sections[section].groups){cout<<group<<" ";}cout<<endl;
            irow.arrangements*=arrangements(irow.sections[section].record,irow.sections[section].groups);
        }
        cout<<"Total arrangements:"<<irow.arrangements<<endl;
    }

    int sum = 0;
    for(auto&irow:rows){
        sum+=irow.arrangements;
    }
    cout<<"For "<<folds << " folds the Sum of all Rows is: "<<sum<<endl;
    return;
}
//Too low 6785











