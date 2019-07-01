#include <vector>
#include <deque>
#include <algorithm> 
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <numeric>
#include "function.h"
#include <boost/random.hpp>
using namespace std;

#define PI 3.141592
#define MAXFunc 18
#define MAXREP  30


// the pointer to each problem;
double (*func_Fitness)(vector <double> x);
double (*func)(vector <double> x);

// for HS algorithm
void InitialParms();
void Initialization();
void Evolve();
void CheckHSM();
void FitnessProblemSpecific();
void ProblemSpecific();
void Statistics();
void QKSort(vector <int> &pw,vector <int> & order,int m, int n);// ordered according to no-increasing pw[i];
void QKSort(vector <double> &pw,vector <int> & order,int m, int n);
void main_MHS();

// parameters of problems.
double UBi[]={100,10,30,100,100,500,5.12,32,600,5,100,100,100,5,100,600,5,100,500,500};
double LBi[]={-100,-10,-30,-100,-100,-500,-5.12,-32,-600,-5,-100,-100,-100,-5,-100,0,-5,-100,-500,-500};
int Dimension[]={100,100,100,100,100,100,100,100,100,2,100,100,100,100,50,50,50,50,100,100};
int FuncNo=0;

// variables of Harmony search algorithm
vector <vector <double> > Harmonies;
vector <double> HarmonyValue;
int SubHMS,HMS,NI;
double HMCR, PAR, BW, bestsofar;
// records for statistics
double FitArray[MAXFunc][MAXREP];
double BWmax,BWmin;
int DIM=0;
double MHS_delta[4]={0.1,0.5,0.8,0.8};
int Nhm,N1,N2;// for MHS: Modified HS by YM Cheng.
vector <double> Pr;
vector <double> ST;
int MHSFEs=0;

void main_HSDS(void);

void main(void)
{
//	main_HSDS();
	main_MHS();
}

void main_HSDS(void)// Local best HS with Dynamic Subpopulations
{	
	for(DIM=30;DIM<51;DIM+=20)
	{
		printf("\n--------DIM=%d-----**",DIM);

		FILE *f;
		fopen_s(&f,"DLHS.txt","a");
		fprintf(f,"\n DIM=%d,",DIM);
		fclose(f);

		fopen_s(&f,"avg.txt","a");
		fprintf(f,"\n");
		fclose(f);

		HMS=9;
		SubHMS=3;
		for(FuncNo=0; FuncNo<MAXFunc; FuncNo++)
		{
			double avg=0,avgF=0;
			FitnessProblemSpecific();				
			ProblemSpecific();

			FILE *f;
			fopen_s(&f,"DLHS.txt","a");
			fprintf(f,"\n");
			fclose(f);

			for(int rep=0;rep<MAXREP;rep++)
			{
				srand(100+rep);
				InitialParms();				
				Initialization();
				Evolve(); 
				CheckHSM();
				bestsofar=*min_element(HarmonyValue.begin(),HarmonyValue.end());

				double minfit=HarmonyValue[0];
				int pt=0;
				for(unsigned int xx=1;xx<HarmonyValue.size();xx++)
				{
					if(HarmonyValue[xx]<minfit)
					{
						minfit=HarmonyValue[xx];
						pt=xx;
					}
				}

				double tempvalF=func_Fitness(Harmonies[pt]);	
				if(tempvalF!=bestsofar) printf("\n---------erro!-----------%f,%f",tempvalF,bestsofar);
				double tempval=func(Harmonies[pt]);
				avgF+=tempvalF;
				avg+=tempval;	
				FitArray[FuncNo][rep]=tempval;

				fopen_s(&f,"DLHS.txt","a");
				fprintf(f,"%e,",tempval);
				fclose(f);
			}

			avg/=MAXREP;
			avgF/=MAXREP;
			printf("\n----Func=%2d, avgF=%e avg=%e\n", FuncNo, avgF, avg);				

			fopen_s(&f,"avg.txt","a");
			fprintf(f,"%e,", avgF);
			fclose(f);
		}

		printf("\n--------DIM=%d-----",DIM);
	}
}

