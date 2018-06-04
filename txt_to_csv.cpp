#include <iostream>
#include <cstdio>
using namespace std;


int main(int argc,char* argv[])
{
	if(argc!=3){
		printf("usage : ./a.out txtfilename csvfilenmae\n");
		return 1;
	}
	FILE *fp1,*fp2;
	fp1=fopen(argv[1],"r");
	char c;
	int rowcount=0;
	for(c=getc(fp1);c!=EOF;c=getc(fp1)){
		if(c=='\n') rowcount++;
	}	
	fclose(fp1);
	int i,j,colcount=11;
	printf(" rowcount : %d, colcount : %d \n",rowcount,colcount );

	float *temprow=new float[colcount];

	fp1=fopen(argv[1],"r");
	fp2=fopen(argv[2],"w");
	for(i=0;i<rowcount;i++){
		for(j=0;j<colcount;j++){
			fscanf(fp1,"%f ",&temprow[j]);
		}
		for(j=0;j<colcount-1;j++){
			fprintf(fp2,"%f,",temprow[j]);
		}
		if(temprow[j]==2)
			fprintf(fp2,"b\n");
		else 
			fprintf(fp2,"a\n");
	}
	fclose(fp1);
	fclose(fp2);
return 0;
}