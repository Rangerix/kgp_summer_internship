#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
	int i=0;
	int needed=50;
	clock_t start,start1,end,end1;
	start=clock();
	system("g++ random_forest2.cpp");
	for(i=0;i<needed;i++)
	{
		cout<<"execution : "<<i<<endl;
		system("./a.out trainfile1.txt testfile1.txt");
	}
	end=clock();
return 0;
}