void FitnessProblemSpecific()
{
	switch(FuncNo)
	{
	case 0:	func_Fitness=abs_function_sphere; break;
	case 1:	func_Fitness=abs_function_schwefel; break;
	case 2:	func_Fitness=abs_function_Rosenbrock; break;
	case 3:	func_Fitness=abs_function_step; break;
	case 4:	func_Fitness=abs_function_Rotated_hyper_ellipsoid; break;
	case 5:	func_Fitness=abs_function_Generalized_swefel; break;
	case 6:	func_Fitness=abs_function_Rastrign; break;
	case 7:	func_Fitness=abs_function_Ackley; break;
	case 8:	func_Fitness=abs_function_Griewank; break;
	case 9:	func_Fitness=abs_function_Six_Hump_Camel_Back; break;
	case 10:func_Fitness=abs_function_shift_sphere;break;
	case 11:func_Fitness=abs_function_shift_schwefel;break;
	case 12:func_Fitness=abs_function_shift_rosenbrock;break;
	case 13:func_Fitness=abs_function_shift_rastrigin;break;
	case 14:func_Fitness=abs_function_shifted_rotated_high_conditioned_elliptic;break;
	case 15:func_Fitness=abs_function_shift_Rotated_Griewans;break;
	case 16:func_Fitness=abs_function_shifted_rotated_rastrign;break;
	case 17:func_Fitness=abs_function_shifted_Rotated_Expanded_Scaffer;break;
	case 18:func_Fitness=abs_function_shift_Generalized_swefel;break;
	case 19:func_Fitness=abs_function_shift_Rorated_Generalized_swefel;break;
	default:func_Fitness=NULL;break;		
	}
}

void ProblemSpecific()
{
	switch(FuncNo)
	{
	case 0:	func=function_sphere; break;
	case 1:	func=function_schwefel; break;
	case 2:	func=function_Rosenbrock; break;
	case 3:	func=function_step; break;	
	case 4:	func=function_Rotated_hyper_ellipsoid; break;
	case 5:	func=function_Generalized_swefel; break;
	case 6:	func=function_Rastrign; break;
	case 7:	func=function_Ackley; break;
	case 8:	func=function_Griewank; break;
	case 9:	func=function_Six_Hump_Camel_Back; break;
	case 10:func=function_shift_sphere;break;
	case 11:func=function_shift_schwefel;break;
	case 12:func=function_shift_rosenbrock;break;
	case 13:func=function_shift_rastrigin;break;
	case 14:func=function_shifted_rotated_high_conditioned_elliptic;break;
	case 15:func=function_shift_Rotated_Griewans;break;
	case 16:func=function_shifted_rotated_rastrign;break;
	case 17:func=function_shifted_Rotated_Expanded_Scaffer;break;
	case 18:func=function_shift_Generalized_swefel;break;
	case 19:func=function_shift_Rorated_Generalized_swefel;break;
	default:func=NULL;break;		
	}
}

void InitialParms()
{
	if(FuncNo!=9) 
	{
		Dimension[FuncNo]=DIM;
	}
	else
		Dimension[FuncNo]=2;

	if(DIM==30)
		NI=30000;
	else 
		NI=50000;
	
	//BWmax=1.0/100*(UBi[FuncNo]-LBi[FuncNo]);
	BWmax=1.0/200*(UBi[FuncNo]-LBi[FuncNo]);
	BWmin=1.0/10000;
}

void InitialParms_MHS()
{
	if(FuncNo!=9) 
	{
		Dimension[FuncNo]=DIM;
	}
	else
		Dimension[FuncNo]=2;

	HMS=2*Dimension[FuncNo];
	Nhm=int(HMS*0.1);
	if(Nhm<10)
		Nhm=10;
	N1=500;
	N2=200;
	HMCR=0.98;
	PAR=0.1;
	
	if(DIM==30)
		NI=30000;
	else 
		NI=50000;
}

