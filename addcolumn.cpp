#include <bits/stdc++.h>
using namespace std;

int main(int argc,char* argv[])
{
	ifstream ifs;
	ofstream ofs;
	int i,j;
	string str;
	vector<string> vec;
	ifs.open(argv[1]);
	while(ifs){
		str="";
		getline(ifs,str);
		if(!ifs) break;
		vec.push_back(str);
	}
	ifs.close();
	ofs.open(argv[1]);
	for(i=0;i<vec.size();i++){
		for(j=0;j<4;j++) ofs<<vec[i][j];
		ofs<<"_";
		ofs<<vec[i][4]<<endl;
	}
	ofs.close();
return 0;
}