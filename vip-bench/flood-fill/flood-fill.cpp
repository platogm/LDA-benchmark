#include <iostream>
#include <iomanip>
#include <cstdint>

using namespace std;

#include "../config.h"
#include "utils.h"
#include "addrDecod.h"
 
// `M × N` matrix
#define M 10
#define N 10

//Used to find adjacent values of an element in a matrix
int row[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int col[] = { -1, 0, 1, -1, 1, -1, 0, 1 }; 
#ifndef VIP_NA_MODE 
 struct node{
 	VIP_ENCINT *ptr = new VIP_ENCINT;//points to a value in the array called group which is created below
 	VIP_ENCBOOL assigned = false;
 };
 
//same size as the given matrix
//An element with location(x,y) here represents the element(x,y) in the input matrix
//Holds the groups of the elements in the input matrix
node struct_mat[M][N];
//Array that holds the group values(Range is from 1 upto M*N)
int group[M*N];
#endif

void floodfill(VIP_ENCCHAR mat[M][N], VIP_ENCINT x, VIP_ENCINT y, VIP_ENCCHAR replacement){	
	
	#ifndef VIP_NA_MODE
		#define SAFELOC(X, Y) ((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N)
		
		//Used for the condition value in VIP_CMOV		
		VIP_ENCBOOL condition = false;
		//Set to true if at least one element adjacent to the 
		//element in question has been assigned and has the same color
		VIP_ENCBOOL found = false;	
		
		//Holds the current position of the groupIndex
		int groupIndex = 0;
		
		VIP_ENCULONG temp;
		VIP_ENCULONG temp2;
		VIP_ENCULONG temp3;				
		
		for(int i = 0;i < M;i++){
			for(int j=0;j<N;j++){		
				for (int k=0; k < 8; k++){
		      		if (SAFELOC(i+row[k], j+col[k])){
		      			
						//Set to true if at least one element adjacent to the 
						//element in question has been assigned and has the same color	      				
		      			condition=(struct_mat[i+row[k]][j+col[k]].assigned && mat[i][j] == mat[i+row[k]][j+col[k]]);
		      			
		      			//The moment condition is set to true this value become true until it is reset to false later on
		      			found = VIP_CMOV(condition,condition,found);
		   				
		   				//Assigns the group of the current element to the adjacent group if condition is true and
		   				//It is assigned any group yet
		   				temp = intVal(struct_mat[i][j].ptr);		   				
		   				temp2 = intVal(struct_mat[i+row[k]][j+col[k]].ptr);
		   				temp3 = VIP_CMOV((condition&&!struct_mat[i][j].assigned),temp2,temp);
		   				struct_mat[i][j].ptr = addrVal(temp3);		   						   			
		   				
		   				//If the condition is true but the current element is already assigned an element then
		   				//the all element in the adjacent's group is changed to the current elements group  				       		
		   				*(struct_mat[i+row[k]][j+col[k]].ptr) = VIP_CMOV(condition&&struct_mat[i][j].assigned,*(struct_mat[i][j].ptr),*(struct_mat[i+row[k]][j+col[k]].ptr));
		   				
		   				//If conditon was set to true the the current element will be assigned(true)          	
			       		struct_mat[i][j].assigned = VIP_CMOV(condition,true,struct_mat[i][j].assigned);          	          
		   			}
			  }		
			  		//value is added to group array
		    		group[groupIndex] = groupIndex+1;
		    		
		    		//If no adjacent value that fullfils the condition is found then it will be assigned a new group
		    		*(struct_mat[i][j].ptr) = VIP_CMOV(!found,group[groupIndex],*(struct_mat[i][j].ptr));
		    		groupIndex++;
		    		
		    		//all value are assigned after the finish this phase
		    		struct_mat[i][j].assigned = true;
		    		//found variable is reset for the next iteration
		    		found=false;
			}	
		}
		
		//Holds the value of the group to be changed
		VIP_ENCINT targetGr = 0;
		for (int ix=0; ix < M; ix++){
			for (int iy=0; iy < N; iy++){
				VIP_ENCBOOL _istarget = (x == ix && y == iy);
				targetGr = VIP_CMOV(_istarget, *(struct_mat[ix][iy].ptr), targetGr);
			}
		}
		//This loop finds each element that is in the target group and assigns it with the replacemnt color	  
		for(int i=0;i<M;i++){
		  	for(int j=0;j<N;j++){
		  		condition = (*(struct_mat[i][j].ptr) == targetGr);
		  		mat[i][j] = VIP_CMOV(condition,replacement,mat[i][j]); 		
		  	}
		}
	#else
		#define ISSAFE(MAT, X, Y, TARGET)     (((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N) && mat[(X)][(Y)] == (TARGET))
		VIP_ENCCHAR target = mat[x][y];
 
		mat[x][y] = replacement;

		for (int k = 0; k < 8; k++)
		{
			if (ISSAFE(mat, x + row[k], y + col[k], target))
		    {
			    floodfill(mat, x + row[k], y + col[k], replacement);
    		}
  		}
	#endif
	
}


void
printMatrix(VIP_ENCCHAR mat[M][N])
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      cout << setw(3) << VIP_DEC(mat[i][j]);
    }
    cout << endl;
  }
}




int main(){

	VIP_ENCINT x = 3, y = 9;
	VIP_ENCCHAR replacement = 'C';
	
	VIP_ENCCHAR mat[M][N] =
	{
    { 'Y', 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G' },
    { 'Y', 'Y', 'G', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X' },
    { 'G', 'X', 'G', 'Y', 'G', 'G', 'G', 'X', 'X', 'X' },
    { 'Y', 'Y', 'Y', 'W', 'W', 'G', 'G', 'G', 'G', 'X' },
    { 'W', 'X', 'R', 'Y', 'R', 'R', 'G', 'X', 'X', 'X' },
    { 'W', 'W', 'W', 'Y', 'R', 'G', 'G', 'X', 'X', 'X' },
    { 'W', 'B', 'W', 'Y', 'R', 'R', 'R', 'R', 'R', 'X' },
    { 'Y', 'Y', 'Y', 'Y', 'B', 'R', 'R', 'X', 'X', 'X' },
    { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X' },
    { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X' }
	};
	
	cout<<"\nBEFORE flooding '"<<VIP_DEC(replacement)<<"' @ "<<"("<<VIP_DEC(x)<<","<<VIP_DEC(y)<<")\n";
	printMatrix(mat);
	{
		Stopwatch start("VIP-bench runtime: ");
		floodfill(mat,x, y, replacement);
	}
	cout<<"\nAfter\n"<<endl;	
	printMatrix(mat);
}
 
