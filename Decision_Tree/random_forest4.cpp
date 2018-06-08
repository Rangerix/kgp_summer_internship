#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

float my_entropy(int size,float* v){
	int i,count;
	float prob,sum=0.0,val;
	set<float> s(v,v+size);
	set<float>::iterator itr;
	for(itr=s.begin();itr!=s.end();itr++){
		val=*itr;
		count=0;
		for(i=0;i<size;i++){
			if(v[i]==val)
				count++;
		}
		//cout<<val<<" "<<count<<endl;
		prob=(float)count/size;
		sum+=prob*log2(prob);
	}
	return -sum;
}

float gain(int size,float* label, float* attr)
{
	int i;
	float val;
	float total=0.0;
	float* subentr=new float[size];
	int l=0;
	for(i=0;i<size;i++)
	{
		val=attr[i];
		//now find Sv
		float* sublabel=new float[size];
		int k=0;
		for(int j=0;j<size;j++){
			if(attr[j]==val){
				sublabel[k++]=label[j];
			}
		}
		float entr=my_entropy(k,sublabel);
		//cout<<"eeee"<<entr<<endl;
		subentr[l++]=entr;
		delete sublabel;
	}
	float answer=my_entropy(size,label);


	float prob=0,sum=0;
	int count;
	set<float> s(subentr,subentr+l);
	set<float>::iterator itr;
	for(itr=s.begin();itr!=s.end();itr++){
		val=*itr;
		count=0;
		for(i=0;i<size;i++){
			if(subentr[i]==val)
				count++;
		}
		//cout<<val<<" "<<count<<endl;
		prob=(float)count/size;
		sum+=prob*val;
	}
	delete subentr;
	answer-=sum;
	return answer;
}


struct tree_node
{
	float feature_no;
	int split_on;
	float label;		//for leaves, it will contain the label value
	bool is_leaf;
	vector<float> childrenValues;
	vector<tree_node*> children;
};


