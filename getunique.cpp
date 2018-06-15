#include <iostream>
using namespace std;


int count_unique(int size,float* arr)
{
	int i,j,isunique;
	int count=0;
	for(i=0;i<size;i++)
	{
		isunique=1;
		for(j=0;j<i&&isunique;j++){
			if(arr[i]==arr[j]) isunique=0;
		}
		if(isunique) count++;
	}
	return count;
}

int main()
{
	int i,n,j,l;
	cin>>n;
	float arr[n];
	for(i=0;i<n;i++){
		cin>>arr[i];
	}
	int x=count_unique(n,arr);
	cout<<"count_unique : "<<x<<endl;

	int unique_value[x];
	l=0;
	for(i=0;i<n;i++){
		int isunique=1;
		for(j=0;j<l;j++){
			if(arr[i]==unique_value[j]) isunique=0;
		}
		if(isunique) unique_value[l++]=arr[i];
	}
	cout<<endl;
	for(i=0;i<x;i++)
		cout<<unique_value[i]<<" ";
	cout<<endl;
return 0;
}