void Initialization_MHS()
{
	vector <vector <double>> TempHarmonies;
	vector <double> TempHValue;
	vector <int> order;
	Harmonies.clear();
	vector <double> temphm;

	for(int j=0;j<HMS;j++)
	{
		for(int i=0;i<Dimension[FuncNo];i++)
		{
			double tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*rand()/RAND_MAX;
			temphm.push_back(tempval);
		}
		TempHarmonies.push_back(temphm);		
		temphm.clear();
	}
	
	// Get the value of each harmony
	TempHValue.clear();
	for(int i=0;i<HMS;i++)
	{
		TempHValue.push_back(-(*func_Fitness)(TempHarmonies[i]));
		order.push_back(i);
	}
	
	// Fill the harmony memory
	QKSort(TempHValue,order,0,HMS-1);	
	Harmonies.clear();
	HarmonyValue.clear();
	for(int i=0;i<HMS;i++)
	{
		Harmonies.push_back(TempHarmonies[order[i]]);
		HarmonyValue.push_back(-TempHValue[i]);
	}

	
	for(int i=0;i<HMS;i++)
	{
		double temp=(*func_Fitness)(Harmonies[i]);
		if(temp!=HarmonyValue[i])
			printf("---------");
	}
}

void Gerneration_MHS()
{
	//----------------one Geeration-----------------
	vector <vector <double>> newHarmonies;
	vector <double> newHSV;
	
	// produce Nhm new harmonies;
	for(int k=0;k<Nhm;k++)
	{
		// produce a coarse harmony
		vector <double> nhs;
		for(int i=0;i<Dimension[FuncNo];i++)
		{
			double r1=rand()/(double)RAND_MAX;
			double tempval;	
			if(r1<HMCR)
			{
				double r2=rand()/(double)RAND_MAX*ST[HMS-1];
				int j=0;		
				while(r2>ST[j])
				{
					j++;			
				}
				tempval=Harmonies[j][i];	
			}
			else
				tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*rand()/RAND_MAX;
			nhs.push_back(tempval);
		}

		// fine-funed;
		for(int i=0;i<Dimension[FuncNo];i++)
		{
			double r1=rand()/(double)RAND_MAX;
			if(r1<PAR)
			{
				double r2=rand()/(double)RAND_MAX;
				if(r2<=0.5)
				{
					nhs[i]=nhs[i]-(nhs[i]-LBi[FuncNo])*rand()/RAND_MAX;
				//	nhs[i]=nhs[i]-0.01*rand()/RAND_MAX;
				}
				else
				{
					nhs[i]=nhs[i]+(UBi[FuncNo]-nhs[i])*rand()/RAND_MAX;
				//	nhs[i]=nhs[i]+0.01*rand()/RAND_MAX;
				}
			}
		}
		newHarmonies.push_back(nhs);
		newHSV.push_back((*func_Fitness)(nhs));
		MHSFEs++;
	}

	// update harmony memory
	vector <vector <double>> Harmonypool;
	vector <double> HVpool;
	vector <int> order;

	Harmonypool=Harmonies;
	HVpool=HarmonyValue;
	for(int k=0;k<Nhm;k++)
	{
		HVpool.push_back(newHSV[k]);
		Harmonypool.push_back(newHarmonies[k]);
	}
	
	for(unsigned int k=0;k<HVpool.size();k++)
	{
		order.push_back(k);
		HVpool[k]=-HVpool[k];
	}
	QKSort(HVpool,order,0,order.size()-1);

	for(int i=0;i<HMS;i++)
	{
		HarmonyValue[i]=-HVpool[i];
		Harmonies[i]=Harmonypool[order[i]];
	}
	
	for(int i=0;i<HMS;i++)
	{
		if(HarmonyValue[i]!=(*func_Fitness)(Harmonies[i])) printf("\n--------");
	}

	//-----------End of the generation-----------------------	

}


