#include<iostream>
#include<math.h>
#include<iomanip>
#include "utils.h"

#include "../config.h"

using namespace std;

//Total array size for training and testing
const int sizet = 800;
//Size of data in class1
const int sizec1 = 400;
//Size of data in class2, sum of sizec1 and sizec2 should give sizet
const int sizec2 = 400;

//Arrays that hold all data for training
VIP_ENCDOUBLE datax[sizet];
VIP_ENCDOUBLE datay[sizet];

//Arrays that hold training data that are in CLASS1
VIP_ENCDOUBLE dataxC1[sizec1];
VIP_ENCDOUBLE datayC1[sizec1];

//Arrays that hold training data that are in CLASS2
VIP_ENCDOUBLE dataxC2[sizec2];
VIP_ENCDOUBLE datayC2[sizec2];

//Arrays that hold testing data that are in CLASS1
VIP_ENCDOUBLE dataxC1test[sizec1];
VIP_ENCDOUBLE datayC1test[sizec1];

//Arrays that hold testing data that are in CLASS2
VIP_ENCDOUBLE dataxC2test[sizec2];
VIP_ENCDOUBLE datayC2test[sizec2];

//Holds the prediction
VIP_ENCINT classPrediction[sizet];

//These are constants that represent the model
VIP_ENCDOUBLE b0=0;
VIP_ENCDOUBLE b1=0;

//These arrays hold the scores of each data, scores are helpful for separating the classes
VIP_ENCDOUBLE scorec1[sizec1];
VIP_ENCDOUBLE scorec2[sizec2];

//This function initializes the test and training data
//It assigns random numbers based on cond1 and cond2 variables
//This parameters allow the initialization to be in an ordered or grouped way
void initializeData(double cond1,double cond2){
		  
	mysrand(10);
	//initialize class1 test data and training data
	for (int i=0; i < sizec1; i++){
		datax[i] = (VIP_ENCDOUBLE)(myrand()%40);
		datay[i] = (VIP_ENCDOUBLE)(cond1*VIP_DEC(datax[i]) + cond2 + myrand()%40);
		datayC1[i] = datay[i];
		dataxC1[i] = datax[i];
			
		dataxC1test[i] = (VIP_ENCDOUBLE)(myrand()%40);
		datayC1test[i] = (VIP_ENCDOUBLE)(cond1*VIP_DEC(dataxC1test[i]) + cond2 + myrand()%40);
	}
	//initialize class2 test data and training data
	for (int i=0; i < sizec2; i++){
		datax[i+sizec1-1] = (VIP_ENCDOUBLE)(myrand()%40);
		datay[i+sizec1-1] = (VIP_ENCDOUBLE)(cond1*VIP_DEC(datax[i+sizec1-1]) + cond2 - myrand()%40);
		datayC2[i] = datay[i+sizec1-1];
		dataxC2[i] = datax[i+sizec1-1];
			
		dataxC2test[i] = (VIP_ENCDOUBLE)(myrand()%40);
		datayC2test[i] = (VIP_ENCDOUBLE)(cond1*VIP_DEC(dataxC2test[i]) + cond2 - myrand()%40);
	}	    
}

//calculates the mean of an array
VIP_ENCDOUBLE mean(VIP_ENCDOUBLE* data,int size){

	VIP_ENCDOUBLE sum = (VIP_ENCDOUBLE)0;
	
	for (int i=0;i<size;i++){
		sum = (VIP_ENCDOUBLE)(VIP_DEC(sum)+VIP_DEC(data[i]));
	}
	return (VIP_ENCDOUBLE)(VIP_DEC(sum)/size);
}

//calculates the variance of an array
VIP_ENCDOUBLE varaince(VIP_ENCDOUBLE data[],VIP_ENCDOUBLE mean,int size){

	VIP_ENCDOUBLE sum = (VIP_ENCDOUBLE)0;
	VIP_ENCDOUBLE dev = (VIP_ENCDOUBLE)0;
	
	for(int i = 0;i<size;i++){			
		dev = (VIP_ENCDOUBLE)(VIP_DEC(data[i]) - VIP_DEC(mean));
		sum = (VIP_ENCDOUBLE)(VIP_DEC(sum)+pow(VIP_DEC(dev),2));
	}		
	return (VIP_ENCDOUBLE)(VIP_DEC(sum)/size);
}

