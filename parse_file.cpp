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
		cout<<asa<<endl;
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
	vector<residue> residues;
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
		residues.push_back(temp_residue);
	}
	mystream.close();
	cout<<dualsum<<endl;
	cout<<"print values : ";
return 0;
}