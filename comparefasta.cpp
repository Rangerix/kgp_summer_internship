#include <bits/stdc++.h>
using namespace std;

struct tocompare{
	char chainid;
	string sequence;
	void show(void){
		cout<<chainid<<":\n";
		cout<<sequence<<endl;
	}
};

int is_substring(string s1, string s2)
{
    int m = s1.length();
    int n = s2.length();
    //cout<<m<<" "<<n<<endl;
    int i,j,k;
    for(i=0;i<n;i++){
    	for(j=0,k=i;j<m&&k<n;){
    		if(s1[j]==s2[k]||s1[j]=='*'){
    			j++;
    			k++;
    		}
    		else if(s1[j]=='('){
    			j++;
    			j++;
    			j++;
    		}
    		else 			// not  match
    		{
    			if(s1[j+1]=='('&&s1[j+2]==s2[k])	//given that two probable residue at a position
    			{
    				j++;
    				j++;
    				j++;
    				j++;
    				k++;
    			}
    			else break;
    		}
    	}
    	//cout<<i<<" "<<j<<" "<<k<<endl;
    	if(j==m) return i;
    }
    return -1;
}

void comparefile(char *fastafile, char *outfilename)
{
	ifstream ifout(outfilename);
	if(!ifout){
		cout<<"problem in opening "<<outfilename<<endl;
		return;
	}
	vector<struct tocompare> compvec1,compvec2;
	char ch;
	string str1,str2,tempstr;
	float start,end;
	int i;
	while(ifout){
		getline(ifout,str1);
		if(!ifout) break;
		getline(ifout,str2);
		if(!ifout) break;
		ch=str1[0];
		stringstream ss(str1);
		ss>>tempstr;
		ss>>start;
		ss>>end;
		struct tocompare temp;
		temp.chainid=ch;
		temp.sequence=str2;
		compvec1.push_back(temp);
	}
	ifout.close();

	/*for(i=0;i<compvec1.size();i++)
		compvec1[i].show();*/

	ifstream ifs(fastafile);
	if(!ifs) {
		cout<<"problem in opening "<<fastafile<<endl;
		return ;
	}

	int j,n,k;
	string str,seq;
	char pch='\0';
	ch='\0';
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
			compvec2.push_back(temp);
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
				compvec2.push_back(temp);
			}
			seq="";
			pch=ch;
		}
		else{
			seq+=str;
		}
	}
	ifs.close();

	/*for(i=0;i<compvec2.size();i++)
		compvec2[i].show();*/
	//now comparison
	for(i=0;i<compvec1.size();i++){
		int flag=1;
		//cout<<compvec1[i].chainid<<endl;
		for(j=0;j<compvec2.size();j++){
			if(compvec1[i].chainid==compvec2[j].chainid) break;
		}
		char filename[100];
		//4_char.fasta
		flag=strlen(fastafile)-10;
		for(k=0;k<4;k++)
			filename[k]=fastafile[flag++];
		filename[k++]=compvec1[i].chainid;
		filename[k++]='.';
		filename[k++]='t';
		filename[k++]='x';
		filename[k++]='t';
		filename[k++]='\0';
		cout<<filename<<endl;
		ofstream ofs(filename);
		ofs<<"PDBID : ";
		for(k=0;k<5;k++)
			ofs<<filename[k];
		ofs<<".pdb\t";
		ofs<<"FASTAID : "<<fastafile<<endl;
		if(j==compvec2.size()) {
			ofs.close();
			continue;
		}
		ofs<<"start : "<<start<<" end : "<<end<<endl;
		ofs<<"PDB sequence : \n";
		ofs<<compvec1[i].sequence<<endl;
		ofs<<"fasta sequence : \n";
		ofs<<compvec2[j].sequence<<endl;
		int chainbreak=0;
		int breakstart=0,breakend=0;
		string tempstr=compvec1[i].sequence;
		for(k=0;k<tempstr.length();k++)
			if(tempstr[k]=='*') chainbreak=1;
		ofs<<"chainbreak : "<<chainbreak<<endl;
		if(chainbreak)//print start and end positions
			for(k=0;k<tempstr.length();k++){
				if(tempstr[k]=='*') chainbreak=1;
				if(tempstr[k]=='*'){
					breakstart=i;
					while(tempstr[k]=='*') i++;
					breakend=i;
				}
				if(chainbreak) ofs<<breakstart<<","<<breakend<<endl;
				chainbreak=0;
			}
		int match=is_substring(compvec1[i].sequence,compvec2[j].sequence);
		if(match==-1) match=0;
		else match=1;
		ofs<<"match : "<<match<<endl;
		int check1=(compvec1[i].sequence.length()==compvec2[j].sequence.length());
		ofs<<"check1 : "<<check1<<endl;
		ofs.close();
	}
	
}


int main(int argc, char* argv[])
{
	if(argc!=2){
		printf("./a.out pdbid\n");
		return 1;
	}
	string pdbid(argv[1]);
	string s1,s2;
	s1="../fasta/";
	s2=pdbid+"_s.txt";
	transform(pdbid.begin(),pdbid.end(),pdbid.begin(),::toupper);
	s1+=pdbid;
	s1+=".fasta";
	comparefile(&s1[0],&s2[0]);
return 0;
}