//calculates the covariance between two arrays
VIP_ENCDOUBLE covariance (VIP_ENCDOUBLE datax[],VIP_ENCDOUBLE datay[],VIP_ENCDOUBLE meanx,VIP_ENCDOUBLE meany,int size){

	VIP_ENCDOUBLE sum = (VIP_ENCDOUBLE)0;
	VIP_ENCDOUBLE devx = (VIP_ENCDOUBLE)0;
	VIP_ENCDOUBLE devy = (VIP_ENCDOUBLE)0;
	
	for(int i=0 ;i<size;i++){
		devx = (VIP_ENCDOUBLE)(VIP_DEC(datax[i])-VIP_DEC(meanx));
		devy = (VIP_ENCDOUBLE)(VIP_DEC(datay[i])-VIP_DEC(meany));
		sum = (VIP_ENCDOUBLE)(VIP_DEC(sum)+VIP_DEC(devx)*VIP_DEC(devy));
	}		
	return VIP_ENCDOUBLE(VIP_DEC(sum)/size);	
}

//Builds the variance-covaraince matrix between two arrays
VIP_ENCDOUBLE** varcovmatrix(VIP_ENCDOUBLE datax[],VIP_ENCDOUBLE datay[],int size){	
	VIP_ENCDOUBLE meanx = mean(datax,size);	
	VIP_ENCDOUBLE meany = mean(datay,size);
	VIP_ENCDOUBLE varx = varaince(datax,meanx,size);
	VIP_ENCDOUBLE vary = varaince(datay,meany,size);
	VIP_ENCDOUBLE cov = covariance(datax,datay,meanx,meany,size);

	VIP_ENCDOUBLE** out = new VIP_ENCDOUBLE*[2];
	out[0] = new VIP_ENCDOUBLE [2];
	out[1] = new VIP_ENCDOUBLE [2];
	out[0][0] = varx;
	out[0][1] = cov;
	out[1][0] = cov;
	out[1][1] = vary;
	return out;
}

//calculates the within group covaraince, which is like the average between two variance-covaraince matrix
VIP_ENCDOUBLE** wgcov(VIP_ENCDOUBLE dataxc1[],VIP_ENCDOUBLE datayc1[],VIP_ENCDOUBLE dataxc2[],VIP_ENCDOUBLE datayc2[],int sizec1,int sizec2){	
	
	//var-cov matrix of class1
	VIP_ENCDOUBLE** c1 = varcovmatrix(dataxc1,datayc1,sizec1);		
	//var-cov matric of class2
	VIP_ENCDOUBLE** c2 = varcovmatrix(dataxc2,datayc2,sizec2);

	VIP_ENCDOUBLE** out = new VIP_ENCDOUBLE*[2];
	out[0] = new VIP_ENCDOUBLE [2];
	out[1] = new VIP_ENCDOUBLE [2];

	for(int i = 0;i<2;i++){
		for(int j = 0;j<2;j++){
			out[i][j] = (VIP_ENCDOUBLE)((VIP_DEC(c1[i][j])*(sizec1-1)+VIP_DEC(c2[i][j])*(sizec2-1))/(sizec1+sizec2-2));
		}
	}

	return out;
}

//calculates the between group covairance, which is the difference between the total var-cov matrix
//and the within-group covaraince
VIP_ENCDOUBLE** bgcov(VIP_ENCDOUBLE datax[],VIP_ENCDOUBLE datay[],VIP_ENCDOUBLE** wgcova,int size){
	
	//var-cov matrix of the overall data	
	VIP_ENCDOUBLE** totcov = varcovmatrix(datax,datay,size);	

	VIP_ENCDOUBLE** out = new VIP_ENCDOUBLE*[2];
	out[0] = new VIP_ENCDOUBLE [2];
	out[1] = new VIP_ENCDOUBLE [2];

	for(int i = 0;i<2;i++){
		for(int j = 0;j<2;j++){
			out[i][j] = (VIP_ENCDOUBLE)(VIP_DEC(totcov[i][j])-VIP_DEC(wgcova[i][j]));
		}
	}

	return out;
}

//calculates the determinant of 2x2 matrix
VIP_ENCDOUBLE determinant(VIP_ENCDOUBLE** matr){
	return (VIP_ENCDOUBLE)(VIP_DEC(matr[0][0])*VIP_DEC(matr[1][1]) - VIP_DEC(matr[0][1])*VIP_DEC(matr[1][0]));
}

