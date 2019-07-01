 // ABC for FJSP with fuzzy processing time.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "ABC for rescheduling.h"
//#include<StdAfx.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/types.h>
#include <time.h>
#include <iostream>
#include<fstream>

using namespace std;
#define u01 ((rand())/(RAND_MAX+1.0))

#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a>b)?b:a)

#define OPERC 0.05
#define COUNTER 1 

#define PB1 0.3

#define RUN 30

#define Empop 150
#define Onpop 300
#define Scout_NO 20

#define B_T 0.95

#define PNBR 0.8
#define PRTB 0.5

#define PERC 0.2

#define PERC1 0.25
#define PERC2 0.15

#define NJb 100 //Number of job
#define Max_Op 50 //Maximum number of operations in a job
#define T_Oper 2500 //Total number of operations in the schedule
#define Mac 50 //Number of Machine

//struct tms buff;
//int utimer,butimer;
//double utime[RUN],butime[RUN],utime_b4ls[RUN],butime_b4ls[RUN];


int PJ[T_Oper],SJ[T_Oper],PM[T_Oper],SM[T_Oper],CE[T_Oper],CL[T_Oper],SE[T_Oper],SL[T_Oper];
int N,M,toper,Jb[Max_Op],NM_O[NJb][Max_Op],M_O[NJb][Max_Op][Mac],p1[NJb][Max_Op][Mac],p2[NJb][Max_Op][Mac],p3[NJb][Max_Op][Mac]; //job number, machine number,total operation number, operation number, number of machine for each operation, machine set, processing time set
int Op[Onpop][T_Oper][7],MV[Empop][1000],OPT[RUN][3];//job number, operation number,  machine number, processing time 
int F[Empop][3],Optimal[3],Op_seq[T_Oper][7],COUNT = 0;
int Onps[Onpop][T_Oper][7],On_F[Onpop][3],On,On_1,On_2,Onlook[Onpop];
int ReFT[NJb][3],ReMST[Mac][3];  //Machine_Spantime and job spantime
int a[3],b[3],c[3],d[3],e[3],f[3];
int repeattime=4000;
int FT[NJb][3];
int seque[T_Oper][7],FL[3];
//int MachStarEnd[Mac][T_Oper][9],OptMachStarEnd[Mac][T_Oper][9],Count[Mac];
int MachStarEnd[Mac][T_Oper][9],OptMachStarEnd[Mac][T_Oper][9],Count[Mac],CC[Mac];
int GEN=4000,Gen;
int inserttime=0;
int Job_Release_Time[NJb][3];
int Mach_Release_Time[Mac][3];
int JobStarEnd[NJb][T_Oper][7];
int JobStarOper[NJb];
ofstream outfile;
int Compare(int a[3],int b[3])
{
   int retur;
   int sum1,sum2;
   sum1=a[0]+2*a[1]+a[2];
   sum2=b[0]+2*b[1]+b[2];
   if(sum1<sum2)
   {
	   retur=0;
   }
   else
   {
	   if(sum1==sum2)
	   {
		   sum1=a[1];
		   sum2=b[1];
		   {
			   if(sum1<sum2)
			   {
				   retur=0;
			   }
			   else
			   {
				   if(sum1==sum2)
				   {
					   sum1=a[2]-a[0];
					   sum2=b[2]-b[0];
					   if((sum1<sum2))
					   {
						   retur=0;
					   }
					   else
					   {
						   if(sum1==sum2)
						   {
							   retur=-1;
						   }
						   else
						   {
							   retur=1;
						   }
					   }
				   }
				   else
				   {
					   retur=1;
				   }
			   }
		   }
	   }
	   else
	   {
		   retur=1;
	   }
   }
   return retur;
}
void Update_Optimal(int l)
{
	int i;
	Optimal[0] = F[l][0];
	Optimal[1] = F[l][1];
	Optimal[2] = F[l][2];
	for(i=0;i<toper;i++)
	{
        Op_seq[i][0] = Op[l][i][0];//  job number
		Op_seq[i][1] = Op[l][i][1];		 //operation number
		Op_seq[i][2] = Op[l][i][2];	// processing machine
		Op_seq[i][3] = Op[l][i][3];	// min processing time
		Op_seq[i][4] = Op[l][i][4];
		Op_seq[i][5] = Op[l][i][5];	// middle processing time
		Op_seq[i][6] = Op[l][i][6];	// max processing time
	}
	
	//cout<<Optimal[0]<<","<<Optimal[1]<<","<<Optimal[2]<<endl;
	/*FILE *fp;
	fp = fopen("best resuts.txt","a");
	fprintf(fp,"%d \n",Optimal);*/
	//	a[0]=Optimal[0];a[1]=Optimal[1];a[2]=Optimal[2];
	//b[0]=20;	 b[1]=28;  b[2]=40;
	//if((Compare(a,b)==0)/*||(Compare(a,b)==-1)*/)
	//{
	//	outfile.open("bset sequence.csv",ios::app);   	
	//	outfile<<a[0]<<","<<a[1]<<","<<a[2]<<endl;
	//	for(i=0;i<toper;i++)
	//		{
	//			outfile<<Op_seq[i][0]+1<<","<<Op_seq[i][1]+1<<","<<Op_seq[i][2]<<","<<Op_seq[i][3]<<","<<Op_seq[i][5]<<","<<Op_seq[i][6]<<","<<Op_seq[i][4]<<endl;
	//		}
	//	outfile<<"///////////////////////////////////"<<endl;
	//	outfile.close();

	//}
}
void Update_MachStarEndTime()
{
	int i,j;
	for(i=1;i<=M;i++)
	{
		Count[i]=CC[i];
		for(j=1;j<=Count[i];j++)
		{
			for(int k=0;k<9;k++)
			{
			   OptMachStarEnd[i][j][k]=	MachStarEnd[i][j][k];
			}

		}
	}
	for(i=1;i<=M;i++)
	{
		Count[i]=CC[i];
		for(j=1;j<Count[i];j++)
		{
		   if((OptMachStarEnd[i][j][1]==OptMachStarEnd[i][j+1][1])&&(OptMachStarEnd[i][j][2]==OptMachStarEnd[i][j+1][2]))
		   {
			   system("pause");
		   }

		}
	}
}

