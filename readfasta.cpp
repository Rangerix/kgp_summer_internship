#include <bits/stdc++.h>
using namespace std;

struct tocompare{
	char chainid;
	string sequence;
	void show(void){
		cout<<chainid<<" : \n"<<sequence<<endl;
	}
};

int main(int argc,char* argv[])
{
	ifstream ifs(argv[1]);
	if(!ifs){
		cout<<"problem openning file\n";
		return 1;
	}
	vector<struct tocompare> compvec;
	int i,j,n;
	string str,seq;
	char ch='\0',pch='\0';
	/*getline(ifs,str);
	for(i=0;i<str.length();i++){
		if(str[i]==':') {
			pch=str[i+1];
			break;
		}
	}*/
	while(ifs){
		str="";
		getline(ifs,str);
		if(!ifs) {
			struct tocompare temp;
			temp.chainid=pch;
			temp.sequence=seq;
			compvec.push_back(temp);
		}
		if(str[0]=='>'){
			for(i=0;i<str.length();i++){
				if(str[i]==':') {
					ch=str[i+1];
					break;
				}
			}
			if(pch=='\0') pch=ch;
			else{
				struct tocompare temp;
				temp.chainid=pch;
				temp.sequence=seq;
				compvec.push_back(temp);
			}
			seq="";
			pch=ch;
		}
		else{
			seq+=str;
		}
	}
	ifs.close();
	for(i=0;i<compvec.size();i++)
		compvec[i].show();
return 0;
}