void Evolve_MHS()
{
	MHSFEs=HMS;	
	
	double vmin,vmax;
	vmin=LBi[FuncNo];
	vmax=UBi[FuncNo];

	for(int rep=0;rep<4;rep++)
	{		
		int Gens,Nit=0;
		double fb=(double)INT_MAX;
		Pr.clear();
		ST.clear();

		//Fill Pr and ST.
		double sum=0;
		for(int i=0;i<HMS;i++)
		{
			double temp=MHS_delta[rep]*pow((1-MHS_delta[rep]),i);
			Pr.push_back(temp);
			sum+=temp;
			ST.push_back(sum);
		}

		Gens=N1;
		Nit=0;
		while(true)
		{
			while(Nit<Gens)
			{
				Nit++;
				Gerneration_MHS();
				if(MHSFEs>NI) goto label1;
			}

			double diff=fabs(HarmonyValue[0]-fb);
			if(diff>0.0001)
			{
				Gens+=N2;
				fb=HarmonyValue[0];
			}
			else
				break;
		}


		//randomly produce HMS-1 harmonies;
		if(rep<3)
		{
			for(int i=1;i<HMS;i++)
			{
				for(int j=0;j<Dimension[FuncNo];j++)
					Harmonies[i][j]=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*rand()/RAND_MAX;
				HarmonyValue[i]=(*func_Fitness)(Harmonies[i]);	
				MHSFEs++;
			}

			if(MHSFEs>NI) goto label1;
		}
		else
		{
			double bmin=Harmonies[0][0]-(UBi[FuncNo]-LBi[FuncNo])*0.1;
			double bmax=Harmonies[0][0]+(UBi[FuncNo]-LBi[FuncNo])*0.1;
			
			LBi[FuncNo]=bmin;
			UBi[FuncNo]=bmax;

			for(int i=1;i<HMS;i++)
			{
				for(int j=0;j<Dimension[FuncNo];j++)
					Harmonies[i][j]=bmin+(bmax-bmin)*rand()/RAND_MAX;
				HarmonyValue[i]=(*func_Fitness)(Harmonies[i]);
				MHSFEs++;
			}
			if(MHSFEs>NI) goto label1;
		}
	}

label1:
	LBi[FuncNo]=vmin;
	UBi[FuncNo]=vmax;


	for(int i=0;i<HMS;i++)
	{
		if((*func_Fitness)(Harmonies[i])!=HarmonyValue[i]) printf("\n error!");
	}
	printf("\nMHSFEs=%d,%e",MHSFEs,HarmonyValue[0]);

}

void main_MHS()
{
	for(DIM=30;DIM<51;DIM+=20)
	{
		FILE *f;
		fopen_s(&f,"MHS.txt","a");
		fprintf(f,"\n DIM=%d,",DIM);
		fclose(f);

		fopen_s(&f,"avg.txt","a");
		fprintf(f,"\n");
		fclose(f);

		for(FuncNo=0; FuncNo<MAXFunc; FuncNo++)
		{						
			double avg=0,avgF=0;
			FitnessProblemSpecific();				
			ProblemSpecific();

			FILE *f;
			fopen_s(&f,"MHS.txt","a");
			fprintf(f,"\n");
			fclose(f);

			for(int rep=0;rep<MAXREP;rep++)
			{
				srand(100+rep);
				InitialParms_MHS();			
				Initialization_MHS();
				Evolve_MHS();
				CheckHSM();
				bestsofar=HarmonyValue[0];			
				double tempval=func(Harmonies[0]);
				avgF+=bestsofar;
				avg+=tempval;	
				FitArray[FuncNo][rep]=tempval;
				
				FILE *f;
				fopen_s(&f,"MHS.txt","a");
				fprintf(f,"%e,",tempval);
				fclose(f);
			}

			avg/=MAXREP;
			avgF/=MAXREP;
			printf("\n----Func=%2d, avgF=%f avg=%f\n", FuncNo, avgF, avg);				

			fopen_s(&f,"avg.txt","a");
			fprintf(f,"%e,", avgF);
			fclose(f);			
		}
	}

	FILE *f;
	fopen_s(&f,"avg.txt","a");
	fprintf(f,"\n");
	fclose(f);	
}


void Initialization()
{
	// produce HMS harmonies;
	Harmonies.clear();
	vector <double> temphm;

	for(int j=0;j<HMS;j++)
	{
		for(int i=0;i<Dimension[FuncNo];i++)
		{
			double tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*rand()/RAND_MAX;
			temphm.push_back(tempval);
		}
		Harmonies.push_back(temphm);		
		temphm.clear();
	}
	
	// Get the value of each harmony
	HarmonyValue.clear();
	for(int i=0;i<HMS;i++)
	{
		HarmonyValue.push_back((*func_Fitness)(Harmonies[i]));
	}
}