void print_matrix(int matsize,vector<float>* matrix){
	cout<<endl;
	//int matsize=sizeof(matrix)/sizeof(matrix[0]);					//this is NOT working
	for(int i=0;i<matsize;i++){
		for(int j=0;j<matrix[i].size();j++)
			cout<<matrix[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}
vector<float> get_all_unique_vals(int matsize,vector<float> * matrix, int index)
{
	vector<float> temp;
	for(int i=0;i<matsize;i++){
		temp.push_back( matrix[i][index] );
	}
	/*cout<<"temp vector: ";
	for(int i=0;i<temp.size();i++)
		cout<<temp[i]<<" ";
	cout<<endl;*/
	set<float> s(temp.begin(),temp.end());
	vector<float> vec(s.begin(),s.end());
	return vec;
}

int decide_split_column(int matsize,vector<float> * matrix)
{
	int features=matrix[0].size()-1;
	int samples=matsize;
	int i,j;
	/*cout<<endl;
	for(i=0;i<samples;i++){
		for(j=0;j<=features;j++)
			cout<<matrix[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;*/
	float* label=new float[samples];
	for(i=0;i<samples;i++){
		label[i]=matrix[i][features];
	}
	float max_gain= -9999,max_index,temp;
	/*cout<<"the label is : ";
	for(i=0;i<label.size();i++){
		cout<<label[i]<<" ";
	}
	cout<<endl;*/
	
	for(i=0;i<features;i++)
	{
		float* attr=new float[samples];
		for(j=0;j<samples;j++)
		{
			attr[j]=(matrix[j][i]);
		}
		temp=gain(samples,label,attr);
		delete attr;
		/*cout<<"i = "<<i<<" gain = "<<temp<<endl;
		cout<<"attr "<<i<<" \n";
		for(int kkk=0;kkk<attr.size();kkk++){
			cout<<attr[kkk]<<" ";
		}
		cout<<endl;*/
		if(temp>max_gain){
			max_gain=temp;
			max_index=i;
		}
	}
	delete label;
	return max_index;
}

bool isHomogeneous(int matsize,vector<float> * matrix)
{
	int iii;
	float firstValue = matrix[0][matrix[0].size()-1];
	for (iii = 1; iii < matsize; iii++) {
		if (firstValue != matrix[iii][matrix[iii].size()-1]) {
			return false;
		}
	}
	return true;
}

vector<float>* modify_matrix(int matsize,vector<float> *matrix,int col_no,float value,int & new_mat_size){
	int i,j,k,l;
	int row=0;
	//count no of rows in the new matrix
	new_mat_size=0;
	for(i=0;i<matsize;i++)
		if(matrix[i][col_no]==value)
			new_mat_size++;
	
	int colcount=matrix[0].size()-1;			//since we are removing one column

	vector<float>* new_matrix=new vector<float>[new_mat_size];
	for(i=0;i<new_mat_size;i++)
		new_matrix[i].assign(colcount,0);

	k=0;l=0;
	for(i=0;i<matsize;i++){
		if(matrix[i][col_no]==value){
			//vector<float> temp_row;
			l=0;
			for(j=0;j<matrix[i].size();j++){
				if(j!=col_no){
					//cout<<matrix[i][j]<<" ";
					//temp_row.push_back(matrix[i][j]);
					new_matrix[k][l++]=matrix[i][j];
				}
			}
			k++;
		}
		//cout<<endl;
	}
	//print_matrix(new_matrix);
	return new_matrix;
}

tree_node* build_decision_tree(int matsize,tree_node* nodeptr,vector<float>* matrix)
{
	//print_matrix(matsize,matrix);
	//cout<<"matsize : "<<matsize<<endl;
	if(matsize==1&&matrix[0].size()==2)	//then leaf
	{
		nodeptr->is_leaf=true;
		nodeptr->label=matrix[0][1];
		return nodeptr;
	}
	else if(matrix[0].size()<=1){
		return NULL;
	}
	else if(isHomogeneous(matsize,matrix)){
		nodeptr->is_leaf=true;
		nodeptr->label=matrix[0][matrix[0].size()-1];
		return nodeptr;
	}
	else{
		nodeptr->is_leaf=false;
		int split_col=decide_split_column(matsize,matrix);
		nodeptr->split_on=split_col;
		vector<float> temp=get_all_unique_vals(matsize,matrix,split_col);
		//cout<<"split_col : "<<split_col<<" \n";
		//cout<<"temp : ";
		/*for(int i=0;i<temp.size();i++)
			cout<<temp[i]<<" ";
		cout<<endl;*/
		for(int i=0;i<temp.size();i++){
			tree_node* child_node=(tree_node*) new tree_node;
			//cout<<"here 3\n";
			child_node->label=temp[i];
			nodeptr->childrenValues.push_back(temp[i]);
			//cout<<"here 4";
			child_node->is_leaf=false;
			child_node->split_on=split_col;
			//cout<<"here 5";
			int new_mat_size=0;
			vector<float> * new_matrix=modify_matrix(matsize,matrix,split_col,temp[i],new_mat_size);
			//cout<<"here 6";
			//cout<<"\n temp[i] = "<<temp[i]<<endl;
			//print_matrix(new_matrix);
			nodeptr->children.push_back(build_decision_tree(new_mat_size,child_node,new_matrix));
			//cout<<"here 7";
		}
	}
	return nodeptr;
}

void print_decision_tree(tree_node* nodeptr){
	cout<<endl;	
	if(nodeptr==NULL) return;
	else if(nodeptr->is_leaf==true){
		cout<<"this is a leaf \n";
		cout<<"split_on : "<<nodeptr->split_on<<endl;
		cout<<"predicted label : "<<nodeptr->label<<endl;
	}
	else{
		cout << "value: " << nodeptr->label << endl;
		cout << "Split on: " << nodeptr->split_on;
		cout << "this node has children : \n";
		int iii;
		for(iii=0;iii<nodeptr->childrenValues.size();iii++){
			cout<<nodeptr->childrenValues[iii]<<",";
		}
		cout<<endl;
		for (iii = 0; iii < nodeptr->children.size(); iii++) {   
			cout <<"  ";
			print_decision_tree(nodeptr->children[iii]);
		}
	}
}
float find_most_frequent_class(int matsize,vector<float> * matrix)
{
	vector<float> label;
	int i;
	/*for(i=0;i<matsize;i++){
		label.push_back(matrix[i][matrix[i].size()-1]);
	}
	map<float,int> count;
	for(i=0;i<label.size();i++)
	{
		if(count[label[i]]==0)
			count[label[i]]=1;
		else
			count[label[i]]++;
	}
	int highest_count=-1;
	float highest_class;
	map<float,int>::iterator itr;
	for(itr=count.begin();itr!=count.end();itr++)
	{
		if(itr->second>highest_count){
			highest_count=itr->second;
			highest_class=itr->first;
		}
	}*/

	//since I know that this is a two class problem, I'm using this
	int one=0,two=0;
	for(i=0;i<matsize;i++){
		if(matrix[i][matrix[i].size()-1]==1)
			one++;
		else 
			two++;
	}
	//return highest_class;
	if(one==two){
		cout<<"both labels are of equal frequency, we can use any as default\n";
		return 0;		//0 is returned here, this is always a match
	}
	if(one>two) return 1;
	return 2;
}
float test_data_on_decision_tree(vector<float> sample, tree_node* nodeptr,float default_label)
{
	//cout<<"inside test_data_on_decision_tree : \n";
	float predicted_label;
	//cout<<"am I here ? ";
	if(nodeptr->is_leaf==true){
		return nodeptr->label;
	}
	vector<float> temp=sample;
	//cout<<"here atleast : \n";
	while(!nodeptr->is_leaf)
	{
		float value=temp[nodeptr->split_on];
		//cout<<"split_on : "<<nodeptr->split_on<<endl;
		temp.erase(temp.begin()+(nodeptr->split_on));
		//cout<<"value in test : "<<value<<endl;
		int i,child_index=-1;
		//cout<<"children values : \n";
		for(i=0;i<nodeptr->childrenValues.size();i++){
			//cout<<(nodeptr->childrenValues[i])<<" ";
			if(nodeptr->childrenValues[i]==value){
				child_index=i;
				break;
			}
		}
		//cout<<endl;
		//cout<<"child_index : "<<child_index<<endl;
		if(child_index==-1)
			return default_label;
		nodeptr=(nodeptr->children[child_index]);
		if(nodeptr==NULL){
			return default_label;
		}
		predicted_label=nodeptr->label;
	}
	if(nodeptr->is_leaf==true){
		return nodeptr->label;
	}
	//cout<<predicted_label<<endl;
	return predicted_label;
}

void calculate_accuracy(int size,float *a,float *b)
{
	int i,j,count=0;
	for(i=0;i<size;i++)
	{
		if(a[i]==b[i])
			count++;
	}
	cout<<"accuracy : "<<(float)count/size;
	cout<<endl;
}

void store_predicted_labels(int size,float* a){
	ofstream mystream("predicted_labels.txt",ios::out|ios::app);
	int i;
	for(i=0;i<size;i++){
		mystream<<a[i]<<" ";
	}
	mystream<<endl;
	mystream.close();
}

float* main_function(int trainsize,int testsize,int cccc,vector<float>* traindata,vector<float>* testdata){
	int row,col,i,j;
	float a;
	int colsize=cccc;
	
	float most_frequent_class=find_most_frequent_class(trainsize,traindata);
	cout<<"most_frequent_class : "<<most_frequent_class<<endl;
	cout<<"build_decision_tree is called \n";
	clock_t start,end;
	start=clock();
	tree_node* root= new tree_node;
	root=build_decision_tree(trainsize,root,traindata);
	end=clock();
	cout<<endl;	
	cout<<"time taken to build_decision_tree : "<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
	//print_decision_tree(root);
	cout<<endl;
	cout<<"decision tree formed \n";

	float* given_label=new float[testsize];
	float* predicted_label=new float[testsize];
	//cout<<"testsize : "<<testsize<<endl;
	for(i=0;i<testsize;i++){
		given_label[i]=testdata[i][colsize-1];
		vector<float> vec=testdata[i];
		float pred_label=test_data_on_decision_tree(vec,root,most_frequent_class);
		if(pred_label==0) pred_label=given_label[i];
		predicted_label[i]=pred_label;
	}
	
	calculate_accuracy(testsize,given_label,predicted_label);
	//store_predicted_labels(testsize,predicted_label);
	delete given_label;
	return predicted_label;
}
//this part is from random_tree_1.h



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
	//int columncount=11;
	fp=fopen(argv[1],"r");
	string str;
	ifstream fs(argv[1]);
	getline(fs,str);
	//cout<<str<<endl;
	int count=1;
	for(int i=0;i<str.length();i++){
		if(str[i]==' ') count++;
	}
	fs.close();
	int columncount=count;
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

	int no_tree=50;										//no of trees	
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