//calculates the inverse of a 2x2 matrix
VIP_ENCDOUBLE** inverse(VIP_ENCDOUBLE** matr){
	
	VIP_ENCDOUBLE** out = new VIP_ENCDOUBLE*[2];
	out[0] = new VIP_ENCDOUBLE [2];
	out[1] = new VIP_ENCDOUBLE [2];

	out[0][0] = (VIP_ENCDOUBLE)(VIP_DEC(matr[1][1])/VIP_DEC(determinant(matr)));
	out[0][1] = (VIP_ENCDOUBLE)(-VIP_DEC(matr[0][1])/VIP_DEC(determinant(matr)));
	out[1][0] = (VIP_ENCDOUBLE)(-VIP_DEC(matr[1][0])/VIP_DEC(determinant(matr)));
	out[1][1] = (VIP_ENCDOUBLE)(VIP_DEC(matr[0][0])/VIP_DEC(determinant(matr)));

	return out;
}

//multiplies 2 matrices with equal size
VIP_ENCDOUBLE** multiply(VIP_ENCDOUBLE** matr1,VIP_ENCDOUBLE** matr2){
	
	VIP_ENCDOUBLE** out = new VIP_ENCDOUBLE*[2];
	out[0] = new VIP_ENCDOUBLE [2];
	out[1] = new VIP_ENCDOUBLE [2];
	
	for(int i = 0; i < 2; ++i){
		for(int j = 0; j < 2; ++j){
			out[i][j] = (VIP_ENCDOUBLE)0;
			for(int k = 0; k < 2; ++k){
				out[i][j] = (VIP_ENCDOUBLE)(VIP_DEC(out[i][j])+VIP_DEC(matr1[i][k])*VIP_DEC(matr2[k][j]));
			}
		}
	}
	return out;
}

//calculates the eignevalue of a 2x2 matrix
VIP_ENCDOUBLE* eignevalue(VIP_ENCDOUBLE** matr){
	
	int a = 1;
	VIP_ENCDOUBLE b = (VIP_ENCDOUBLE)(-(VIP_DEC(matr[0][0])+VIP_DEC(matr[1][1])));	
	VIP_ENCDOUBLE c = (VIP_ENCDOUBLE)(VIP_DEC(matr[0][0])*VIP_DEC(matr[1][1]) - VIP_DEC(matr[0][1])*VIP_DEC(matr[1][0]));	

	VIP_ENCDOUBLE* out = new VIP_ENCDOUBLE[2];
	out[0] = (VIP_ENCDOUBLE)((-VIP_DEC(b)+sqrt(pow(VIP_DEC(b),2)-4*a*VIP_DEC(c)))/(2*a));
	out[1] = (VIP_ENCDOUBLE)((-VIP_DEC(b)-sqrt(pow(VIP_DEC(b),2)-4*a*VIP_DEC(c)))/(2*a));
	
	return out;
}

//calculates an eignevector given an eignevalue and a 2x2 matrix
VIP_ENCDOUBLE* eignevector(VIP_ENCDOUBLE** matr, VIP_ENCDOUBLE eigneval){
	VIP_ENCDOUBLE* out = new VIP_ENCDOUBLE[2];
	out[0] = (VIP_ENCDOUBLE)(VIP_DEC(matr[0][1])/sqrt(pow((VIP_DEC(matr[0][0])-VIP_DEC(eigneval)),2)+pow(VIP_DEC(matr[0][1]),2)));
	out[1] = VIP_ENCDOUBLE(-(VIP_DEC(matr[0][0])-VIP_DEC(eigneval))/sqrt(pow((VIP_DEC(matr[0][0])-VIP_DEC(eigneval)),2)+pow(VIP_DEC(matr[0][1]),2)));
	return out;
}

//calculates the scores of the data base on the model variables b0 and b1
void calcScore(){
	for(int i=0;i<sizec1;i++){
		scorec1[i] = (VIP_ENCDOUBLE)(VIP_DEC(b0)*VIP_DEC(dataxC1[i])+VIP_DEC(b1)*VIP_DEC(datayC1[i]));
	}

	for(int i=0;i<sizec2;i++){
		scorec2[i] = (VIP_ENCDOUBLE)(VIP_DEC(b0)*VIP_DEC(dataxC2[i])+VIP_DEC(b1)*VIP_DEC(datayC2[i]));
	}
}