// check the harmony memory
void CheckHSM()
{
	double temp;
	for(int i=0;i<HMS;i++)
	{
		temp=func_Fitness(Harmonies[i]);
		if(temp!=HarmonyValue[i])
			printf("\n--------error!--%f,%f-------\n",temp,HarmonyValue[i]);
	}
}

int Split(vector <int> &pw,vector <int> &order,int m, int n)
{
	int i,j,k,To;
	int T;
	k=(m+n)/2;
	T=pw[k];
	To=order[k];

	i=m;
	j=n;
	pw[k]=pw[m];
	order[k]=order[m];

	while (i!=j)
	{
		while((pw[j]<=T)&&(i<j))  j=j-1;
		if(i<j)
		{
			pw[i]=pw[j];
			order[i]=order[j];
			i=i+1;
			while((pw[i]>=T)&&(i<j)) { i=i+1;}
			if(i<j)
			{
				pw[j]=pw[i];
				order[j]=order[i];
				j=j-1;
			}
		}
	}
	pw[i]=T;
	order[i]=To;
	return i;
}

void QKSort(vector <int> &pw,vector <int> & order,int m, int n)// ordered according to no-increasing pw[i];
{
	int i;
	if(m<n)
	{
		i=Split(pw,order,m,n);
		QKSort(pw,order,i+1,n);
		QKSort(pw,order,m,i-1);	
	}
}

int Split(vector <double> &pw,vector <int> &order,int m, int n)
{
	int i,j,k,To;
	double T;
	k=(m+n)/2;
	T=pw[k];
	To=order[k];

	i=m;
	j=n;
	pw[k]=pw[m];
	order[k]=order[m];

	while (i!=j)
	{
		while((pw[j]<=T)&&(i<j))  j=j-1;
		if(i<j)
		{
			pw[i]=pw[j];
			order[i]=order[j];
			i=i+1;
			while((pw[i]>=T)&&(i<j)) { i=i+1;}
			if(i<j)
			{
				pw[j]=pw[i];
				order[j]=order[i];
				j=j-1;
			}
		}
	}
	pw[i]=T;
	order[i]=To;
	return i;
}

void QKSort(vector <double> &pw,vector <int> & order,int m, int n)// ordered according to no-increasing pw[i];
{
	int i;
	if(m<n)
	{
		i=Split(pw,order,m,n);
		QKSort(pw,order,i+1,n);
		QKSort(pw,order,m,i-1);	
	}
}

