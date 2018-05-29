
#include <iostream>
#include <cstdio>
using namespace std;

int main(int argc,char* argv[])
{
	FILE *fp=fopen(argv[1],"r");
	if(!fp){
		cout<<"problem to open subtest1.txt"<<endl;
		return 0;
	}
	char c;
	int i,j,testsize=0;
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') testsize++;
	}
	cout<<"testsize : "<<testsize<<endl;
	fclose(fp);
	int colcount=5;
	fp=fopen(argv[1],"r");
	float a;
	float *givenlabels=new float[testsize];
	for(i=0;i<testsize;i++){
		for(j=0;j<colcount-1;j++)
			fscanf(fp,"%f ",&a);
		fscanf(fp,"%f ",&givenlabels[i]);
	}
	fclose(fp);

	int progrun=0;
	fp=fopen(argv[2],"r");
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') progrun++; 
	}
	fclose(fp);
	cout<<"prog run : "<<progrun<<endl;
	float *one=new float[testsize]();
	float *two=new float[testsize]();
	float *res=new float[testsize]();
	fp=fopen(argv[2],"r");
	for(i=0;i<progrun;i++){
		for(j=0;j<testsize;j++){
			fscanf(fp,"%f ",&a);
			if(a==1) one[j]++;
			if(a==2) two[j]++;
		}
	}
	fclose(fp);
	for(i=0;i<testsize;i++)
		if(one[i]>two[i])
			res[i]=1;
		else
			res[i]=2;
	int match=0;
	for(i=0;i<testsize;i++){
		if(res[i]==givenlabels[i])
			match++;
	}
	cout<<"match : "<<match<<" of "<<testsize<<endl;
	cout<<"accurcy : "<<(float)(match*100)/testsize<<endl;

return 0;
}