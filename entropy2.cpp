// using dynamically allocated array instead of vectors

#include <iostream>
#include <cmath>
#include <set>
using namespace std;


float my_entropy(int size,float* v){
	int i,count;
	float prob,sum=0.0,val;
	set<float> s(v,v+size);
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

float gain(int size,float* label, float* attr)
{
	int i;
	float val;
	float total=0.0;
	float* subentr=new float[size];
	int l=0;
	for(i=0;i<size;i++)
	{
		val=attr[i];
		//now find Sv
		float* sublabel=new float[size];
		int k=0;
		for(int j=0;j<size;j++){
			if(attr[j]==val){
				sublabel[k++]=label[j];
			}
		}
		float entr=my_entropy(k,sublabel);
		//cout<<"eeee"<<entr<<endl;
		subentr[l++]=entr;
	}
	float answer=my_entropy(size,label);


	float prob=0,sum=0;
	int count;
	set<float> s(subentr,subentr+l);
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


int main()
{
	float *arr;
	int n,i;
	cout<<"how many : ";
	cin>>n;
	arr=new float[n];
	/*for(int i=0;i<n;i++)
		cin>>arr[i];*/
	//cout<<my_entropy(n,arr)<<endl;

	float *label = new float[n];
	float *feature=new float[n];
	cout<<"enter label : \n";
	for(i=0;i<n;i++)
		cin>>label[i];
	while(1){
		cout<<"enter feature : \n";
		for(i=0;i<n;i++)
			cin>>feature[i];
		cout<<gain(n,label,feature)<<endl;
	}
return 0;
}