void Evolve()	// For ensamble GHS
{
	double TempBW=BW;
	vector <vector <double>> usedParameterSet, betterParameterSet;
	int Padistance=200;
	int FEs=0;
	int RecordDistance=0;
	int RecordGroup=0,ReGroupLen=50;

	// Initialize used parameter sets
	for(int i=0;i<Padistance;i++)
	{
		vector <double> Parameter;
		Parameter.push_back(0.9+rand()*0.1/RAND_MAX);
		Parameter.push_back(rand()*1.0/RAND_MAX);
		usedParameterSet.push_back(Parameter);
	}		

	// The first 90% FEs
	do
	{	// perform GHS for each group;
		int GroupStart=0, GroupEnd=0;
		while(GroupEnd+SubHMS<=HMS)
		{
			if(RecordDistance>=Padistance)
			{
				RecordDistance=0;
				int len=Padistance*3/4;
				if(betterParameterSet.size()) // update the parameter sets
				{
					//the first 3/4 parater sets come from better parameter sets
					for(int i=0;i<len;i++)
					{
						int j=rand()%betterParameterSet.size();
						usedParameterSet[i][0]=betterParameterSet[j][0];
						usedParameterSet[i][1]=betterParameterSet[j][1];
					}				
					betterParameterSet.clear();

					// the last 1/4 generated randomly
					for(int i=len;i<Padistance;i++)
					{
						usedParameterSet[i][0]=0.9+rand()*0.1/RAND_MAX;
						usedParameterSet[i][1]=rand()*1.0/RAND_MAX;
					}
				}
			}

			// select parameter sets for evolution
			bool bFlag=false;
			HMCR=usedParameterSet[FEs%Padistance][0];
			PAR=usedParameterSet[FEs%Padistance][1];
			if(FEs<NI*3/4)
				TempBW=BWmax-(BWmax-BWmin)*FEs/(NI*3/4);
			else
				TempBW=BWmin;

			GroupStart=GroupEnd;
			GroupEnd=GroupStart+SubHMS;

			//--------Find the best and worst harmonies in the Group memory
			int i, minpt=GroupStart,maxpt=GroupStart; 
			double tempval, minv=HarmonyValue[GroupStart],maxv=HarmonyValue[GroupStart];
			for(i=GroupStart+1;i<GroupEnd;i++)
			{
				if(minv>HarmonyValue[i])
				{
					minv=HarmonyValue[i];
					minpt=i;
				}

				if(maxv<HarmonyValue[i])
				{
					maxv=HarmonyValue[i];
					maxpt=i;
				}
			}

			//-----------improvise a harmony---------
			vector <double> nhm;		
			for(i=0;i<Dimension[FuncNo];i++)
			{
				if(rand()<HMCR*RAND_MAX) //Memory consideration
				{
					double newPAR=1-PAR;					

					if(rand()<PAR*RAND_MAX)
					{
						if(rand()%2)
						{
							tempval=Harmonies[GroupStart+rand()%SubHMS][i]+rand()*TempBW/RAND_MAX;
						}
						else
						{
							tempval=Harmonies[GroupStart+rand()%SubHMS][i]-rand()*TempBW/RAND_MAX;
						}					
					}
					else
					{
						tempval=Harmonies[minpt][i];
					}
				}
				else //Randomly selection 
				{
					tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*rand()/RAND_MAX;
				}									
				nhm.push_back(tempval);
			}


			// update the harmony memory;
			double tv=(*func_Fitness)(nhm);
			if(tv<maxv)
			{
				HarmonyValue[maxpt]=tv;	
				copy(nhm.begin(),nhm.end(),Harmonies[maxpt].begin());
				bFlag=true;
			}
		
			// record the beter parameter set
			if(bFlag)
			{
				vector <double> Parameter;
				Parameter.push_back(HMCR);
				Parameter.push_back(PAR);
				betterParameterSet.push_back(Parameter);
			}

			FEs++;
			RecordDistance++;
		}

		if(RecordGroup>=ReGroupLen)//regroup the subpopulation
		{			
			RecordGroup=0;
			vector <vector <double>> TempHarmonies;
			vector <double> TempValue;
			vector <int> RandPermutation, Order;

			for(unsigned int i=0;i<Harmonies.size();i++)
			{
				TempHarmonies.push_back(Harmonies[i]);
				TempValue.push_back(HarmonyValue[i]);
			}

			for(unsigned int i=0;i<Harmonies.size();i++)
			{
				RandPermutation.push_back(rand());
				Order.push_back(i);
			}
		
			QKSort(RandPermutation,Order,0,Harmonies.size()-1);
			
			for(unsigned int i=0;i<Harmonies.size();i++)
			{
				copy(TempHarmonies[Order[i]].begin(),TempHarmonies[Order[i]].end(),Harmonies[i].begin());
				HarmonyValue[i]=TempValue[Order[i]];
			}
		}
		
		RecordGroup++;

	}while(FEs<int(NI*0.9));

	// the rest 10% FEs is used GHS the parameter is used in usedParameter sets
	//Find the best subHMS vectors.
	vector <vector <double>> TempHarmonies;
	vector <double> TempValue;
	vector <bool> bFlag;

	for(unsigned int i=0;i<Harmonies.size();i++)
	{
		bFlag.push_back(false);
	}

	int number=0;
	while(number<SubHMS)
	{
		double minv=DBL_MAX;
		int pt=-1;
		for(unsigned int i=0;i<Harmonies.size();i++)
		{
			if(bFlag[i]) continue;
			if(HarmonyValue[i]<minv)
			{
				minv=HarmonyValue[i];
				pt=i;
			}
		}
		TempHarmonies.push_back(Harmonies[pt]);
		TempValue.push_back(HarmonyValue[pt]);
		bFlag[pt]=true;
		number++;
	}

	for(unsigned int i=0;i<TempHarmonies.size();i++)
	{
		copy(TempHarmonies[i].begin(),TempHarmonies[i].end(),Harmonies[i].begin());
		HarmonyValue[i]=TempValue[i];
	}

	do{
		// determine the parameters
		HMCR=usedParameterSet[FEs%Padistance][0];
		PAR=usedParameterSet[FEs%Padistance][1];
		TempBW=BWmin;

		//--------Find the best and worst harmonies in the memory
		int i, minpt=0,maxpt=0; 
		double tempval, minv=HarmonyValue[0],maxv=HarmonyValue[0];
		for(i=1;i<SubHMS;i++)
		{
			if(minv>HarmonyValue[i])
			{
				minv=HarmonyValue[i];
				minpt=i;
			}

			if(maxv<HarmonyValue[i])
			{
				maxv=HarmonyValue[i];
				maxpt=i;
			}
		}

		//-----------improvise a harmony---------
		vector <double> nhm;		
		for(i=0;i<Dimension[FuncNo];i++)
		{
			if(rand()<HMCR*RAND_MAX) //Memory consideration
			{
				if(rand()<PAR*RAND_MAX)
				{
					tempval=Harmonies[minpt][i];					
				}
				else
				{
					if(rand()%2)
					{
						tempval=Harmonies[rand()%SubHMS][i]+rand()*TempBW/RAND_MAX;
					}
					else
					{
						tempval=Harmonies[rand()%SubHMS][i]-rand()*TempBW/RAND_MAX;
					}
				}
			}
			else //Randomly selection 
			{
				tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*rand()/RAND_MAX;
			}									
			nhm.push_back(tempval);
		}

		// update the harmony memory;
		double tv=(*func_Fitness)(nhm);			
		if(tv<maxv)
		{
			HarmonyValue[maxpt]=tv;	
			copy(nhm.begin(),nhm.end(),Harmonies[maxpt].begin());
		}
		FEs++;
	}while(FEs<NI);
}



