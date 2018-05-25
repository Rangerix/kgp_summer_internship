#include <iostream>
#include <cstdlib>
#include "split_file.h"
using namespace std;

int main()
{
	string filename;
	cin>>filename;
	
	string command_string = "grep ^ATOM "+filename+" > mainfile.pdb";
	int i,len=command_string.length();
	char command[len+1];
	for(i=0;i<len;i++)
	{
		command[i]=command_string[i];
	}
	command[i]='\0';
	system(command);
	filename="mainfile.pdb";													//here filename is being changed
	char filename_arr[filename.length()+1];
	for(i=0;i<filename.length();i++)
		filename_arr[i]=filename[i];
	filename_arr[i]='\0';
	split_function(filename_arr);
	
	
return 0;
}