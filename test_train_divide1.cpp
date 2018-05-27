//using array instead of vector 
#include <iostream>
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
	
	//vector<vector<float> > dataset;
	string mystr;
	int i,j,linecount=0;
	while(getline(inputstr,mystr)){
		linecount++;
	}
	inputstr.close();
	float** dataset=new float*[linecount];
	for(i=0;i<linecount;i++){
		dataset[i]=new float[11];
	}
	/*for(int i=0;i<dataset.size();i++){
		for(int j=0;j<dataset[i].size();j++){
			cout<<dataset[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	*/
	int crossvalidation=10;
	int testrow=linecount/crossvalidation;						//no of rows in train data
	int trainrow=linecount-testrow;
	cout<<"testdata size : "<<testrow<<endl;
	vector<int> testrows(testrow,-1);								//randomly generate testrows
	for(i = 0; i<testrow; ++i)
	{
	   int r;
	   int flag=1;
	   while(flag)
	   {
	   		clock_t t = clock();
			srand((int)t);
	      	r = rand()%linecount;
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
	float testdata[testrow][11];
	for(i=0;i<testrow;i++){
		int rowno=testrows[i];
		for(j=0;j<11;j++)
			testdata[i][j]=dataset[rowno][j];
	}
	float trainrowind[linecount]={1};
	for(i=0;i<testrow;i++){
		trainrowind[testrows[i]]=0;
	}
	cout<<"preparing train data ...\n";
	float traindata[trainrow][11];
	int traindataindex=0;
	for(i=0;i<linecount;i++)
	{
		if(trainrowind[i]){					//then the row must be in train dataset
			for(j=0;j<11;j++){
				traindata[traindataindex][j]=dataset[i][j];
			}
			traindataindex++;
		}
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
											
	delete dataset;
	
	cout<<"writing testdata into file \n";
	char testfilename[20];
	string testfilestr="testfile.txt";
	for(i=0;i<testfilestr.length();i++){
		testfilename[i]=testfilestr[i];
	}
	testfilename[i]='\0';
	fstream teststream(testfilename,ios::out);
	cout<<"here I am";
	if(!teststream){
		cout<<"cannot open testsile\n";
		return 0;
	}
	for(i=0;i<testrow;i++){
		for(j=0;j<11;j++){
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
	for(i=0;i<trainrow;i++){
		for(j=0;j<11;j++){
			trainstream<<traindata[i][j]<<" ";
		}
		trainstream<<"\n";
	}	
	trainstream.close();

	//cout<<"decision_tree function is called ... \n";
	//main_dec_tree(trainfilename,testfilename);
return 0;
}