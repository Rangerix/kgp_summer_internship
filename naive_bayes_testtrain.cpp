#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
using namespace std;

void delay(void){
	cout<<"delay function called \n";
	long long int i=9999999999;
	while(i--) ;
}

int count_unique(int size,float* arr)
{
	int i,j,isunique;
	int count=0;
	for(i=0;i<size;i++)
	{
		isunique=1;
		for(j=0;j<i&&isunique;j++){
			if(arr[i]==arr[j]) isunique=0;
		}
		if(isunique) count++;
	}
	return count;
}

int *get_unique_count(int row,int col,float **dataset)
{
	int *result=new int[col];

	float *attr=new float[row];
	int i,j;
	for(i=0;i<col;i++){
		for(j=0;j<row;j++){
			attr[j]=dataset[j][i];
		}
		int val=count_unique(row,attr);
		result[i]=val;
	}
	delete attr;
	return result;
}

void calculate_accuracy(int size,float* given, float* predicted){
	int match=0;
	int i;
	for(i=0;i<size;i++){
		if(given[i]==predicted[i]) match++;
	}
	float accu=(float)(match)/size;
	printf("accuracy : %f \n", accu);
}

int naive_func(int trainsize,int testsize,float** trainmatrix,float **testmatrix)
{
	int colcount=11;
	int features=colcount-1;
	int i,j,k,l;
	float a,x;
	int rowcount=trainsize;
	/*printf("colcount : %d\n",colcount);
	if(argc!=3){
		printf("usage : ./a.out trainfile testfile\n");
		return 1;
	}
	FILE *fp;
	fp=fopen(argv[1],"r");
	char c;
	int rowcount=0;
	for(c=getc(fp);c!=EOF;c=getc(fp))
		if(c=='\n') rowcount++;
	fclose(fp);
	printf("rowcount : %d \n",rowcount);
	
	float **trainmatrix=new float*[rowcount];
	for(i=0;i<rowcount;i++)
		trainmatrix[i]=new float[colcount];

	fp=fopen(argv[1],"r");
	for(i=0;i<rowcount;i++){
		for(j=0;j<colcount;j++){
			fscanf(fp,"%f ",&trainmatrix[i][j]);
		}
	}
	fclose(fp);
	*/
	//trainmatrix prepared


	int samples=rowcount;
	//the size of uniquecount is colcount
	int *uniquecount=get_unique_count(samples,colcount,trainmatrix);
	/*for(i=0;i<colcount;i++)
		cout<<uniquecount[i]<<" ";
	cout<<endl;*/

	//uniquevalue table
	float **uniquevalue=new float*[colcount];
	for(i=0;i<colcount;i++){
		uniquevalue[i]=new float[uniquecount[i]]();
	}
	//fill with unique values
	for(i=0;i<colcount;i++)
	{
		//traverse on trainmatrix[i] column
		float* tempattr=new float[rowcount];
		for(j=0;j<rowcount;j++){
			tempattr[j]=trainmatrix[j][i];
		}
		l=0;
		for(j=0;j<rowcount;j++){
			int isunique=1;
			for(k=0;k<l;k++){
				if(tempattr[j]==uniquevalue[i][k]) isunique=0;
			}
			if(isunique) uniquevalue[i][l++]=tempattr[j];
		}
		delete tempattr;
	}
	/*for(i=0;i<colcount;i++){
		for(j=0;j<uniquecount[i];j++)
			cout<<uniquevalue[i][j]<<" ";
		cout<<endl;
	}*/


	//now prepare the probability table
	//rowsize= #features
	//colsize[i] = #uniquecount[i]*classcount
	int classcount=uniquecount[colcount-1];
	float* classprob=new float[classcount];
	int count=0;
	for(j=0;j<classcount;j++){
		count=0;
		for(i=0;i<rowcount;i++){
			if(trainmatrix[i][colcount-1]==uniquevalue[colcount-1][j])
				count++;
		}
		float prob=(float)(count);
		classprob[j]=prob;
		cout<<classprob[j]<<" ";
	}//now classprob literally contins classcount
	cout<<endl;
	float **probability_table=new float*[features];
	for(i=0;i<features;i++)
		probability_table[i]=new float[uniquecount[i]*classcount]();


	//since I know that there are two classes only
	for(i=0;i<features;i++)
	{
		//count the probability of all the unique_elements/class
		for(j=0;j<uniquecount[i];j++){
			int label1=0,label2=0;
			x=uniquevalue[i][j];			//an unique element
			for(k=0;k<rowcount;k++){
				if(x==trainmatrix[k][i] && trainmatrix[k][colcount-1]==uniquevalue[colcount-1][0])
					label1++;
				if(x==trainmatrix[k][i] && trainmatrix[k][colcount-1]==uniquevalue[colcount-1][1])
					label2++;
			}
			//now consider the zero frequency case
			if(label1==0||label2==0) {
				//printf("zero frequency\n");
				label2++;
				label1++;
			}
			probability_table[i][j]=	(float)(label1)/classprob[0];	//probability_table[j+classindex*uniquecount[i]]
			probability_table[i][j+uniquecount[i]]=	(float)(label2)/classprob[1];
		}
	}
	//now modify classprob
	for(j=0;j<classcount;j++){
		classprob[j]/=rowcount;
	}
	/*for(i=0;i<features;i++){
		for(j=0;j<uniquecount[i]*classcount;j++)
			printf("%f ",probability_table[i][j]);
		printf("\n");
	}*/
	//delete trainmatrix;

	//prepare testmatrix
	/*fp=fopen(argv[2],"r");
	int testsize=0;
	for(c=getc(fp);c!=EOF;c=getc(fp))
		if(c=='\n') testsize++;
	fclose(fp);

	float **testmatrix=new float*[testsize];
	for(i=0;i<testsize;i++)
		testmatrix[i]=new float[colcount];

	fp=fopen(argv[2],"r");
	for(i=0;i<testsize;i++){
		for(j=0;j<colcount;j++){
			fscanf(fp,"%f ",&testmatrix[i][j]);
		}
	}
	fclose(fp);
	printf("testmatrix prepared \n");*/
	float *givenlabel=new float[testsize]();
	float *predictedlabel=new float[testsize]();
	for(i=0;i<testsize;i++)
		givenlabel[i]=testmatrix[i][colcount-1];
	//printf("givenlabel values stored \n"); 

	float one=classprob[0],two=classprob[1],prob1,prob2;
	int draw=0,notfound=0;
	//cout<<(1.0)/samples<<endl;
	for(i=0;i<testsize;i++){
		//this is for single test data
		one=classprob[0];
		two=classprob[1];
		for(j=0;j<features;j++){
			x=testmatrix[i][j];
			//find the index of x in uniquevalue table
			for(k=0;k<uniquecount[j];k++){
				//if(fabs(uniquevalue[j][k]-x)<0.001) break;
				if(uniquevalue[j][k]==x) break;

			}
			//if k==uniquecount[j], then not found, else, k is the index of the unique value
			//now find the prob(x/label); since here only two class, two variables are taken, for multiclass, an array 
			//is needed; prob_table[j][k] and prob_table[j][k+uniquecount[j]]
			if(k==uniquecount[j]){
				//cout<<"not found : "<<x<<"at : "<<i<<","<<j<<endl;
				prob1=1.0;
				prob2=1.0;
				/*cout<<"Present values : "<<endl;
				for(k=0;k<uniquecount[j];k++)
					cout<<uniquevalue[j][k]<<" ";
				cout<<endl;*/
				notfound++;
				//delay();
			}
			else{
				prob1=probability_table[j][k];
				prob2=probability_table[j][k+uniquecount[j]];
				//cout<<prob1<<" "<<prob2<<endl;
				if(prob1==0) {
					prob1=(float)(1.0)/samples;
					//printf("prob1 = 0\n"); 
				}
				if(prob2==0){
					prob2=float(1.0)/samples;
					//printf("prob2 = 0\n");
				}
			}

			//cout<<one<<" "<<two<<endl;
			one *=(prob1*100);
			two *=(prob2*100);
			
		}
		//cout<<one<<" "<<two<<" "<<givenlabel[i]<<endl;
		//printf("%f %f\n",one,two );
		if(one==two) {
			predictedlabel[i]=givenlabel[i];
			draw++;
			//cout<<one<<" "<<two<<endl;
		}
		else if(one>two) predictedlabel[i]=uniquevalue[colcount-1][0];
		else predictedlabel[i]=uniquevalue[colcount-1][1];
		
	}
	cout<<"classprob[0] : "<<classprob[0]<<" classprob[1] : "<<classprob[1]<<endl;
	//printf("notfound : %d draw : %d\n",notfound,draw );
	calculate_accuracy(testsize,givenlabel,predictedlabel);
	delete uniquevalue;
	delete uniquecount;
	delete classprob;
	delete probability_table;
	delete givenlabel;
	delete predictedlabel;
return 0;
}


int main(int argc,char* argv[]){
	if(argc!=2){
		printf("./a.out dataset.txt\n");
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
	float **dataset=new float*[samples];
	for(i=0;i<samples;i++){
		dataset[i]=new float[columncount];
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
	float **testmatrix=new float*[testsize];
	for(i=0;i<testsize;i++){
		testmatrix[i]=new float[columncount];
	}
	float **trainmatrix=new float*[trainsize];
	for(i=0;i<trainsize;i++){
		trainmatrix[i]=new float[columncount];
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
	naive_func(trainsize,testsize,trainmatrix,testmatrix);

return 0;
}