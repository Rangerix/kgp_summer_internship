#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <set>
#include <algorithm>
using namespace std;


void delay(void){
	cout<<"delay function called "<<endl;
	long long int i,j;
	for(i=0;i<99900;i++){
		for(j=0;j<i/2;j++) ;
	};
	cout<<"delay function finished\n";
}

struct atom
{
	string atomtype;
	int resinum;
	string resiname;
	float x,y,z;
	void show(){
		cout<<atomtype<<" "<<resinum<<" ";
		printf("%f,%f,%f\n",x,y,z );
	}
};

int isconsecutive(vector<int> v){
	sort(v.begin(),v.end());
	int i,size=v.size();
	for(i=1;i<size;i++){
		if(v[i]!=v[i-1]+1) return 0;
	}
	return 1;
}

void calculate(vector<struct atom> &v,int radius,int length,char* outfile,int fixlen){
	//cout<<"calculate() called with v.size() = "<<v.size()<<endl;
	int minatom=ceil((float)(length)/2.3);
	int size=v.size();
	int i,j,k,flag;

	vector<int> chain;
	float a,b,c,temp;
	for(i=0;i<size;i++){
		if(v[i].atomtype=="CA"){
			flag=1;
			for(j=0;j<size;j++){
					temp=0;
					a=(v[i].x-v[j].x);
					b=(v[i].y-v[j].y);
					c=(v[i].z-v[j].z);
					temp=(a*a)+(b*b)+(c*c);
					temp=sqrt(temp);
					//cout<<temp<<" ";
					if(temp<radius&&abs(v[j].resinum-v[i].resinum)>minatom){		//we have to discard it
						flag=0;
						break;					
				}
			}	
			if(flag){
				chain.push_back(v[i].resinum);
			}
		}	
	}

	/*for(i=0;i<chain.size();i++)
		cout<<chain[i]<<" ";
	cout<<endl;
*/
	ofstream ofs;
	ofs.open(outfile,ios::app);
	int n=chain.size();
	vector<vector<int> > allchain;
 	vector<int> tmp;
	int maxlen=0;
	ofs<<"minimum length of a string : "<<length<<endl;
	for(i=0;i<n;i++){
		j=i+1;
		if(chain[i]+1==chain[j]) tmp.push_back(chain[i]);
		else{
			tmp.push_back(chain[i]);
			allchain.push_back(tmp);
			if(tmp.size()>length){
				for(int k=0;k<tmp.size();k++)
					ofs<<tmp[k]<<" ";
				ofs<<endl;
			}
			if(tmp.size()>maxlen) maxlen=tmp.size();
			tmp.clear();
		}
	}
	n=allchain.size();
	if(maxlen>1){
		ofs<<"\nmaxlen : "<<maxlen<<endl;
		for(i=0;i<n;i++){
			int col=allchain[i].size();
			if(col==maxlen){
				for(int k=0;k<col;k++)
					ofs<<allchain[i][k]<<" ";
				ofs<<endl;
			}
		}
	}
	
	if(fixlen!=-1){
		ofs<<"\nfor length : "<<fixlen <<" \n";
		for(i=0;i<n;i++){
			int col=allchain[i].size();
			if(col==fixlen){
				for(j=0;j<col;j++)
					ofs<<allchain[i][j]<<" ";
				ofs<<endl;
			}
		}
	}
	ofs<<endl;
	ofs.close();
}

int main(int argc,char* argv[])
{
	if(argc!=5&&argc!=6){
		cout<<"exe ,"
			<<"pdbfilename (string) ,"
			<<"radius(int): we will search around each CA with this length ,"
			<<"minlen: minimum length of our required strings ,"
			<<"outputfilename: in which file output will be stored ,\n OR \n"
			<<"exe pdbfilename length minatom outputfilename fixlen\n";
		return 1;
	}

	int chainlength=atoi(argv[2]);
	int length=atoi(argv[3]);
	int fixlen=-1;
	if(argc==6) {
		fixlen=atoi(argv[5]);
	}
	ofstream ofs(argv[4]);
	ofs.close();


	float a,b;
	ifstream ifs(argv[1]);
	string str,tmp;
	if(!ifs){
		cout<<"problem in opening inputfile";
		return 1;
	}
	vector<struct atom> myvector;

	while(ifs){
		str="";
		getline(ifs,str);
		if(str.substr(0,4)=="ATOM"){
			stringstream ss(str);
			struct atom tempatom;
			ss>>tmp;
			ss>>tmp;
			ss>>tempatom.atomtype;
			ss>>tempatom.resiname;
			ss>>tempatom.resinum;
			ss>>tempatom.x;
			ss>>tempatom.y;
			ss>>tempatom.z;
			ss>>a;
			ss>>b;
			myvector.push_back(tempatom);
		}
		else if(str.substr(0,3)=="TER"){
			if(myvector.size()) calculate(myvector,chainlength,length,argv[4],fixlen);
			//for(int i=0;i<myvector.size();i++)
			//	myvector[i].show();
			myvector.clear();
			//delay();
		}
		else{
			ofs.open(argv[4],ios::app);
			if(str.substr(0,5)=="MODEL") {
				ofs<<str<<endl;
			}
			ofs.close();
		}
	}
	ifs.close();	


return 0;
}