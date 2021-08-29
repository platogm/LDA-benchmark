#include<iostream>
#include<iomanip>
#include<climits>
#include<string>

#include "../config.h"
#include "utils.h"

using std::cout;
using std::endl;

//V represents the number of vertex in the graph
const int V = 10;

//This are the names(representation) of each vertex
const std::string vertName[V] = {"Home","z-mall","st.pet","office","school","motel","restr.","library","airport","barber"};

//This function finds the vertex with min distance from the unknown vertexes
void minVal(VIP_ENCINT dist[V],VIP_ENCBOOL known[V],VIP_ENCINT& min){
	min = -1;
	VIP_ENCINT distVal = INT_MAX;
			
	#ifndef VIP_NA_MODE	
		VIP_ENCBOOL condition = false;	
		for(int i=0;i<V;i++){
			condition = (distVal>dist[i])&&!known[i];
			distVal = VIP_CMOV(condition,dist[i],distVal);
			min = VIP_CMOV(condition,i,min);					
		}
		
		for(int i=0;i<V;i++){
			condition = (min==i);
			known[i] = VIP_CMOV(condition,VIP_ENCBOOL(true),known[i]);
		}
	#else	
		for(int i=0;i<V;i++){
			if(distVal>dist[i] && !known[i]){
				distVal = dist[i];
				min = i;
			}					
		}
				
		for(int i=0;i<V;i++){
			if(min == i){
				known[i] = true;
				break;
			}
		}
	#endif
	
}

//Finds the shortest path from the source to all other vertexes
void minSpanTree(VIP_ENCINT graph[V][V],VIP_ENCINT path[V]){
	VIP_ENCINT dist[V];
	VIP_ENCBOOL known[V];
	VIP_ENCINT min = 0;
	
	#ifndef VIP_NA_MODE
		VIP_ENCBOOL condition=false;
		
		//sets the source vertex as known and gives it a distance of 0;	
		for(int i=0;i<V;i++){
			condition = (min==i);
			dist[i] = VIP_CMOV(condition,VIP_ENCINT(0),VIP_ENCINT(INT_MAX));
			known[i] = VIP_CMOV(condition,VIP_ENCBOOL(true),VIP_ENCBOOL(false));	
		}
		
		for(int k = 0;k<V;k++){	
			for(int i=0;i<V;i++){
				for(int j=0;j<V;j++){
					
					//This states if the ith element is the min vertex from the unknowns and 
					//if it has a connection with element j(graph[i][j]!=0) and if the distance is smaller than
					//the previous distance then update the path and the distance					 
					condition = (i==min) && !known[j] && (graph[i][j]!=0) && (graph[i][j] < dist[j]);
					dist[j]=VIP_CMOV(condition,graph[i][j],dist[j]);
					path[j]=VIP_CMOV(condition,min,path[j]);
				}
			}
			minVal(dist,known,min);
		}
	#else		
		for(int i=0;i<V;i++){
			if(min == i){
				dist[i] = 0;
				known[i] = true;
			}else{
				dist[i] = INT_MAX;
				known[i] = false;
			}
		}
		
		for(int i=0;i<V;i++){
			for(int j = 0;j<V;j++){
				if(!known[j] && graph[min][j]!=0 && (graph[min][j] < dist[j])){
					dist[j] = graph[min][j];
					path[j] = min;				
				}
			}
			minVal(dist,known,min);
		}
	#endif
}

//Used to initialize the graph
void initializeData(VIP_ENCINT graph[V][V]){
		  
	mysrand(10);
	for (int i=0; i < V; i++){
		for(int j=0;j<V;j++){
			if(i>j){
				graph[i][j] = graph[j][i];
			}else if(i==j){
				graph[i][j]=0;
			}else{
				if(myrand()%5 == myrand()%5){
					graph[i][j]=0;	
				}else
					graph[i][j]=myrand()%10;			
			}
		}		
	}	   
}

void displayGraph(VIP_ENCINT graph[V][V]){
	int index = 0;
	for(int i=-1;i<V;i++){
		for(int j=-1;j<V;j++){
			if(i==-1){
				if(j==-1){
					cout<<std::setw(7)<<" ";
				}else{
					cout<<std::setw(8)<<vertName[j];
				}
			}else{
				if(j==-1){
					cout<<std::setw(8)<<vertName[index];
					index++;
				}else{						
					cout<<std::setw(8)<<VIP_DEC(graph[i][j]);
				}
			}
		}
			cout<<endl;
	}
	cout<<endl<<endl;
}

//Displays the path from source to destination
void displayPath(VIP_ENCINT source,VIP_ENCINT destination,VIP_ENCINT path[V]){
	static int sourceF = VIP_DEC(source);
	static int destF = VIP_DEC(destination);
	static int count = 0;
	
	int currPath = VIP_DEC(destination);
	
	if(count == 0){
		cout<<"\nSource: "<<vertName[sourceF]<<"\tDestination: "<<vertName[currPath]<<endl<<endl;
		cout<<"Path: "<<vertName[sourceF];
		count++;
	}	
	if(VIP_DEC(path[currPath]) != sourceF){
		displayPath(sourceF,path[currPath],path);
	}
	cout<<"->"<<vertName[currPath];
	if(currPath == destF){
		cout<<endl;
	}
}

int main(){
	VIP_ENCINT source = 0;
	VIP_ENCINT destination = 1;
	VIP_ENCINT graph[V][V];
	VIP_ENCINT path[V];	
	for(int i=0;i<V;i++){
		path[i]=-1;
	}
	initializeData(graph);	
	displayGraph(graph);
	{
		Stopwatch start("VIP_BENCH_RUN_TIME");
		minSpanTree(graph,path);
	}
	displayPath(source,destination,path);
}
