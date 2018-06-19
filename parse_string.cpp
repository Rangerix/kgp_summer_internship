//this file generates the sequence from pdb file
#include <bits/stdc++.h>
using namespace std;

void delay(void){
	cout<<"delay function called";
	long long int i,j;
	for(i=0;i<99999;i++)
		for(j=0;j<i;j++) ;
}
struct atom{
	string resiname;
	int resinum;
	char chainid;
	float prob;
	void show(void){
		cout<<resiname<<" "<<resinum<<endl;
	}
};

char shorten(string str){
	map<string,char> m;
	m["CYS"]= 'C'; m["ASP"]= 'D'; m["SER"]= 'S'; m["GLN"]= 'Q'; m["LYS"]= 'K';
     m["ILE"]= 'I'; m["PRO"]= 'P'; m["THR"]= 'T'; m["PHE"]= 'F'; m["ASN"]= 'N'; 
     m["GLY"]= 'G'; m["HIS"]= 'H'; m["LEU"]= 'L'; m["ARG"]= 'R'; m["TRP"]= 'W'; 
     m["ALA"]= 'A'; m["VAL"]='V';m["GLU"]= 'E'; m["TYR"]= 'Y'; m["MET"]= 'M';

     return m[str];
}

void comparefile(char *,char *, char *,char *);
string is_substring(string s1, string s2);
void matchwithfasta(char *,char *);

void buildchain(char filename[],char outfilename[],char resultfile[])
{
	//cout<<"buildchain called with "<<filename<<" "<<outfilename<<endl;
	ofstream ofs;
	ofs.open(outfilename,ios::app);
	ofs<<filename[strlen(filename)-5];
	ofs.close();
	ifstream inputstr(filename);
	if(!inputstr){
		cout<<"problem while opening "<<filename<<endl;
		return;
	}
	int i,j,flag,k;
	string tmp;
	string str="";
	float a;
	char c;
	vector<struct atom> atomvec;
	while(inputstr){
		str="";
		getline(inputstr,str);
		if(!inputstr) break;
		//cout<<str[26]<<endl;
		if(str[26]>='A'&&str[26]<='Z'){
			ofstream ofs;
			ofs.open(resultfile,ios::app);
			ofs<<outfilename<<" : problem \n\n";
			ofs.close();
			return;
		}
		stringstream ss(str);
		struct atom tempatom;
		ss>>tmp;
		ss>>tmp;
		ss>>tmp;
		ss>>tempatom.resiname;
		ss>>tempatom.chainid;
		ss>>tempatom.resinum;
		ss>>a;
		ss>>a;
		ss>>a;
		ss>>tempatom.prob;
		ss>>a;
		atomvec.push_back(tempatom);
	}

	/*for(i=0;i<atomvec.size();i++){
		cout<<i<<" ";
		atomvec[i].show();
	}*/
	//find the sequence of the chain
	ofs.open(outfilename,ios::app);
	ofs<<" "<<atomvec[0].resinum<<" "<<atomvec[atomvec.size()-1].resinum<<endl;
	ofs.close();
	int val=atomvec[0].resinum;		
	cout<<"first residue is indexed as : "<<val<<endl;
	str="";
	tmp="";
	
	for(i=0;i<atomvec.size()-1;i++){
		//cout<<atomvec[i].resinum<<" "<<val<<endl;
		if(atomvec[i+1].resinum==1+atomvec[i].resinum){//continuous
			//cout<<atomvec[i].resiname;
			tmp=shorten(atomvec[i].resiname);
		}
		else if(atomvec[i+1].resinum==atomvec[i].resinum){//since it is given that only two probable residue is possible at a particular position
			if(atomvec[i].prob>atomvec[i+1].prob)
				j=i,k=i+1;
			else 
				j=i+1,k=i;
			i++;
			cout<<i<<","<<j<<","<<k<<endl;
			string rname=atomvec[j].resiname;
			string rn=rname.substr(1);
			tmp+=shorten(rn);
			tmp+="(";
			rname=atomvec[k].resiname;
			rn=rname.substr(1);
			tmp+=shorten(rn);
			tmp+=")";
		}
		else{		//chain break
			j=atomvec[i].resinum+1;
			tmp=shorten(atomvec[i].resiname);
			while(j!=atomvec[i+1].resinum){
				tmp+="*";
				j++;
				if(j>atomvec[atomvec.size()-1].resinum+10){
					ofstream rfs;
					rfs.open(resultfile,ios::app);
					rfs<<outfilename<<" : problem \n";
					rfs.close();
					return;
				}
			}
		}
		str+=tmp;
	}
	if(i<atomvec.size()) str+=shorten(atomvec[i].resiname);
	//cout<<endl;
	//cout<<str<<endl;
	ofs.open(outfilename,ios::app);
	ofs<<str<<endl;
	ofs.close();


}


