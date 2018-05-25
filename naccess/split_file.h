#include <iostream>
#include <fstream>
#include <cstdlib>
#include "parse_file.h"
using namespace std;

int split_function(char filename[])
{
	ifstream inputstr;
	//strcpy(filename,argv[1]);
	inputstr.open(filename);
	if(!inputstr)
	{
		cout<<"file not found \n";
		return 0;
	}
	vector<char> allchars;
	string store;
	int i,j;
	char x,y;
	int count=1;
	getline(inputstr,store);
	x=store[21];
	allchars.push_back(x);
	while(!inputstr.eof())						//these counts how many chains are there
	{
		getline(inputstr,store);
		if(!inputstr) break;
		y=store[21];
		if(x!=y){
			count++;
			x=y;
			allchars.push_back(x);
		}
	}
	cout<<"count : "<<count<<endl;
	char allfiles[count][100];
	for(i=0;i<count;i++)
	{
		for(j=0;j<strlen(filename)-4;j++)
		{
			allfiles[i][j]=filename[j];
		}
		allfiles[i][j++]=(i+'0');
		allfiles[i][j++]='.';
		allfiles[i][j++]='p';
		allfiles[i][j++]='d';
		allfiles[i][j++]='b';
		allfiles[i][j++]='\0';
	}
	for(i=0;i<count;i++){
		for(j=0;j<strlen(allfiles[i]);j++){
			cout<<allfiles[i][j];
		}
		cout<<endl;
	}
	inputstr.close();



	//now seperate the chains
	ofstream mystream[count];
	for(i=0;i<count;i++)
	{
		mystream[i].open(allfiles[i]);
		if(!mystream[i]){
			cout<<"problem @ opening file "<<i<<endl;
			return 0;
		}
	}
	inputstr.open(filename);
	while(!inputstr.eof()){
		getline(inputstr,store);
		if(!inputstr) break;
		x=store[21];
		for(i=0;i<allchars.size();i++){
			if(x==allchars[i]){
				mystream[i]<<store<<endl;
			}
		}
	}
	inputstr.close();
	for(i=0;i<count;i++){
		mystream[i].close();
	}

	//work on main file
	char inputstrr[500]="echo \"PDBFILE ";
	strcat(inputstrr,filename);
	strcat(inputstrr," \nVDWFILE /home/kkg/Documents/kgp_summer_internship/naccess//vdw.radii \nSTDFILE /home/kkg/Documents/kgp_summer_internship/naccess//standard.data \nPROBE 1.40\nZSLICE 0.05\" > temp.input");
	system(inputstrr);
	system("./accall < temp.input");

	//work on sub-files
	for(i=0;i<count;i++){
		char inputstring[500]="echo \"PDBFILE ";
		strcat(inputstring,allfiles[i]);
		strcat(inputstring," \nVDWFILE /home/kkg/Documents/kgp_summer_internship/naccess//vdw.radii \nSTDFILE /home/kkg/Documents/kgp_summer_internship/naccess//standard.data \nPROBE 1.40\nZSLICE 0.05\" > temp.input");
		system(inputstring);
		system("./accall < temp.input");
	}
	parse_asa_files(filename,count);

	
	
return 0;
}