#include <bits/stdc++.h>
using namespace std;

int main(int argc,char* argv[])
{
	if(argc!=3){
		cout<<"./a.out files.txt outputfilename\n";
		return 1;
	}
	ifstream ifs(argv[1]);
	ofstream ofs;
	ofs.open(argv[2],ios::app);
	if(!ifs){
		cout<<"problem in opening"<<argv[1]<<endl;
		return 1;
	}
	string str1="",str2="";
	string pdbid="";
	char cb;
	int count,i,j;
	int number=0,flag=0;
	while(ifs){
		getline(ifs,str1);
		if(!ifs) break;
		if(str1==""){
			pdbid="";
		}
		if(str1.substr(0,6)=="PDB_ID")
		{
			pdbid=str1.substr(9,4);
			ofs<<pdbid<<endl;
			
		}
		/*if(str1.substr(0,10)=="chainbreak"){
			cb=str1[13];
			if(cb=='0'){
				ofs<<pdbid<<endl;
				pdbid="";
			}
		}*/
		pdbid="";			
		str1="";
	}
	ifs.close();
	ofs.close();



return 0;
}