//normalizes the model variables bo and b1 so that the variance of the overall score is 1
void normalizeCoff(){
	calcScore();	
	VIP_ENCDOUBLE vars1 = varaince(scorec1,mean(scorec1,sizec1),sizec1);
	VIP_ENCDOUBLE vars2 = varaince(scorec2,mean(scorec2,sizec2),sizec2);
	
	VIP_ENCDOUBLE pooledvar = (VIP_ENCDOUBLE)(((sizec1-1)*VIP_ENCDOUBLE(vars1)+(sizec2-1)*VIP_DEC(vars2))/(sizec1+sizec2-2));

	b0 = (VIP_ENCDOUBLE)(VIP_DEC(b0)/sqrt(VIP_DEC(pooledvar)));
	b1 = (VIP_ENCDOUBLE)(VIP_DEC(b1)/sqrt(VIP_DEC(pooledvar)));			
}

//displays the output of the training stage and the testing stage
void displayOutput(int size){
	
	cout<<"The LDA/Score will have a function of LDA = "<<VIP_DEC(b0)<<"*(x-mean(datax)) + "<<VIP_DEC(b1)<<"*(y-mean(datay))\n"<<endl;
	cout<<"The classes will be separated by score with 0 as the middle,\nso scores with positive values are probably in one class while negative scores are in the other are separated by scores above zero and scores below zero\n";
	
	cout<<"\n//////////////////TRAINING RESULT////////////////////\n";
	cout<<"DataX"<<"\t"<<"Datay"<<"\t"<<"Score"<<"\t"<<"Class"<<endl;

	for(int i = 0;i<sizec1;i++){		
		cout<<setprecision(3)<<VIP_DEC(datax[i])<<"\t"<<VIP_DEC(datay[i])<<"\t"<<VIP_DEC(scorec1[i])<<"\t"<<"CLASS1"<<endl;		
	}
	for(int i = 0;i<sizec2;i++){
		cout<<setprecision(3)<<VIP_DEC(datax[i+sizec1-1])<<"\t"<<VIP_DEC(datay[i+sizec1-1])<<"\t"<<VIP_DEC(scorec2[i])<<"\t"<<"CLASS2"<<endl;
	}
	
	cout<<"\n//////////////////TESTING RESULT////////////////////\n";
	cout<<"DataX"<<"\t"<<"Datay"<<"\t"<<"CLASS"<<"\t"<<"Prediction"<<endl;
	
	for(int i = 0;i<sizec1;i++){		
		cout<<setprecision(3)<<VIP_DEC(dataxC1test[i])<<"\t"<<VIP_DEC(datayC1test[i])<<"\t"<<"CLASS1";
		cout<<"\t"<<"CLASS"<<VIP_DEC(classPrediction[i])<<endl;
		
	}
	for(int i = 0;i<sizec2;i++){		
		cout<<setprecision(3)<<VIP_DEC(dataxC2test[i])<<"\t"<<VIP_DEC(datayC2test[i])<<"\t"<<"CLASS2";
		cout<<"\t"<<"CLASS"<<VIP_DEC(classPrediction[i+sizec1-1])<<endl;
	}
}
int main(void){	
	
	initializeData(3,4);		
	VIP_ENCDOUBLE meanx = mean(datax,sizet);
	VIP_ENCDOUBLE meany = mean(datay,sizet);
	
	//This variable tells the model which path to take
	//Path 1 tells that positive score means CLASS1 and negative means CLASS2
	//Path 2 tells that positive score means CLASS2 and negative score means CLASS1
	VIP_ENCINT path = (VIP_ENCINT)0;
	{		
		Stopwatch start("VIP Bench Runtime for Train");		
		
		VIP_ENCDOUBLE** wg = wgcov(dataxC1,datayC1,dataxC2,datayC2,sizec1,sizec2);
		VIP_ENCDOUBLE** bg = bgcov(datax,datay,wg,sizet);			
		
		//This matrix hold the ratio of the between group and within group covariance
		//Ideally we want this value to be maximum to have great separation
		VIP_ENCDOUBLE** s = multiply(inverse(wg),bg);
		
		VIP_ENCDOUBLE* e = eignevalue(s);		
		VIP_ENCDOUBLE* ev = eignevector(s,e[0]);
		
		b0=ev[0];
		b1=ev[1];		
	
		normalizeCoff();		
		
		//Holds the amount of positive scores in class1 while training
		VIP_ENCINT c1pos = (VIP_ENCINT)0;
		//Holds the amount of positive scores in class2 while training
		VIP_ENCINT c2pos = (VIP_ENCINT)0;
		
		#ifndef VIP_NA_MODE
			VIP_ENCBOOL condition = (VIP_ENCBOOL)false;
			VIP_ENCINT increment=VIP_ENCINT(0);
		#endif
		
		for(int i=0;i<sizec1;i++){
			scorec1[i] = (VIP_ENCDOUBLE)(VIP_DEC(b0)*(VIP_DEC(dataxC1[i])-VIP_DEC(meanx))+VIP_DEC(b1)*(VIP_DEC(datayC1[i])-VIP_DEC(meany)));
			#ifdef VIP_NA_MODE
				if(scorec1[i]>=0){
					c1pos++;				
				}
			#else
				condition = (VIP_ENCBOOL)(VIP_DEC(scorec1[i])>=0);
				increment = (VIP_ENCINT)(VIP_DEC(c1pos)+1);
				c1pos = VIP_CMOV(condition, increment, c1pos);
			#endif
		}

		for(int i=0;i<sizec2;i++){
			scorec2[i] = (VIP_ENCDOUBLE)(VIP_DEC(b0)*(VIP_DEC(dataxC2[i])-VIP_DEC(meanx))+VIP_DEC(b1)*(VIP_DEC(datayC2[i])-VIP_DEC(meany)));
			#ifdef VIP_NA_MODE
				if(scorec2[i]>=0){
					c2pos++;				
				}
			#else
				condition = (VIP_ENCBOOL)(VIP_DEC(scorec2[i])>=0);
				increment = (VIP_ENCINT)(VIP_DEC(c2pos)+1);
				c2pos = VIP_CMOV(condition, increment, c2pos);
			#endif
		}
		#ifdef VIP_NA_MODE
			if(c1pos<c2pos){
				path = 2;		
			}else
				path = 1;
		#else
			condition = (VIP_ENCBOOL)(VIP_DEC(c1pos)<VIP_DEC(c2pos));
			path = VIP_CMOV(condition,(VIP_ENCINT)2,(VIP_ENCINT)1);
		#endif
		
	}
	{
		Stopwatch start("VIP Bench Runtime for Test");
	
		#ifndef VIP_NA_MODE
			VIP_ENCBOOL condition = (VIP_ENCBOOL)false;			
		#endif
		
		for(int i = 0;i<sizec1;i++){			
			VIP_ENCDOUBLE score = (VIP_ENCDOUBLE)(VIP_DEC(b0)*(VIP_DEC(dataxC1test[i])-VIP_DEC(meanx))+VIP_DEC(b1)*(VIP_DEC(datayC1test[i])-VIP_DEC(meany)));	
			#ifdef VIP_NA_MODE				
				if(score>=0 && path ==1){
					classPrediction[i] = 1;					
				}else if(score<0 && path ==2){
					classPrediction[i] = 1;				
				}else{
					classPrediction[i] = 2;					
				}
			#else
				condition = (VIP_ENCBOOL)(VIP_DEC(score)>=0 && VIP_DEC(path) ==1) || (VIP_DEC(score)<0 && VIP_DEC(path) ==2);
				classPrediction[i] = VIP_CMOV(condition,VIP_ENCINT(1),VIP_ENCINT(2));
			#endif			
		}for(int i = 0;i<sizec2;i++){			
			VIP_ENCDOUBLE score = (VIP_ENCDOUBLE)(VIP_DEC(b0)*(VIP_DEC(dataxC2test[i])-VIP_DEC(meanx))+VIP_DEC(b1)*(VIP_DEC(datayC2test[i])-VIP_DEC(meany)));				
			#ifdef VIP_NA_MODE
				if(score>=0 && path ==1){
					classPrediction[i+sizec1-1] = 1;					
				}else if(score<0 && path ==2){
					classPrediction[i+sizec1-1] = 1;				
				}else{
					classPrediction[i+sizec1-1] = 2;					
				}
			#else
				condition = VIP_ENCBOOL((VIP_DEC(score)>=0 && VIP_DEC(path) ==1) || (VIP_DEC(score)<0 && VIP_DEC(path) ==2));
				classPrediction[i+sizec1-1] = VIP_CMOV(condition,(VIP_ENCINT)1,(VIP_ENCINT)2);
			#endif				
		}		
	}
	
	displayOutput(sizet);
	
}
