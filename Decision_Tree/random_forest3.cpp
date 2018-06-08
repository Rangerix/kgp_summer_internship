//after the test train split is done
//randomly select the features

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "random_tree_1.h"
using namespace std;

int main(int argc, char *argv[])
{
	if(argc!=2) {
		printf("./a.out dataset filename\n");
		return 1;
	}
	FILE *fp;
	fp=fopen(argv[1],"r");
	if(fp==NULL){
		printf("cannot open dataset file\n");
		return 1;
	}
	char c;
	int linecount=0;
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') linecount++;
	}
	fclose(fp);
	
	
	int rowcount=linecount;
	int columncount=11;
	printf("rowcount = %d columncount = %d\n",rowcount,columncount); 
	int i,j,k,l;
	float a;
	int features=columncount-1;						//last col is label
	int samples=rowcount;

	printf("reading dataset ... \n");
	vector<float> *dataset=new vector<float>[samples];
	for(i=0;i<samples;i++){
		dataset[i].assign(columncount,0);
	}

	fp=fopen(argv[1],"r");
	/*for(i=0;i<samples;i++)						//read the dataset from file
	{
		for(j=0;j<columncount;j++)
			fscanf(fp,"%f ",&dataset[i][j]);
	}
	fclose(fp);*/

	printf("setting precision as .2f ...\n");
	char tempdataset[]="tempdataset.txt";
	FILE *fp1=fopen(tempdataset,"w");
	for(i=0;i<samples;i++){
		for(j=0;j<columncount;j++){
			fscanf(fp,"%f ",&a);
			fprintf(fp1,"%.2f ",a);
		}
		fprintf(fp1,"\n");
	}
	fclose(fp);
	fclose(fp1);
	fp1=fopen(tempdataset,"r");
	for(i=0;i<samples;i++)					
	{
		for(j=0;j<columncount;j++)
			fscanf(fp1,"%f ",&dataset[i][j]);
	}
	fclose(fp1);
	//if precision not required, comment from ("setting precision as .2f ...\n")

	//dataset reading done

	int crossvalidation=10;
	printf("crossvalidation = %d \n",crossvalidation);
	int testsize=linecount/crossvalidation;
	int trainsize=linecount-testsize;
	printf("trainsize: %d testsize: %d\n",trainsize,testsize );
	int *randomno=new int[testsize];

	cout<<"generating row numbers randomly for test data ...\n";
	for(i=0;i<testsize;i++){
		int r;
		int flag=1;
	   	while(flag){
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
	int *testtrain=new int[samples]();
	for(i=0;i<testsize;i++){
		testtrain[randomno[i]]=2;// 2 for testdata
	}
	delete randomno;
	vector<float> *testmatrix=new vector<float>[testsize];
	for(i=0;i<testsize;i++){
		testmatrix[i].assign(columncount,0);
	}
	vector<float> *trainmatrix=new vector<float>[trainsize];
	for(i=0;i<trainsize;i++){
		trainmatrix[i].assign(columncount,0);
	}
	l=0,k=0;
	for(i=0;i<rowcount;i++){
		if(testtrain[i]==2){	//then test data
			for(j=0;j<columncount;j++){
				testmatrix[l][j]=dataset[i][j];
			}
			l++;
		}
		else{						//traindata
			for(j=0;j<columncount;j++){
				trainmatrix[k][j]=dataset[i][j];
			}
			k++;
		}
	}
	delete testtrain;
	printf("the testmatrix and trainmatrix is prepared\n");

	/*int just=1;
	if(just){
		char tsf[]="testfile1.txt";
		char tnf[]="trainfile1.txt";
		fp=fopen(tsf,"w");
		for(i=0;i<testsize;i++){
			for(j=0;j<11;j++){
				fprintf(fp, "%f ",testmatrix[i][j] );
			}
			fprintf(fp, "\n");
		}
		fclose(fp);

		fp=fopen(tnf,"w");
		for(i=0;i<trainsize;i++){
			for(j=0;j<11;j++){
				fprintf(fp, "%f ",trainmatrix[i][j] );
			}
			fprintf(fp, "\n");
		}
		fclose(fp);

	}*/

	float* given_label=new float[testsize];
	for(i=0;i<testsize;i++)
		given_label[i]=testmatrix[i][columncount-1];

	int no_tree=100;										//no of trees	
	int p=log2(features)+1;								//no of features in each tree
	int subcolsize=p+1;

	float **forest_result=new float*[no_tree];
	for(i=0;i<no_tree;i++)
		forest_result[i]=new float[testsize];

	clock_t starttime=clock();
	for(int treecount=0;treecount<no_tree;treecount++){
		printf("\n tree : %d \ngenerating %d random features ...\n",treecount,p);
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

		//declare trainsize
		vector<float> *subtrain=new vector<float>[trainsize];
		for(i=0;i<trainsize;i++){
			subtrain[i].assign(subcolsize,0);
		}

		printf("preparing train data for a tree...\n");
		for(i=0;i<trainsize;i++){
			k=0;
			for(j=0;j<columncount;j++){
				if(ispresent[j]){
					subtrain[i][k++]=trainmatrix[i][j];
				}
			}
		}
		
		//declare testsize
		vector<float> *subtest=new vector<float>[testsize];
		for(i=0;i<testsize;i++){
			subtest[i].assign(subcolsize,0);
		}
		printf("preparing test data for  tree\n");
		for(i=0;i<testsize;i++){
			k=0;
			for(j=0;j<columncount;j++){
				if(ispresent[j]){
					subtest[i][k++]=testmatrix[i][j];
				}
			}
		}
		/*for(i=0;i<testsize;i++){
			for(j=0;j<subcolsize;j++)
				printf("%.2f ",subtest[i][j] );
			printf("\n");
		}*/
		
		float *predicted_label =main_function(trainsize,testsize,subcolsize,subtrain,subtest);	
		//size of predicted_label = testsize
		for(i=0;i<testsize;i++)
			forest_result[treecount][i]=predicted_label[i];
	}
	cout<<"calculating accuracy ..."<<endl;
	float *one=new float[testsize]();
	float *two=new float[testsize]();
	float *res=new float[testsize]();
	for(i=0;i<no_tree;i++){
		for(j=0;j<testsize;j++){
			a=forest_result[i][j];
			if(a==1) one[j]++;
			if(a==2) two[j]++;
		}
	}
	for(i=0;i<testsize;i++)
		if(one[i]==two[i])
			res[i]=given_label[i];
		else if(one[i]>two[i])
			res[i]=1;
		else
			res[i]=2;
	delete one;
	delete two;
	int match=0;
	for(i=0;i<testsize;i++){
		if(res[i]==given_label[i])
			match++;
	}
	float accuracy=(float)(match)/testsize;
	delete given_label;
	delete res;
	delete forest_result;
	clock_t endtime=clock();
	cout<<"\ntotal time : "<<(float)(endtime-starttime)/CLOCKS_PER_SEC<<endl;
	cout<<"accuracy : "<<accuracy<<endl<<endl;
	return 0;
}