int split_function(char filename[],char outfilename[],char resultfile[]){
	ifstream inputstr;
	//strcpy(filename,argv[1]);
	inputstr.open(filename);
	if(!inputstr)
	{
		cout<<"file not found \n";
		return 0;
	}
	vector<char> allchars;
	string store;
	int i,j;
	char x,y;
	int count=1;
	getline(inputstr,store);
	x=store[21];
	allchars.push_back(x);
	while(!inputstr.eof())						//these counts how many chains are there
	{
		getline(inputstr,store);
		if(!inputstr) break;
		y=store[21];
		if(x!=y){
			count++;
			x=y;
			allchars.push_back(x);
		}
	}
	cout<<"count : "<<count<<endl;
	char allfiles[count][100];
	for(i=0;i<count;i++)
	{
		for(j=0;j<strlen(filename)-4;j++)
		{
			allfiles[i][j]=filename[j];
		}
		allfiles[i][j++]=(allchars[i]);
		allfiles[i][j++]='.';
		allfiles[i][j++]='t';
		allfiles[i][j++]='x';
		allfiles[i][j++]='t';
		allfiles[i][j++]='\0';
	}
	for(i=0;i<count;i++){
		for(j=0;j<strlen(allfiles[i]);j++){
			cout<<allfiles[i][j];
		}
		cout<<endl;
	}
	inputstr.close();



	//now seperate the chains
	ofstream mystream[count];
	for(i=0;i<count;i++)
	{
		mystream[i].open(allfiles[i]);
		if(!mystream[i]){
			cout<<"problem @ opening file "<<i<<endl;
			return 0;
		}
	}
	inputstr.open(filename);
	while(!inputstr.eof()){
		getline(inputstr,store);
		if(!inputstr) break;
		x=store[21];
		for(i=0;i<allchars.size();i++){
			if(x==allchars[i]){
				mystream[i]<<store<<endl;
			}
		}
	}
	inputstr.close();
	for(i=0;i<count;i++){
		mystream[i].close();
	}
	
	ofstream ofs(outfilename);
	ofs.close();
	for(i=0;i<count;i++){
		buildchain(allfiles[i],outfilename,resultfile);
	}

	char pdbid[10];
	for(i=0;i<4;i++)
		pdbid[i]=outfilename[i];
	pdbid[i]='\0';
	matchwithfasta(pdbid,resultfile);
}


int main(int argc,char* argv[])
{
	if(argc!=3){
		cout<<"./a.out pdb_filename resultfilename";
		return 1;
	}
	string filename(argv[1]);
	char tempfilenamech[]="tempfile.txt";
	string tempfilename(tempfilenamech);
	string str;
	ifstream ifs(argv[1]);
	ofstream ofs(tempfilenamech);
	while(ifs){
		getline(ifs,str);
		if(!ifs) break;
		if(str.substr(0,6)=="ENDMDL") break;
		ofs<<str<<endl;
	}
	ifs.close();
	ofs.close();
	string command_string = "grep ^ATOM "+tempfilename+" > mainfile.txt";
    int i,len=command_string.length();
    char command[len+1];
    for(i=0;i<len;i++)
    {
            command[i]=command_string[i];
    }
    command[i]='\0';
    system(command);

    //extract only the CA LINES
    char s1[]="mainfile.txt";
    char s2[]="mainCAfile.txt";
    ifs.open(s1);
    ofs.open(s2);
    while(ifs){
    	getline(ifs,str);
    	if(!ifs) break;
    	if(str[13]=='C'&&str[14]=='A')
    		ofs<<str<<endl;
    }
    ifs.close();
    ofs.close();
    //now seperate this mainfile.txt into its chainfiles
    char mainfilename[]="mainCAfile.txt";
    char outfilename[100];
    outfilename[0]=argv[1][strlen(argv[1])-8];
    outfilename[1]=argv[1][strlen(argv[1])-7];
    outfilename[2]=argv[1][strlen(argv[1])-6];
    outfilename[3]=argv[1][strlen(argv[1])-5];
    outfilename[4]='_';
    outfilename[5]='s';
    outfilename[6]='.';
    outfilename[7]='t';
    outfilename[8]='x';
    outfilename[9]='t';
    outfilename[10]='\0';
    split_function(mainfilename,outfilename,argv[2]);

    /*string s2="isaline";
    string s1="a**n";
    cout<<is_substring(s1,s2)<<endl;*/
    //comparefile(argv[2],outfilename);
return 0;
}


