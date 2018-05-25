#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cstring>
using namespace std;

void delay(){
	long long int i=999999999;
	while(i--);
}


struct residue{
	string atomname;
	string resiname;
	char chainid;
	int resino;
	float x,y,z;
	float asa;
	void show_residue(){
		cout<<atomname<<endl;
		cout<<resiname<<endl;
		cout<<chainid<<endl;
		cout<<x<<","<<y<<","<<z<<endl;
		cout<<asa<<endl;
	}
};

void parse_asa_files(char filename[],int count )
{
	int i,j;
	char allasafiles[count][100];
	for(i=0;i<count;i++)
	{
		for(j=0;j<strlen(filename)-4;j++)
		{
			allasafiles[i][j]=filename[j];
		}
		allasafiles[i][j++]=(i+'0');
		allasafiles[i][j++]='.';
		allasafiles[i][j++]='a';
		allasafiles[i][j++]='s';
		allasafiles[i][j++]='a';
		allasafiles[i][j++]='\0';
	}
	for(i=0;i<count;i++){
		for(j=0;j<strlen(allasafiles[i]);j++){
			cout<<allasafiles[i][j];
		}
		cout<<endl;
	}
	int file_len=strlen(filename);
	filename[file_len-3]='a';
	filename[file_len-2]='s';
	filename[file_len-1]='a';

	ifstream mystream;
	mystream.open(filename);
	if(!mystream){
		cout<<"some problem in opening the asa files \n";
		return;
	}
	float dualsum=0.0;
	vector<residue> main_residues;
	while(!mystream.eof())
	{
		string temp;
		char ch;
		int a;
		float f;
		struct residue temp_residue;
		mystream>>temp;	 	//not required
		if(!mystream) break;
		mystream>>a;			//not required
		mystream>>temp_residue.atomname;
		mystream>>temp_residue.resiname;
		mystream>>temp_residue.chainid;
		mystream>>temp_residue.resino;			
		mystream>>temp_residue.x;
		mystream>>temp_residue.y;
		mystream>>temp_residue.z;
		mystream>>temp_residue.asa;
		dualsum+=temp_residue.asa;
		mystream>>f;
		main_residues.push_back(temp_residue);
	}
	mystream.close();
	cout<<"for main .pdb : "<<dualsum<<endl;
	
	float subsumall=0.0;

	vector<vector<residue> > sub_residues(count);
	for(i=0;i<count;i++){
		float subsum=0.0;
		mystream.open(allasafiles[i]);
		while(!mystream.eof())
		{
			string temp;
			char ch;
			int a;
			float f;
			struct residue temp_residue;
			mystream>>temp;	 	//not required
			if(!mystream) break;
			mystream>>a;			//not required
			mystream>>temp_residue.atomname;
			mystream>>temp_residue.resiname;
			mystream>>temp_residue.chainid;
			mystream>>temp_residue.resino;			
			mystream>>temp_residue.x;
			mystream>>temp_residue.y;
			mystream>>temp_residue.z;
			mystream>>temp_residue.asa;
			subsum+=temp_residue.asa;
			mystream>>f;
			sub_residues[i].push_back(temp_residue);
		}
		mystream.close();
		cout<<"subsum = "<<subsum<<endl;
		subsumall+=subsum;
	}
	float calculated_value=subsumall-dualsum;
	cout<<" calculated_value : "<<calculated_value<<endl;
	
}