void Init_pop(int l)  
{
	
	int i,j,z1,r,r1,cc,tmp1[NJb],tmp2[NJb][Max_Op],tmp3,tmp[7],oc[Mac];
	int Min[3],tmp4[Max_Op],TA[Mac][3],ADT[Mac][3];
	if(u01 < PB1)
	{
		for(i=0;i<N;i++)
		{
			tmp1[i] = Jb[i];
			for(j=0;j<Jb[i];j++)
				tmp2[i][j] = 0;
		}
		cc= 0;
		tmp3 = N;
		while(1)
		{
			/*if(p3[0][0][0]!=14)
			{
				system("pause");

			}*/
			do
			{
				r = (int) (u01*N);
			}while(tmp1[r] == 0);

			Op[l][cc][0] = r;

			j = 0;
			do
			{
				Op[l][cc][1] = j;
			}while(tmp2[r][j++] == 1);

			tmp2[r][--j] = 1;

			if(NM_O[r][j] > 1)
			{
				r1 = (int) (u01*NM_O[r][j]); //randomly select a machine

				//////////////////////////////////////////////////////			
				//if(u01 < 0.5)
				//{
				//	r1 = (int) (u01*NM_O[r][j]); //randomly select a machine

					/*Total = 0.0;
					for(i=0;i<NM_O[r][j];i++)
						Total += 1.0/p[r][j][i];
              			
					for(i=0;i<NM_O[r][j];i++)
					{
						if(i==0)
							prob[i] = (double)(1)/(p[r][j][i]*Total);
						else
							prob[i] = prob[i-1] + (double)(1)/(p[r][j][i]*Total);
					}
					r01 = u01;
					r1 = 0;
					while(prob[r1] < r01)
					{
						r1++;
					}*/
				/*}
				else
				{
					Min = 50000;
					for(i=0;i<NM_O[r][j];i++)
					{
						if(Min > p[r][j][i])
							Min = p[r][j][i];
					}
					z1 = 0;
					for(i=0;i<NM_O[r][j];i++)
					{
						if(Min == p[r][j][i])
							tmp4[z1++] = i;
					}
					if(z1 == 1)
						r1 = tmp4[0];
					else
					{
						z2 = (int)(u01*z1);
						r1 = tmp4[z2];
					} 
				}*/
				//////////////////////////////////////////////////////

				Op[l][cc][2] = M_O[r][j][r1];
				Op[l][cc][3] = p1[r][j][r1];
				Op[l][cc][5] = p2[r][j][r1];
				Op[l][cc][6] = p3[r][j][r1];
			}
			else
			{
				Op[l][cc][2] = M_O[r][j][0];
				Op[l][cc][3] = p1[r][j][0];
				Op[l][cc][5] = p2[r][j][0];
				Op[l][cc][6] = p3[r][j][0];
			}
			if(Op[l][cc][0] == 0)
			{
				Op[l][cc][4] = Op[l][cc][1]; //use for giving unique sequence number
			}
			else
			{
				j = 0;
				for(i=0;i<Op[l][cc][0];i++)
					j += Jb[i];
				Op[l][cc][4] = j + Op[l][cc][1];
			}
			cc++;

			if(--tmp1[r] == 0)
			{
				if(--tmp3 == 0)
					break;
			}	  
		/*	if(p3[0][0][0]!=14)
			{
				system("pause");
			}
			cout<<u01<<","<<p3[0][0][0]<<endl;*/
		}
		
	}
	else
	{

		for(i=1;i<=M;i++)
		{
			TA[i][0] = 0;
			TA[i][1] = 0;
			TA[i][2] = 0;
		}
		for(i=0;i<N;i++)
		{
			tmp1[i] = Jb[i];
			for(j=0;j<Jb[i];j++)
				tmp2[i][j] = 0;
		}
		cc = 0;
		tmp3 = N;
		while(1)
		{
			/*if(p3[0][0][0]!=14)
			{
				system("pause");

			}*/
			do
			{
				r = (int) (u01*N);
			}while(tmp1[r] == 0);

			Op[l][cc][0] = r;
			j = 0;
			do
			{
				Op[l][cc][1] = j;
			}while(tmp2[r][j++] == 1);

			tmp2[r][--j] = 1;

			if(NM_O[r][j] > 1)
			{
				for(i=0;i<NM_O[r][j];i++)
				{
					ADT[M_O[r][j][i]][0] = TA[M_O[r][j][i]][0] + p1[r][j][i];
					ADT[M_O[r][j][i]][1] = TA[M_O[r][j][i]][1] + p2[r][j][i];
					ADT[M_O[r][j][i]][2] = TA[M_O[r][j][i]][2] + p3[r][j][i];
				}
				Min[0]=50000; Min[1]=50000; Min[2]=50000;
				
				for(i=0;i<NM_O[r][j];i++)
				{
					b[0]=ADT[M_O[r][j][i]][0]; b[1]=ADT[M_O[r][j][i]][1]; b[2]=ADT[M_O[r][j][i]][2];
					if(Compare(Min,b)==1)
					{
						Min[0] = ADT[M_O[r][j][i]][0];
						Min[1] = ADT[M_O[r][j][i]][1];
						Min[2] = ADT[M_O[r][j][i]][2];
					}
				}
				z1 = 0;
				for(i=0;i<NM_O[r][j];i++)
				{
					b[0]=ADT[M_O[r][j][i]][0]; b[1]=ADT[M_O[r][j][i]][1]; b[2]=ADT[M_O[r][j][i]][2];
					if(Compare(Min,b)==-1)
						tmp4[z1++] = i;
				}
				if(z1 == 1)
					r1 = tmp4[0];//maybe z1 be replace by "0"
				else
				{
					int zz2 = (int)(u01*z1); //tie break
					r1 = tmp4[zz2];
				}
		
				Op[l][cc][2] = M_O[r][j][r1];
				Op[l][cc][3] = p1[r][j][r1];
				Op[l][cc][5] = p2[r][j][r1];
				Op[l][cc][6] = p3[r][j][r1];
				TA[M_O[r][j][r1]][0] = p1[r][j][r1];
				TA[M_O[r][j][r1]][1] = p2[r][j][r1];
				TA[M_O[r][j][r1]][2] = p3[r][j][r1];
			}
			else
			{
				Op[l][cc][2] = M_O[r][j][0];
				Op[l][cc][3] = p1[r][j][0];
				Op[l][cc][5] = p2[r][j][0];
				Op[l][cc][6] = p3[r][j][0];
				TA[M_O[r][j][0]][0] = p1[r][j][0];
				TA[M_O[r][j][0]][1] = p2[r][j][0];
				TA[M_O[r][j][0]][2] = p3[r][j][0];
			}
			if(Op[l][cc][0] == 0)
				Op[l][cc][4] = Op[l][cc][1]; //use for giving unique sequence number
			else
			{
				j = 0;
				for(i=0;i<Op[l][cc][0];i++)
					j += Jb[i];
				Op[l][cc][4] = j + Op[l][cc][1];
			}
			cc++;

			if(--tmp1[r] == 0)
			{
				if(--tmp3 == 0)
					break;
			}	  
			/*if(p3[0][0][0]!=14)
			{
				system("pause");
			}
			cout<<u01<<","<<p3[0][0][0]<<endl;*/
		}
		
	}
//Evaluate makespan
//Left-Shift operator
	/*if(l==0)
	{
	outfile.open("Sequence.csv",ios::app);
	for(i=0;i<toper;i++)
	{
		outfile<<Op[l][i][0]+1<<","<<Op[l][i][1]+1<<","<<Op[l][i][2]<<","<<Op[l][i][3]<<","<<Op[l][i][5]<<","<<Op[l][i][6]<<","<<Op[l][i][4]<<endl;
	}
	outfile<<"********************"<<endl;
			outfile.close();
	}*/
}
void RemainMaxWorkInitOS(int l,int Op[Onpop][T_Oper][7])
{
	int i,j;
	int RW[NJb][3];
	int Job[NJb]; //for random order job 
	int count[NJb];
	//Init RW
	for(i=0;i<N;i++)
	{
		RW[i][0]=0;
		RW[i][1]=0;
		RW[i][2]=0;
		Job[i]=N-i-1;
		count[i]=0;
	}
	//count total processing time for each job
	for(i=0;i<toper;i++)
	{
		RW[Op[l][i][0]][0]+=Op[l][i][3];
		RW[Op[l][i][0]][1]+=Op[l][i][5];
		RW[Op[l][i][0]][2]+=Op[l][i][6];
	}
	//random order job
	for(i=0;i<2*toper;i++)
	{
	   int r = (int) (u01*N);
	   int temp=Job[0];
	   Job[0]=Job[r];
	   Job[r]=temp;
	}
	for(i=0;i<toper;i++)
	{
		a[0]=RW[Job[0]][0];	a[1]=RW[Job[0]][1];	a[2]=RW[Job[0]][2];
		int m=Job[0];
		for(j=0;j<N;j++)
		{
			b[0]=RW[Job[j]][0];	b[1]=RW[Job[j]][1];	b[2]=RW[Job[j]][2];
			if(Compare(a,b)==0)
			{
				 a[0]=b[0];	a[1]=b[1];	a[2]=b[2];
				 m=Job[j];
			}
		}
			if(a[0]>0)
			{
				for(int k=i;k<toper;k++)
				{
					if((Op[l][k][0]==m)&&(Op[l][k][1]==count[m]))
					{
						int tem[7];
						for(int K=0;K<7;K++)
						{
							tem[K]=Op[l][i][K];
							Op[l][i][K]=Op[l][k][K];
							Op[l][k][K]=tem[K];
						}
						count[m]++;
						RW[m][0]-=Op[l][i][3]; RW[m][1]-=Op[l][i][5]; RW[m][0]-=Op[l][i][6];
						break;
					}
				}
			}
	}
	/*for(i=0;i<toper;i++)
			{
				cout<<Op[l][i][0]+1<<","<<Op[l][i][1]+1<<","<<Op[l][i][2]<<","<<Op[l][i][3]<<","<<Op[l][i][5]<<","<<Op[l][i][6]<<","<<Op[l][i][4]<<endl;
			}*/



}
void RemainOperInitOS(int l,int Op[Onpop][T_Oper][7])
{
	int i,j;
	int ROper[NJb];
	int Job[NJb]; //for random order job 
	int count[NJb];
	//Init RW
	for(i=0;i<N;i++)
	{
		ROper[i]=Jb[i];
		Job[i]=N-i-1;
		count[i]=0;
	}
	//random order job
	for(i=0;i<2*toper;i++)
	{
	   int r = (int) (u01*N);
	   int temp=Job[0];
	   Job[0]=Job[r];
	   Job[r]=temp;
	}
	for(i=0;i<toper;i++)
	{
		int max=ROper[Job[0]];
		int m=Job[0];
		for(j=0;j<N;j++)
		{
			if(max<ROper[Job[j]])
			{
				 max=ROper[Job[j]];
				 m=Job[j];
			}
		}
		/*if(m==14)
		{
			system("pause");
		}*/
			if(max>0)
			{
				for(int k=i;k<toper;k++)
				{
					if((Op[l][k][0]==m)&&(Op[l][k][1]==count[m]))
					{
						int tem[7];
						for(int K=0;K<7;K++)
						{
							tem[K]=Op[l][i][K];
							Op[l][i][K]=Op[l][k][K];
							Op[l][k][K]=tem[K];
						}
						count[m]++;
						ROper[m]--;
						break;
					}
				}
			}
	}
	/*for(i=0;i<toper;i++)
			{
				cout<<Op[l][i][0]+1<<","<<Op[l][i][1]+1<<","<<Op[l][i][2]<<","<<Op[l][i][3]<<","<<Op[l][i][5]<<","<<Op[l][i][6]<<","<<Op[l][i][4]<<endl;
			}
*/

}
void LSMinLoad(int l,int Op[Onpop][T_Oper][7])
{
	int i,j;
	int MachWorkLoad[Mac][3];
	int Mach,J;
	int Job[NJb],jj;
	for(i=0;i<N;i++)
	{
		Job[i]=i;
	}
	for(i=0;i<10*N;i++)
	{
		int r1=rand()%N;
		int r2=rand()%N;
		int temp=Job[r1];
		Job[r1]=Job[r2];
		Job[r2]=temp;
	}
	for(jj=0;jj<N;jj++)
	{
		for(i=0;i<M;i++)
		{
			MachWorkLoad[i][0]=0;
			MachWorkLoad[i][1]=0;
			MachWorkLoad[i][2]=0;
		}
		for(i=0;i<toper;i++)
		{
			if(Op[l][i][0]==Job[jj])
			{
				int m=M_O[Op[l][i][0]][Op[l][i][1]][0];
				a[0]=MachWorkLoad[m-1][0]+p1[Op[l][i][0]][Op[l][i][1]][0];
				a[1]=MachWorkLoad[m-1][1]+p2[Op[l][i][0]][Op[l][i][1]][0];
				a[2]=MachWorkLoad[m-1][2]+p3[Op[l][i][0]][Op[l][i][1]][0];
				Mach=m;
				J=0;
				for(j=0;j<NM_O[Op[l][i][0]][Op[l][i][1]];j++)
				{
						int m1=M_O[Op[l][i][0]][Op[l][i][1]][j];
						b[0]=MachWorkLoad[m1-1][0]+p1[Op[l][i][0]][Op[l][i][1]][j];
						b[1]=MachWorkLoad[m1-1][1]+p2[Op[l][i][0]][Op[l][i][1]][j];
						b[2]=MachWorkLoad[m1-1][2]+p3[Op[l][i][0]][Op[l][i][1]][j];
						if(Compare(a,b)==1)
						{
							a[0]=b[0];
							a[1]=b[1];
							a[2]=b[2];
							Mach=m1;
							J=j;
						}
				}
				Op[l][i][2]=Mach;
				Op[l][i][3]=p1[Op[l][i][0]][Op[l][i][1]][J];
				Op[l][i][5]=p2[Op[l][i][0]][Op[l][i][1]][J];
				Op[l][i][6]=p3[Op[l][i][0]][Op[l][i][1]][J];
				MachWorkLoad[Mach-1][0]=a[0];
				MachWorkLoad[Mach-1][1]=a[1];
				MachWorkLoad[Mach-1][2]=a[2];
			}
		}
	}
}
void GSMinLoad(int l,int Op[Onpop][T_Oper][7])
{
	int i,j;
	int MachWorkLoad[Mac][3];
	int Mach,J;
	for(i=0;i<M;i++)
	{
		MachWorkLoad[i][0]=0;
		MachWorkLoad[i][1]=0;
		MachWorkLoad[i][2]=0;
	}
	for(i=0;i<toper;i++)
	{
		int m=M_O[Op[l][i][0]][Op[l][i][1]][0];
		a[0]=MachWorkLoad[m-1][0]+p1[Op[l][i][0]][Op[l][i][1]][0];
		a[1]=MachWorkLoad[m-1][1]+p2[Op[l][i][0]][Op[l][i][1]][0];
		a[2]=MachWorkLoad[m-1][2]+p3[Op[l][i][0]][Op[l][i][1]][0];
		Mach=m;
		J=0;
		for(j=0;j<NM_O[Op[l][i][0]][Op[l][i][1]];j++)
		{
				int m1=M_O[Op[l][i][0]][Op[l][i][1]][j];
				b[0]=MachWorkLoad[m1-1][0]+p1[Op[l][i][0]][Op[l][i][1]][j];
				b[1]=MachWorkLoad[m1-1][1]+p2[Op[l][i][0]][Op[l][i][1]][j];
				b[2]=MachWorkLoad[m1-1][2]+p3[Op[l][i][0]][Op[l][i][1]][j];
				if(Compare(a,b)==1)
				{
					a[0]=b[0];
					a[1]=b[1];
					a[2]=b[2];
					Mach=m1;
					J=j;
				}
		}
		Op[l][i][2]=Mach;
		Op[l][i][3]=p1[Op[l][i][0]][Op[l][i][1]][J];
		Op[l][i][5]=p2[Op[l][i][0]][Op[l][i][1]][J];
		Op[l][i][6]=p3[Op[l][i][0]][Op[l][i][1]][J];
		MachWorkLoad[Mach-1][0]=a[0];
		MachWorkLoad[Mach-1][1]=a[1];
		MachWorkLoad[Mach-1][2]=a[2];
	}
	/*for(i=0;i<toper;i++)
			{
				cout<<Op[l][i][0]+1<<","<<Op[l][i][1]+1<<","<<Op[l][i][2]<<","<<Op[l][i][3]<<","<<Op[l][i][5]<<","<<Op[l][i][6]<<","<<Op[l][i][4]<<endl;
			}*/
}
void MinEndTime(int l,int Op[Onpop][T_Oper][7])
{
   int i,j,z1,oc[Mac],MST[Mac][3],z3,z2[3],k,z,k2,k3,LS[Mac][T_Oper][8],flag,k1,z4,FT[NJb][3],tmp[7];
	int Mach,J;
	for(i=1;i<=M;i++)
	{
		oc[i] = 0; //Op_counter
		MST[i][0]=ReMST[i][0];
		MST[i][1]=ReMST[i][1];
		MST[i][2]=ReMST[i][2];
	}
	for(i=0;i<N;i++)
	{
		FT[i][0]=ReFT[i][0];
		FT[i][1]=ReFT[i][1];
		FT[i][2]=ReFT[i][2];
	}
	for(i=0;i<toper;i++)
	{
		a[0]=FT[Op[l][i][0]][0]; a[1]=FT[Op[l][i][0]][1]; a[2]=FT[Op[l][i][0]][2];
		int m=M_O[Op[l][i][0]][Op[l][i][1]][0];
		b[0]=MST[m][0];	b[1]=MST[m][1];	 b[2]=MST[m][2];
		Mach=m;
		J=0;
		if(Compare(a,b)==0)
		{
			c[0]=b[0]+p1[Op[l][i][0]][Op[l][i][1]][0];
			c[1]=b[1]+p2[Op[l][i][0]][Op[l][i][1]][0];
			c[2]=b[2]+p3[Op[l][i][0]][Op[l][i][1]][0];
		}
		else
		{
			c[0]=a[0]+p1[Op[l][i][0]][Op[l][i][1]][0];
			c[1]=a[1]+p2[Op[l][i][0]][Op[l][i][1]][0];
			c[2]=a[2]+p3[Op[l][i][0]][Op[l][i][1]][0];
		}
		for(j=0;j<NM_O[Op[l][i][0]][Op[l][i][1]];j++)
		{
			int m1=M_O[Op[l][i][0]][Op[l][i][1]][j];
			b[0]=MST[m1][0];	b[1]=MST[m1][1];	 b[2]=MST[m1][2];
			if(Compare(a,b)==0)
			{
			    d[0]=b[0]+p1[Op[l][i][0]][Op[l][i][1]][j];
				d[1]=b[1]+p2[Op[l][i][0]][Op[l][i][1]][j];
				d[2]=b[2]+p3[Op[l][i][0]][Op[l][i][1]][j];
			}
			else
			{
				d[0]=a[0]+p1[Op[l][i][0]][Op[l][i][1]][j];
				d[1]=a[1]+p2[Op[l][i][0]][Op[l][i][1]][j];
				d[2]=a[2]+p3[Op[l][i][0]][Op[l][i][1]][j];
			}
			if(Compare(c,d)==1)
			{
				c[0]=d[0];
				c[1]=d[1];
				c[2]=d[2];
				Mach=m1;
				J=j;
			}
		}
		Op[l][i][2]=Mach;
		Op[l][i][3]=p1[Op[l][i][0]][Op[l][i][1]][J];
		Op[l][i][5]=p2[Op[l][i][0]][Op[l][i][1]][J];
		Op[l][i][6]=p3[Op[l][i][0]][Op[l][i][1]][J];
		FT[Op[l][i][0]][0]=c[0];
		FT[Op[l][i][0]][1]=c[1];
		FT[Op[l][i][0]][2]=c[2];
		MST[Mach][0]=c[0];
		MST[Mach][1]=c[1];
		MST[Mach][2]=c[2];
	}
	/*for(i=0;i<toper;i++)
			{
				cout<<Op[l][i][0]+1<<","<<Op[l][i][1]+1<<","<<Op[l][i][2]<<","<<Op[l][i][3]<<","<<Op[l][i][5]<<","<<Op[l][i][6]<<","<<Op[l][i][4]<<endl;
			}*/
}
//Evaluate makespan
//Left-Shift operator
void Evala_Makespan(int seque[T_Oper][7],int FL[3])
{
	int i,j,z1,oc[Mac],MST[Mac][3],z3,z2[3],k,z,k2,k3,LS[Mac][T_Oper][8],flag,k1,z4,FT[NJb][3],tmp[7];
	for(i=1;i<=M;i++)
	{
		oc[i] = 0; //Op_counter
		MST[i][0]=ReMST[i][0];
		MST[i][1]=ReMST[i][1];
		MST[i][2]=ReMST[i][2];
		Count[i]=0;
		CC[i]=0;
	}
	for(i=0;i<N;i++)
	{
		FT[i][0]=ReFT[i][0];
		FT[i][1]=ReFT[i][1];
		FT[i][2]=ReFT[i][2];
	}
	/*outfile.open("sequence.csv",ios::app);   	
	outfile<<a[0]<<","<<a[1]<<","<<a[2]<<endl;
	for(i=0;i<toper;i++)
		{
			outfile<<seque[i][0]+1<<","<<seque[i][1]+1<<","<<seque[i][2]<<","<<seque[i][3]<<","<<seque[i][5]<<","<<seque[i][6]<<","<<seque[i][4]<<endl;
		}
	outfile<<"///////////////////////////////////"<<endl;
	outfile.close();*/
	for(i=0;i<toper;i++)
	{
		Count[seque[i][2]]++;
		CC[seque[i][2]]++;
		if(oc[seque[i][2]] == 0)
			{
				a[0]=MST[seque[i][2]][0];b[0]=FT[seque[i][0]][0];
				a[1]=MST[seque[i][2]][1];b[1]=FT[seque[i][0]][1];
				a[2]=MST[seque[i][2]][2];b[2]=FT[seque[i][0]][2];
				if((Compare(a,b)==1)||(Compare(a,b)==-1)/*MST[seque[i][2]] >= FT[seque[i][0]]*/)
				{
					MST[seque[i][2]][0] += seque[i][3];
					MST[seque[i][2]][1] += seque[i][5];
					MST[seque[i][2]][2] += seque[i][6];
					FT[seque[i][0]][0] = MST[seque[i][2]][0];
					FT[seque[i][0]][1] = MST[seque[i][2]][1];
					FT[seque[i][0]][2] = MST[seque[i][2]][2];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][0]=seque[i][2];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][1]=seque[i][0];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][2]=seque[i][1];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][3]=MST[seque[i][2]][0]-seque[i][3];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][4]=MST[seque[i][2]][1]-seque[i][5];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][5]=MST[seque[i][2]][2]-seque[i][6];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][6]=MST[seque[i][2]][0];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][7]=MST[seque[i][2]][1];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][8]=MST[seque[i][2]][2];
				}
				else
				{
					MST[seque[i][2]][0] = FT[seque[i][0]][0] + seque[i][3];
					MST[seque[i][2]][1] = FT[seque[i][0]][1] + seque[i][5];
					MST[seque[i][2]][2] = FT[seque[i][0]][2] + seque[i][6];
					FT[seque[i][0]][0] = MST[seque[i][2]][0];
					FT[seque[i][0]][1] = MST[seque[i][2]][1];
					FT[seque[i][0]][2] = MST[seque[i][2]][2];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][0]=seque[i][2];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][1]=seque[i][0];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][2]=seque[i][1];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][3]=MST[seque[i][2]][0]-seque[i][3];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][4]=MST[seque[i][2]][1]-seque[i][5];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][5]=MST[seque[i][2]][2]-seque[i][6];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][6]=MST[seque[i][2]][0];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][7]=MST[seque[i][2]][1];
					MachStarEnd[seque[i][2]][CC[seque[i][2]]][8]=MST[seque[i][2]][2];
				}
				oc[seque[i][2]]++;
			}
		else
			{
				z1 = seque[i][2]; //processing machine
				z2[0] = seque[i][3];z2[1] = seque[i][5];z2[2] = seque[i][6]; //processing time
				z3 = seque[i][0]; //job 
				z4 = seque[i][1]; //operation
				flag = 0;
				if(/*(j == oc[z1]) &&*/ (flag == 0))
				{
					a[0]=MST[z1][0]; a[1]=MST[z1][1]; a[2]=MST[z1][2];
					b[0]=FT[z3][0];	 b[1]=FT[z3][1];  b[2]=FT[z3][2];
					if((Compare(a,b)==1)||(Compare(a,b)==-1)/*MST[z1] >= FT[z3]*/)
					{
						MST[z1][0] += z2[0];
						MST[z1][1] += z2[1];
						MST[z1][2] += z2[2];
						FT[z3][0] = MST[z1][0];
						FT[z3][1] = MST[z1][1];
						FT[z3][2] = MST[z1][2];
						MachStarEnd[z1][CC[seque[i][2]]][0]=z1;
						MachStarEnd[z1][CC[seque[i][2]]][1]=z3;
						MachStarEnd[z1][CC[seque[i][2]]][2]=z4;
						MachStarEnd[z1][CC[seque[i][2]]][3]=MST[z1][0]-z2[0];
						MachStarEnd[z1][CC[seque[i][2]]][4]=MST[z1][1]-z2[1];
						MachStarEnd[z1][CC[seque[i][2]]][5]=MST[z1][2]-z2[2];
						MachStarEnd[z1][CC[seque[i][2]]][6]=MST[z1][0];
						MachStarEnd[z1][CC[seque[i][2]]][7]=MST[z1][1];
						MachStarEnd[z1][CC[seque[i][2]]][8]=MST[z1][2];
					}
					else
					{
						MST[z1][0] = FT[z3][0] + z2[0];
						MST[z1][1] = FT[z3][1] + z2[1];
						MST[z1][2] = FT[z3][2] + z2[2];
						FT[z3][0] = MST[z1][0];
						FT[z3][1] = MST[z1][1];
						FT[z3][2] = MST[z1][2];
						MachStarEnd[z1][CC[seque[i][2]]][0]=z1;
						MachStarEnd[z1][CC[seque[i][2]]][1]=z3;
						MachStarEnd[z1][CC[seque[i][2]]][2]=z4;
						MachStarEnd[z1][CC[seque[i][2]]][3]=MST[z1][0]-z2[0];
						MachStarEnd[z1][CC[seque[i][2]]][4]=MST[z1][1]-z2[1];
						MachStarEnd[z1][CC[seque[i][2]]][5]=MST[z1][2]-z2[2];
						MachStarEnd[z1][CC[seque[i][2]]][6]=MST[z1][0];
						MachStarEnd[z1][CC[seque[i][2]]][7]=MST[z1][1];
						MachStarEnd[z1][CC[seque[i][2]]][8]=MST[z1][2];
					}
					oc[seque[i][2]]++;
				}
			}
	}
	FL[0] = 0;
	FL[1] = 0;
	FL[2] = 0;
	for(i=0;i<N;i++)
	{
		a[0]=FL[0]; a[1]=FL[1]; a[2]=FL[2];
		b[0]=FT[i][0];	 b[1]=FT[i][1];  b[2]=FT[i][2];
		if(Compare(a,b)==0)
		{
			FL[0] = FT[i][0];
			FL[1] = FT[i][1];
			FL[2] = FT[i][2];
		}
	}
	//cout<<FL[0]<<","<<FL[1]<<","<<FL[2]<<endl;
	//a[0]=FL[0]; a[1]=FL[1]; a[2]=FL[2];
	//b[0]=20;	 b[1]=28;  b[2]=40;
	//if((Compare(a,b)==0)/*||(Compare(a,b)==-1)*/)
	//{
	//	outfile.open("best sequence.csv",ios::app);   	
	//	outfile<<a[0]<<","<<a[1]<<","<<a[2]<<endl;
	//	for(i=0;i<toper;i++)
	//		{
	//			outfile<<seque[i][0]+1<<","<<seque[i][1]+1<<","<<seque[i][2]<<","<<seque[i][3]<<","<<seque[i][5]<<","<<seque[i][6]<<","<<seque[i][4]<<endl;
	//		}
	//	outfile<<"///////////////////////////////////"<<endl;
	//	outfile.close();
	//	outfile.open("best sequence.csv",ios::app);   	
	//	outfile<<a[0]<<","<<a[1]<<","<<a[2]<<endl;
	//	for(i=0;i<toper;i++)
	//		{
	//			outfile<<sequence[i][0]+1<<","<<sequence[i][1]+1<<","<<sequence[i][2]<<","<<sequence[i][3]<<","<<sequence[i][5]<<","<<sequence[i][6]<<","<<sequence[i][4]<<endl;
	//		}
	//	outfile<<"///////////////////////////////////"<<endl;
	//	outfile.close();
	//	//Evala_Makespan(seque,FL);
	//}
	
	
}
void New_Job_Inserting()
{
	FILE *fp;
	
	int i,j,k;
	
	fp = fopen("insert time.txt","r");
	if(fp == NULL)
	{
		printf("\nError in opening insert time file\n");
		system("pause");
	}
	fscanf(fp,"%d",&inserttime);
	printf("\n%d ",inserttime); 
	printf("\n" );
	//machine release time
	for(i=1;i<=M;i++)
	{
		for(j=1;j<=CC[i];j++)
		{
		   	if((MachStarEnd[i][j][4]>inserttime)|| (MachStarEnd[i][j][4]==inserttime))
			{
				if(j==1)
				{
				   Mach_Release_Time[i][0]=inserttime;
				   Mach_Release_Time[i][1]=inserttime;
				   Mach_Release_Time[i][2]=inserttime;
				}
				else
				{
					if(MachStarEnd[i][j-1][6]<inserttime)
					{
					   Mach_Release_Time[i][0]=inserttime;
					}
					else
					{
					   Mach_Release_Time[i][0]=MachStarEnd[i][j-1][6];
					}
					if(MachStarEnd[i][j-1][7]<inserttime)
					{
					   Mach_Release_Time[i][1]=inserttime;
					}
					else
					{
					   Mach_Release_Time[i][1]=MachStarEnd[i][j-1][7];
					}
					if(MachStarEnd[i][j-1][8]<inserttime)
					{
					   Mach_Release_Time[i][2]=inserttime;
					}
					else
					{
					   Mach_Release_Time[i][2]=MachStarEnd[i][j-1][8];
					}
				}
				break;
			}
			else
			{
				if(j==CC[i])
				{
				    if(MachStarEnd[i][j][6]<inserttime)
					{
					   Mach_Release_Time[i][0]=inserttime;
					}
					else
					{
					   Mach_Release_Time[i][0]=MachStarEnd[i][j][6];
					}
				   if(MachStarEnd[i][j][7]<inserttime)
					{
					   Mach_Release_Time[i][1]=inserttime;
					}
					else
					{
					   Mach_Release_Time[i][1]=MachStarEnd[i][j][7];
					}
					if(MachStarEnd[i][j][8]<inserttime)
					{
					   Mach_Release_Time[i][2]=inserttime;
					}
					else
					{
					   Mach_Release_Time[i][2]=MachStarEnd[i][j][8];
					}
				}
			}
		}							
	}
	outfile.open("machine release time1.csv",ios::app);
	for(i=1;i<=M;i++)
	{
	   outfile<<Mach_Release_Time[i][0]<<" "<<Mach_Release_Time[i][1]<<" "<<Mach_Release_Time[i][2]<<endl;
	   outfile<<endl;
	}
	outfile.close();
	// count operation of each job start time and end time from machine start time and end time
	for(i=1;i<=M;i++)
	{
	   for(j=1;j<=CC[i];j++)
		{
		   	int job=MachStarEnd[i][j][1];
			int oper=MachStarEnd[i][j][2];
			JobStarEnd[job][oper][0]=MachStarEnd[i][j][3];
			JobStarEnd[job][oper][1]=MachStarEnd[i][j][4];
			JobStarEnd[job][oper][2]=MachStarEnd[i][j][5];
			JobStarEnd[job][oper][3]=MachStarEnd[i][j][6];
			JobStarEnd[job][oper][4]=MachStarEnd[i][j][7];
			JobStarEnd[job][oper][5]=MachStarEnd[i][j][8];
			JobStarEnd[job][oper][6]=i;
	   }
	}
	outfile.open("JobStarEnd.txt",ios::app);
	for(i=0;i<N;i++)
	{
	   for(j=0;j<Jb[i];j++)
	   {
		   for(k=0;k<7;k++)
		   {
			   outfile<<JobStarEnd[i][j][k]<<" ";
		   }
		   outfile<<endl;
	   }
	   outfile<<endl;
	}
	outfile.close();
	for(i=0;i<N;i++)
	{
		for(j=0;j<Jb[i];j++)
		{
			if((JobStarEnd[i][j][1]>inserttime)||(JobStarEnd[i][j][1]==inserttime))
			{
				if(j==0)
				{
				   Job_Release_Time[i][0]=inserttime;
				   Job_Release_Time[i][1]=inserttime;
				   Job_Release_Time[i][2]=inserttime;
				}
				else
				{
				   if(JobStarEnd[i][j-1][3]<inserttime)
				   {
					  Job_Release_Time[i][0]=inserttime;
				   }
				   else
				   {
					   Job_Release_Time[i][0]=JobStarEnd[i][j-1][3];
				   }
				   if(JobStarEnd[i][j-1][4]<inserttime)
				   {
					  Job_Release_Time[i][1]=inserttime;
				   }
				   else
				   {
					   Job_Release_Time[i][1]=JobStarEnd[i][j-1][4];
				   }
				   if(JobStarEnd[i][j-1][5]<inserttime)
				   {
					  Job_Release_Time[i][2]=inserttime;
				   }
				   else
				   {
					   Job_Release_Time[i][2]=JobStarEnd[i][j-1][5];
				   }
				}
				JobStarOper[i]=j;
				break;
			}
			else
			{
				if(j==Jb[i]-1)
				{
				  if(JobStarEnd[i][j][3]<inserttime)
				   {
					  Job_Release_Time[i][0]=inserttime;
				   }
				   else
				   {
					   Job_Release_Time[i][0]=JobStarEnd[i][j][3];
				   }
				   if(JobStarEnd[i][j][4]<inserttime)
				   {
					  Job_Release_Time[i][1]=inserttime;
				   }
				   else
				   {
					   Job_Release_Time[i][1]=JobStarEnd[i][j][4];
				   }
				   if(JobStarEnd[i][j][5]<inserttime)
				   {
					  Job_Release_Time[i][2]=inserttime;
				   }
				   else
				   {
					   Job_Release_Time[i][2]=JobStarEnd[i][j][5];
				   }
				   JobStarOper[i]=j+1;
				}
			}
		}
	}
	outfile.open("job release time1.csv",ios::app);
	for(i=0;i<N;i++)
	{
	   outfile<<Job_Release_Time[i][0]<<" "<<Job_Release_Time[i][1]<<" "<<Job_Release_Time[i][2]<<endl;
	   outfile<<endl;
	}
	outfile.close();
	outfile.open("addingremanu02.csv",ios::app);
	outfile<<N<<" "<<M<<" "<<2<<endl;
	for(i=0;i<N;i++)
	{
	   outfile<<Jb[i]-JobStarOper[i]<<" ";
	   for(j=JobStarOper[i];j<Jb[i];j++)
		{
		   	outfile<<NM_O[i][j]<<" ";
			for(k=0;k<NM_O[i][j];k++)
			{
				outfile<<M_O[i][j][k]<<" "<<p1[i][j][k]<<" "<<p2[i][j][k]<<" "<<p3[i][j][k]<<" ";
			}
	   }
	   outfile<<endl;
	}
	outfile.close();
}
int main()
{
	FILE *fp,*fp1,*fp2;
	int i,j,k,x,l,it,r,r1,r2,uc,u[T_Oper],co[T_Oper],ncp[T_Oper],pos[T_Oper],nb[T_Oper],val[3],flag,Min[3];
	int x1;
	
	int MST[Mac][3],PN,PN1,count;
	int Scout[Empop],Emp[T_Oper][7],Emp_F[3];
	int TMP[T_Oper][7];
	int z,z1,z2[3],z3,z4,k1,k2,k3,tmp[7],oc[Mac],LS[Mac][T_Oper][8],Sum[3];
	unsigned int seed[RUN],Seed_val[RUN],myseed = 10;
	double Avrg[3],Std_dev,Av_btime,Av_utime,To_btime,To_utime,A_Sd,Std_dev_Val;
	fp = fopen("addingremanu01.txt","r");
	//fp = fopen("reman.txt","r");
	if(fp == NULL)
	{
		printf("\nError in opening file\n");
		system("pause");
	}
	fscanf(fp,"%d%d%d",&N,&M,&x1);
	printf("\nN = %d M = %d x1 = %d",N,M,x1);
	toper = 0; //total number of operations
	for(i=0;i<N;i++)
	{
		fscanf(fp,"%d",&Jb[i]);
		printf("\n%d  ",Jb[i]);

		toper += Jb[i];
		for(j=0;j<Jb[i];j++)
		{
			fscanf(fp,"%d",&NM_O[i][j]); //Number of machines for the operation j of J[i]
			printf("%d ",NM_O[i][j]);
			
			for(k=0;k<NM_O[i][j];k++)
			{
				fscanf(fp,"%d%d%d%d",&M_O[i][j][k],&p1[i][j][k],&p2[i][j][k],&p3[i][j][k]);
				printf("%d %d %d %d ",M_O[i][j][k],p1[i][j][k],p2[i][j][k],p3[i][j][k]);
			}
			printf("\n");
		}
	}
	fp = fopen("job release time.txt","r");
	if(fp == NULL)
	{
		printf("\nError in opening job release time file\n");
		system("pause");
	}
	for(i=0;i<N;i++)
	{
		fscanf(fp,"%d%d%d",&ReFT[i][0],&ReFT[i][1],&ReFT[i][2]);
		printf("\n%d %d %d ",ReFT[i][0],ReFT[i][1],ReFT[i][2]);
	}
	printf("\n");
	fp = fopen("machine release time.txt","r");
	if(fp == NULL)
	{
		printf("\nError in opening machine release time file\n");
		system("pause");
	}
	for(i=1;i<=M;i++)
	{
		fscanf(fp,"%d%d%d",&ReMST[i][0],&ReMST[i][1],&ReMST[i][2]);
		printf("\n%d %d %d ",ReMST[i][0],ReMST[i][1],ReMST[i][2]);
	}
	printf("\n" );
	/*int aa[10]={1,2,3,4,5,6,7,8,9,10};
	for(i=0;i<10;i++)
	{
		cout<<i<<",";
		cout<<aa[i++]<<",";
	}
	cout<<endl;
	for(i=0;i<10;i++)
	{
		cout<<i<<","<<aa[i++]<<",";
	}*/
	srand(myseed);
	int re;
	it=0;
		time_t start,end; /*注意计时所用的变量名称*/	
		start=clock();
		seed[it] = rand();
        srand(seed[it]);
		x = (int)(PERC*toper);
		PN = (int)(PERC1*toper);
		PN1 = (int)(PERC2*toper);
		Optimal[0] = 500000; //Max val is assigned
		Optimal[1] = 500000; 
		Optimal[2] = 500000; 
		//fp2 = fopen("best resuts.txt","a");
		for(l=0;l<Empop;l++)
		{

			/*if((l==148)||(l==99)||(l==100))
			{
				system("pause");
			}*/
			for(i=0;i<toper;i++)
			{
				for(j=0;j<7;j++)
				{
					Op[l][i][j]=0;
				}
			}
			Init_pop(l);
			//cout<<l<<","<<p3[0][0][0]<<endl;
			/*if(l<30)
			{
				RemainMaxWorkInitOS(l,Op);
			}
			if((l>29)&&(l<60))
			{
				RemainOperInitOS(l,Op);
			}*/
			/*if((l%10)<2)
			{
				LSMinLoad(l,Op);
			}*/
			if(((l%10)>2)&&((l%10)<5))
			{
				GSMinLoad(l,Op);
			}
			if(((l%10)>6)&&((l%10)<9))
			{	
				MinEndTime(l,Op);
			}
			/*if((l>59)&&(l<90))
			{
				LSMinLoad(l,Op);
			}
			if((l>89)&&(l<120))
			{
				GSMinLoad(l,Op);
			}
			if(l>119)
			{	
				MinEndTime(l,Op);
			}*/
			for(i=0;i<toper;i++)
			{
				for(j=0;j<7;j++)
				{
					seque[i][j]=Op[l][i][j];
				}
			}
			Evala_Makespan(seque,FL);
			/*for(i=0;i<toper;i++)
			{
				for(j=0;j<7;j++)
				{
					seque[i][j]=Op[l][i][j];
				}
			}
			Evala_Makespan(seque,FL);*/
			F[l][0]=FL[0];F[l][1]=FL[1];F[l][2]=FL[2];
	/*if(l<30)*/
	/*if((l>29)&&(l<60))*/
	/*if((l>59)&&(l<90))*/
	/*if((l>89)&&(l<120))*/
	/*if(l>119)*/
	{
			outfile.open("heurisitc fuzzy compeletion time.csv",ios::app);
			outfile<<FL[0]<<","<<FL[1]<<","<<FL[2]<<endl;
			outfile.close();
			outfile.open("MachStarEndTime.txt",ios::app);
		for(i=1;i<=M;i++)
		{
			for(j=1;j<=CC[i];j++)
			{
				for(int k=0;k<9;k++)
				{
					if((k==1)||(k==2))
					{
						outfile<<MachStarEnd[i][j][k]+1<<" ";
					}
					else
					{
						outfile<<MachStarEnd[i][j][k]<<" ";
					}
				}
				outfile<<";"<<endl;
			}
		}
		outfile<<"********************"<<endl;
		outfile.close();
		New_Job_Inserting();
		
	
		end=clock();
		outfile.open("Time.csv",ios::app);
		outfile<<end-start<<endl;
		outfile.close();
		
	}
		
		}
	   return 0;
}
