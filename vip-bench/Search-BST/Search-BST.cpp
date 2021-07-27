/*This program is a search algorithm which is implemented using the binary
search tree algorithm*/

#include <iostream>
#include <cmath>

#include "../config.h"
#include "utils.h"

using namespace std;

//Max size of the array
const int size = 100;

VIP_ENCINT values[size];

//retives the data from values vai an encrypted index
#ifdef VIP_ENC_MODE
	VIP_ENCINT getValAtIndex(VIP_ENCINT index){
		VIP_ENCINT out = -1;
		VIP_ENCBOOL condition = false;
		for(int i = 0;i<size;i++){
			condition = (index == i);
			out = VIP_CMOV(condition,values[i],out);
		}
		return out;
	}
#endif	

//Initializes the values array in ascending order
void initializeData(){
		  
	mysrand(10);
	for (int i=0; i < size; i++){
		if(i==0)
			values[i]=myrand()%40;
		else{
			values[i]=values[i-1]+myrand()%40;
		}		
	}	   
}

//Implementation of the search algorithm
VIP_ENCINT binarySearch(VIP_ENCINT findMe) {
	VIP_ENCINT left = 0;
	VIP_ENCINT right = size;
	VIP_ENCINT mid = 0;

	#ifndef VIP_NA_MODE		
		VIP_ENCBOOL condition1 = false;
		VIP_ENCBOOL condition2 = false;
		VIP_ENCBOOL condition3 = false;
		VIP_ENCINT out = -1;
	#endif
	
	#ifdef VIP_NA_MODE
	
		while (left <= right) {
		  	mid = left + (right - left) / 2;
		  	if (values[mid] == findMe) {
				return mid;
			} else if (values[mid] < findMe) {
				left = mid + 1;
			} else {
				right = mid - 1;
			}
		}
		return -1;
	#else
		int limit = ceil(log(size)/log(2));
		for(int i = 0; i<=limit;i++){		
			mid = left + (right - left) / 2;
			
			#ifdef VIP_ENC_MODE
				VIP_ENCINT temp = getValAtIndex(mid);
			#else
				VIP_ENCINT temp = values[mid];
			#endif
			condition1 = (temp == findMe);		
			condition2 = (temp < findMe);		
			condition3 = (temp > findMe);		
					
			out = VIP_CMOV(condition1,mid,out);
			left = VIP_CMOV(condition2,mid+1,left);
			right = VIP_CMOV(condition3,mid-1,right);
		}
		return out;
	#endif
}

int main() {
	initializeData();
	{
  		Stopwatch start("VIP-BENCH TIME: ");	
  		for(int i=0;i<size;i++)
  			binarySearch(values[i]);	  	  
	
	}	

	return 0;
}
