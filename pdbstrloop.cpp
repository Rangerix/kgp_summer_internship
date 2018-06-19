#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[])
{
	if(argc!=3){
		cout<<"./a.out listofallfiles resultfilename\n";
		return 1;
	}
	ifstream ifs(argv[1]);
	if(!ifs){
		cout<<"problem in opening file"<<argv[1]<<endl;
		return 1;
	}
	string str1,str2;
	str1="../../pdb/";
	string tempstr(argv[2]);
	while(ifs){
		ifs>>str2;
		str1+=str2;
		string comstr="./exec ";
		comstr+=str1;
		comstr+=" ";
		comstr+=tempstr;
		char command[100];
		int i;
		for(i=0;i<comstr.length();i++){
			command[i]=comstr[i];
		}
		command[i]='\0';
		cout<<command<<endl;
		system(command);
		str1="../../pdb/";
	}
	ifs.close();
return 0;
}