void Statistics()
{
	FILE *fp;
	fopen_s(&fp,"statistic.txt","a");

	fprintf(fp,"\n HMS=%2d, HMCR=%5.3f, PAR=%5.3f, BW=%5.3f\n",HMS,HMCR,BW);
	
	double meanmin=(double)INT_MAX;
	double meanmax=(double)-INT_MAX;
	double meanavg=0;
	double meanSD=0;

	for(FuncNo=0;FuncNo<MAXFunc; FuncNo++)
	{
	//	fprintf(fp,"\n");
		double avg=0,min,max,SD;
		int rep;
		min=max=FitArray[FuncNo][0];
		for(int rep=0;rep<MAXREP;rep++)
		{
			//	fprintf(fp,"%f",FitArray[FuncNo][rep]);
			avg+=FitArray[FuncNo][rep];
			if(min>FitArray[FuncNo][rep])
				min=FitArray[FuncNo][rep];
			if(max<FitArray[FuncNo][rep])
				max=FitArray[FuncNo][rep];
		}
		avg/=MAXREP;
		
		SD=0;
		for(rep=0;rep<MAXREP;rep++)
		{
			SD+=(FitArray[FuncNo][rep]-avg)*(FitArray[FuncNo][rep]-avg);
		}
		SD=sqrt(SD/(rep-1));

		fprintf(fp,"\n%16.6f",avg);
		fprintf(fp,"\n%16.6f",SD);
		
		meanmin+=min;
		meanavg+=avg;
		meanmax+=max;
		meanSD+=SD;
	}

	meanmin/=FuncNo;
	meanavg/=FuncNo;
	meanmax/=FuncNo;
	meanSD/=FuncNo;
	fprintf(fp,"\n%16.6f,  %16.6f,  %16.6f,  %16.6f",meanavg,meanmin,meanmax,meanSD);
	fclose(fp);
	
	fopen_s(&fp,"data.txt","a");
	fprintf(fp,"\n\nHMCR=%f, PAR=%f",HMCR,PAR);
	for(FuncNo=0;FuncNo<MAXFunc; FuncNo++)
	{
		fprintf(fp,"\n");
		for(int rep=0;rep<MAXREP;rep++)
		{
			fprintf(fp,"%f,",FitArray[FuncNo][rep]);
		}
	}
	fclose(fp);
}