struct tocompare{
	char chainid;
	int start,end;
	string sequence;
	void show(void){
		cout<<chainid<<":\n";
		cout<<sequence<<endl;
	}
};

string is_substring(string s1, string s2)
{
    int m = s1.length();
    int n = s2.length();
    string str;
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
    	if(j==m) {
    		for(int temp=i;temp<k;temp++)
    			str+=s2[temp];
    		return str;
    	}
    }
    return "";
}

void comparefile(char *pdbid,char *fastafile, char *outfilename, char *resultfile)
{
	ifstream ifout(outfilename);
	if(!ifout){
		cout<<"problem in opening "<<outfilename<<endl;
		return;
	}
	vector<struct tocompare> compvec1,compvec2;
	char ch;
	string str1,str2,chainidtemp;
	float start,end;
	int i;
	while(ifout){
		getline(ifout,str1);
		if(!ifout) break;
		getline(ifout,str2);
		if(!ifout) break;
		ch=str1[0];
		stringstream ss(str1);
		ss>>chainidtemp;
		ss>>start;
		ss>>end;
		struct tocompare temp;
		temp.chainid=ch;
		temp.start=start;
		temp.end=end;
		temp.sequence=str2;
		compvec1.push_back(temp);
	}
	ifout.close();

	/*for(i=0;i<compvec1.size();i++)
		compvec1[i].show();*/

	ifstream ifs(fastafile);
	if(!ifs) {			//if fasta file is not found
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
			temp.start=0;
			temp.end=0;
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

		ofstream ofs;
		ofs.open(resultfile,ios::app);
		ofs<<"PDBID : ";
		ofs<<pdbid;
		ofs<<compvec1[i].chainid;
		ofs<<".pdb\t";
		ofs<<"FASTAID : "<<fastafile<<endl;

		if(j==compvec2.size()) {
			ofs<<endl;
			ofs.close();
			continue;
		}
		ofs<<"start : "<<compvec1[i].start<<" end : "<<compvec1[i].end<<endl;
		ofs<<"PDB sequence : \n";
		ofs<<compvec1[i].sequence<<endl;
		ofs<<"fasta sequence : \n";
		string matchstr=is_substring(compvec1[i].sequence,compvec2[j].sequence);
		ofs<<matchstr<<endl;
		int chainbreak=0;
		int breakstart=0,breakend=0;
		string tempstr=compvec1[i].sequence;
		for(k=0;k<tempstr.length();k++)
			if(tempstr[k]=='*') chainbreak=1;
		ofs<<"chainbreak : "<<chainbreak<<endl;
		if(chainbreak)//print start and end positions
			for(k=0;k<tempstr.length();k++){
				chainbreak=0;
				if(tempstr[k]=='*'){
					chainbreak=1;
					breakstart=k;
					while(tempstr[k]=='*') k++;
					breakend=k;
				}
				if(chainbreak) ofs<<breakstart<<","<<breakend<<endl;
			}
		int match;
		//string matchstr=is_substring(compvec1[i].sequence,compvec2[j].sequence);
		if(matchstr=="") match=0;
		else match=1;
		ofs<<"match : "<<match<<endl;
		int check1=(compvec1[i].sequence.length()==compvec1[i].end-compvec1[i].start+1);
		ofs<<"check1 : "<<check1<<endl;
		ofs<<endl;
		ofs.close();
	}
	
}

void matchwithfasta(char *pdbidch, char *resultfile){
	string pdbid(pdbidch);
	string s1,s2;
	s1="../fasta/";
	s2=pdbid+"_s.txt";
	transform(pdbid.begin(),pdbid.end(),pdbid.begin(),::toupper);
	s1+=pdbid;
	s1+=".fasta";
	comparefile(pdbidch,&s1[0],&s2[0],resultfile);
}