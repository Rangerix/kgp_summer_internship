#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
	int i=0;
	int needed=60;
	system("g++ random_forest2.cpp");
	for(i=0;i<needed;i++)
	{
		cout<<"execution : "<<i<<endl;
		system("./a.out trainfile.txt testfile.txt");
	}
return 0;
}