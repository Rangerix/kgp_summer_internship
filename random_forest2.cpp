//after the test train split is done
//randomly select the features

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "random_tree.h"
using namespace std;

int main(int argc, char *argv[])
{
	if(argc!=3){
		printf("./a.out trainfile.txt testfile.txt\n");
		return 1;
	}
	//argv[1] trainfile
	//argv[2] testfile
	int columncount=11;
	int features=columncount-1;						//last col is label
	int samples=0;
	int i,j,k;
	float a;

	char c;
	FILE *fp;
	fp=fopen(argv[1],"r");
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') samples++;
	}
	fclose(fp);

	int p=log2(features)+1;								//no of features in each tree
	int no_tree=50;										//no of trees
	int subcolsize=p+1;

	printf("generating %d random features ...\n",p);
	int *arr=(int*)malloc (p*sizeof(int));
	int r,flag;
	for(i=0;i<p;i++){
		flag=1;
		while(flag){
			clock_t t=clock();
			srand((int)t);
			r=rand()%features;

			for(j=0;j<=i;j++){
				if(arr[j]==r){
					flag=1;
					break;
				}
				else{
					flag=0;
				}
			}
		}
		arr[i]=r;
		printf("%d ",r );
	}
	printf("\n");

	
	int *ispresent=(int*)malloc(columncount*sizeof(int));
	ispresent[columncount-1]=1;						//label must be present
	for(i=0;i<columncount-1;i++){
		ispresent[i]=0;
	}
	for(i=0;i<p;i++){
		ispresent[arr[i]]=1;
	}
	free(arr);

	int trainsize=samples;
	vector<float> *traindata=new vector<float>[trainsize];
	for(i=0;i<trainsize;i++){
		traindata[i].assign(subcolsize,0);
	}

	printf("reading train data ...\n");
	fp=fopen(argv[1],"r");
	for(i=0;i<samples;i++){
		k=0;
		for(j=0;j<columncount;j++){
			fscanf(fp,"%f",&a);
			if(ispresent[j]){
				traindata[i][k++]=a;
			}
		}
	}
	fclose(fp);
	
	int testsize=0;
	fp=fopen(argv[2],"r");
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') testsize++;
	}
	fclose(fp);
	vector<float> *testdata=new vector<float>[testsize];
	for(i=0;i<testsize;i++){
		testdata[i].assign(subcolsize,0);
	}

	fp=fopen(argv[2],"r");
	for(i=0;i<testsize;i++){
		k=0;
		for(j=0;j<columncount;j++){
			fscanf(fp,"%f ",&a);
			if(ispresent[j]){
				testdata[i][k++]=a;
			}
		}
	}
	fclose(fp);
	/*char subtest[]="subtest.txt";
	fp=fopen(subtest,"w");
	for(i=0;i<testsample;i++){
		for(j=0;j<columncount;j++){
			if(ispresent[j]){
				fprintf(fp,"%.2f ",testdata[i][j]);
			}
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	free (testdata);*/
	main_function(trainsize,testsize,subcolsize,traindata,testdata);
	return 0;
}