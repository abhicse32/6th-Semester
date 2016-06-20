#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;

int main(){
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	for(int i=0;i<v.size(); i++)
		printf("%d ",v[i]);
	printf("\n");
	v[1]=100;
	for(int i=0;i<v.size(); i++)
		printf("%d ",v[i]);
	printf("\n");
	std::vector<int> v1=v;
	v1.push_back(12);
	for(int i=0;i<v.size(); i++)
		printf("%d ",v[i]);
	printf("\n");

return 0;
}