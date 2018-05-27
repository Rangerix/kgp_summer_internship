// the difference between decision_tree.cpp and decision_tree1.cpp is that in later case, the matrix is 
//represented is as matrix and the last column is represented as label

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
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

vector<float> parsestring(string str)
{
	istringstream mystream(str);
	float a;
	vector<float> vec;
	while(mystream>>a){
		vec.push_back(a);
	}
	return vec;
}

void print_matrix(vector<vector<float> > matrix){
	cout<<endl;
	for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++)
			cout<<matrix[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}
vector<float> get_all_unique_vals(vector<vector<float> > matrix, int index)
{
	vector<float> temp;
	int size=matrix.size();
	for(int i=0;i<size;i++){
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

int decide_split_column(vector<vector<float> > matrix)
{
	int features=matrix[0].size()-1;
	int samples=matrix.size();
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
bool matrixIsEmpty(vector<vector<float> > matrix)
{
	return (matrix.size() <= 1 );
}
bool isHomogeneous(vector<vector<float> > matrix)
{
	int iii;
	vector<float> label;
	for(int i=0;i<matrix.size();i++){
		label.push_back(matrix[i][matrix[i].size()-1]);
	}
	float firstValue = label[0];
	for (iii = 1; iii < label.size(); iii++) {
		if (firstValue != label[iii]) {
			return false;
		}
	}
	return true;
}

vector<vector<float> > modify_matrix(vector<vector<float> > matrix,int col_no,float value){
	int i,j,k,l;
	int row=0;
	//count no of rows in the new matrix
	vector<vector<float> > new_matrix;
	for(i=0;i<matrix.size();i++){
		if(matrix[i][col_no]==value){
			vector<float> temp_row;
			for(j=0;j<matrix[i].size();j++){
				if(j!=col_no){
					//cout<<matrix[i][j]<<" ";
					temp_row.push_back(matrix[i][j]);
				}
			}
			new_matrix.push_back(temp_row);
		}
		//cout<<endl;
	}
	//print_matrix(new_matrix);
	return new_matrix;
}

tree_node* build_decision_tree(tree_node* nodeptr,vector<vector<float> > matrix)
{
	//cout<<"here \n";
	//print_matrix(matrix);
	if(matrix.size()<=1){
		//cout<<"here 1\n";
		return NULL;
	}
	else if(isHomogeneous(matrix)){
		nodeptr->is_leaf=true;
		nodeptr->label=matrix[0][matrix[0].size()-1];
		//cout<<"here 2\n";
		return nodeptr;
	}
	else{
		nodeptr->is_leaf=false;
		int split_col=decide_split_column(matrix);
		nodeptr->split_on=split_col;
		vector<float> temp=get_all_unique_vals(matrix,split_col);
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
			vector<vector<float> > new_matrix=modify_matrix(matrix,split_col,temp[i]);
			//cout<<"here 6";
			//cout<<"\n temp[i] = "<<temp[i]<<endl;
			//print_matrix(new_matrix);
			nodeptr->children.push_back(build_decision_tree(child_node,new_matrix));
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
float find_most_frequent_class(vector<vector<float> > matrix)
{
	vector<float> label;
	int i;
	for(i=0;i<matrix.size();i++){
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
	}
	return highest_class;
}
float test_data_on_decision_tree(vector<float> sample, tree_node* nodeptr,float default_label)
{
	cout<<"inside test_data_on_decision_tree : \n";
	float predicted_label;
	if(nodeptr->is_leaf==true){
		return nodeptr->label;
	}
	vector<float> temp=sample;
	while(!nodeptr->is_leaf)
	{
		int value=temp[nodeptr->split_on];
		temp.erase(temp.begin()+nodeptr->split_on);
		int i,child_index;
		for(i=0;i<nodeptr->childrenValues.size();i++){
			if(nodeptr->childrenValues[i]==value){
				child_index=i;
				break;
			}
		}
		if(i==nodeptr->childrenValues.size())
			return default_label;
		nodeptr=nodeptr->children[child_index];
		/*if(nodeptr==NULL){
			predicted_label=default_label;
			break;
		}*/
		predicted_label=nodeptr->label;
	}
	if(nodeptr->is_leaf==true){
		return nodeptr->label;
	}
	//cout<<predicted_label<<endl;
	return predicted_label;
}

void calculate_accuracy(vector<float> a,vector<float> b)
{
	int i,j,count;
	cout<<endl;
	for(i=0;i<a.size();i++)
	{
		cout<<a[i]<<"   "<<b[i]<<endl;
		if(a[i]==b[i])
			count++;
	}
	cout<<endl;
	cout<<"accuracy : "<<(float)count/a.size();
	cout<<endl;
}

float main_dec_tree(char trainfilename[20],char testfilename[20]){
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
	cout<<"reading trainfile \n";
	ifstream inputstr;
	inputstr.open(trainfilename);
	if(!inputstr){
		cout<<"cannot open trainfile ";
		return 0;
	}
	vector<vector<float> > matrix;
	string mystr;
	while(getline(inputstr,mystr)){
		vector<float> vec=parsestring(mystr);
		matrix.push_back(vec);
	}
	inputstr.close();
	float most_frequent_class=find_most_frequent_class(matrix);
	cout<<"most_frequent_class : "<<most_frequent_class<<endl;
	cout<<"build_decision_tree is called \n";
	tree_node* root= new tree_node;
	root=build_decision_tree(root,matrix);
	cout<<endl;	
	print_decision_tree(root);
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
	cout<<"reading testfile\n";
	inputstr.open(testfilename);
	if(!inputstr){
		cout<<"cannot open testfile ";
		return 0;
	}
	vector<vector<float> > test_matrix;
	string mystrtest;
	while(getline(inputstr,mystrtest)){
		vector<float> vec=parsestring(mystrtest);
		test_matrix.push_back(vec);
	}
	inputstr.close();


	vector<float> given_label;
	vector<float> predicted_label;
	for(i=0;i<test_matrix.size();i++){
		given_label.push_back(test_matrix[i][test_matrix[i].size()-1]);
	}
	cout<<"given_label considered\n";
	//test using the decision tree model
	for(i=0;i<test_matrix.size();i++){
		//cout<<"predicted_label for "<<i;
		float pred_label=test_data_on_decision_tree(test_matrix[i],root,most_frequent_class);
		predicted_label.push_back(pred_label);
	}
	calculate_accuracy(given_label,predicted_label);

return 0;
}
