#include <iostream>
using namespace std;

int main()
{
	
	int arr[5];
	try{
		for(int i=0;i<8;i++){
			arr[i]=i*10;
			cout<<arr[i]<<endl;
		}
	}
	catch(std::exception &e){
		cout<<"segmentation fault happened\n";
	}
	return 0;
}