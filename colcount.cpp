#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

int main(int argc,char* argv[]){
	if(argc!=2){
		cout<<"use: ./a.out filename";
		return 1;
	}

	char str[10000];
	FILE *fp;
	fp=fopen(argv[1],"r");
	fgets(str,10000,fp);
	fclose(fp);
	int count=1;
	for(int i=0;str[i]!='\0';i++)
		if(str[i]==' ') count++;
	printf("colcount : %d\n", count);
	/*
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
	/*int no_tree=atoi(argv[2]);
	cout<<"no_tree : "<<no_tree<<endl;*/
return 0;
}