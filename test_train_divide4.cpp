#include <cstdio>
#include <cstdlib>
#include "decision_tree1.h"
//#include "random_tree.h"
using namespace std;

int main(int argc, char *argv[])
{
	/*int aaa=1;
	if(aaa){
		char trainfile[]="trainfile.txt";
		char testfile[]="testfile.txt";
		main_function(trainfile,testfile);
		return 0;
	}*/

	if(argc!=2) {
		printf("./a.out dataset filename\n");
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

	char trainfile[]="subtrain.txt";
	char testfile[]="subtest.txt";
	int crossvalidation=3;
	cout<<"crossvalidation : "<<crossvalidation<<endl;
	int testsize=linecount/crossvalidation;
	int trainsize=linecount-testsize;
	printf("trainsize: %d testsize: %d\n",trainsize,testsize );
	int *randomno=new int[testsize];

	cout<<"generating row numbers randomly for test data ...\n";
	for(i = 0; i<testsize; ++i)
	{
	   int r;
	   int flag=1;
	   while(flag)
	   {
	   		clock_t t = clock();
			srand((int)t);
	      	r = rand()%(linecount-1);					// -1 to avoid any seg fault
	      	for(int j=0;j<=i;j++){
	      		if(randomno[j]==r){
					flag=1;	  
					break;    		
	      		}
	      		else
	      			flag=0;
	      	}
	   }
	   randomno[i] = r;   
	  // cout<<randomno[i]<<" ";
	}
	int *testtrain=new int[linecount]();
	for(i=0;i<testsize;i++){
		testtrain[randomno[i]]=2;
	}


	int p=log2(columncount)+1;
	cout<<"#features in a tree : "<<p<<endl;
	int *randomno1=new int[p];
	cout<<"selected cols : ";
	for(i = 0;i<p; ++i)
	{
	   int r;
	   int flag=1;
	   while(flag)
	   {
	   		clock_t t = clock();
			srand((int)t);
	      	r = rand()%(columncount-1);					// -1 to avoid any seg fault
	      	for(int j=0;j<=i;j++){
	      		if(randomno1[j]==r){
					flag=1;	  
					break;    		
	      		}
	      		else
	      			flag=0;
	      	}
	   }
	   randomno1[i] = r;   
	  cout<<randomno1[i]<<" ";
	}
	cout<<endl;
	int *ispresent=new int[columncount]();
	ispresent[columncount-1]=1;
	for(i=0;i<p;i++){
		//cout<<ispresent[i]<<" ";
		ispresent[randomno1[i]]=1;
	}

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
	for(i=0;i<rowcount;i++){
		for(j=0;j<columncount;j++){
			fscanf(fp,"%f ",&temprow[j]);
		}
		if(testtrain[i]==2){	//then test data
			for(j=0;j<columncount;j++){
				if(ispresent[j])
					fprintf(fptest, "%.2f ",temprow[j]);
			}
			fprintf(fptest,"\n" );
		}
		else{						//traindata
			for(j=0;j<columncount;j++){
				if(ispresent[j])
					fprintf(fptrain, "%.2f ",temprow[j]);
			}
			fprintf(fptrain,"\n" );
		}
	}
	fclose(fp);
	fclose(fptest);
	fclose(fptrain);
	//main_dec_tree(trainfile,testfile);
	//main_function(trainfile,testfile);
	return 0;
}
