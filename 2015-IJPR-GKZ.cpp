// DHS for FJSP with fuzzy processing time.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include<fstream>
#include<math.h>
#include <algorithm>
#include<vector>
#include<string.h>
#include<string>
//#include <cstdlib>
//#include <stdio.h>
#include "Instance.h"
//#include "Job.h"
//#include "Machine.h"
//#include "Operation.h"
using namespace std;
#define u01 ((rand())/(RAND_MAX+1.0))
const int MaxJobNum=100;
const int MaxMachNum=50;
const int MaxOperNum=50;
const int HMS=1000;
const int Repeat=30;
const int Gen=100;
int JobNum;
int MachNum;
int OperNum[MaxJobNum];
int TOperNum;
int SeleMachNum[MaxJobNum][MaxOperNum];
int SeleMachSet[MaxJobNum][MaxOperNum][MaxMachNum];
int ProceTime1[MaxJobNum][MaxOperNum][MaxMachNum];
int ProceTime2[MaxJobNum][MaxOperNum][MaxMachNum];
int ProceTime3[MaxJobNum][MaxOperNum][MaxMachNum];
int JobStarTime[MaxJobNum][3];
int MachStarTime[MaxMachNum][3];
int DueD[MaxJobNum];
int Result[HMS][4][3];//makespan,E/T,Mworkload and Tworkload
int BestResult[4][3]; //for 4 objectives best results
int BestHM[MaxJobNum*MaxOperNum][7];
int HM[HMS][MaxJobNum*MaxOperNum][7];
int NewHM[HMS][MaxJobNum*MaxOperNum][7];
int iter;
int Itercount[2]={0,0};
int seque[MaxJobNum*MaxOperNum][7],FL[3];
int a[3],b[3];
void SetOperNum()
{
	ifstream myFile;
	string line;
	int i; 
	myFile.open("acc0.txt"); 
	i=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find acc0.txt."<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{
		getline(myFile,line,'\n');
		OperNum[i]=atoi(line.c_str());
		i++	;
	}
	myFile.close();
}
void SetSeleMachNum()
{
	ifstream myFile;
	string line;
	int i,j; 
	myFile.open("acc4.txt"); 
	i=0,j=0,TOperNum=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find acc4.txt."<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{
		getline(myFile,line,'\n');
		SeleMachNum[i][j]=atoi(line.c_str());
		j++	;
		if(j==OperNum[i])
		{
			i++; 
			j=0;
		}
		TOperNum++;
	}
	myFile.close();
}
void SetSeleMachSet()
{
	ifstream myFile;
	string line;
	int i,j,k; 
	myFile.open("acc5.csv"); 
	i=0,j=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find acc5.csv."<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{
		for(k=0;k<MachNum;k++)
		{
			getline(myFile,line,'\n');
			SeleMachSet[i][j][k]=atoi(line.c_str());
		}
		j++	;
		if(j==OperNum[i])
		{
			i++;
			j=0;
		}
		//TOperNum++;
	}
	myFile.close();
}
void SetProceTime1()
{
	ifstream myFile;
	string line;
	int i,j,k; 
	myFile.open("acc1.txt"); 
	i=0,j=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find acc1.txt."<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{
		for(k=0;k<SeleMachNum[i][j];k++)
		{
			getline(myFile,line,'\n');
			ProceTime1[i][j][k]=atoi(line.c_str());
		}
		j++;
		if(j==OperNum[i])
		{
			i++	;
			j=0;
		}
	}
	myFile.close();
}
void SetProceTime2()
{
	ifstream myFile;
	string line;
	int i,j,k; 
	myFile.open("acc2.txt"); 
	i=0,j=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find acc2.txt."<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{
		for(k=0;k<SeleMachNum[i][j];k++)
		{
			getline(myFile,line,'\n');
			ProceTime2[i][j][k]=atoi(line.c_str());
		}
		j++;
		if(j==OperNum[i])
		{
			i++	;
			j=0;
		}
	}
	myFile.close();
}
void SetProceTime3()
{
	ifstream myFile;
	string line;
	int i,j,k; 
	myFile.open("acc3.txt"); 
	i=0,j=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find acc3.txt."<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{
		for(k=0;k<SeleMachNum[i][j];k++)
		{
			getline(myFile,line,'\n');
			ProceTime3[i][j][k]=atoi(line.c_str());
		}
		j++;
		if(j==OperNum[i])
		{
			i++	;
			j=0;
		}
	}
	myFile.close();
}
void SetJobStarTime()
{
	ifstream myFile;
	string line;
	int i,j; 
	myFile.open("JobStarTime.csv"); 
	i=0,j=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find JobStarTime.csv"<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{	
		getline(myFile,line,'\n');	
		JobStarTime[i][j]=atoi(line.c_str());
		j++;
		if(j==3)
		{
			i++;
			j=0;
		}
	}
	myFile.close();
}
void SetMachStarTime()
{
	ifstream myFile;
	string line;
	int i,j; 
	myFile.open("MachStarTime.csv"); 
	i=0,j=0;
	if(!myFile)
	{
		cout<<"Sorry, we can not find MachStarTime.csv"<<endl;
		system("pause");
		exit(0);
	}
	while(!myFile.eof())
	{	
		getline(myFile,line,'\n');	
		MachStarTime[i][j]=atoi(line.c_str());
		j++;
		if(j==3)
		{
			i++;
			j=0;
		}
	}
	myFile.close();
}
//void Calculate_DueDate()
//{
//        int i,j,k;
//        int P;
//        /*int T;
//        T=0.3;*/
//        for(i=0;i<JobNum;i++)
//        {
//                DueD[i]=0;
//                for(j=0;j<OperNum[i];j++)
//                {
//                        P=0;
//                        for(k=0;k<SeleMachNum[i][j];k++)
//                        {
//                                P+=ProceTime1[i][j][k];
//								P+=ProceTime2[i][j][k];
//								P+=ProceTime3[i][j][k];
//                        }
//						P=P/3;
//                        P=P/SeleMachNum[i][j];
//                        DueD[i]+=P;
//                }
//                DueD[i]=(1+0.3*JobNum/MachNum)*DueD[i];
//				/*if((i==JobNumber-insertnumber)||(i>JobNumber-insertnumber))
//				{
//				 DueD[i]+=inserttime0;
//				}*/
//        }
//        ofstream outfile;
//        outfile.open("DueD.csv",ios::app);
//        for(i=0;i<JobNum;i++)
//        {
//              outfile<<DueD[i]<<endl;
//        }
//        outfile.close();
//}
//void inializ_DueDate()
//{
//	ifstream myFile;
//	string line;
//	int i,j; 
//	myFile.open("DueD.csv"); 
//	i=0,j=0;
//	if(!myFile)
//	{
//		cout<<"Sorry, we can not find DueD.csv"<<endl;
//		system("pause");
//		exit(0);
//	}
//	while(!myFile.eof())
//	{	
//		getline(myFile,line,'\n');	
//		DueD[i]=atoi(line.c_str());
//		i++;
//	}
//	myFile.close();
//}
//void RandomInitMA(int I)
//{
//	int i,j,r; 
//    int count=0;
//	for(i=0;i<JobNum;i++)
//	{
//		for(j=0;j<OperNum[i];j++)
//		{
//		  r=rand()%SeleMachNum[i][j];	   
//		  HM[I][count+j]=SeleMachSet[i][j][r];
//		}
//		count+=OperNum[i];
//	}
//}
////select machine with min porcessing time for each operation
//int MinProcTimeMach(int I,int J)
//	{
//	int temp[3]={ ProceTime1[I][J][0], ProceTime2[I][J][1], ProceTime2[I][J][2]},machnum=0;
//		int i,sum1,sum2;
//		sum1=temp[0]+2*temp[1]+temp[2];
//		for(i=0;i<SeleMachNum[I][J];i++)
//		{
//			sum2=ProceTime1[I][J][i]+2*ProceTime2[I][J][i]+ProceTime2[I][J][i];
//			if(sum1>sum2)
//			{
//			   temp[0]=ProceTime1[I][J][i];
//			   temp[1]=ProceTime2[I][J][i];
//			   temp[2]=ProceTime3[I][J][i];
//			   machnum=i;
//			}
//			else
//			{
//				if(sum1==sum2)
//				{
//				   if(temp[1]>ProceTime2[I][J][i])
//				   {
//					   temp[0]=ProceTime1[I][J][i];
//					   temp[1]=ProceTime2[I][J][i];
//					   temp[2]=ProceTime3[I][J][i];
//					   machnum=i;
//				   }
//				   else
//				   {
//						if(temp[1]==ProceTime2[I][J][i])
//						{
//						   if((temp[2]-temp[0])>(ProceTime3[I][J][i]-ProceTime1[I][J][i]))
//							{
//							   temp[0]=ProceTime1[I][J][i];
//							   temp[1]=ProceTime2[I][J][i];
//							   temp[2]=ProceTime3[I][J][i];
//							   machnum=i;
//							}
//						}
//				   }
//				}
//			}
//		}
//		return machnum;
//
//	}
//void MinProcTimeInit(int I)
//{
//        int i,j,count=0;
//        for(i=0;i<JobNum;i++)
//			{
//				for(j=0;j<OperNum[i];j++)
//				{
//					int m=MinProcTimeMach(i,j);
//					HM[I][count+j]=SeleMachSet[i][j][m];
//				 }
//				 count+=OperNum[i];
//			}
//}
////select machine with min workload
//int MinMachWorkload(int I,int JJ,int J[MaxJobNum],int T[MaxMachNum][3])
//	{
//		int min=SeleMachSet[J[I]-1][JJ][0];	
//		//int temp[3]={ ProceTime1[J[I]-1][J][0], ProceTime2[J[I]-1][J][1], ProceTime2[J[I]-1][J][2]},
//		//int machnum=0;
//		int i,sum1,sum2;
//		//sum1=T[min-1][0]+2*T[min-1][1]+T[min-1][2];
//		sum1=T[min-1][0]+2*T[min-1][1]+T[min-1][2]+ProceTime1[J[I]-1][JJ][min-1]+2*ProceTime2[J[I]-1][JJ][min-1]+ProceTime3[J[I]-1][JJ][min-1];
//		for(i=0;i<SeleMachNum[J[I]-1][JJ];i++)
//		{
//			//sum2=T[i][0]+2*T[i][1]+T[i][2];
//			sum2=T[i][0]+2*T[i][1]+T[i][2]+ProceTime1[J[I]-1][JJ][i]+2*ProceTime2[J[I]-1][JJ][i]+ProceTime3[J[I]-1][JJ][i];
//			if(sum1>sum2)
//			{
//			   min=i+1;
//			   
//			}
//			else
//			{
//				if(sum1==sum2)
//				{
//				   if(T[min-1][1]>T[i][1])
//				   {
//					     min=i+1;
//				   }
//				   else
//				   {
//						if(T[min-1][1]>T[i][1])
//						{
//						   if((T[min-1][2]-T[min-1][0])>(T[i][2]-T[i][0]))
//							{
//								    min=i+1;
//							}
//						}
//				   }
//				}
//			}
//		}
//		return min;
//	}
//void GloMinMachWorkLoadInit(int I)
//{
//        int i,j,r;
//        int T[MaxMachNum][3],J[MaxJobNum];
//        int count=0;
//		//srand((unsigned) time(NULL));
//        for(i=0;i<MachNum;i++)
//        {
//			for(j=0;j<3;j++)
//				{
//					T[i][j]=0;
//				}
//        }
//		for(i=0;i<JobNum;i++)
//		{
//			J[i]=JobNum-i;
//		}
//		for(i=0;i<4*JobNum;i++)
//		{
//			r=rand()%JobNum;
//			int temp=J[0];
//			J[0]=J[r];
//			J[r]=temp;
//		}
//		for(i=0;i<JobNum;i++)
//		{
//			for(int j=0;j<J[i]-1;j++)
//						{
//							count+=OperNum[j];
//						}
//			for(j=0;j<OperNum[J[i]-1];j++)
//			{        
//				int m=MinMachWorkload(i,j,J,T);		
//				HM[I][count+j]=m;
//                T[m-1][0]+=ProceTime1[J[i]-1][j][m-1];
//				T[m-1][1]+=ProceTime2[J[i]-1][j][m-1];
//				T[m-1][2]+=ProceTime3[J[i]-1][j][m-1];
//
//			}
//			count=0;
//		}
//        
//}
//
//void LocMinMachWorkLoadInit(int I)
//{
//        int i,j,r;
//        int T[MaxMachNum][3],J[MaxJobNum];
//        int count=0;
//		//srand((unsigned) time(NULL));
//        for(i=0;i<MachNum;i++)
//        {
//			for(j=0;j<3;j++)
//				{
//					T[i][j]=0;
//				}
//        }
//		for(i=0;i<JobNum;i++)
//		{
//			J[i]=JobNum-i;
//		}
//		for(i=0;i<4*JobNum;i++)
//		{
//			r=rand()%JobNum;
//			int temp=J[0];
//			J[0]=J[r];
//			J[r]=temp;
//		}
//		for(i=0;i<JobNum;i++)
//		{
//			for(int j=0;j<J[i]-1;j++)
//						{
//							count+=OperNum[j];
//						}
//			for(j=0;j<OperNum[J[i]-1];j++)
//			{        
//				int m=MinMachWorkload(i,j,J,T);		
//				HM[I][count+j]=m;
//                T[m-1][0]+=ProceTime1[J[i]-1][j][m-1];
//				T[m-1][1]+=ProceTime2[J[i]-1][j][m-1];
//				T[m-1][2]+=ProceTime3[J[i]-1][j][m-1];
//
//			}
//			for(j=0;j<MachNum;j++)
//				{
//					T[j][0]=0;
//					T[j][1]=0;
//					T[j][2]=0;
//				}
//			count=0;
//		}
//        
//}
//void RandomInitOS(int I)
//{
//	int i,j;
//	int OS[MaxJobNum*MaxOperNum];
//    int count=0;
//    for(i=0;i<JobNum;i++)
//    {
//        for(j=0;j<OperNum[i];j++)
//        {
//            OS[count+j]=i+1;
//        }
//        count+=OperNum[i];
//    }
//	 for(i=0;i<2*TOperNum;i++)
//    {
//
//        int R1=rand()%TOperNum;
//		int R2=rand()%TOperNum;
//        int temp;
//        temp=OS[R1];
//        OS[R1]=OS[R2];
//        OS[R2]=temp;
//    }
//	for(i=0;i<TOperNum;i++)
//	{
//		HM[I][i+TOperNum]=OS[i];
//	}
//}
////job remain max work criteria to order operation sequence
//int MaxRemaWork(int RW[MaxJobNum][3])
//{
//	int temp[3]={RW[0][0],RW[0][1],RW[0][2]},jobnum=0;
//	int i,sum1,sum2;
//	sum1=temp[0]+2*temp[1]+temp[2];
//	for(i=0;i<JobNum;i++)
//	{
//		sum2=RW[i][0]+2*RW[i][1]+RW[i][2];
//		if(sum1<sum2)
//		{
//		   temp[0]=RW[i][0];
//		   temp[1]=RW[i][1];
//		   temp[2]=RW[i][2];
//		   jobnum=i;
//		}
//		else
//		{
//			if(sum1==sum2)
//			{
//			   if(temp[1]<RW[i][1])
//			   {
//				   temp[0]=RW[i][0];
//				   temp[1]=RW[i][1];
//				   temp[2]=RW[i][2];
//				   jobnum=i;
//			   }
//			   else
//			   {
//					if(temp[1]==RW[i][1])
//					{
//					   if((temp[2]-temp[0])<(RW[i][2]-RW[i][0]))
//						{
//						   temp[0]=RW[i][0];
//						   temp[1]=RW[i][1];
//						   temp[2]=RW[i][2];
//						   jobnum=i;
//						}
//					}
//			   }
//			}
//		}
//	}
//	return jobnum;
//}
//void MaxRemaWorkInitOS(int I)
//{
//	int i,j,count=0;
//    int RW[MaxJobNum][3];
//    int J[MaxJobNum];
//    for(i=0;i<JobNum;i++)
//    {
//        RW[i][0]=0;
//		RW[i][1]=0;
//		RW[i][2]=0;
//        for(j=0;j<OperNum[i];j++)
//        {
//            for(int k=0;k<SeleMachNum[i][j];k++)
//                {
//                    if(SeleMachSet[i][j][k]==HM[I][count+j])
//                    {
//                        RW[i][0]+=ProceTime1[i][j][k];
//						RW[i][1]+=ProceTime2[i][j][k];
//						RW[i][2]+=ProceTime3[i][j][k];
//                    }
//                }
//
//        }
//        count+=OperNum[i];
//    }
//    for(i=0;i<JobNum;i++)//initializing scheduled operation number as 0
//    {
//        J[i]=0;
//    }
//    for(i=TOperNum;i<2*TOperNum;i++)//ordering operation sequence
//    {
//        int m;
//		m=MaxRemaWork(RW);
//        HM[I][i]=m+1;
//        int p=0;
//        for(int k=0;k<m;k++)
//            {
//                p+=OperNum[k];
//            }
//        for(j=0;j<SeleMachNum[m][J[m]];j++)
//            {
//
//                if(SeleMachSet[m][J[m]][j]==HM[I][p+J[m]])
//                    {
//                        RW[m][0]-=ProceTime1[m][J[m]][j];
//						RW[m][1]-=ProceTime2[m][J[m]][j];
//						RW[m][2]-=ProceTime3[m][J[m]][j];
//                        break;
//                    }
//            }
//        J[m]++;
//    }
//}
////job remain max operation number criteria to order operation sequence
//void MaxRemaOperInitOS(int I)
//{
//	int i,j;
//    int RP[MaxJobNum];
//    for(i=0;i<JobNum;i++)
//    {
//            RP[i]=OperNum[i];
//    }
//    for(i=TOperNum;i<2*TOperNum;i++)
//    {
//		int max=RP[0];
//		int m=0;
//		for(j=0;j<JobNum;j++)
//		{
//			if(max<RP[j])
//					{
//						max=RP[j];
//						m=j;
//					}
//		}
//		RP[m]--;
//		HM[I][i]=m+1;
//    }
//}
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
void Init_pop(int l)  
{
	
	int i,j,z1,r,r1,cc,tmp1[MaxJobNum],tmp2[MaxJobNum][MaxOperNum],tmp3,tmp[7],oc[MaxMachNum];
	int Min[3],tmp4[MaxOperNum],TA[MaxMachNum][3],ADT[MaxMachNum][3];
	
	if(u01 < 0.3)
	{
		for(i=0;i<JobNum;i++)
		{
			tmp1[i] = OperNum[i];
			for(j=0;j<OperNum[i];j++)
				tmp2[i][j] = 0;
		}
		cc= 0;
		tmp3 = JobNum;
		while(1)
		{
			do
			{
				r = (int) (u01*JobNum);
			}while(tmp1[r] == 0);

			HM[l][cc][0] = r;

			j = 0;
			do
			{
				HM[l][cc][1] = j;
			}while(tmp2[r][j++] == 1);

			tmp2[r][--j] = 1;

			if(SeleMachNum[r][j] > 1)
			{
				r1 = (int) (u01*SeleMachNum[r][j]); //randomly select a machine

				//////////////////////////////////////////////////////			

				HM[l][cc][2] =SeleMachSet[r][j][r1];
				HM[l][cc][3] = ProceTime1[r][j][r1];
				HM[l][cc][5] = ProceTime2[r][j][r1];
				HM[l][cc][6] = ProceTime3[r][j][r1];
			}
			else
			{
				HM[l][cc][2] = SeleMachSet[r][j][0];
				HM[l][cc][3] = ProceTime1[r][j][0];
				HM[l][cc][5] = ProceTime2[r][j][0];
				HM[l][cc][6] = ProceTime3[r][j][0];
			}
			if(HM[l][cc][0] == 0)
			{
				HM[l][cc][4] = HM[l][cc][1]; //use for giving unique sequence number
			}
			else
			{
				j = 0;
				for(i=0;i<HM[l][cc][0];i++)
					j += OperNum[i];
				HM[l][cc][4] = j + HM[l][cc][1];
			}
			cc++;

			if(--tmp1[r] == 0)
			{
				if(--tmp3 == 0)
					break;
			}
		}
	}
	else
	{
		for(i=1;i<=MachNum;i++)
		{
			TA[i][0] = 0;
			TA[i][1] = 0;
			TA[i][2] = 0;
		}
		for(i=0;i<JobNum;i++)
		{
			tmp1[i] = OperNum[i];
			for(j=0;j<OperNum[i];j++)
				tmp2[i][j] = 0;
		}
		cc = 0;
		tmp3 = JobNum;
		while(1)
		{
			do
			{
				r = (int) (u01*JobNum);
			}while(tmp1[r] == 0);

			HM[l][cc][0] = r;
			j = 0;
			do
			{
				HM[l][cc][1] = j;
			}while(tmp2[r][j++] == 1);

			tmp2[r][--j] = 1;

			if(SeleMachNum[r][j] > 1)
			{
				for(i=0;i<SeleMachNum[r][j];i++)
				{
					ADT[SeleMachSet[r][j][i]][0] = TA[SeleMachSet[r][j][i]][0] + ProceTime1[r][j][i];
					ADT[SeleMachSet[r][j][i]][1] = TA[SeleMachSet[r][j][i]][1] + ProceTime2[r][j][i];
					ADT[SeleMachSet[r][j][i]][2] = TA[SeleMachSet[r][j][i]][2] + ProceTime3[r][j][i];
				}
				Min[0]=50000; Min[1]=50000; Min[2]=50000;
				
				for(i=0;i<SeleMachNum[r][j];i++)
				{
					b[0]=ADT[SeleMachSet[r][j][i]][0]; b[1]=ADT[SeleMachSet[r][j][i]][1]; b[2]=ADT[SeleMachSet[r][j][i]][2];
					if(Compare(Min,b)==1)
					{
						Min[0] = ADT[SeleMachSet[r][j][i]][0];
						Min[1] = ADT[SeleMachSet[r][j][i]][1];
						Min[2] = ADT[SeleMachSet[r][j][i]][2];
					}
				}
				z1 = 0;
				for(i=0;i<SeleMachNum[r][j];i++)
				{
					b[0]=ADT[SeleMachSet[r][j][i]][0]; b[1]=ADT[SeleMachSet[r][j][i]][1]; b[2]=ADT[SeleMachSet[r][j][i]][2];
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
		
				HM[l][cc][2] = SeleMachSet[r][j][r1];
				HM[l][cc][3] = ProceTime1[r][j][r1];
				HM[l][cc][5] = ProceTime2[r][j][r1];
				HM[l][cc][6] = ProceTime3[r][j][r1];
				TA[SeleMachSet[r][j][r1]][0] =ProceTime1[r][j][r1];
				TA[SeleMachSet[r][j][r1]][1] = ProceTime2[r][j][r1];
				TA[SeleMachSet[r][j][r1]][2] = ProceTime3[r][j][r1];
			}
			else
			{
				HM[l][cc][2] = SeleMachSet[r][j][0];
				HM[l][cc][3] = ProceTime1[r][j][0];
				HM[l][cc][5] =ProceTime2[r][j][0];
				HM[l][cc][6] = ProceTime3[r][j][0];
				TA[SeleMachSet[r][j][0]][0] = ProceTime1[r][j][0];
				TA[SeleMachSet[r][j][0]][1] = ProceTime2[r][j][0];
				TA[SeleMachSet[r][j][0]][2] = ProceTime3[r][j][0];
			}
			if(HM[l][cc][0] == 0)
				HM[l][cc][4] = HM[l][cc][1]; //use for giving unique sequence number
			else
			{
				j = 0;
				for(i=0;i<HM[l][cc][0];i++)
				{
					j += OperNum[i];
				}
				HM[l][cc][4] = j + HM[l][cc][1];
			}
			cc++;

			if(--tmp1[r] == 0)
			{
				if(--tmp3 == 0)
					break;
			}
		}
	}
//for(i=0;i<TOperNum;i++)
//	{
//		cout<<HM[l][i][0]+1<<","<<HM[l][i][1]+1<<","<<HM[l][i][2]<<","<<HM[l][i][3]<<","<<HM[l][i][5]<<","<<HM[l][i][6]<<","<<HM[l][i][4]<<endl;
//	}
}
void Inializ_HM()
{
	int i;
	/*ofstream outfile;
	outfile.open("HM.csv",ios::app);*/
	//srand((unsigned) time(NULL)+JobNum*MachNum*TOperNum*1000);
	for(i=0;i<HMS;i++)
	{
		Init_pop(i);
		double R2=(rand()%1000)*0.001;
		//cout<<R2<<",";
		//if(R2<0.6)
		//	{
		//		//MinProcTimeInit(i);
		//		GloMinMachWorkLoadInit(i);
		//	}
		//else
			//{
			//	
			//	/*if(R2<0.9)
			//		{
			//			LocMinMachWorkLoadInit(i);
			//		}
			//	else
			//		{
			//			RandomInitMA(i);
			//		}*/
			//}
		//R2=(rand()%1000)*0.001;
		////cout<<R2<<endl;
		//if(R2<0.2)
		//{
		//	RandomInitOS(i);
		//}
		//else
		//{
		//	if(R2<0.6)
		//	{
		//		MaxRemaWorkInitOS(i);
		//	}
		//	else
		//	{
		//		MaxRemaOperInitOS(i);
		//	}
		//}
	}
		//system("pause");
		/*for(int j=0;j<2*TOperNum;j++)
		{
			outfile<<HM[i][j]<<",";
		}
		outfile<<endl;*/
		
	 
	/*outfile.close();
	system("pause");*/
}
//int CompareJobMachStarTime(int job,int mach)
//{
//	int sum1,sum2;
//	sum1=JobStarTime[job-1][0]+2*JobStarTime[job-1][1]+JobStarTime[job-1][2];
//	sum2=MachStarTime[mach-1][0]+2*MachStarTime[mach-1][1]+MachStarTime[mach-1][2];
//	if(sum1>sum2)
//	{
//		return 0;
//	}
//	else
//	{
//		if(sum1<sum2)
//		{
//			return 1;
//		}
//		else
//		{
//			sum1=JobStarTime[job-1][1];
//			sum2=MachStarTime[mach-1][1];
//			if(sum1>sum2)
//			{
//				return 0;
//			}
//			else
//			{
//				if(sum1<sum2)
//				{
//					return 1;
//				}
//				else
//				{
//					sum1=JobStarTime[job-1][2]-JobStarTime[job-1][0];
//					sum2=MachStarTime[mach-1][2]-MachStarTime[mach-1][0];
//					 if(sum1>sum2)
//					 {
//						return 0;
//					 }
//					 else
//					 {
//						return 1;
//					 }
//				}
//			}
//		}
//	}
//
//}
//int CompareJobMachStarTime(int job,int mach,int MachSche[MaxMachNum],int MachStarEnd[MaxMachNum][MaxJobNum*MaxOperNum][8])
//{
//	int sum1,sum2;
//	sum1=JobStarTime[job-1][0]+2*JobStarTime[job-1][1]+JobStarTime[job-1][2];
//	sum2=MachStarEnd[mach-1][MachSche[mach-1]-1-1][3]+2*MachStarEnd[mach-1][MachSche[mach-1]-1-1][4]+MachStarEnd[mach-1][MachSche[mach-1]-1-1][5];
//	if(sum1>sum2)
//	{
//		return 0;
//	}
//	else
//	{
//		if(sum1<sum2)
//		{
//			return 1;
//		}
//		else
//		{
//			sum1=JobStarTime[job-1][1];
//			sum2=MachStarEnd[mach-1][MachSche[mach-1]-1-1][4];
//			if(sum1>sum2)
//			{
//				return 0;
//			}
//			else
//			{
//				if(sum1<sum2)
//				{
//					return 1;
//				}
//				else
//				{
//					sum1=JobStarTime[job-1][2]-JobStarTime[job-1][0];
//					sum2=MachStarEnd[mach-1][MachSche[mach-1]-1-1][5]-MachStarEnd[mach-1][MachSche[mach-1]-1-1][3];
//					 if(sum1>sum2)
//					 {
//						return 0;
//					 }
//					 else
//					 {
//						return 1;
//					 }
//				}
//			}
//		}
//	}
//}
//int CompareJobMachStarTime(int job,int mach,int MachSche[MaxMachNum],int MachStarEnd[MaxMachNum][MaxJobNum*MaxOperNum][8],int I)
//{
//	int sum1,sum2;
//	sum1=JobStarTime[job-1][0]+2*JobStarTime[job-1][1]+JobStarTime[job-1][2];
//	sum2=MachStarEnd[mach-1][I][3]+2*MachStarEnd[mach-1][I][4]+MachStarEnd[mach-1][I][5];
//	if(sum1>sum2)
//	{
//		return 0;
//	}
//	else
//	{
//		if(sum1<sum2)
//		{
//			return 1;
//		}
//		else
//		{
//			sum1=JobStarTime[job-1][1];
//			sum2=MachStarEnd[mach-1][I][4];
//			if(sum1>sum2)
//			{
//				return 0;
//			}
//			else
//			{
//				if(sum1<sum2)
//				{
//					return 1;
//				}
//				else
//				{
//					sum1=JobStarTime[job-1][2]-JobStarTime[job-1][0];
//					sum2=MachStarEnd[mach-1][I][5]-MachStarEnd[mach-1][I][3];
//					 if(sum1>sum2)
//					 {
//						return 0;
//					 }
//					 else
//					 {
//						return 1;
//					 }
//				}
//			}
//		}
//	}
//} 
//int CompareJobMachStarTime(int job,int mach,int JobSche[MaxJobNum],int JobStarEnd[MaxJobNum][MaxOperNum][6])
//{
//	int sum1,sum2;
//	sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][3]+2*JobStarEnd[job-1][JobSche[job-1]-1-1][4]+JobStarEnd[job-1][JobSche[job-1]-1-1][5];
//	sum2=MachStarTime[mach-1][0]+2*MachStarTime[mach-1][1]+MachStarTime[mach-1][2];
//	if(sum1>sum2)
//	{
//		return 0;
//	}
//	else
//	{
//		if(sum1<sum2)
//		{
//			return 1;
//		}
//		else
//		{
//			sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][4];
//			sum2=MachStarTime[mach-1][1];
//			if(sum1>sum2)
//			{
//				return 0;
//			}
//			else
//			{
//				if(sum1<sum2)
//				{
//					return 1;
//				}
//				else
//				{
//					sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][5]-JobStarEnd[job-1][JobSche[job-1]-1-1][3];
//					sum2=MachStarTime[mach-1][2]-MachStarTime[mach-1][0];
//					 if(sum1>sum2)
//					 {
//						return 0;
//					 }
//					 else
//					 {
//						return 1;
//					 }
//				}
//			}
//		}
//	}
//}
//int CompareJobMachStarTime(int job,int mach,int JobSche[MaxJobNum],int JobStarEnd[MaxJobNum][MaxOperNum][6],int MachSche[MaxMachNum],int MachStarEnd[MaxMachNum][MaxJobNum*MaxOperNum][8])
//{
//	int sum1,sum2;
//	sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][3]+2*JobStarEnd[job-1][JobSche[job-1]-1-1][4]+JobStarEnd[job-1][JobSche[job-1]-1-1][5];
//	sum2=MachStarEnd[mach-1][MachSche[mach-1]-1-1][3]+2*MachStarEnd[mach-1][MachSche[mach-1]-1-1][4]+MachStarEnd[mach-1][MachSche[mach-1]-1-1][5];
//	if(sum1>sum2)
//	{
//		return 0;
//	}
//	else
//	{
//		if(sum1<sum2)
//		{
//			return 1;
//		}
//		else
//		{
//			sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][4];
//			sum2=MachStarEnd[mach-1][MachSche[mach-1]-1-1][4];
//			if(sum1>sum2)
//			{
//				return 0;
//			}
//			else
//			{
//				if(sum1<sum2)
//				{
//					return 1;
//				}
//				else
//				{
//					sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][5]-JobStarEnd[job-1][JobSche[job-1]-1-1][3];
//					sum2=MachStarEnd[mach-1][MachSche[mach-1]-1-1][5]-MachStarEnd[mach-1][MachSche[mach-1]-1-1][3];
//					 if(sum1>sum2)
//					 {
//						return 0;
//					 }
//					 else
//					 {
//						return 1;
//					 }
//				}
//			}
//		}
//	}
//}
//int CompareJobMachStarTime(int job,int mach,int JobSche[MaxJobNum],int JobStarEnd[MaxJobNum][MaxOperNum][6],int MachSche[MaxMachNum],int MachStarEnd[MaxMachNum][MaxJobNum*MaxOperNum][8],int I)
//{
//	int sum1,sum2;
//	sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][3]+2*JobStarEnd[job-1][JobSche[job-1]-1-1][4]+JobStarEnd[job-1][JobSche[job-1]-1-1][5];
//	sum2=MachStarEnd[mach-1][I][3]+2*MachStarEnd[mach-1][I][4]+MachStarEnd[mach-1][I][5];
//	if(sum1>sum2)
//	{
//		return 0;
//	}
//	else
//	{
//		if(sum1<sum2)
//		{
//			return 1;
//		}
//		else
//		{
//			sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][4];
//			sum2=MachStarEnd[mach-1][I][4];
//			if(sum1>sum2)
//			{
//				return 0;
//			}
//			else
//			{
//				if(sum1<sum2)
//				{
//					return 1;
//				}
//				else
//				{
//					sum1=JobStarEnd[job-1][JobSche[job-1]-1-1][5]-JobStarEnd[job-1][JobSche[job-1]-1-1][3];
//					sum2=MachStarEnd[mach-1][I][5]-MachStarEnd[mach-1][I][3];
//					 if(sum1>sum2)
//					 {
//						return 0;
//					 }
//					 else
//					 {
//						return 1;
//					 }
//				}
//			}
//		}
//	}
//}
//int Comapre(int max[3],int JobStarEnd[MaxJobNum][MaxOperNum][6],int I)
//{
//	 int sum1,sum2;
//	sum1=max[0]+2*max[1]+max[2];
//	sum2=JobStarEnd[I][OperNum[I]-1][3]+2*JobStarEnd[I][OperNum[I]-1][4]+JobStarEnd[I][OperNum[I]-1][5];
//	if(sum1>sum2)
//	{
//		return 0;
//	}
//	else
//	{
//		if(sum1<sum2)
//		{
//			return 1;
//		}
//		else
//		{
//			sum1=max[1];
//			sum2=JobStarEnd[I][OperNum[I]-1][4];
//			if(sum1>sum2)
//			{
//				return 0;
//			}
//			else
//			{
//				if(sum1<sum2)
//				{
//					return 1;
//				}
//				else
//				{
//					sum1=max[2]-max[0];
//					sum2=JobStarEnd[I][OperNum[I]-1][5]-JobStarEnd[I][OperNum[I]-1][3];
//					 if(sum1>sum2)
//					 {
//						return 0;
//					 }
//					 else
//					 {
//						return 1;
//					 }
//				}
//			}
//		}
//	}
//}
////void Comapre(int max[3],int JobStarEnd[MaxJobNum][MaxOperNum][6],int I)
////{
////	 int sum1,sum2;
////	sum1=max[0]+2*max[1]+max[2];
////	sum2=JobStarEnd[I][OPerNum[I]-1][3]+2*JobStarEnd[I][OPerNum[I]-1][4]+JobStarEnd[I][OPerNum[I]-1][5];
////	if(sum1>sum2)
////	{
////		return 0;
////	}
////	else
////	{
////		if(sum1<sum2)
////		{
////			return 1;
////		}
////		else
////		{
////			sum1=max[1];
////			sum2=JobStarEnd[I][OPerNum[I]-1][4];
////			if(sum1>sum2)
////			{
////				return 0;
////			}
////			else
////			{
////				if(sum1<sum2)
////				{
////					return 1;
////				}
////				else
////				{
////					sum1=max[2]-max[0];
////					sum2=JobStarEnd[I][OPerNum[I]-1][5]-JobStarEnd[I][OPerNum[I]-1][3];
////					 if(sum1>sum2)
////					 {
////						return 0;
////					 }
////					 else
////					 {
////						return 1;
////					 }
////				}
////			}
////		}
////	}
////}
//int Compare(int Result[HMS][4][3],int BestResult[4][3],int I)
//{
//	if(BestResult[0][0]==0)
//	{
//	   return -1;
//	}
//	else
//	{
//		int sum1,sum2;
//		sum1=Result[I][0][0]+2*Result[I][0][1]+Result[I][0][2];
//		sum2=BestResult[0][0]+2*BestResult[0][1]+BestResult[0][2];
//		if(sum1>sum2)
//		{
//			return 0;
//		}
//		else
//		{
//			if(sum1<sum2)
//			{
//				return 1;
//			}
//			else
//			{
//				sum1=Result[I][0][1];
//				sum2=BestResult[0][1];
//				if(sum1>sum2)
//				{
//					return 0;
//				}
//				else
//				{
//					if(sum1<sum2)
//					{
//						return 1;
//					}
//					else
//					{
//						sum1=Result[I][0][2]-Result[I][0][0];
//						sum2=BestResult[0][2]-BestResult[0][0];
//						 if(sum1>sum2)
//						 {
//							return 0;
//						 }
//						 else
//						 {
//							return 1;
//						 }
//					}
//				}
//			}
//		}
//	}
//}
//void Update_BestResult(int I)
//{
//	int turn=Compare(Result,BestResult,I);
//	if((turn==-1)||(turn==1))
//	{
//		BestResult[0][0]=Result[I][0][0];
//	   BestResult[0][1]=Result[I][0][1];
//	   BestResult[0][2]=Result[I][0][2];
//	   for(int i=0;i<2*TOperNum;i++)
//	   {
//		   BestHM[i]=HM[I][i];
//	   }
//	   Itercount[1]=I;
//	}
//	/*else
//	{
//	   Itercount[0]++;
//	}*/
//	
//}
//int Compare(int Result[HMS][4][3],int max[3],int I)
//{
//	if(Result[I][0][0]==0)
//	{
//	   return -1;
//	}
//	else
//	{
//		int sum1,sum2;
//		sum1=Result[I][0][0]+2*Result[I][0][1]+Result[I][0][2];
//		sum2=max[0]+2*max[1]+max[2];
//		if(sum1>sum2)
//		{
//			return 0;
//		}
//		else
//		{
//			if(sum1<sum2)
//			{
//				return 1;
//			}
//			else
//			{
//				sum1=Result[I][0][1];
//				sum2=max[1];
//				if(sum1>sum2)
//				{
//					return 0;
//				}
//				else
//				{
//					if(sum1<sum2)
//					{
//						return 1;
//					}
//					else
//					{
//						sum1=Result[I][0][2]-Result[I][0][0];
//						sum2=max[2]-max[0];
//						 if(sum1>sum2)
//						 {
//							return 0;
//						 }
//						 else
//						 {
//							return 1;
//						 }
//					}
//				}
//			}
//		}
//	}
//}
//void Update_Result(int I,int JobStarEnd[MaxJobNum][MaxOperNum][6],int MachSche[MaxMachNum],int MachStarEnd[MaxMachNum][MaxJobNum*MaxOperNum][8])
//{
//	int i,max[3],flag;
//	//makespan
//	max[0]=JobStarEnd[0][OperNum[0]-1][3];
//	max[1]=JobStarEnd[0][OperNum[0]-1][4];
//	max[2]=JobStarEnd[0][OperNum[0]-1][5];
//	for(i=0;i<JobNum;i++)
//	{
//		int retu=Comapre(max,JobStarEnd,i);
//		if(retu==1)
//		{
//			max[0]=JobStarEnd[i][OperNum[i]-1][3];
//			max[1]=JobStarEnd[i][OperNum[i]-1][4];
//			max[2]=JobStarEnd[i][OperNum[i]-1][5];
//			flag=i;
//		}
//	}
//	int turn=Compare(Result,max,I);
//	if((turn==-1)||(turn==0))
//	{
//		Result[I][0][0]=max[0];
//		Result[I][0][1]=max[1];
//		Result[I][0][2]=max[2];
//		if(iter>0)
//		{
//			for(i=0;i<2*TOperNum;i++)
//			{
//				HM[I][i]=NewHM[I][i];
//			}
//		}
//
//	}
//	/*if(Itercount[0]>100)
//	{
//	    Result[I][0][0]=max[0];
//		Result[I][0][1]=max[1];
//		Result[I][0][2]=max[2];
//	}*/
//	//E/T
//	/*Result[I][1][0]=0;
//	Result[I][1][1]=0;
//	Result[I][1][2]=0;
//	for(i=0;i<JobNum;i++)
//	{
//	   if(JobStarEnd[i][OperNum[i]-1][3]>DueD[i])
//	   {
//		   Result[I][1][0]+=JobStarEnd[i][OperNum[i]-1][3]-DueD[i];
//	   }
//	   else
//	   {
//		   Result[I][1][0]+=DueD[i]-JobStarEnd[i][OperNum[i]-1][3];
//	   }
//	}*/
//	Update_BestResult(I);
//
//}
//void LeftShift(int job,int JobSche[MaxJobNum],int JobStarEnd[MaxJobNum][MaxOperNum][6],int mach,int MachSche[MaxMachNum],int MachStarEnd[MaxMachNum][MaxJobNum*MaxOperNum][8],int Time[MaxJobNum][MaxOperNum][3])
//{
//	int i;
//	int gap[3];
//	int turn;
//	//int temp[8];
//	if(MachSche[mach-1]==2)
//	{
//		if(JobSche[job-1]==1)
//		{
//			turn=CompareJobMachStarTime(job,mach);
//			if(turn==0)
//			{
//				gap[0]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]-JobStarTime[job-1][0];
//				gap[1]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]-JobStarTime[job-1][1];
//				gap[2]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]-JobStarTime[job-1][2];
//				if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//				{
//					for(i=0;i<8;i++)
//					{
//						MachStarEnd[mach-1][MachSche[mach-1]-1][i]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][i];
//					}  
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]=JobStarTime[job-1][0];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]=JobStarTime[job-1][1];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]=JobStarTime[job-1][2];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][3]=JobStarTime[job-1][0]+Time[job-1][JobSche[job-1]-1][0];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][4]=JobStarTime[job-1][1]+Time[job-1][JobSche[job-1]-1][1];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][5]=JobStarTime[job-1][2]+Time[job-1][JobSche[job-1]-1][2];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][6]=job;
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][7]=JobSche[job-1];
//				}
//			}
//			if(turn==1)
//			{
//				gap[0]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]-MachStarTime[mach-1][0];
//				gap[1]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]-MachStarTime[mach-1][1];
//				gap[2]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]-MachStarTime[mach-1][2];
//				if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//				{
//					for(i=0;i<8;i++)
//					{
//						MachStarEnd[mach-1][MachSche[mach-1]-1][i]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][i];
//					}  
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]=MachStarTime[mach-1][0];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]=MachStarTime[mach-1][1];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]=MachStarTime[mach-1][2];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][3]=MachStarTime[mach-1][0]+Time[job-1][JobSche[job-1]-1][0];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][4]=MachStarTime[mach-1][1]+Time[job-1][JobSche[job-1]-1][1];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][5]=MachStarTime[mach-1][2]+Time[job-1][JobSche[job-1]-1][2];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][6]=job;
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][7]=JobSche[job-1];
//				}
//			}
//		}
//		else
//		{
//			turn=CompareJobMachStarTime(job,mach,JobSche,JobStarEnd);
//			if(turn==0)
//			{
//				gap[0]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]-JobStarEnd[job-1][JobSche[job-1]-1-1][3];
//				gap[1]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]-JobStarEnd[job-1][JobSche[job-1]-1-1][4];
//				gap[2]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]-JobStarEnd[job-1][JobSche[job-1]-1-1][5];
//				if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//				{
//					for(i=0;i<8;i++)
//					{
//						MachStarEnd[mach-1][MachSche[mach-1]-1][i]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][i];
//					}  
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]=JobStarEnd[job-1][JobSche[job-1]-1-1][3];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]=JobStarEnd[job-1][JobSche[job-1]-1-1][4];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]=JobStarEnd[job-1][JobSche[job-1]-1-1][5];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][3]=JobStarEnd[job-1][JobSche[job-1]-1-1][3]+Time[job-1][JobSche[job-1]-1][0];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][4]=JobStarEnd[job-1][JobSche[job-1]-1-1][4]+Time[job-1][JobSche[job-1]-1][1];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][5]=JobStarEnd[job-1][JobSche[job-1]-1-1][5]+Time[job-1][JobSche[job-1]-1][2];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][6]=job;
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][7]=JobSche[job-1];
//				}
//			}
//			if(turn==1)
//			{
//				gap[0]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]-MachStarTime[mach-1][0];
//				gap[1]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]-MachStarTime[mach-1][1];
//				gap[2]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]-MachStarTime[mach-1][2];
//				if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//				{
//					for(i=0;i<8;i++)
//					{
//						MachStarEnd[mach-1][MachSche[mach-1]-1][i]=MachStarEnd[mach-1][MachSche[mach-1]-1-1][i];
//					}  
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][0]=MachStarTime[mach-1][0];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][1]=MachStarTime[mach-1][1];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][2]=MachStarTime[mach-1][2];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][3]=MachStarTime[mach-1][0]+Time[job-1][JobSche[job-1]-1][0];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][4]=MachStarTime[mach-1][1]+Time[job-1][JobSche[job-1]-1][1];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][5]=MachStarTime[mach-1][2]+Time[job-1][JobSche[job-1]-1][2];
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][6]=job;
//					MachStarEnd[mach-1][MachSche[mach-1]-1-1][7]=JobSche[job-1];
//				}
//			}
//		}
//	}
//	else
//	{
//		if(JobSche[job-1]==1)
//		{
//			for(i=0;i<MachSche[mach-1]-1;i++)
//			{
//				gap[0]=MachStarEnd[mach-1][i+1][0]-MachStarEnd[mach-1][i][3];
//				gap[1]=MachStarEnd[mach-1][i+1][1]-MachStarEnd[mach-1][i][4];
//				gap[2]=MachStarEnd[mach-1][i+1][2]-MachStarEnd[mach-1][i][5];
//				if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//				{
//				  turn=CompareJobMachStarTime(job,mach,MachSche,MachStarEnd,i);
//				  if(turn==1)
//				  {
//					 for(int j=MachSche[mach-1]-1;j>i+1;j--)
//					  {
//						for(int k=0;k<8;k++)
//						{
//							MachStarEnd[mach-1][j][k]=MachStarEnd[mach-1][j-1][k];
//						}
//					  }
//					MachStarEnd[mach-1][i+1][0]=MachStarEnd[mach-1][i][3];
//					MachStarEnd[mach-1][i+1][1]=MachStarEnd[mach-1][i][4];
//					MachStarEnd[mach-1][i+1][2]=MachStarEnd[mach-1][i][5];
//					MachStarEnd[mach-1][i+1][3]=MachStarEnd[mach-1][i][3]+Time[job-1][JobSche[job-1]-1][0];
//					MachStarEnd[mach-1][i+1][4]=MachStarEnd[mach-1][i][4]+Time[job-1][JobSche[job-1]-1][1];
//					MachStarEnd[mach-1][i+1][5]=MachStarEnd[mach-1][i][5]+Time[job-1][JobSche[job-1]-1][2];
//					MachStarEnd[mach-1][i+1][6]=job;
//					MachStarEnd[mach-1][i+1][7]=JobSche[job-1];
//					break;
//				  }
//				  else
//				  {
//					 gap[0]=MachStarEnd[mach-1][i+1][0]-JobStarTime[job-1][0];
//					 gap[1]=MachStarEnd[mach-1][i+1][1]-JobStarTime[job-1][1];
//					 gap[2]=MachStarEnd[mach-1][i+1][2]-JobStarTime[job-1][2];
//					 if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//					 {
//						for(int j=MachSche[mach-1]-1;j>i+1;j--)
//						{
//							for(int k=0;k<8;k++)
//							{
//								MachStarEnd[mach-1][j][k]=MachStarEnd[mach-1][j-1][k];
//							}
//						}
//						MachStarEnd[mach-1][i+1][0]=JobStarTime[job-1][0];
//						MachStarEnd[mach-1][i+1][1]=JobStarTime[job-1][1];
//						MachStarEnd[mach-1][i+1][2]=JobStarTime[job-1][2];
//						MachStarEnd[mach-1][i+1][3]=JobStarTime[job-1][0]+Time[job-1][JobSche[job-1]-1][0];
//						MachStarEnd[mach-1][i+1][4]=JobStarTime[job-1][1]+Time[job-1][JobSche[job-1]-1][1];
//						MachStarEnd[mach-1][i+1][5]=JobStarTime[job-1][2]+Time[job-1][JobSche[job-1]-1][2];
//						MachStarEnd[mach-1][i+1][6]=job;
//						MachStarEnd[mach-1][i+1][7]=JobSche[job-1];
//						break;
//					 }
//				}
//				}
//			}
//		}
//		else
//		{
//			for(i=0;i<MachSche[mach-1]-1;i++)
//			{
//				gap[0]=MachStarEnd[mach-1][i+1][0]-MachStarEnd[mach-1][i][3];
//				gap[1]=MachStarEnd[mach-1][i+1][1]-MachStarEnd[mach-1][i][4];
//				gap[2]=MachStarEnd[mach-1][i+1][2]-MachStarEnd[mach-1][i][5];
//				if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//				{
//				  turn=CompareJobMachStarTime(job,mach,JobSche,JobStarEnd,MachSche,MachStarEnd,i);
//				  if(turn==1)
//				  {
//					 for(int j=MachSche[mach-1]-1;j>i+1;j--)
//					  {
//						for(int k=0;k<8;k++)
//						{
//							MachStarEnd[mach-1][j][k]=MachStarEnd[mach-1][j-1][k];
//						}
//					  }
//					MachStarEnd[mach-1][i+1][0]=MachStarEnd[mach-1][i][3];
//					MachStarEnd[mach-1][i+1][1]=MachStarEnd[mach-1][i][4];
//					MachStarEnd[mach-1][i+1][2]=MachStarEnd[mach-1][i][5];
//					MachStarEnd[mach-1][i+1][3]=MachStarEnd[mach-1][i][3]+Time[job-1][JobSche[job-1]-1][0];
//					MachStarEnd[mach-1][i+1][4]=MachStarEnd[mach-1][i][4]+Time[job-1][JobSche[job-1]-1][1];
//					MachStarEnd[mach-1][i+1][5]=MachStarEnd[mach-1][i][5]+Time[job-1][JobSche[job-1]-1][2];
//					MachStarEnd[mach-1][i+1][6]=job;
//					MachStarEnd[mach-1][i+1][7]=JobSche[job-1];
//					break;
//				  }
//				  else
//				  {
//					 gap[0]=MachStarEnd[mach-1][i+1][0]-JobStarEnd[job-1][JobSche[job-1]-1-1][3];
//					 gap[1]=MachStarEnd[mach-1][i+1][1]-JobStarEnd[job-1][JobSche[job-1]-1-1][4];
//					 gap[2]=MachStarEnd[mach-1][i+1][2]-JobStarEnd[job-1][JobSche[job-1]-1-1][5];
//					 if(((gap[0]>Time[job-1][JobSche[job-1]-1][0])||(gap[0]==Time[job-1][JobSche[job-1]-1][0]))&&((gap[1]>Time[job-1][JobSche[job-1]-1][1])||(gap[1]==Time[job-1][JobSche[job-1]-1][1]))&&((gap[2]>Time[job-1][JobSche[job-1]-1][2])||(gap[2]==Time[job-1][JobSche[job-1]-1][2])))
//					 {
//						for(int j=MachSche[mach-1]-1;j>i+1;j--)
//						{
//							for(int k=0;k<8;k++)
//							{
//								MachStarEnd[mach-1][j][k]=MachStarEnd[mach-1][j-1][k];
//							}
//						}
//						MachStarEnd[mach-1][i+1][0]=JobStarEnd[job-1][JobSche[job-1]-1-1][3];
//						MachStarEnd[mach-1][i+1][1]=JobStarEnd[job-1][JobSche[job-1]-1-1][4];
//						MachStarEnd[mach-1][i+1][2]=JobStarEnd[job-1][JobSche[job-1]-1-1][5];
//						MachStarEnd[mach-1][i+1][3]=JobStarEnd[job-1][JobSche[job-1]-1-1][3]+Time[job-1][JobSche[job-1]-1][0];
//						MachStarEnd[mach-1][i+1][4]=JobStarEnd[job-1][JobSche[job-1]-1-1][4]+Time[job-1][JobSche[job-1]-1][1];
//						MachStarEnd[mach-1][i+1][5]=JobStarEnd[job-1][JobSche[job-1]-1-1][5]+Time[job-1][JobSche[job-1]-1][2];
//						MachStarEnd[mach-1][i+1][6]=job;
//						MachStarEnd[mach-1][i+1][7]=JobSche[job-1];
//						break;
//					 }
//				}
//				}
//			}
//		}
//	}
//
//}
void Calculate_Harmony(int seque[MaxJobNum*MaxOperNum][7],int FL[3])
{
	int i,j,z1,oc[MaxMachNum],MST[MaxMachNum][3],z3,z2[3],k,z,k2,k3,LS[MaxMachNum][MaxJobNum*MaxOperNum][8],flag,k1,z4,FT[MaxJobNum][3],tmp[7];
	for(i=1;i<=MachNum;i++)
	{
		oc[i] = 0; //Op_counter
		MST[i][0]=MachStarTime[i-1][0];
		MST[i][1]=MachStarTime[i-1][1];
		MST[i][2]=MachStarTime[i-1][2];
	}
	for(i=0;i<JobNum;i++)
	{
		FT[i][0]=JobStarTime[i][0];
		FT[i][1]=JobStarTime[i][1];
		FT[i][2]=JobStarTime[i][2];
	}
	for(i=0;i<TOperNum;i++)
	{
		
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
				}
				else
				{			
					MST[seque[i][2]][0] = FT[seque[i][0]][0] + seque[i][3];
					MST[seque[i][2]][1] = FT[seque[i][0]][1] + seque[i][5];
					MST[seque[i][2]][2] = FT[seque[i][0]][2] + seque[i][6];
					FT[seque[i][0]][0] = MST[seque[i][2]][0];
					FT[seque[i][0]][1] = MST[seque[i][2]][1];
					FT[seque[i][0]][2] = MST[seque[i][2]][2];
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
				
				if( (flag == 0))
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
					}
					else
					{
						
						MST[z1][0] = FT[z3][0] + z2[0];
						MST[z1][1] = FT[z3][1] + z2[1];
						MST[z1][2] = FT[z3][2] + z2[2];
						FT[z3][0] = MST[z1][0];
						FT[z3][1] = MST[z1][1];
						FT[z3][2] = MST[z1][2];
					}
					oc[seque[i][2]]++;
				}
			}
	}
	FL[0] = 0;
	FL[1] = 0;
	FL[2] = 0;
	for(i=0;i<JobNum;i++)
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
void Calculate_HM()
{
	int i;
	for(i=0;i<HMS;i++)
	{
		for(int j=0;j<TOperNum;j++)
			{
				for(int k=0;k<7;k++)
				{
					seque[j][k]=HM[i][j][k];
				}
			}
		Calculate_Harmony(seque,FL);
		Result[i][0][0]=FL[0];Result[i][0][1]=FL[1];Result[i][0][2]=FL[2];
		cout<<Result[i][0][0]<<","<<Result[i][0][1]<<","<<Result[i][0][2]<<endl;
		
	}
}
//void ImprovisingNewHar(int I)
//{
//	int i,j;
//    double HMCR=0.8;
//    double PAR=0.5;
//    double R1;
//	int count;
//	int R2,temp;
//	//machine assignment
//	if(1/*(I==0)||(I%10)==19*/)
//	{
//		for(int num=0;num<HMS;num+=2)
//		{
//			count=0;
//			for(i=0;i<JobNum;i++)
//				{
//					for(j=0;j<OperNum[i];j++)
//					{
//	            
//						R1=(rand()%100)*0.01;
//						if(R1<HMCR)
//						{
//							//srand((unsigned) time(NULL));
//							R2=rand()%2;
//							if(R2==1)
//							{
//							NewHM[num][count+j]=HM[num][count+j];
//							}
//							else
//							{
//							  NewHM[num][count+j]=HM[num+1][count+j];
//							}
//							//srand((unsigned) time(NULL));
//							R1=(rand()%100)*0.01;
//							if(R1<PAR)
//							{
//								for(int k=0;k<SeleMachNum[i][j];k++)
//								{
//									if(SeleMachSet[i][j][k]==NewHM[num][count+j])
//									{
//										int K=(k+1)%SeleMachNum[i][j];
//										NewHM[num][count+j]=SeleMachSet[i][j][K];
//										break;
//									}
//								}
//							}
//							
//						}
//						else
//						{
//							//srand((unsigned) time(NULL));
//							int R2=rand()%SeleMachNum[i][j];
//							NewHM[num][count+j]=SeleMachSet[i][j][R2];
//						}
//					}
//					count+=OperNum[i];
//				}
//			count=0;
//			for(i=0;i<JobNum;i++)
//				{
//					for(j=0;j<OperNum[i];j++)
//					{
//	            
//						R1=(rand()%100)*0.01;
//						if(R1<HMCR)
//						{
//							//srand((unsigned) time(NULL));
//							R2=rand()%2;
//							if(R2==0)
//							{
//
//								NewHM[num+1][count+j]=HM[num][count+j]; 
//							}
//							else
//							{
//								NewHM[num+1][count+j]=HM[num+1][count+j];
//							}
//							//srand((unsigned) time(NULL));
//							R1=(rand()%100)*0.01;
//							if(R1<PAR)
//							{
//								for(int k=0;k<SeleMachNum[i][j];k++)
//								{
//									if(SeleMachSet[i][j][k]==NewHM[num+1][count+j])
//									{
//										int K=(k+1)%SeleMachNum[i][j];
//										NewHM[num+1][count+j]=SeleMachSet[i][j][K];
//										break;
//									}
//								}
//							}
//							
//						}
//						else
//						{
//							//srand((unsigned) time(NULL));
//							int R2=rand()%SeleMachNum[i][j];
//							NewHM[num+1][count+j]=SeleMachSet[i][j][R2];
//						}
//					}
//					count+=OperNum[i];
//				}
//		}
//		//for(int num=0;num<HMS;num+=2)
//		//{
//		//	count=0;
//		//	for(i=0;i<JobNum;i++)
//		//		{
//		//			for(j=0;j<OperNum[i];j++)
//		//			{
//	 //           
//		//				R1=(rand()%100)*0.01;
//		//				if(R1<HMCR)
//		//				{
//		//					//srand((unsigned) time(NULL));
//		//					R2=rand()%TOperNum;
//		//					//cout<<R2<<",";
//		//					if((count+j)<R2)
//		//					{
//		//					NewHM[num][count+j]=HM[num][count+j];
//		//					}
//		//					else
//		//					{
//		//					  NewHM[num][count+j]=BestHM[count+j];
//		//					}
//		//					//srand((unsigned) time(NULL));
//		//					R1=(rand()%100)*0.01;
//		//					if(R1<PAR)
//		//					{
//		//						for(int k=0;k<SeleMachNum[i][j];k++)
//		//						{
//		//							if(SeleMachSet[i][j][k]==NewHM[num][count+j])
//		//							{
//		//								int K=(k+1)%SeleMachNum[i][j];
//		//								NewHM[num][count+j]=SeleMachSet[i][j][K];
//		//								break;
//		//							}
//		//						}
//		//					}
//		//					
//		//				}
//		//				else
//		//				{
//		//					//srand((unsigned) time(NULL));
//		//					int R2=rand()%SeleMachNum[i][j];
//		//					NewHM[num][count+j]=SeleMachSet[i][j][R2];
//		//				}
//		//			}
//		//			count+=OperNum[i];
//		//		}
//		//	count=0;
//		//	for(i=0;i<JobNum;i++)
//		//		{
//		//			for(j=0;j<OperNum[i];j++)
//		//			{
//	 //           
//		//				R1=(rand()%100)*0.01;
//		//				if(R1<HMCR)
//		//				{
//		//					//srand((unsigned) time(NULL));
//		//					R2=rand()%TOperNum;
//		//					//cout<<R2<<",";
//		//					if((count+j)<R2)
//		//					{
//		//					NewHM[num+1][count+j]=HM[num+1][count+j];
//		//					}
//		//					else
//		//					{
//		//					  NewHM[num+1][count+j]=BestHM[count+j];
//		//					}
//		//					//srand((unsigned) time(NULL));
//		//					R1=(rand()%100)*0.01;
//		//					if(R1<PAR)
//		//					{
//		//						for(int k=0;k<SeleMachNum[i][j];k++)
//		//						{
//		//							if(SeleMachSet[i][j][k]==NewHM[num+1][count+j])
//		//							{
//		//								int K=(k+1)%SeleMachNum[i][j];
//		//								NewHM[num+1][count+j]=SeleMachSet[i][j][K];
//		//								break;
//		//							}
//		//						}
//		//					}
//		//					
//		//				}
//		//				else
//		//				{
//		//					//srand((unsigned) time(NULL));
//		//					int R2=rand()%SeleMachNum[i][j];
//		//					NewHM[num+1][count+j]=SeleMachSet[i][j][R2];
//		//				}
//		//			}
//		//			count+=OperNum[i];
//		//		}
//		//}
//	//system("pause");
//	//operation sequence
//	for(i=0;i<HMS;i++)
//		{
//			for(j=TOperNum;j<2*TOperNum;j++)
//			{
//				NewHM[i][j]=0;
//			}
//		}
//	for(i=0;i<HMS;i+=2)
//	{
//		for(j=TOperNum;j<2*TOperNum;j++)
//		{
//			if((HM[i][j]%2)==1)
//			{
//				NewHM[i][j]=HM[i][j];
//			}
//			else
//			{
//				NewHM[i+1][j]=HM[i][j];
//			}
//		}
//		for(j=TOperNum;j<2*TOperNum;j++)
//		{            
//			if((HM[i+1][j]%2)==1)
//			{
//				int K=TOperNum;
//				for(int k=K;k<2*TOperNum;k++)
//				{
//					if(NewHM[i+1][k]==0)
//					{
//						NewHM[i+1][k]=HM[i+1][j];
//						K=k+1;
//						break;
//					}
//				}
//			}
//			else
//			{
//				int K=TOperNum;
//				for(int k=K;k<2*TOperNum;k++)
//				{
//					if(NewHM[i][k]==0)
//					{
//						NewHM[i][k]=HM[i+1][j];
//						K=k+1;
//						break;
//					}
//				}
//			}
//		}
//	}
//	//for(i=0;i<HMS;i+=2)
//	//{
//
//	//	R2=rand()%TOperNum;
//	//	//cout<<R2;
//	//	int c1[MaxJobNum];
//	//	int c2[MaxJobNum];
//	//	for(j=0;j<JobNum;j++)
//	//	{
//	//		c1[j]=0;
//	//		c2[j]=0;
//	//	}
//	//	for(j=TOperNum;j<TOperNum+R2;j++)
//	//	{
//	//		NewHM[i][j]=HM[i][j];
//	//		c1[HM[i][j]-1]++;
//	//		NewHM[i+1][2*TOperNum-(j-TOperNum)-1]=BestHM[2*TOperNum-(j-TOperNum)-1];
//	//		c2[BestHM[2*TOperNum-(j-TOperNum)-1]-1]++;
//	//	}
//	//	int K=TOperNum;
//	//	for(j=TOperNum+R2;j<2*TOperNum;j++)
//	//	{            
//	//		
//	//		for(int k=K;k<2*TOperNum;k++)
//	//		{
//	//		   if(c1[BestHM[k]-1]>0)
//	//		   {
//	//			  c1[BestHM[k]-1]--;
//	//		   }
//	//		   else
//	//		   {
//	//			  NewHM[i][j]=BestHM[k];
//	//			  K=k+1;
//	//			  break;
//	//		   }
//	//		}
//	//	}
//	//	K=TOperNum;
//	//	for(j=TOperNum+R2;j<2*TOperNum;j++)
//	//	{ 
//	//		for(int k=K;k<2*TOperNum;k++)
//	//		{
//	//		   if(c2[HM[i][2*TOperNum-(k-TOperNum)-1]-1]>0)
//	//		   {
//	//			  c2[HM[i][2*TOperNum-(k-TOperNum)-1]-1]--;
//	//		   }
//	//		   else
//	//		   {
//	//			  NewHM[i+1][2*TOperNum-(j-TOperNum)-1]=HM[i][2*TOperNum-(k-TOperNum)-1];
//	//			  K=k+1;
//	//			  break;
//	//		   }
//	//		}
//	//	}
//	//} 
//	}
//	/*for(i=0;i<HMS;i++)
//	{
//		int R1;
//			R1=rand()%TOperNum;
//		R2=rand()%TOperNum;
//		if(R1<R2)
//			{
//				for(j=TOperNum+R1;j<(R1+R2)/2+TOperNum;j++)
//					{
//						temp=HM[i][R1+j];
//						HM[i][R1+j]=HM[i][R2-j];
//						HM[i][R2-j]=temp;
//					}
//			}
//		else
//			{
//				if(R1>R2)
//					{
//						temp=R1;
//						R1=R2;
//						R2=temp;
//						for(j=TOperNum+R1;j<(R1+R2)/2+TOperNum;j++)
//						{
//							temp=HM[i][R1+j];
//							HM[i][R1+j]=HM[i][R2-j];
//							HM[i][R2-j]=temp;
//						}
//					}
//				else
//					{
//						 for(j=TOperNum+R1;j<(R1+TOperNum)/2+TOperNum;j++)
//							{
//								temp=HM[i][R1+j];
//								HM[i][R1+j]=HM[i][R2-j];
//								HM[i][R2-j]=temp;
//							}
//					}
//			}
//	}*/ 
//	//LocSear();
//}
//void Calculate_NewHM()
//{
//	int i;
//	for(i=0;i<HMS;i++)
//	{
//		Calculate_Harmony(i,NewHM);
//		
//	}
//}
//void RandomNewBestHM()
//{
//	int I=Itercount[1],i;
//	RandomInitMA(I);  	 
//	RandomInitOS(I);
//	for(i=0;i<2*TOperNum;i++)
//	{
//		BestHM[i]=HM[I][i];
//	}
//	Calculate_Harmony(I,HM);
//	for(i=0;i<3;i++)
//	{
//		BestResult[0][i]=Result[I][0][i];
//	}
//}
//void LocalSearchMA(int J)
//{
//	for(int i=0;i<HMS;i++)
//	{
//		NewHM[i][J]=(rand()%MachNum)+1;
//	}
//}
//void LocalSearchOS(int J)
//{
//	int R=rand()%TOperNum;
//	for(int i=0;i<HMS;i++)
//	{
//		int temp=NewHM[i][J];
//		NewHM[i][J]=NewHM[i][J+1];
//		NewHM[i][J+1]=temp;
//	}
//}
void DHS_fFJSP(time_t start)
{
	Inializ_HM();
	Calculate_HM();
	//ofstream outfile;
	//int i;
	//int flag;
	///*outfile.open("Result.csv",ios::app);
	//for(int i=0;i<HMS;i++)
	//{
	//	for(int j=0;j<3;j++)
	//	{
	//		outfile<<Result[i][0][j]<<",";
	//	}
	//	outfile<<endl;
	//}
	//outfile.close();*/
	///*outfile.open("BestResult.csv",ios::app);
	//	for(int j=0;j<3;j++)
	//	{
	//		outfile<<BestResult[0][j]<<",";
	//	}
	//	outfile<<endl;
	//	outfile.close();*/
	///*outfile.open("InitHM.csv",ios::app);
	//for(i=0;i<2;i++)
	//{					   
	//	for(int j=0;j<2*TOperNum;j++)
	//	{
	//		outfile<<HM[i][j]<<",";
	//	}
	//	outfile<<endl;


	//}
	//outfile.close();
	//outfile.open("Result.csv",ios::app);
	//for(i=0;i<2;i++)
	//{					   
	//	outfile<<Result[i][0][0]<<","<<Result[i][0][1]<<","<<Result[i][0][2]<<endl;
	//}
	//outfile.close();*/
	////system("pause");
	//for(i=0;i<Gen;i++)
	//{
	//	iter=i;
	//	ImprovisingNewHar(i);
	//	Calculate_NewHM();
	//	/*for(int j=0;j<TOperNum;j++)
	//	{
	//		LocalSearchMA(j);
	//		Calculate_NewHM();
	//	}
	//	for(int j=TOperNum;j<2*TOperNum;j+=2)
	//	{
	//		LocalSearchOS(j);
	//		Calculate_NewHM();
	//	}
	//	outfile.open("NewHM.csv",ios::app);
	//	for(int k=0;k<2;k++)
	//	{					   
	//		for(int j=0;j<2*TOperNum;j++)
	//		{
	//			outfile<<HM[k][j]<<",";
	//		}
	//		outfile<<endl;
	//	}
	//	outfile.close();
	//	outfile.open("Result.csv",ios::app);
	//	for(int k=0;k<2;k++)
	//	{					   
	//		outfile<<Result[k][0][0]<<","<<Result[k][0][1]<<","<<Result[k][0][2]<<endl;
	//	}
	//	outfile.close();*/
	//	//system("pause");
	//	if((iter%20)==19)
	//	{
	//		for(int k=0;k<HMS;k++)
	//		{
	//			int R=rand()%HMS;
	//			//cout<<R<<",";
	//			int temp[2*MaxJobNum*MaxOperNum];
	//			int temp1[3];
	//			for(int j=0;j<2*TOperNum;j++)
	//			{
	//				temp[j]=HM[k][j];
	//				HM[k][j]=HM[R][j];
	//				HM[R][j]=temp[j];
	//			}
	//			for(int j=0;j<3;j++)
	//			{
	//				temp1[j]=Result[k][0][j];
	//				Result[k][0][j]=Result[R][0][j];
	//				Result[R][0][j]=temp1[j];
	//			}
	//		}
	//	//	//system("pause");
	//	}
	//	//cout<<Result[0][0][0]<<",";
	//	/*if(flag==BestResult[0][0])
	//	{
	//		Itercount[0]++;
	//	}
	//	if(Itercount[0]>100)
	//	{
	//	  RandomNewBestHM();
	//	  Itercount[0]=0;
	//	}*/
	//	if(i==0)
	//	{
	//		outfile.open("BestResult.csv",ios::app);
	//		for(int j=0;j<3;j++)
	//		{
	//			outfile<<BestResult[0][j]<<",";
	//		}
	//		outfile<<endl;
	//		outfile.close(); 
	//	
	//		//cout<<BestResult[0][0]<<","<<BestResult[0][1]<<","<<BestResult[0][2]<<","<<endl;
	//	}
	//} 
	//flag=BestResult[0][0];
	/*outfile.open("Result.csv",ios::app);
	for(int i=0;i<HMS;i++)
	{
		for(int j=0;j<3;j++)
		{
			outfile<<Result[i][0][j]<<",";
		}
		outfile<<endl;
	}
	outfile.close();*/
	//system("pause");
}
int main()
{
	Instance case1;
	case1.Setinstance(JobNum,MachNum);
	SetOperNum();
	SetSeleMachNum();
	SetSeleMachSet();
	SetProceTime1();
	SetProceTime2();
	SetProceTime3();
	SetJobStarTime();
	//inializ_DueDate();
	time_t start;
	start=clock();
	srand((unsigned) time(NULL)+JobNum*MachNum*TOperNum*1000);
	for(int i=0;i<Repeat;i++)
	{
	DHS_fFJSP(start);	
	ofstream outfile;
		/*outfile.open("Result.csv",ios::app);
		for(int i=0;i<HMS;i++)
		{
			for(int j=0;j<3;j++)
			{
				outfile<<Result[i][0][j]<<",";
			}
			outfile<<endl;
		}
		outfile.close();  */

		outfile.open("BestResult.csv",ios::app);
		for(int j=0;j<3;j++)
		{
			outfile<<BestResult[0][j]<<",";
		}
		outfile<<endl;
		outfile<<"-------------------------------------------"<<endl;
		outfile.close();
	}
	/*outfile.open("acc5.csv",ios::app);
	for(int i=0;i<JobNum;i++)
	{
		for(int j=0;j<OperNum[i];j++)
		{
			 for(int k=0;k<MachNum;k++)
			 {
				outfile<<k+1<<endl;
			 }
		}
	}
	outfile.close();
	outfile.open("JobStarTime.csv",ios::app);
	for(int i=0;i<JobNum;i++)
	{
		for(int j=0;j<3;j++)
		{
			 outfile<<"0"<<endl;
		}
	}
	outfile.close();
	outfile.open("MachStarTime.csv",ios::app);
	for(int i=0;i<MachNum;i++)
	{
		for(int j=0;j<3;j++)
		{
			 outfile<<"0"<<endl;
		}
	}
	outfile.close();  
	Calculate_DueDate(); */

}



