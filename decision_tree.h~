#include "entropy.h"
#include <iostream>
#include <vector>
#include <set>
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

vector<float> get_all_unique_vals(vector<vector<float> > matrix, int index)
{
	vector<float> temp;
	int size=matrix.size();
	for(int i=0;i<size;i++){
		temp.push_back( matrix[index][i] );
	}
	set<float> s(temp.begin(),temp.end());
	vector<float> vec(s.begin(),s.end());
	return vec;
}

int decide_split_column(vector<vector<float> > matrix,vector<float> label)
{
	int features=matrix[0].size();
	int samples=matrix.size();
	int i,j;
	vector<bool> visited(features,false);
	float max_gain= -9999,max_index,temp;
	for(i=0;i<features;i++)
	{
		vector<float> attr;
		for(j=0;j<samples;j++)
		{
			attr.push_back(matrix[i][j]);
		}
		temp=gain(attr,label);
		if(temp>max_gain){
			max_gain=temp;
			max_index=i;
		}
	}
	return max_index;
}

vector<vector<float> > modify_matrix(vector<vector<float> > &matrix,int col_no,vector<float> label ,float column_value){
	int i,j,k,l;
	int row=0;
	for(i=0;i<matrix.size();i++){
		if(matrix[i][col_no]==column_value)
			row++;
	}
	vector<vector<float> > new_matrix(row);
	for(i=0;i<matrix.size();i++){
		if(matrix[i][col_no]==column_value){
			for(j=0;j<matrix[i].size();j++){
				new_matrix[i].push_back(matrix[i][j]);
			}
			new_matrix[i].push_back(label[i]);
		}
	}
	return new_matrix;
}
bool matrixIsEmpty(vector<vector<float> > matrix)
{
	return (matrix.size() < 1 );
}

bool isHomogeneous(vector<vector<float> > matrix,vector<float> label)
{
	int iii;
	float firstValue = label[0];
	for (iii = 1; iii < label.size(); iii++) {
		if (firstValue != label[iii]) {
			return false;
		}
	}
	return true;
}

tree_node* decision_tree(tree_node* node_ptr ,vector<vector<float> > matrix,vector<float> label)
{
	cout<<"here "<<endl;
	for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++)
			cout<<matrix[i][j]<<" ";
		cout<<endl;
	}
	cout<<"here 3";
	if(matrixIsEmpty(matrix)){
		cout<<"returned ";
		return NULL;
	}
	else if(isHomogeneous(matrix,label)){
		//then leaf node
		node_ptr-> is_leaf=true;
		node_ptr->label=label[1];
		cout<<"homogenous ";
		return node_ptr;
	}
	else{
		int spliton=decide_split_column(matrix,label);
		node_ptr->split_on=spliton;
		vector<float> possible_children=get_all_unique_vals(matrix,spliton);
		cout<<"here 2";
		for(int i=0;i<possible_children.size();i++){
			tree_node* child_node=(tree_node*) new tree_node;
			child_node->label=possible_children[i];
			node_ptr->childrenValues.push_back(possible_children[i]);
			child_node->is_leaf=false;
			child_node->split_on=spliton;
			vector<vector<float> > combo_matrix=modify_matrix(matrix,spliton,label,possible_children[i]);
			//last column is the label
			vector<float> new_label;
			for(int iii=0;iii<combo_matrix.size();iii++)
				new_label.push_back(combo_matrix[iii][combo_matrix[iii].size()-1]);
			vector<vector<float> > new_matrix(combo_matrix.size());
			for(int iii=0;iii<combo_matrix.size();iii++){
				for(int jjj=0;jjj<combo_matrix[iii].size()-1;jjj++){
					new_matrix[iii].push_back(combo_matrix[iii][jjj]);
				}
			}
			node_ptr->children.push_back(decision_tree(child_node,new_matrix,new_label));
		}
	}
	return node_ptr;
}


void print_decision_tree(tree_node* node_ptr)
{
	if(node_ptr==NULL)
		return;
	if(node_ptr->is_leaf==true){
		cout<<"predicted class : "<<node_ptr->label<<endl;
	}
	else{
		cout<<"value : "<<node_ptr->label<<endl;
		cout<<"split on : "<<node_ptr->split_on<<endl;
		for(int i=0;i<node_ptr->children.size();i++)
		{
			cout<<"\t";
			print_decision_tree(node_ptr->children[i]);
		}	
	}
}

int main()
{
	int i,j,n,samples,features;
	float f;
	cout<<"Enter size : ";
	cin>>n;
	
	cout<<"Enter sample : ";
	cin>>samples;
	cout<<"enter feature : ";
	cin>>features;
	n=samples;
	vector<float> label(n);
	//cout<<"Enter label : ";
	for(i=0;i<n;i++){
		cin>>label[i];
	}
	//cout<<my_entropy1(label)<<endl;
	vector<vector<float> > matrix(samples);
	for(i=0;i<samples;i++){
		matrix[i].assign(features,0);
	}
	//cout<<"enter feature : ";
	for(j=0;j<features;j++)
	{
		
		for(i=0;i<samples;i++)
			cin>>matrix[i][j];
	}
	tree_node * node=new tree_node;
	node=decision_tree(node,matrix,label);
	cout<<"generated ";
	print_decision_tree(node);

return 0;
}