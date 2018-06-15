#include <iostream>
#include <vector>
#include <set>
#include <cmath>
using namespace std;

float my_entropy1(vector<float> v){
	int size=v.size();
	int i,count;
	float prob,sum=0.0,val;
	set<float> s(v.begin(),v.end());
	set<float>::iterator itr;
	for(itr=s.begin();itr!=s.end();itr++){
		val=*itr;
		count=0;
		for(i=0;i<size;i++){
			if(v[i]==val)
				count++;
		}
		//cout<<val<<" "<<count<<endl;
		prob=(float)count/size;
		sum+=prob*log2(prob);
	}
	return -sum;
}

float gain(vector<float> label, vector<float> attr)
{
	int size=attr.size(),i;
	float val;
	float total=0.0;
	vector<float> subentr;
	for(i=0;i<size;i++)
	{
		val=attr[i];
		//now find Sv
		vector<float> sublabel;
		for(int j=0;j<size;j++){
			if(attr[j]==val){
				sublabel.push_back(label[j]);
			}
		}
		float entr=my_entropy1(sublabel);
		//cout<<"eeee"<<entr<<endl;
		subentr.push_back(entr);
	}
	float answer=my_entropy1(label);


	float prob=0,sum=0;
	int count;
	set<float> s(subentr.begin(),subentr.end());
	set<float>::iterator itr;
	for(itr=s.begin();itr!=s.end();itr++){
		val=*itr;
		count=0;
		for(i=0;i<size;i++){
			if(subentr[i]==val)
				count++;
		}
		//cout<<val<<" "<<count<<endl;
		prob=(float)count/size;
		sum+=prob*val;
	}
	answer-=sum;
	return answer;
}

int main(){
	int n;
	float f;
	cout<<"Enter size : ";
	cin>>n;
	vector<float> vec(n);
	cout<<"Enter label : ";
	for(int i=0;i<n;i++){
		cin>>vec[i];
	}
	cout<<my_entropy1(vec)<<endl;
	while(1){
		cout<<"Enter attribute : ";
		vector<float> attr(n);
		for(int i=0;i<n;i++){
			cin>>attr[i];
		}
		cout<<gain(vec,attr)<<endl;
	}
return 0;
}