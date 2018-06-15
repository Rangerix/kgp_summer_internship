#include <iostream>
#include <vector>
using namespace std;

int main(){
	int i,n,j;
	cin>>n;
	vector<int> v(n);
	for(i=0;i<n;i++)
		cin>>v[i];
	vector<int> temp;
	for(i=0;i<n;i++){
		j=i+1;
		if(v[i]+1==v[j]) temp.push_back(v[i]);
		else{
			temp.push_back(v[i]);
			if(temp.size()>1)
				for(int k=0;k<temp.size();k++)
					cout<<temp[k]<<" ";
			cout<<endl;
			temp.clear();
		}
	}
	return 0;
}