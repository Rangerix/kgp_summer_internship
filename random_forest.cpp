//randomizing the samples
#include "decision_tree1.h"
#include <cstdlib>
using namespace std;

vector<vector<float> > build_random_matrix(vector<vector<float> > matrix,int samples_per_tree)
{
	int i,j,m=samples_per_tree,n=matrix.size(),flag;
	vector<int> result(m,-1);
	for(i = 0; i<m; ++i)
	{
	   int r;
	   flag=1;
	   while(flag)
	   {
	   		clock_t t = clock();
			srand((int)t);
	      	r = rand()%n;
	      	for(j=0;j<m;j++){
	      		if(result[j]==r){
					flag=1;	  
					break;    		
	      		}
	      		else
	      			flag=0;
	      	}
	   }
	   result[i] = r;   
	}
	vector<vector<float> > answer(m);
	for(i=0;i<m;i++)
	{
		int row=result[i];
		for(j=0;j<matrix[row].size();j++){
			answer[i].push_back(matrix[row][j]);
		}
	}
	//print_matrix(answer);
	return answer;

}
float find_most_frequent_value(vector<float> label)
{
	int i,j;
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

vector<float> predict_label(vector<vector<float> > matrix,vector<tree_node*> forest,vector<float> most_frequent_class_submat)
{
	int i,j,k;
	vector<float> vec;
	float most_frequent_class=find_most_frequent_class(matrix);
	for(i=0;i<matrix.size();i++){
		vector<float> predicted_label;
		for(j=0;j<forest.size();j++){
			float pred_label=(test_data_on_decision_tree(matrix[i],forest[j],most_frequent_class_submat[j]));
			predicted_label.push_back(pred_label);
			//here I need to work on the most_frequent_class , 
			//instead of using the most frequent class of the whole matrix, the most frequent class of the submatrices
			//can be used. I have to see which one gives better accuracy
		}
		vec.push_back(find_most_frequent_value(predicted_label));
	}
	return vec;
}

void random_forest(vector<vector<float> > matrix){
	int samples=matrix.size();
	int features=matrix[0].size();
	vector<bool> visited(samples,false);			//we have to visit all the samples
	int no_tree=50;
	vector<tree_node*> forest(no_tree);
	int i,j;
	int samples_per_tree;
	//cout<<"enter number of samples in a decision tree : ";
	samples_per_tree=log2(samples)+1;
	vector<float> most_frequent_class_submat;
	for(i=0;i<no_tree;i++)
	{
		vector<vector<float> >new_matrix=build_random_matrix(matrix,samples_per_tree);
		tree_node* root= new tree_node;
		root=build_decision_tree(root,new_matrix);
		forest[i]=root;
		most_frequent_class_submat.push_back(find_most_frequent_class(new_matrix));
		cout<<"forest "<<i<<" formed \n";
	}

	int row,col;
	float a;
	cout<<"enter test matrix : ";
	cout<<"enter row : ";
	cin>>row;
	cout<<"enter col : ";
	cin>>col;
	vector<vector<float> > test_matrix(row);
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++){
			cin>>a;
			test_matrix[i].push_back(a);
		}
	}
	vector<float> predicted_label=predict_label(test_matrix,forest,most_frequent_class_submat);
	cout<<endl;
	for(i=0;i<predicted_label.size();i++)
		cout<<predicted_label[i]<<endl;
	cout<<endl;

}


int main()
{
	int row,col,i,j;
	float a;
	cout<<"row : ";
	cin>>row;
	cout<<"col : ";
	cin>>col;
	vector<vector<float> > matrix(row);
	cout<<"enter dataset : \n";
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			cin>>a;
			matrix[i].push_back(a);
		}
	}
	random_forest(matrix);
return 0;
}