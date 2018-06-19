#include <iostream>
using namespace std;

int main()
{
	int n;
	cin>>n;
	int arr[n];
	int i,j,k;
	float x,y;
	for(i=0;i<n;i++) 
		cin>>arr[i];
	float prob[n];
	for(i=0;i<n;i++) 
		cin>>prob[i];
	//cout<<arr[0]<<" ";
	for(i=0;i<n-1;i++){
		if(arr[i+1]==arr[i]+1) 
			cout<<arr[i]<<" ";
		else if(arr[i+1]==arr[i]){ 			//given that only two possible residues
			if (prob[i]>prob[i+1])
				j=i,k=i+1;
			else j=i+1,k=i;
			cout<<arr[j]<<" ( "<<arr[k]<<" ) ";
			i++;
		}
		else{
			j=arr[i]+1;
			cout<<arr[i]<<" ";
			while(j!=arr[i+1]){
				cout<<"* ";
				j++;	
			}
		}
	}
	if(i<n) cout<<arr[i];
	cout<<endl;
	cout<<i<<","<<j<<endl;
	return 0;
}