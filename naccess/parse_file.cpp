#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;

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
	}
};

int main()
{
	char filename[100];
	cout<<"enter filename : ";
	cin>>filename;
	ifstream mystream;
	mystream.open(filename);
	if(!mystream){
		cout<<"file not found \n";
		return 0;
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
	cout<<dualsum<<endl;
	/*cout<<"print values : ";
	residues[5].show_residue();*/


	cout<<"enter filename : ";
	cin>>filename;
	mystream.open(filename);
	if(!mystream){
		cout<<"file not found \n";
		return 0;
	}
	float subsum1=0.0;
	vector<residue> sub_residues1;
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
		subsum1+=temp_residue.asa;
		mystream>>f;
		sub_residues1.push_back(temp_residue);
	}
	mystream.close();
	cout<<"subsum1 = "<<subsum1<<endl;


	cout<<"enter filename : ";
	cin>>filename;
	mystream.open(filename);
	if(!mystream){
		cout<<"file not found \n";
		return 0;
	}
	float subsum2=0.0;
	vector<residue> sub_residues2;
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
		subsum2+=temp_residue.asa;
		mystream>>f;
		sub_residues2.push_back(temp_residue);
	}
	mystream.close();
	cout<<"subsum2 = "<<subsum2<<endl;

	float calculated_value=subsum2+subsum1-dualsum;
	cout<<" calculated_value : "<<calculated_value<<endl;
return 0;
}