#include <cstdio>
#include <cstdlib>
#include "decision_tree1.h"
using namespace std;

int main(int argc, char *argv[])
{
	/*int aaa=1;
	if(aaa){
		char trainfile[]="trainfile.txt";
		char testfile[]="testfile.txt";
		main_dec_tree(trainfile,testfile);
		return 0;
	}*/

	if(argc==1) {
		printf("./a.out filename");
		return 1;
	}
	FILE *fp;
	fp=fopen(argv[1],"r");
	if(fp==NULL){
		printf("cannot open file\n");
		return 1;
	}
	char c;
	int linecount=0;
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') linecount++;
	}
	fclose(fp);
	/*fp=fopen(argv[1],"r");
	if(fp==NULL){
		printf("cannot open file for reading dataset\n");
		return 1;
	}
	*/
	int rowcount=linecount;
	int columncount=11;
	printf("rowcount = %d\n",rowcount); 
	int i,j;
	float a;
	/*float **dataset=(float **)malloc(rowcount*sizeof(float*));
	for(i=0;i<rowcount;i++){
		dataset[i]=(float *)malloc(columncount*sizeof(float));
	}
	for(i=0;i<rowcount;i++){
		for(j=0;j<columncount;j++){
			fscanf(fp,"%f ",&dataset[i][j]);
		}
	}
	fclose(fp);

	for(i=0;i<rowcount;i++){
		for(j=0;j<columncount;j++){
			printf("%.2f ",dataset[i][j]);
		}
		printf("\n");
	}*/

	char trainfile[]="trainfile.txt";
	char testfile[]="testfile.txt";
	int crossvalidation=10;
	int testsize=linecount/crossvalidation;
	int trainsize=linecount-testsize;
	printf("trainsize: %d testsize: %d\n",trainsize,testsize );

	/*float **testdata=(float *)malloc(testsize*sizeof(float*));
	for(i=0;i<testsize;i++){
		testdata[i]=(float*)malloc(columncount*sizeof(float));
	}
	float **traindata=(float *)malloc(trainsize*sizeof(float*));
	for(i=0;i<trainsize;i++){
		traindata[i]=(float*)malloc(columncount*sizeof(float));
	}*/

	FILE *fptrain,*fptest;
	fp=fopen(argv[1],"r");
	fptrain=fopen(trainfile,"w");
	fptest=fopen(testfile,"w");
	float *temprow=(float*)malloc(columncount*sizeof(float));
	for(i=1;i<=rowcount;i++){
		for(j=0;j<columncount;j++){
			fscanf(fp,"%f ",&temprow[j]);
		}
		if(i%crossvalidation==0){	//then test data
			for(j=0;j<columncount;j++){
				fprintf(fptest, "%.2f ",temprow[j]);
			}
			fprintf(fptest,"\n" );
		}
		else{						//traindata
			for(j=0;j<columncount;j++){
				fprintf(fptrain, "%.2f ",temprow[j]);
			}
			fprintf(fptrain,"\n" );
		}
	}
	fclose(fp);
	fclose(fptest);
	fclose(fptrain);
	main_dec_tree(trainfile,testfile);
	return 0;
}