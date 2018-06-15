// using dynamically allocated array instead of vectors

#include <iostream>
#include <cmath>
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

float my_entropy(int size,float* v){
	int i,j,k,count;
	float prob,sum=0.0,val;
	int n=count_unique(size,v);
	int l=0;
	float uniquevalue[n];
	for(i=0;i<size;i++){
		int isunique=1;
		for(k=0;k<l;k++){
			if(v[i]==uniquevalue[k]) isunique=0;
		}
		if(isunique) uniquevalue[l++]=v[i];
	}
	/*cout<<"l : "<<l<<" n : "<<n<<endl;
	for(i=0;i<l;i++){
		cout<<uniquevalue[i]<<" , ";
	}
	cout<<endl;*/
	for(j=0;j<l;j++){											//iterate over the unique values
		val=uniquevalue[j];
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
	int i,j,k;
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


	int n=count_unique(size,subentr);
	l=0;
	float uniquevalue[n];
	for(i=0;i<size;i++){
		int isunique=1;
		for(k=0;k<l;k++){
			if(subentr[i]==uniquevalue[k]) isunique=0;
		}
		if(isunique) uniquevalue[l++]=subentr[i];
	}
	float prob=0,sum=0;
	int count;
	for(j=0;j<l;j++){
		val=uniquevalue[j];
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
	/*arr=new float[n];
	for(int i=0;i<n;i++)
		cin>>arr[i];
	cout<<my_entropy(n,arr)<<endl;
	*/
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