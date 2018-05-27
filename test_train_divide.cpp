#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "decision_tree1.h"
using namespace std;


int main()
{
	char filename[200];
	cout<<"filename : ";
	cin>>filename;
	ifstream inputstr;
	inputstr.open(filename);
	vector<vector<float> > dataset;
	string mystr;
	while(getline(inputstr,mystr)){
		vector<float> vec=parsestring(mystr);
		dataset.push_back(vec);
	}
	inputstr.close();
	cout<<"dataset size : "<<dataset.size()<< " , "<<dataset[0].size()<<endl;
	/*for(int i=0;i<dataset.size();i++){
		for(int j=0;j<dataset[i].size();j++){
			cout<<dataset[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	*/
	int crossvalidation=4;
	int testrow=dataset.size()/crossvalidation;						//no of rows in train data
	int trainrow=dataset.size()-testrow;
	cout<<"testdata size : "<<testrow<<endl;
	vector<int> testrows(testrow,-1);								//randomly generate testrows
	for(int i = 0; i<testrow; ++i)
	{
	   int r;
	   int flag=1;
	   while(flag)
	   {
	   		clock_t t = clock();
			srand((int)t);
	      	r = rand()%dataset.size();
	      	for(int j=0;j<testrow;j++){
	      		if(testrows[j]==r){
					flag=1;	  
					break;    		
	      		}
	      		else
	      			flag=0;
	      	}
	   }
	   testrows[i] = r;   
	}
	/*for(int i=0;i<testrows.size();i++){
		cout<<testrows[i]<< " ";
	}
	cout<<endl;
	*/
	cout<<"preparing  test data ...\n";
	vector<vector<float> > testdata(testrow);
	for(int i=0;i<testrow;i++){
		testdata[i]=dataset[testrows[i]];
	}
	cout<<"preparing train data ...\n";
	vector<vector<float> > traindata=dataset;
	for(int i=0;i<testrow;i++)
	{
		int idx=testrows[i];
		traindata[idx]=traindata.back();
		traindata.pop_back();
	}
	/*for(int i=0;i<traindata.size();i++){
		for(int j=0;j<traindata[i].size();j++){
			cout<<traindata[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;*/
	/*for(int i=0;i<testdata.size();i++){
		for(int j=0;j<testdata[i].size();j++){
			cout<<testdata[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	*/
											
	int i,j;


	
	cout<<"writing testdata into file \n";
	char testfilename[20];
	string testfilestr="testfile.txt";
	for(i=0;i<testfilestr.length();i++){
		testfilename[i]=testfilestr[i];
	}
	testfilename[i]='\0';
	fstream teststream(testfilename,ios::out);
	if(!teststream){
		cout<<"cannot open testsile\n";
		return 0;
	}
	for(i=0;i<testdata.size();i++){
		for(j=0;j<testdata[i].size();j++){
			teststream<<testdata[i][j]<<" ";
		}
		teststream<<endl;
	}	
	teststream.close();


	cout<<"writing traindata into file \n";
	char trainfilename[20];
	string trainfilestr="trainfile.txt";
	for(i=0;i<trainfilestr.length();i++){
		trainfilename[i]=trainfilestr[i];
	}
	trainfilename[i]='\0';
	fstream trainstream(trainfilename,ios::out);
	if(!trainstream){
		cout<<"cannot open trainfile \n";
		return 0;
	}
	for(i=0;i<traindata.size();i++){
		for(j=0;j<traindata[i].size();j++){
			trainstream<<traindata[i][j]<<" ";
		}
		trainstream<<"\n";
	}	
	trainstream.close();

	cout<<"decision_tree function is called ... ";
	main_dec_tree(trainfilename,testfilename);
return 0;
}