// the difference between decision_tree.cpp and decision_tree1.cpp is that in later case, the matrix is 
//represented is as matrix and the last column is represented as label

//this is the modified version of decision_tree1.h
//here, instead of calculating the accuracy, the predicted labels are being stored in a file 
//named, "predicted_labels.txt"

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include "entropy.h"
using namespace std;


struct tree_node
{
	float feature_no;
	int split_on;
	float label;		//for leaves, it will contain the label value
	bool is_leaf;
	vector<float> childrenValues;
	vector<tree_node*> children;
};


void print_matrix(vector<float>* matrix){
	cout<<endl;
	int matsize=sizeof(matrix)/sizeof(matrix[0]);
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
	vector<float> label;
	for(i=0;i<samples;i++){
		label.push_back(matrix[i][features]);
	}
	float max_gain= -9999,max_index,temp;
	/*cout<<"the label is : ";
	for(i=0;i<label.size();i++){
		cout<<label[i]<<" ";
	}
	cout<<endl;*/
	for(i=0;i<features;i++)
	{
		vector<float> attr;
		for(j=0;j<samples;j++)
		{
			attr.push_back(matrix[j][i]);
		}
		temp=gain(label,attr);
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
	//cout<<"here \n";
	//print_matrix(matrix);
	//int matsize=sizeof(matrix)/(matrix[0].size());
	//cout<<"matsize : "<<matsize<<endl;
	if(matsize<=1){
		//cout<<"here 1\n";
		return NULL;
	}
	else if(isHomogeneous(matsize,matrix)){
		nodeptr->is_leaf=true;
		nodeptr->label=matrix[0][matrix[0].size()-1];
		//cout<<"here 2\n";
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
	int i,j,count;
	cout<<endl;
	char accu[]="accuracy.txt";
	ofstream out(accu);
	cout<<"size of the label : "<<size<<endl;
	for(i=0;i<size;i++)
	{
		out<<a[i]<<"   "<<b[i]<<endl;
		if(a[i]==b[i])
			count++;
	}
	out<<endl;
	out<<"correctly classified "<<count<<" out of "<<size<<endl;
	cout<<"accuracy : "<<(float)count/size;
	out<<"accuracy : "<<(float)count/size;
	cout<<endl;
	out.close();
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

float main_function(char trainfilename[20],char testfilename[20],int cccc=11){
	int row,col,i,j;
	float a;
	/*cout<<"row : ";
	cin>>row;
	cout<<"col : ";
	cin>>col;
	vector<vector<float> > matrix(row);
	cout<<"enter matrix : \n";
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			cin>>a;
			matrix[i].push_back(a);
		}
	}*/
	int colsize=cccc;
	cout<<"col size : "<<colsize<<endl;
	cout<<"reading trainfile \n";
	FILE *fp;
	fp=fopen(trainfilename,"r");
	int trainsize=0;
	char c;
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') trainsize++;
	}
	fclose(fp);
	cout<<"trainsize : "<<trainsize<<endl;
	vector<float> *matrix=new vector<float>[trainsize];
	for(i=0;i<trainsize;i++){
		matrix[i].assign(colsize,0);
	}
	fp=fopen(trainfilename,"r");
	for(i=0;i<trainsize;i++){
		for(j=0;j<colsize;j++){
			fscanf(fp,"%f ",&matrix[i][j]);
		}
	}
	fclose(fp);
	cout<<"trainfile reading done ...";
	
	float most_frequent_class=find_most_frequent_class(trainsize,matrix);
	cout<<"most_frequent_class : "<<most_frequent_class<<endl;
	cout<<"build_decision_tree is called \n";
	tree_node* root= new tree_node;
	root=build_decision_tree(trainsize,root,matrix);
	cout<<endl;	
	//print_decision_tree(root);
	cout<<endl;
	cout<<"decision tree formed \n";

/*
	cout<<"enter test matrix : \n";
	cout<<"row : ";
	cin>>row;
	cout<<"col : ";
	cin>>col;
	vector<vector<float> > test_matrix(row);
	cout<<"enter matrix : \n";
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			cin>>a;
			test_matrix[i].push_back(a);
		}
	}*/
	int testsize=0;
	cout<<"reading testfile\n";
	fp=fopen(testfilename,"r");
	for(c=getc(fp);c!=EOF;c=getc(fp)){
		if(c=='\n') testsize++; 
	}
	fclose(fp);

	float *given_label=new float[testsize];
	float *predicted_label=new float[testsize];
	vector<float> * test_matrix=new vector<float>[testsize];
	for(i=0;i<testsize;i++){
		test_matrix[i].assign(colsize,0);											//colsize==11
	}

	cout<<"testsize : "<<testsize<<endl;
	fp=fopen(testfilename,"r");
	for(i=0;i<testsize;i++){
		for(j=0;j<colsize;j++){
			fscanf(fp,"%f ",&test_matrix[i][j]);
		}
		given_label[i]=test_matrix[i][colsize-1];
		vector<float> vec=test_matrix[i];
		float pred_label=test_data_on_decision_tree(vec,root,most_frequent_class);
		//cout<<given_label.back()<<" , "<<pred_label<<endl;
		predicted_label[i]=pred_label;
		//test_matrix.push_back(vec);
	}
	fclose(fp);


	
	calculate_accuracy(testsize,given_label,predicted_label);
	store_predicted_labels(testsize,predicted_label);
return 0;
}

int main(int argc,char* argv[]){
	if(argc!=3){
		cout<<"./a.out trainfile.txt testfile.txt\n";
		return 0;
	}
	main_function(argv[1],argv[2]);
return 0;
}
