#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

int main(int argc,char* argv[]){
	if(argc!=3){
		cout<<"use: ./a.out filename an_integer";
		return 1;
	}
	string str;
	ifstream fs(argv[1]);
	getline(fs,str);
	//cout<<str<<endl;
	int count=1;
	for(int i=0;i<str.length();i++){
		if(str[i]==' ') count++;
	}
	fs.close();
	cout<<"colcount : "<<count<<endl;
	int no_tree=atoi(argv[2]);
	cout<<"no_tree : "<<no_tree<<endl;
return 0;
}