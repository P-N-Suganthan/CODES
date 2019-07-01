// ABC for rescheduling.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ABC for rescheduling.h"
//#include<StdAfx.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/types.h>
#include <time.h>
#define u01 ((rand())/(RAND_MAX+1.0))

#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a>b)?b:a)

#define OPERC 0.05
#define COUNTER 1 

#define PB1 0.3

#define RUN 1

#define Empop 50
#define Onpop 100
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
int N,M,toper,Jb[Max_Op],NM_O[NJb][Max_Op],M_O[NJb][Max_Op][Mac],p[NJb][Max_Op][Mac];
int Op[Empop][T_Oper][5],MV[Empop][1000],OPT[RUN];//job number, operation number,  machine number, processing time 
int F[Empop],Optimal,Op_seq[T_Oper][5],COUNT = 0;
int Onps[Onpop][T_Oper][5],On_F[Onpop],On,On_1,On_2,Onlook[Onpop];
int ReFT[NJb],ReMST[Mac];  //Machine_Spantime and job spantime
int repeattime=30; 
void Update_Optimal(int l)
{
	int i;
	Optimal = F[l];
	for(i=0;i<toper;i++)
	{
        Op_seq[i][0] = Op[l][i][0];//  job number
		Op_seq[i][1] = Op[l][i][1];		 //operation number
		Op_seq[i][2] = Op[l][i][2];	// processing machine
		Op_seq[i][3] = Op[l][i][3];	// processing time
		Op_seq[i][4] = Op[l][i][4];	
	}
	/*FILE *fp;
	fp = fopen("best resuts.txt","a");
	fprintf(fp,"%d \n",Optimal);*/
}

void Init_pop(int l)
{
	int i,j,k,z,z1,z2,z3,z4,r,r1,c,k1,k2,k3,tmp1[NJb],tmp2[NJb][Max_Op],tmp3,FT[NJb],MST[Mac],tmp[5],oc[Mac],LS[Mac][T_Oper][4],flag;
	int Min,tmp4[Max_Op],TA[Mac],ADT[Mac];
	
	if(u01 < PB1)
	{
		for(i=0;i<N;i++)
		{
			tmp1[i] = Jb[i];
			for(j=0;j<Jb[i];j++)
				tmp2[i][j] = 0;
		}
		c = 0;
		tmp3 = N;
		while(1)
		{
			do
			{
				r = (int) (u01*N);
			}while(tmp1[r] == 0);

			Op[l][c][0] = r;
			j = 0;
			do
			{
				Op[l][c][1] = j;
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

				Op[l][c][2] = M_O[r][j][r1];
				Op[l][c][3] = p[r][j][r1];
			}
			else
			{
				Op[l][c][2] = M_O[r][j][0];
				Op[l][c][3] = p[r][j][0];
			}
			if(Op[l][c][0] == 0)
				Op[l][c][4] = Op[l][c][1]; //use for giving unique sequence number
			else
			{
				j = 0;
				for(i=0;i<Op[l][c][0];i++)
					j += Jb[i];
				Op[l][c][4] = j + Op[l][c][1];
			}
			c++;

			if(--tmp1[r] == 0)
			{
				if(--tmp3 == 0)
					break;
			}
		}
	}
	else
	{
		for(i=1;i<=M;i++)
			TA[i] = 0;
		for(i=0;i<N;i++)
		{
			tmp1[i] = Jb[i];
			for(j=0;j<Jb[i];j++)
				tmp2[i][j] = 0;
		}
		c = 0;
		tmp3 = N;
		while(1)
		{
			do
			{
				r = (int) (u01*N);
			}while(tmp1[r] == 0);

			Op[l][c][0] = r;
			j = 0;
			do
			{
				Op[l][c][1] = j;
			}while(tmp2[r][j++] == 1);

			tmp2[r][--j] = 1;

			if(NM_O[r][j] > 1)
			{
				for(i=0;i<NM_O[r][j];i++)
					ADT[M_O[r][j][i]] = TA[M_O[r][j][i]] + p[r][j][i];

				Min = 50000;
				for(i=0;i<NM_O[r][j];i++)
				{
					if(Min > ADT[M_O[r][j][i]])
						Min = ADT[M_O[r][j][i]];
				}
				z1 = 0;
				for(i=0;i<NM_O[r][j];i++)
				{
					if(Min == ADT[M_O[r][j][i]])
						tmp4[z1++] = i;
				}
				if(z1 == 1)
					r1 = tmp4[0];
				else
				{
					z2 = (int)(u01*z1); //tie break
					r1 = tmp4[z2];
				}
		
				Op[l][c][2] = M_O[r][j][r1];
				Op[l][c][3] = p[r][j][r1];

				TA[M_O[r][j][r1]] = p[r][j][r1];
			}
			else
			{
				Op[l][c][2] = M_O[r][j][0];
				Op[l][c][3] = p[r][j][0];
	
				TA[M_O[r][j][0]] = p[r][j][0];
			}
			if(Op[l][c][0] == 0)
				Op[l][c][4] = Op[l][c][1]; //use for giving unique sequence number
			else
			{
				j = 0;
				for(i=0;i<Op[l][c][0];i++)
					j += Jb[i];
				Op[l][c][4] = j + Op[l][c][1];
			}
			c++;

			if(--tmp1[r] == 0)
			{
				if(--tmp3 == 0)
					break;
			}
		}
	}
	//Evaluate makespan
	//Left-Shift operator
	for(i=1;i<=M;i++)
	{
		oc[i] = 0; //Op_counter
		MST[i]=ReMST[i];
	}
	for(i=0;i<N;i++)
	{
		FT[i]=ReFT[i];
	}
	for(i=0;i<toper;i++)
	{
		//if(MST[Op[l][i][2]] == 0 && FT[Op[l][i][0]] == 0)
		//{
		//	MST[Op[l][i][2]] = Op[l][i][3];
		//	FT[Op[l][i][0]] = Op[l][i][3];
		//	continue;
		//}
		//if(MST[Op[l][i][2]] == 0 && FT[Op[l][i][0]] != 0)
		//{
		//	LS[Op[l][i][2]][oc[Op[l][i][2]]][0] = 0;
		//	LS[Op[l][i][2]][oc[Op[l][i][2]]][1] = FT[Op[l][i][0]];
		//	LS[Op[l][i][2]][oc[Op[l][i][2]]][2] = Op[l][i][0];	//job
		//	LS[Op[l][i][2]][oc[Op[l][i][2]]++][3] = Op[l][i][1]; //op

		//	FT[Op[l][i][0]] += Op[l][i][3];
		//	MST[Op[l][i][2]] = FT[Op[l][i][0]];

		//	continue;
		//}

		//if(MST[Op[l][i][2]] != 0 && FT[Op[l][i][0]] == 0)
		//{
		//	if(oc[Op[l][i][2]] == 0)
		//	{
		//		MST[Op[l][i][2]] += Op[l][i][3];
		//		FT[Op[l][i][0]] = MST[Op[l][i][2]];
		//	}
		//	else
		//	{
		//		z1 = Op[l][i][2];
		//		z2 = Op[l][i][3];
		//		z3 = Op[l][i][0];

		//		flag = 0;

		//		for(j=0;j<oc[Op[l][i][2]];j++)
		//		{
		//			if(Op[l][i][3]==(LS[Op[l][i][2]][j][1]-LS[Op[l][i][2]][j][0]))
		//			{
		//				FT[Op[l][i][0]] += LS[Op[l][i][2]][j][1];

		//				for(z=0;z<i;z++)
		//				{
		//					if((Op[l][z][0] == LS[Op[l][i][2]][j][2]) && (Op[l][z][1] == LS[Op[l][i][2]][j][3]))
		//						break;
		//				}
		//				for(k=j+1;k<oc[Op[l][i][2]];k++)
		//				{
		//					LS[Op[l][i][2]][k-1][0] = LS[Op[l][i][2]][k][0];

		//					LS[Op[l][i][2]][k-1][1] = LS[Op[l][i][2]][k][1];

		//					LS[Op[l][i][2]][k-1][2] = LS[Op[l][i][2]][k][2];

		//					LS[Op[l][i][2]][k-1][3] = LS[Op[l][i][2]][k][3];
		//				}
		//				oc[Op[l][i][2]]--;

		//				tmp[0] = Op[l][i][0];
		//				tmp[1] = Op[l][i][1];
		//				tmp[2] = Op[l][i][2];
		//				tmp[3] = Op[l][i][3];
		//				tmp[4] = Op[l][i][4];

		//				for(k=i-1;k>=z;k--)
		//				{
		//					Op[l][k+1][0] = Op[l][k][0];
		//					Op[l][k+1][1] = Op[l][k][1];
		//					Op[l][k+1][2] = Op[l][k][2];
		//					Op[l][k+1][3] = Op[l][k][3];
		//					Op[l][k+1][4] = Op[l][k][4];
		//				}

		//				Op[l][z][0] = tmp[0];
		//				Op[l][z][1] = tmp[1];
		//				Op[l][z][2] = tmp[2];
		//				Op[l][z][3] = tmp[3];
		//				Op[l][z][4] = tmp[4];
		//				//-----------------------------------------------
		//				flag = 1;
		//				break;
		//			}
		//			if(Op[l][i][3] < (LS[Op[l][i][2]][j][1] - LS[Op[l][i][2]][j][0]))
		//			{
		//				LS[Op[l][i][2]][j][0] += Op[l][i][3];

		//				FT[Op[l][i][0]] = LS[Op[l][i][2]][j][0];

		//				for(z=0;z<i;z++)
		//				{
		//					if((Op[l][z][0] == LS[Op[l][i][2]][j][2]) && (Op[l][z][1] == LS[Op[l][i][2]][j][3]))
		//						break;
		//				}
		//				
		//				tmp[0] = Op[l][i][0];
		//				tmp[1] = Op[l][i][1];
		//				tmp[2] = Op[l][i][2];
		//				tmp[3] = Op[l][i][3];
		//				tmp[4] = Op[l][i][4];

		//				for(k=i-1;k>=z;k--)
		//				{
		//					Op[l][k+1][0] = Op[l][k][0];
		//					Op[l][k+1][1] = Op[l][k][1];
		//					Op[l][k+1][2] = Op[l][k][2];
		//					Op[l][k+1][3] = Op[l][k][3];
		//					Op[l][k+1][4] = Op[l][k][4];
		//				}

		//				Op[l][z][0] = tmp[0];
		//				Op[l][z][1] = tmp[1];
		//				Op[l][z][2] = tmp[2];
		//				Op[l][z][3] = tmp[3];
		//				Op[l][z][4] = tmp[4];

		//				flag = 1;
		//				break;
		//			}
		//		}
		//		if((j == oc[z1]) && (flag == 0))
		//		{
		//			MST[z1] += z2;
		//			FT[z3] = MST[z1];
		//		}
		//	}
		//	continue;
		//}
		if(oc[Op[l][i][2]] == 0)
			{
				if(MST[Op[l][i][2]] >= FT[Op[l][i][0]])
				{
					MST[Op[l][i][2]] += Op[l][i][3];
					FT[Op[l][i][0]] = MST[Op[l][i][2]];
				}
				else
				{
					LS[Op[l][i][2]][oc[Op[l][i][2]]][0] = MST[Op[l][i][2]];
					LS[Op[l][i][2]][oc[Op[l][i][2]]][1] = FT[Op[l][i][0]];
					LS[Op[l][i][2]][oc[Op[l][i][2]]][2] = Op[l][i][0];
					LS[Op[l][i][2]][oc[Op[l][i][2]]++][3] = Op[l][i][1];
			
					MST[Op[l][i][2]] = FT[Op[l][i][0]] + Op[l][i][3];
					FT[Op[l][i][0]] = MST[Op[l][i][2]];
				}
			}
		else
			{
				z1 = Op[l][i][2];
				z2 = Op[l][i][3];
				z3 = Op[l][i][0];
				z4 = Op[l][i][1];

				flag = 0;

				for(j=0;j<oc[Op[l][i][2]];j++)
				{
					if((Op[l][i][3]==(LS[Op[l][i][2]][j][1]-LS[Op[l][i][2]][j][0])) && ((FT[Op[l][i][0]]+Op[l][i][3])==LS[Op[l][i][2]][j][1]))
					{
						for(z=0;z<i;z++)
						{
							if((Op[l][z][0] == LS[Op[l][i][2]][j][2]) && (Op[l][z][1] == LS[Op[l][i][2]][j][3]))
								break;
						}
						for(k=z;k<i;k++)
						{
							if((Op[l][i][0] == Op[l][k][0]) && (Op[l][i][1] > Op[l][k][1]))
								break;
						}
						if(k != i)
							continue;
						else
						{
							FT[Op[l][i][0]] += Op[l][i][3];	

							for(k=j+1;k<oc[Op[l][i][2]];k++)
							{
								LS[Op[l][i][2]][k-1][0] = LS[Op[l][i][2]][k][0];
								LS[Op[l][i][2]][k-1][1] = LS[Op[l][i][2]][k][1];
								LS[Op[l][i][2]][k-1][2] = LS[Op[l][i][2]][k][2];
								LS[Op[l][i][2]][k-1][3] = LS[Op[l][i][2]][k][3];
							}
							oc[Op[l][i][2]]--;

							tmp[0] = Op[l][i][0];
							tmp[1] = Op[l][i][1];
							tmp[2] = Op[l][i][2];
							tmp[3] = Op[l][i][3];
							tmp[4] = Op[l][i][4];

							for(k=i-1;k>=z;k--)
							{
								Op[l][k+1][0] = Op[l][k][0];
								Op[l][k+1][1] = Op[l][k][1];
								Op[l][k+1][2] = Op[l][k][2];
								Op[l][k+1][3] = Op[l][k][3];
								Op[l][k+1][4] = Op[l][k][4];
							}

							Op[l][z][0] = tmp[0];
							Op[l][z][1] = tmp[1];
							Op[l][z][2] = tmp[2];
							Op[l][z][3] = tmp[3];
							Op[l][z][4] = tmp[4];
							//-----------------------------------------------
							flag = 1;
							break;
						}
					}

					if((Op[l][i][3]<(LS[Op[l][i][2]][j][1]-LS[Op[l][i][2]][j][0])) && ((FT[Op[l][i][0]]+Op[l][i][3])==LS[Op[l][i][2]][j][1]))
					{
						for(z=0;z<i;z++)
						{
							if((Op[l][z][0] == LS[Op[l][i][2]][j][2]) && (Op[l][z][1] == LS[Op[l][i][2]][j][3]))
								break;
						}
						for(k=z;k<i;k++)
						{
							if((Op[l][i][0] == Op[l][k][0]) && (Op[l][i][1] > Op[l][k][1]))
								break;
						}

						if(k != i)
							continue;
						else
						{
							LS[Op[l][i][2]][j][1] = FT[Op[l][i][0]];
							LS[Op[l][i][2]][j][2] = Op[l][i][0];
							LS[Op[l][i][2]][j][3] = Op[l][i][1];

							FT[Op[l][i][0]] += Op[l][i][3];
							tmp[0] = Op[l][i][0];
							tmp[1] = Op[l][i][1];
							tmp[2] = Op[l][i][2];
							tmp[3] = Op[l][i][3];
							tmp[4] = Op[l][i][4];

							for(k=i-1;k>=z;k--)
							{
								Op[l][k+1][0] = Op[l][k][0];
								Op[l][k+1][1] = Op[l][k][1];
								Op[l][k+1][2] = Op[l][k][2];
								Op[l][k+1][3] = Op[l][k][3];
								Op[l][k+1][4] = Op[l][k][4];
							}
							Op[l][z][0] = tmp[0];
							Op[l][z][1] = tmp[1];
							Op[l][z][2] = tmp[2];
							Op[l][z][3] = tmp[3];
							Op[l][z][4] = tmp[4];
							//-----------------------------------------------
							flag = 1;
							break;
						}
					}
					if((Op[l][i][3]<(LS[Op[l][i][2]][j][1]-LS[Op[l][i][2]][j][0])) && (LS[Op[l][i][2]][j][0] < FT[Op[l][i][0]]) && ((FT[Op[l][i][0]]+Op[l][i][3])<LS[Op[l][i][2]][j][1]))
					{
						for(z=0;z<i;z++)
						{
							if((Op[l][z][0] == LS[Op[l][i][2]][j][2]) && (Op[l][z][1] == LS[Op[l][i][2]][j][3]))
								break;
						}
						for(k=z;k<i;k++)
						{
							if((Op[l][i][0] == Op[l][k][0]) && (Op[l][i][1] > Op[l][k][1]))
								break;
						}
						if(k != i)
							continue;
						else
						{
							k1 = LS[Op[l][i][2]][j][1];
							k2 = LS[Op[l][i][2]][j][2];
							k3 = LS[Op[l][i][2]][j][3];

							LS[Op[l][i][2]][j][1] = FT[Op[l][i][0]];
							LS[Op[l][i][2]][j][2] = Op[l][i][0];
							LS[Op[l][i][2]][j][3] = Op[l][i][1];

							FT[Op[l][i][0]] += Op[l][i][3];
							for(k=oc[Op[l][i][2]]-1;k>j;k--)
							{
								LS[Op[l][i][2]][k+1][0] = LS[Op[l][i][2]][k][0];

								LS[Op[l][i][2]][k+1][1] = LS[Op[l][i][2]][k][1];

								LS[Op[l][i][2]][k+1][2] = LS[Op[l][i][2]][k][2];
								LS[Op[l][i][2]][k+1][3] = LS[Op[l][i][2]][k][3];
							}
							oc[Op[l][i][2]]++;

							LS[Op[l][i][2]][j+1][0] = FT[Op[l][i][0]];
							LS[Op[l][i][2]][j+1][1] = k1;
							LS[Op[l][i][2]][j+1][2] = k2;
							LS[Op[l][i][2]][j+1][3] = k3;

							tmp[0] = Op[l][i][0];
							tmp[1] = Op[l][i][1];
							tmp[2] = Op[l][i][2];
							tmp[3] = Op[l][i][3];
							tmp[4] = Op[l][i][4];

							for(k=i-1;k>=z;k--)
							{
								Op[l][k+1][0] = Op[l][k][0];
								Op[l][k+1][1] = Op[l][k][1];
								Op[l][k+1][2] = Op[l][k][2];
								Op[l][k+1][3] = Op[l][k][3];
								Op[l][k+1][4] = Op[l][k][4];
							}

							Op[l][z][0] = tmp[0];
							Op[l][z][1] = tmp[1];
							Op[l][z][2] = tmp[2];
							Op[l][z][3] = tmp[3];
							Op[l][z][4] = tmp[4];
							//-----------------------------------------------
							flag = 1;							
							break;
						}
					}
					if((Op[l][i][3]<(LS[Op[l][i][2]][j][1]-LS[Op[l][i][2]][j][0])) && (LS[Op[l][i][2]][j][0] >= FT[Op[l][i][0]]) && ((FT[Op[l][i][0]]+Op[l][i][3])<LS[Op[l][i][2]][j][1]))
					{
						for(z=0;z<i;z++)
						{
							if((Op[l][z][0] == LS[Op[l][i][2]][j][2]) && (Op[l][z][1] == LS[Op[l][i][2]][j][3]))
								break;
						}
						for(k=z;k<i;k++)
						{
							if((Op[l][i][0] == Op[l][k][0]) && (Op[l][i][1] > Op[l][k][1]))
								break;
						}
						if(k != i)
							continue;
						else
						{
							LS[Op[l][i][2]][j][0] += Op[l][i][3];
							FT[Op[l][i][0]] = LS[Op[l][i][2]][j][0];

							tmp[0] = Op[l][i][0];
							tmp[1] = Op[l][i][1];
							tmp[2] = Op[l][i][2];
							tmp[3] = Op[l][i][3];
							tmp[4] = Op[l][i][4];

							for(k=i-1;k>=z;k--)
							{
								Op[l][k+1][0] = Op[l][k][0];
								Op[l][k+1][1] = Op[l][k][1];
								Op[l][k+1][2] = Op[l][k][2];
								Op[l][k+1][3] = Op[l][k][3];
								Op[l][k+1][4] = Op[l][k][4];
							}

							Op[l][z][0] = tmp[0];
							Op[l][z][1] = tmp[1];
							Op[l][z][2] = tmp[2];
							Op[l][z][3] = tmp[3];
							Op[l][z][4] = tmp[4];
							//-----------------------------------------------
							flag = 1;							
							break;
						}
					}
				}
				if((j == oc[z1]) && (flag == 0))
				{
					if(MST[z1] >= FT[z3])
					{
						MST[z1] += z2;
						FT[z3] = MST[z1];
					}
					else
					{
						LS[z1][oc[z1]][0] = MST[z1];
						LS[z1][oc[z1]][1] = FT[z3];			
						LS[z1][oc[z1]][2] = z3;
						LS[z1][oc[z1]++][3] = z4; 

						MST[z1] = FT[z3] + z2;
						FT[z3] = MST[z1];
					}
				}
			}
	}
	F[l] = 0;
	for(i=1;i<=M;i++)
	{
		if(F[l] < MST[i])
			F[l] = MST[i];
	}
	//printf("\nF[%d] = %d",l,F[l]);
}

int main()
{
	
	FILE *fp,*fp1,*fp2;
	int i,j,k,x,l,it,Gen,GEN,r,r1,r2,uc,u[T_Oper],co[T_Oper],ncp[T_Oper],pos[T_Oper],nb[T_Oper],val,flag,Min;
	int x1;
	
	int FT[NJb],MST[Mac],PN,PN1,count;
	int Scout[Empop],Emp[T_Oper][5],Emp_F;
	int TMP[T_Oper][5];
	int z,z1,z2,z3,z4,k1,k2,k3,tmp[5],oc[Mac],LS[Mac][T_Oper][4],Sum;

	unsigned int seed[RUN],Seed_val[RUN],myseed = 10;

	double Avrg,Std_dev,Av_btime,Av_utime,To_btime,To_utime,A_Sd,Std_dev_Val;

	
	
	fp = fopen("addingremanu01.txt","r");
	//fp = fopen("reman.txt","r");

	if(fp == NULL)
	{
		printf("\nError in opening file\n");
		system("pause");
	}

	fscanf(fp,"%d%d%d",&N,&M,&x1);
	//printf("\nN = %d M = %d x1 = %d",N,M,x1);

	toper = 0; //total number of operations

	for(i=0;i<N;i++)
	{
		fscanf(fp,"%d",&Jb[i]);
		//printf("\n%d  ",Jb[i]);

		toper += Jb[i];
		for(j=0;j<Jb[i];j++)
		{
			fscanf(fp,"%d",&NM_O[i][j]); //Number of machines for the operation j of J[i]
			//printf("%d ",NM_O[i][j]);
			
			for(k=0;k<NM_O[i][j];k++)
			{
				fscanf(fp,"%d%d",&M_O[i][j][k],&p[i][j][k]);
				//printf("%d %d ",M_O[i][j][k],p[i][j][k]);
			}
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
		fscanf(fp,"%d",&ReFT[i]);
	}
	fp = fopen("machine release time.txt","r");
	if(fp == NULL)
	{
		printf("\nError in opening machine release time file\n");
		system("pause");
	}
	for(i=1;i<=M;i++)
	{
		fscanf(fp,"%d",&ReMST[i]);
	}
	srand(myseed);
	int re;
	/*for(re=0;re<repeattime;re++)
	{*/

	for(it=0;it<RUN;it++)
	{
		//times(&buff);
               // utimer=buff.tms_utime;
		time_t start; /*注意计时所用的变量名称*/	
					start=clock();

		seed[it] = rand();
                srand(seed[it]);

		x = (int)(PERC*toper);

		PN = (int)(PERC1*toper);
		PN1 = (int)(PERC2*toper);

		Optimal = 500000; //Max val is assigned
		//fp2 = fopen("best resuts.txt","a");
		for(l=0;l<Empop;l++)
		{
			Init_pop(l);
			//if(l == 92)
			//{
			//	printf("\nHii l = %d Initial Schedule = \n",l);
			//	for(i=0;i<toper;i++)
			//		printf("%d %d %d %d %d <=> ",Op[l][i][0],Op[l][i][1],Op[l][i][2],Op[l][i][3],Op[l][i][4]);
			//}
			Scout[l] = 0;
            if(F[l] < Optimal)
                        {
                                //times(&buff);
                                /*butimer = buff.tms_utime;*/
                                Update_Optimal(l);
								//printf("\nInit l = %d Best known Schedule = \n",l);
								//		for(i=0;i<toper;i++)
								//			printf("%d %d %d %d %d <=> ",Op_seq[i][0],Op_seq[i][1],Op_seq[i][2],Op_seq[i][3],Op_seq[i][4]);
								//printf("\nOptimal = %d\n",Optimal);

                        }
		}
		i = 10*M*N;
		if(i < 3000)
			GEN = 3000;
		else
			GEN = 4000;

		//GEN = 100;

		Gen = 0;
		
		//fprintf(fp2,"%d \n",Optimal);
		while(Gen++ <GEN)
		{
			//Employed Bee Phase
			for(l=0;l<Empop;l++)
			{
				if(u01 < PNBR)
				{
//if(l == 92)
//printf("\nwwwwwwwww\n");
					do
					{
						k1 = (int)(u01*Empop);
					}while(k1 == l);

                        		do
					{
						k2 = (int)(u01*Empop);
					}while(k2 == l || k2 == k1);

                        		
                        		if(u01 < B_T)
                        		{
                        			if(F[k1] <= F[k2])
                        		        	r = k1;
                        		        else
                        		        	r = k2;
                        		}
                        		else
                        		{
                        			if(F[k1] > F[k2])
                        		        	r = k1;
                        		        else
                        		        	r = k2;
					}
					/*do
					{
						r = (int)(u01*Empop);
					}while(r == l);*/
						
					// ..No need of collision check ..
					for(i=0;i<toper;i++)
					{
						u[i] = i;
						pos[i] = nb[i] = 0;
					}
					uc = toper;
					i = 0;

					while(i++ < x)
					{
						r1 = (int)(u01*uc);
						nb[u[r1]] = 1;
						pos[Op[r][u[r1]][4]] = 1;
	
						Emp[u[r1]][0] = Op[r][u[r1]][0];
						Emp[u[r1]][1] = Op[r][u[r1]][1];
						Emp[u[r1]][2] = Op[r][u[r1]][2];
						Emp[u[r1]][3] = Op[r][u[r1]][3];
						Emp[u[r1]][4] = Op[r][u[r1]][4];

						u[r1] = u[--uc];
					}
					j = 0;
					for(i=0;i<toper;i++)
                               		{
						if(pos[Op[l][j][4]] == 0)
						{
							if(nb[i] == 0)
							{
								Emp[i][0] = Op[l][j][0];
								Emp[i][1] = Op[l][j][1];
								Emp[i][2] = Op[l][j][2];
								Emp[i][3] = Op[l][j][3];
								Emp[i][4] = Op[l][j][4];
							}
							else
								continue;
						}
						else
							i--;
						if(++j == toper)
                                       			break;
					}
					//Repair Operator
					for(i=0;i<toper;i++)
					{
						for(j=i-1;j>=0;j--)
						{		
							if((Emp[i][0] == Emp[j][0]) && ((Emp[i][1] < Emp[j][1])))
							{
								//Swap
								tmp[0] = Emp[i][0];
								tmp[1] = Emp[i][1];
								tmp[2] = Emp[i][2];
								tmp[3] = Emp[i][3];
								tmp[4] = Emp[i][4];

								Emp[i][0] = Emp[j][0];
								Emp[i][1] = Emp[j][1];
								Emp[i][2] = Emp[j][2];
								Emp[i][3] = Emp[j][3];
								Emp[i][4] = Emp[j][4];

								Emp[j][0] = tmp[0];
								Emp[j][1] = tmp[1];
								Emp[j][2] = tmp[2];
								Emp[j][3] = tmp[3];
								Emp[j][4] = tmp[4];

								i = j-1;

								break;
							}
						}
					}
//if(l > 100)
//{
//	printf("\n1111 l = %d Employed Schedule = \n",l);
//	for(i=0;i<toper;i++)
//		printf("%d %d %d %d %d <=> ",Emp[i][0],Emp[i][1],Emp[i][2],Emp[i][3],Emp[i][4]);
//}
				}
				else
				{
					for(i=0;i<toper;i++)
					{
						Emp[i][0] = Op[l][i][0];	
						Emp[i][1] = Op[l][i][1];
						Emp[i][2] = Op[l][i][2];
						Emp[i][3] = Op[l][i][3];
						Emp[i][4] = Op[l][i][4];
					}
					//Perturbation
					if(N>1)
					{
					if(u01 < PRTB)
					{
//if(l == 92)
//printf("\nyyyyyyyy\n");
//if(l == 92)
//{
//	printf("\nPerturbation l = %d Employed Schedule = \n",l);
//	for(i=0;i<toper;i++)
//		printf("%d %d %d %d %d <=> ",Emp[i][0],Emp[i][1],Emp[i][2],Emp[i][3],Emp[i][4]);
//}

						for(i=0;i<PN;i++)
						{
							r1 = (int)(u01*toper);
//if(l == 92)
//	printf("\nr1 = %d\n",r1);
						
							if(NM_O[Emp[r1][0]][Emp[r1][1]] > 1)
							{
								Min = 500000;
								for(j=0;j<NM_O[Emp[r1][0]][Emp[r1][1]];j++)
								{
									if(Min > p[Emp[r1][0]][Emp[r1][1]][j])
									{
										Min = p[Emp[r1][0]][Emp[r1][1]][j];
										k1 = j;
									}
								}
								if(Min == Emp[r1][3] && M_O[Emp[r1][0]][Emp[r1][1]][k1] == Emp[r1][2])
								{
									do
									{
										r2 = (int)(u01*NM_O[Emp[r1][0]][Emp[r1][1]]);
									}while(M_O[Emp[r1][0]][Emp[r1][1]][r2] == Emp[r1][2]);
									
									Emp[r1][2] = M_O[Emp[r1][0]][Emp[r1][1]][r2];
									Emp[r1][3] = p[Emp[r1][0]][Emp[r1][1]][r2];
								}
								else
								{
									Emp[r1][2] = M_O[Emp[r1][0]][Emp[r1][1]][k1];
									Emp[r1][3] = p[Emp[r1][0]][Emp[r1][1]][k1];
								}
							}
						}
//if(l == 92)
//{
//	printf("\nAfter Perturbation l = %d Employed Schedule = \n",l);
//	for(i=0;i<toper;i++)
//		printf("%d %d %d %d %d <=> ",Emp[i][0],Emp[i][1],Emp[i][2],Emp[i][3],Emp[i][4]);
//}
					}
					else
					{
//if(l == 92)
//printf("\nzzzzzzzz\n");
						for(z1=0;z1<PN1;z1++)
						{
							do
							{
								r1 = (int)(u01*(toper-1));
								r2 = r1+1;
							}while(Emp[r1][0] == Emp[r2][0]);
						
							if(Emp[r1][1] == (Jb[Emp[r1][0]]-1))
								continue;
							
							for(i=r2+1;i<toper;i++)
							{
								if((Emp[r1][0] == Emp[i][0]) && ((Emp[r1][1]+1) == Emp[i][1]))
									break;
							}

							k = i-r2;
							j = r2 + (int)(u01*k);

							tmp[0] = Emp[r1][0];
							tmp[1] = Emp[r1][1];
							tmp[2] = Emp[r1][2];
							tmp[3] = Emp[r1][3];
							tmp[4] = Emp[r1][4];

							for(i=r1;i<j;i++)
							{
								Emp[i][0] = Emp[i+1][0];
								Emp[i][1] = Emp[i+1][1];
								Emp[i][2] = Emp[i+1][2];
								Emp[i][3] = Emp[i+1][3];
								Emp[i][4] = Emp[i+1][4];
							}							
							Emp[j][0] = tmp[0];
							Emp[j][1] = tmp[1];
							Emp[j][2] = tmp[2];
							Emp[j][3] = tmp[3];
							Emp[j][4] = tmp[4];
						}
					}
					}
//if(l > 100)
//{
//	printf("\n222222 l = %d Employed  Schedule = \n",l);
//	for(i=0;i<toper;i++)
//		printf("%d %d %d %d %d <=> ",Emp[i][0],Emp[i][1],Emp[i][2],Emp[i][3],Emp[i][4]);
//}		   

				}
				//****************************************************************
				//Left-Shift operator
				for(i=1;i<=M;i++)
				{
					oc[i] = 0; //Op_counter
					MST[i] = ReMST[i]; //Machine_Spantime
				}
				for(i=0;i<N;i++)
				{
					FT[i] =ReFT[i]; //Start time and Completion time
				}
				for(i=0;i<toper;i++)
				{
					if(oc[Emp[i][2]] == 0)
						{
							if(MST[Emp[i][2]] >= FT[Emp[i][0]])
							{
								MST[Emp[i][2]] += Emp[i][3];
								FT[Emp[i][0]] = MST[Emp[i][2]];
							}
							else
							{
								LS[Emp[i][2]][oc[Emp[i][2]]][0] = MST[Emp[i][2]];
								LS[Emp[i][2]][oc[Emp[i][2]]][1] = FT[Emp[i][0]];
								LS[Emp[i][2]][oc[Emp[i][2]]][2] = Emp[i][0];
								LS[Emp[i][2]][oc[Emp[i][2]]++][3] = Emp[i][1];
								
								MST[Emp[i][2]] = FT[Emp[i][0]] + Emp[i][3];
								FT[Emp[i][0]] = MST[Emp[i][2]];
							}
						}
					else
						{
							z1 = Emp[i][2];
							z2 = Emp[i][3];
							z3 = Emp[i][0];
							z4 = Emp[i][1];
							flag = 0;

							for(j=0;j<oc[Emp[i][2]];j++)
							{
								if((Emp[i][3]==(LS[Emp[i][2]][j][1]-LS[Emp[i][2]][j][0])) && ((FT[Emp[i][0]]+Emp[i][3])==LS[Emp[i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Emp[z][0] == LS[Emp[i][2]][j][2]) && (Emp[z][1] == LS[Emp[i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Emp[i][0] == Emp[k][0]) && (Emp[i][1] > Emp[k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										FT[Emp[i][0]] += Emp[i][3];	

										for(k=j+1;k<oc[Emp[i][2]];k++)
										{
											LS[Emp[i][2]][k-1][0] = LS[Emp[i][2]][k][0];
											LS[Emp[i][2]][k-1][1] = LS[Emp[i][2]][k][1];
											LS[Emp[i][2]][k-1][2] = LS[Emp[i][2]][k][2];
											LS[Emp[i][2]][k-1][3] = LS[Emp[i][2]][k][3];
										}
										oc[Emp[i][2]]--;

										tmp[0] = Emp[i][0];
										tmp[1] = Emp[i][1];
										tmp[2] = Emp[i][2];
										tmp[3] = Emp[i][3];
										tmp[4] = Emp[i][4];

										for(k=i-1;k>=z;k--)
										{
											Emp[k+1][0] = Emp[k][0];
											Emp[k+1][1] = Emp[k][1];
											Emp[k+1][2] = Emp[k][2];
											Emp[k+1][3] = Emp[k][3];
											Emp[k+1][4] = Emp[k][4];
										}
										Emp[z][0] = tmp[0];
										Emp[z][1] = tmp[1];
										Emp[z][2] = tmp[2];
										Emp[z][3] = tmp[3];
										Emp[z][4] = tmp[4];
										//-----------------------------------------------
										flag = 1;
										break;
									}
								}	
								if((Emp[i][3]<(LS[Emp[i][2]][j][1]-LS[Emp[i][2]][j][0])) && ((FT[Emp[i][0]]+Emp[i][3])==LS[Emp[i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Emp[z][0] == LS[Emp[i][2]][j][2]) && (Emp[z][1] == LS[Emp[i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Emp[i][0] == Emp[k][0]) && (Emp[i][1] > Emp[k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										LS[Emp[i][2]][j][1] = FT[Emp[i][0]];
										LS[Emp[i][2]][j][2] = Emp[i][0];
										LS[Emp[i][2]][j][3] = Emp[i][1];

										FT[Emp[i][0]] += Emp[i][3];

										tmp[0] = Emp[i][0];
										tmp[1] = Emp[i][1];
										tmp[2] = Emp[i][2];
										tmp[3] = Emp[i][3];
										tmp[4] = Emp[i][4];
										for(k=i-1;k>=z;k--)
										{
											Emp[k+1][0] = Emp[k][0];
											Emp[k+1][1] = Emp[k][1];
											Emp[k+1][2] = Emp[k][2];
											Emp[k+1][3] = Emp[k][3];
											Emp[k+1][4] = Emp[k][4];
										}
										Emp[z][0] = tmp[0];
										Emp[z][1] = tmp[1];
										Emp[z][2] = tmp[2];
										Emp[z][3] = tmp[3];
										Emp[z][4] = tmp[4];
										//-----------------------------------------------

										flag = 1;
										break;
									}
								}
								if((Emp[i][3]<(LS[Emp[i][2]][j][1]-LS[Emp[i][2]][j][0])) && (LS[Emp[i][2]][j][0] < FT[Emp[i][0]]) && ((FT[Emp[i][0]]+Emp[i][3])<LS[Emp[i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Emp[z][0] == LS[Emp[i][2]][j][2]) && (Emp[z][1] == LS[Emp[i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Emp[i][0] == Emp[k][0]) && (Emp[i][1] > Emp[k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										k1 = LS[Emp[i][2]][j][1];
										k2 = LS[Emp[i][2]][j][2];
										k3 = LS[Emp[i][2]][j][3];

										LS[Emp[i][2]][j][1] = FT[Emp[i][0]];
										LS[Emp[i][2]][j][2] = Emp[i][0];
										LS[Emp[i][2]][j][3] = Emp[i][1];

										FT[Emp[i][0]] += Emp[i][3];

										for(k=oc[Emp[i][2]]-1;k>j;k--)
										{
											LS[Emp[i][2]][k+1][0] = LS[Emp[i][2]][k][0];

											LS[Emp[i][2]][k+1][1] = LS[Emp[i][2]][k][1];

											LS[Emp[i][2]][k+1][2] = LS[Emp[i][2]][k][2];
											LS[Emp[i][2]][k+1][3] = LS[Emp[i][2]][k][3];
										}
										oc[Emp[i][2]]++;

										LS[Emp[i][2]][j+1][0] = FT[Emp[i][0]];
										LS[Emp[i][2]][j+1][1] = k1;
										LS[Emp[i][2]][j+1][2] = k2;
										LS[Emp[i][2]][j+1][3] = k3;

										tmp[0] = Emp[i][0];
										tmp[1] = Emp[i][1];
										tmp[2] = Emp[i][2];
										tmp[3] = Emp[i][3];
										tmp[4] = Emp[i][4];

										for(k=i-1;k>=z;k--)
										{
											Emp[k+1][0] = Emp[k][0];
											Emp[k+1][1] = Emp[k][1];
											Emp[k+1][2] = Emp[k][2];
											Emp[k+1][3] = Emp[k][3];
											Emp[k+1][4] = Emp[k][4];
										}
										Emp[z][0] = tmp[0];
										Emp[z][1] = tmp[1];
										Emp[z][2] = tmp[2];
										Emp[z][3] = tmp[3];
										Emp[z][4] = tmp[4];
										//-----------------------------------------------
										flag = 1;
										break;
									}
								}
								if((Emp[i][3]<(LS[Emp[i][2]][j][1]-LS[Emp[i][2]][j][0])) && (LS[Emp[i][2]][j][0] >= FT[Emp[i][0]]) && ((FT[Emp[i][0]]+Emp[i][3])<LS[Emp[i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Emp[z][0] == LS[Emp[i][2]][j][2]) && (Emp[z][1] == LS[Emp[i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Emp[i][0] == Emp[k][0]) && (Emp[i][1] > Emp[k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										LS[Emp[i][2]][j][0] += Emp[i][3];

										FT[Emp[i][0]] = LS[Emp[i][2]][j][0];

										tmp[0] = Emp[i][0];
										tmp[1] = Emp[i][1];
										tmp[2] = Emp[i][2];
										tmp[3] = Emp[i][3];
										tmp[4] = Emp[i][4];

										for(k=i-1;k>=z;k--)
										{
											Emp[k+1][0] = Emp[k][0];
											Emp[k+1][1] = Emp[k][1];
											Emp[k+1][2] = Emp[k][2];
											Emp[k+1][3] = Emp[k][3];
											Emp[k+1][4] = Emp[k][4];
										}
										Emp[z][0] = tmp[0];
										Emp[z][1] = tmp[1];
										Emp[z][2] = tmp[2];
										Emp[z][3] = tmp[3];
										Emp[z][4] = tmp[4];
										//-----------------------------------------------
										flag = 1;
										break;

									}
								}
							}
							if((j == oc[z1]) && (flag == 0))
							{
								if(MST[z1] >= FT[z3])
								{
									MST[z1] += z2;
									FT[z3] = MST[z1];
								}
								else
								{
									LS[z1][oc[z1]][0] = MST[z1];
									LS[z1][oc[z1]][1] = FT[z3];
									LS[z1][oc[z1]][2] = z3;
									LS[z1][oc[z1]++][3] = z4; 

									MST[z1] = FT[z3] + z2;
									FT[z3] = MST[z1];
								}
							}
						}
				}
				Emp_F = 0;
				for(i=1;i<=M;i++)
				{
					if(Emp_F < MST[i])
						Emp_F = MST[i];
				}
			}

			//-------- Onlooker Bee Phase ---------
            for(l=0;l<Onpop;l++)
			{
				On_1 = (int)(u01*Empop);
                On_2 = (int)(u01*Empop);

                while(On_1 == On_2)
                   On_2 = (int)(u01*Empop);
                    if(u01 < B_T)
                        {
                        	if(F[On_1] <= F[On_2])
                        	        On = On_1;
                        	    else
                        	        On = On_2;
                        }
                    else
                        {
                        	if(F[On_1] > F[On_2])
                        	        On = On_1;
                        	    else
                        	        On = On_2;
					      }
				Onlook[l] = On;				

				if(u01 < PNBR)
				{
					do
					{
						k1 = (int)(u01*Empop);
					}while(k1 == On);

                    do
					{
						k2 = (int)(u01*Empop);
					}while(k2 == On || k2 == k1);
                        		
                    if(u01 < B_T)
                    {
                        if(F[k1] <= F[k2])
                        		r = k1;
                        	else
                        		r = k2;
                    }
                    else
                    {
                        if(F[k1] > F[k2])
                        		r = k1;
                        	else
                        		r = k2;
					}

					//do
					//{
					//	r = (int) (u01*Empop);
					//}while(r == On);

					//No need of collision check ..						

					for(i=0;i<toper;i++)
					{
						u[i] = i;
						pos[i] = nb[i] = 0;
					}
					uc = toper;
					i = 0;

					while(i++ < x)
					{
						r1 = (int)(u01*uc);
						nb[u[r1]] = 1;
						pos[Op[r][u[r1]][4]] = 1;

						Onps[l][u[r1]][0] = Op[r][u[r1]][0];
						Onps[l][u[r1]][1] = Op[r][u[r1]][1];
						Onps[l][u[r1]][2] = Op[r][u[r1]][2];
						Onps[l][u[r1]][3] = Op[r][u[r1]][3];
						Onps[l][u[r1]][4] = Op[r][u[r1]][4];

						u[r1] = u[--uc];
					}

					j = 0;
					for(i=0;i<toper;i++)
                               		{
						if(pos[Op[On][j][4]] == 0)
						{
							if(nb[i] == 0)
							{
								Onps[l][i][0] = Op[On][j][0];
								Onps[l][i][1] = Op[On][j][1];
								Onps[l][i][2] = Op[On][j][2];
								Onps[l][i][3] = Op[On][j][3];
								Onps[l][i][4] = Op[On][j][4];
							}
							else
								continue;
						}
						else
							i--;
						if(++j == toper)
                                       			break;
					}
					//Repair Operator
					for(i=0;i<toper;i++)
					{
						for(j=i-1;j>=0;j--)
						{		
							if((Onps[l][i][0] == Onps[l][j][0]) && ((Onps[l][i][1] < Onps[l][j][1])))
							{
								//Swap
								tmp[0] = Onps[l][i][0];
								tmp[1] = Onps[l][i][1];
								tmp[2] = Onps[l][i][2];
								tmp[3] = Onps[l][i][3];
								tmp[4] = Onps[l][i][4];

								Onps[l][i][0] = Onps[l][j][0];
								Onps[l][i][1] = Onps[l][j][1];
								Onps[l][i][2] = Onps[l][j][2];
								Onps[l][i][3] = Onps[l][j][3];
								Onps[l][i][4] = Onps[l][j][4];

								Onps[l][j][0] = tmp[0];
								Onps[l][j][1] = tmp[1];
								Onps[l][j][2] = tmp[2];
								Onps[l][j][3] = tmp[3];
								Onps[l][j][4] = tmp[4];

								i = j-1;

								break;
							}
						}
					}
				}
				else
				{
					On_F[l] = F[On];
					for(i=0;i<toper;i++)
					{
						Onps[l][i][0] = Op[On][i][0];
						Onps[l][i][1] = Op[On][i][1];
						Onps[l][i][2] = Op[On][i][2];
						Onps[l][i][3] = Op[On][i][3];
						Onps[l][i][4] = Op[On][i][4];
					}
					//Perturbation
					if(N>1)
					{
					if(u01 < PRTB)
					{
						for(i=0;i<PN;i++)
						{
							r1 = (int)(u01*toper);
					
							if(NM_O[Onps[l][r1][0]][Onps[l][r1][1]] > 1)
							{
								Min = 500000;
								for(j=0;j<NM_O[Onps[l][r1][0]][Onps[l][r1][1]];j++)
								{
									if(Min > p[Onps[l][r1][0]][Onps[l][r1][1]][j])
									{
										Min = p[Onps[l][r1][0]][Onps[l][r1][1]][j];
										k1 = j;
									}
								}
								if(Min == Onps[l][r1][3] && M_O[Onps[l][r1][0]][Onps[l][r1][1]][k1] == Onps[l][r1][2])
								{
									do
									{
										r2 = (int)(u01*NM_O[Onps[l][r1][0]][Onps[l][r1][1]]);
									}while(M_O[Onps[l][r1][0]][Onps[l][r1][1]][r2] == Onps[l][r1][2]);
								
									Onps[l][r1][2] = M_O[Onps[l][r1][0]][Onps[l][r1][1]][r2];
									Onps[l][r1][3] = p[Onps[l][r1][0]][Onps[l][r1][1]][r2];
								}
								else
								{
									Onps[l][r1][2] = M_O[Onps[l][r1][0]][Onps[l][r1][1]][k1];
									Onps[l][r1][3] = p[Onps[l][r1][0]][Onps[l][r1][1]][k1];
								}
							}
						}	
					}
					else
					{
						for(z1=0;z1<PN1;z1++)
						{
							do
							{
								r1 = (int)(u01*(toper-1));
								r2 = r1+1;
							}while(Onps[l][r1][0] == Onps[l][r2][0]);
							
							if(Onps[l][r1][1] == Jb[Onps[l][r1][0]]-1)
								continue;
							for(i=r2+1;i<toper;i++)
							{
								if((Onps[l][r1][0] == Onps[l][i][0]) && ((Onps[l][r1][1]+1) == Onps[l][i][1]))
									break;
							}
							k = i-r2;					

							j = r2 + (int)(u01*k);

							tmp[0] = Onps[l][r1][0];
							tmp[1] = Onps[l][r1][1];
							tmp[2] = Onps[l][r1][2];
							tmp[3] = Onps[l][r1][3];
							tmp[4] = Onps[l][r1][4];

							for(i=r1;i<j;i++)
							{
								Onps[l][i][0] = Onps[l][i+1][0];
								Onps[l][i][1] = Onps[l][i+1][1];
								Onps[l][i][2] = Onps[l][i+1][2];
								Onps[l][i][3] = Onps[l][i+1][3];
								Onps[l][i][4] = Onps[l][i+1][4];
							}							
							Onps[l][j][0] = tmp[0];
							Onps[l][j][1] = tmp[1];
							Onps[l][j][2] = tmp[2];
							Onps[l][j][3] = tmp[3];
							Onps[l][j][4] = tmp[4];
						}
					}
					}
				}
				//Left-Shift operator********************************************************************************************************
				for(i=1;i<=M;i++)
					oc[i] = 0; //Op_counter
				for(i=1;i<=M;i++)
					MST[i] = ReMST[i]; //Machine_Spantime
				for(i=0;i<N;i++)
					FT[i] = ReFT[i]; //Start time and Completion time
				for(i=0;i<toper;i++)
				{
					if(oc[Onps[l][i][2]] == 0)
						{
							if(MST[Onps[l][i][2]] >= FT[Onps[l][i][0]])
							{
								MST[Onps[l][i][2]] += Onps[l][i][3];
								FT[Onps[l][i][0]] = MST[Onps[l][i][2]];
							}
							else
							{
								LS[Onps[l][i][2]][oc[Onps[l][i][2]]][0] = MST[Onps[l][i][2]];
								LS[Onps[l][i][2]][oc[Onps[l][i][2]]][1] = FT[Onps[l][i][0]];
								LS[Onps[l][i][2]][oc[Onps[l][i][2]]][2] = Onps[l][i][0];
								LS[Onps[l][i][2]][oc[Onps[l][i][2]]++][3] = Onps[l][i][1];
					
								MST[Onps[l][i][2]] = FT[Onps[l][i][0]] + Onps[l][i][3];
								FT[Onps[l][i][0]] = MST[Onps[l][i][2]];
							}
						}
					else
						{
							z1 = Onps[l][i][2];
							z2 = Onps[l][i][3];
							z3 = Onps[l][i][0];
							z4 = Onps[l][i][1];
							flag = 0;

							for(j=0;j<oc[Onps[l][i][2]];j++)
							{
								if((Onps[l][i][3]==(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && ((FT[Onps[l][i][0]]+Onps[l][i][3])==LS[Onps[l][i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										FT[Onps[l][i][0]] += Onps[l][i][3];	

										for(k=j+1;k<oc[Onps[l][i][2]];k++)
										{
											LS[Onps[l][i][2]][k-1][0] = LS[Onps[l][i][2]][k][0];
											LS[Onps[l][i][2]][k-1][1] = LS[Onps[l][i][2]][k][1];
											LS[Onps[l][i][2]][k-1][2] = LS[Onps[l][i][2]][k][2];
											LS[Onps[l][i][2]][k-1][3] = LS[Onps[l][i][2]][k][3];
										}
										oc[Onps[l][i][2]]--;

										tmp[0] = Onps[l][i][0];
										tmp[1] = Onps[l][i][1];
										tmp[2] = Onps[l][i][2];
										tmp[3] = Onps[l][i][3];
										tmp[4] = Onps[l][i][4];

										for(k=i-1;k>=z;k--)
										{
											Onps[l][k+1][0] = Onps[l][k][0];
											Onps[l][k+1][1] = Onps[l][k][1];
											Onps[l][k+1][2] = Onps[l][k][2];
											Onps[l][k+1][3] = Onps[l][k][3];
											Onps[l][k+1][4] = Onps[l][k][4];
										}
										Onps[l][z][0] = tmp[0];
										Onps[l][z][1] = tmp[1];
										Onps[l][z][2] = tmp[2];
										Onps[l][z][3] = tmp[3];
										Onps[l][z][4] = tmp[4];
										//-----------------------------------------------
										flag = 1;
										break;
									}
								}	
								if((Onps[l][i][3]<(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && ((FT[Onps[l][i][0]]+Onps[l][i][3])==LS[Onps[l][i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										LS[Onps[l][i][2]][j][1] = FT[Onps[l][i][0]];
										LS[Onps[l][i][2]][j][2] = Onps[l][i][0];
										LS[Onps[l][i][2]][j][3] = Onps[l][i][1];

										FT[Onps[l][i][0]] += Onps[l][i][3];

										tmp[0] = Onps[l][i][0];
										tmp[1] = Onps[l][i][1];
										tmp[2] = Onps[l][i][2];
										tmp[3] = Onps[l][i][3];
										tmp[4] = Onps[l][i][4];

										for(k=i-1;k>=z;k--)
										{
											Onps[l][k+1][0] = Onps[l][k][0];
											Onps[l][k+1][1] = Onps[l][k][1];
											Onps[l][k+1][2] = Onps[l][k][2];
											Onps[l][k+1][3] = Onps[l][k][3];
											Onps[l][k+1][4] = Onps[l][k][4];
										}
										Onps[l][z][0] = tmp[0];
										Onps[l][z][1] = tmp[1];
										Onps[l][z][2] = tmp[2];
										Onps[l][z][3] = tmp[3];
										Onps[l][z][4] = tmp[4];
										//-----------------------------------------------
										flag = 1;
										break;
									}
								}
								if((Onps[l][i][3]<(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && (LS[Onps[l][i][2]][j][0] < FT[Onps[l][i][0]]) && ((FT[Onps[l][i][0]]+Onps[l][i][3])<LS[Onps[l][i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										k1 = LS[Onps[l][i][2]][j][1];
										k2 = LS[Onps[l][i][2]][j][2];
										k3 = LS[Onps[l][i][2]][j][3];

										LS[Onps[l][i][2]][j][1] = FT[Onps[l][i][0]];
										LS[Onps[l][i][2]][j][2] = Onps[l][i][0];
										LS[Onps[l][i][2]][j][3] = Onps[l][i][1];

										FT[Onps[l][i][0]] += Onps[l][i][3];
							
										for(k=oc[Onps[l][i][2]]-1;k>j;k--)
										{
											LS[Onps[l][i][2]][k+1][0] = LS[Onps[l][i][2]][k][0];

											LS[Onps[l][i][2]][k+1][1] = LS[Onps[l][i][2]][k][1];

											LS[Onps[l][i][2]][k+1][2] = LS[Onps[l][i][2]][k][2];
											LS[Onps[l][i][2]][k+1][3] = LS[Onps[l][i][2]][k][3];
										}
										oc[Onps[l][i][2]]++;

										LS[Onps[l][i][2]][j+1][0] = FT[Onps[l][i][0]];
										LS[Onps[l][i][2]][j+1][1] = k1;
										LS[Onps[l][i][2]][j+1][2] = k2;
										LS[Onps[l][i][2]][j+1][3] = k3;

										tmp[0] = Onps[l][i][0];
										tmp[1] = Onps[l][i][1];
										tmp[2] = Onps[l][i][2];
										tmp[3] = Onps[l][i][3];
										tmp[4] = Onps[l][i][4];

										for(k=i-1;k>=z;k--)
										{
											Onps[l][k+1][0] = Onps[l][k][0];
											Onps[l][k+1][1] = Onps[l][k][1];
											Onps[l][k+1][2] = Onps[l][k][2];
											Onps[l][k+1][3] = Onps[l][k][3];
											Onps[l][k+1][4] = Onps[l][k][4];
										}
										Onps[l][z][0] = tmp[0];
										Onps[l][z][1] = tmp[1];
										Onps[l][z][2] = tmp[2];
										Onps[l][z][3] = tmp[3];
										Onps[l][z][4] = tmp[4];
										//-----------------------------------------------

										flag = 1;
										break;
									}
								}
								if((Onps[l][i][3]<(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && (LS[Onps[l][i][2]][j][0] >= FT[Onps[l][i][0]]) && ((FT[Onps[l][i][0]]+Onps[l][i][3])<LS[Onps[l][i][2]][j][1]))
								{
									for(z=0;z<i;z++)
									{
										if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
											break;
									}
									for(k=z;k<i;k++)
									{
										if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
											break;
									}
									if(k != i)
										continue;
									else
									{
										LS[Onps[l][i][2]][j][0] += Onps[l][i][3];

										FT[Onps[l][i][0]] = LS[Onps[l][i][2]][j][0];

										tmp[0] = Onps[l][i][0];
										tmp[1] = Onps[l][i][1];
										tmp[2] = Onps[l][i][2];
										tmp[3] = Onps[l][i][3];
										tmp[4] = Onps[l][i][4];

										for(k=i-1;k>=z;k--)
										{
											Onps[l][k+1][0] = Onps[l][k][0];
											Onps[l][k+1][1] = Onps[l][k][1];
											Onps[l][k+1][2] = Onps[l][k][2];
											Onps[l][k+1][3] = Onps[l][k][3];
											Onps[l][k+1][4] = Onps[l][k][4];
										}
										Onps[l][z][0] = tmp[0];
										Onps[l][z][1] = tmp[1];
										Onps[l][z][2] = tmp[2];
										Onps[l][z][3] = tmp[3];
										Onps[l][z][4] = tmp[4];
										//-----------------------------------------------
										flag = 1;
										break;

									}
								}
							}
							if((j == oc[z1]) && (flag == 0))
							{
								if(MST[z1] >= FT[z3])
								{
									MST[z1] += z2;
									FT[z3] = MST[z1];
								}
								else
								{
									LS[z1][oc[z1]][0] = MST[z1];
									LS[z1][oc[z1]][1] = FT[z3];
									LS[z1][oc[z1]][2] = z3;
									LS[z1][oc[z1]++][3] = z4; 

									MST[z1] = FT[z3] + z2;
									FT[z3] = MST[z1];
								}
							}
						}

				}
				On_F[l] = 0;
				for(i=1;i<=M;i++)
				{
					if(On_F[l] < MST[i])
						On_F[l] = MST[i];
				}
              }
			  //------------------ Local Search ----------------------------------------
				if(N>1)
				{

				if((On_F[l] - Optimal) < (Optimal*OPERC))
				{
					count = 0;
					while(1)
					{
						for(i=0;i<toper;i++)
						{
							TMP[i][0] = Onps[l][i][0];
							TMP[i][1] = Onps[l][i][1];
							TMP[i][2] = Onps[l][i][2];
							TMP[i][3] = Onps[l][i][3];
							TMP[i][4] = Onps[l][i][4];
						}
						val = On_F[l];

						//----------- Critical Path -----------
						//Earliest start or completion time
						for(i=0;i<toper;i++)
						{
							if(Onps[l][i][1] == 0)
							{
								z2 = 0;
								for(j=i-1;j>=0;j--)
								{
									if(Onps[l][j][2] == Onps[l][i][2])
									{
										PM[i] = j;
										z2 = 1;
										break;
									}
								}
								if(z2 == 0)
									SE[i] = 0;
								if(z2 == 1)
									SE[i] = CE[PM[i]];
								CE[i] = SE[i] + Onps[l][i][3]; //Earliest completion time
							}
							else
							{
								z1 = z2 = 0;
								for(j=i-1;j>=0;j--)
								{
									if(Onps[l][j][0] == Onps[l][i][0])
									{
										PJ[i] = j;
										z1 = 1;
										break;
									}
								}
								for(j=i-1;j>=0;j--)
								{
									if(Onps[l][j][2] == Onps[l][i][2])
									{
										PM[i] = j;
										z2 = 1;
										break;
									}
								}
								if(z1 == 1 && z2 == 0)
									SE[i] = CE[PJ[i]];
								if(z1 == 0 && z2 == 1)
									SE[i] = CE[PM[i]];
								if(z1 == 1 && z2 == 1)
								{
									if(CE[PJ[i]] > CE[PM[i]])
										SE[i] = CE[PJ[i]];
									else
										SE[i] = CE[PM[i]];	
								}
								//SE[i] = (CE[PJ[i]] > CE[PM[i]])?CE[PJ[i]]:CE[PM[i]];
								CE[i] = SE[i] + Onps[l][i][3];
							}
						}
						//Latest start or completion time
						for(i=toper-1;i>=0;i--)
						{
							if(Onps[l][i][1] == Jb[Onps[l][i][0]]-1)
							{			
								z2 = 0;
								for(j=i+1;j<toper;j++)
								{
									if(Onps[l][j][2] == Onps[l][i][2])
									{
										SM[i] = j;
										z2 = 1;
										break;
									}
								}
								if(z2 == 0)
									CL[i] = On_F[l];
								if(z2 == 1)
									CL[i] = SL[SM[i]];
								SL[i] = CL[i] - Onps[l][i][3]; //Earliest latest time
							}
							else
							{
								z1 = z2 = 0;
								for(j=i+1;j<toper;j++)
								{	
									if(Onps[l][j][0] == Onps[l][i][0])
									{
										z1 = 1;
										SJ[i] = j;
										break;
									}	
								}
								for(j=i+1;j<toper;j++)
								{	
									if(Onps[l][j][2] == Onps[l][i][2])
									{
										z2 = 1;
										SM[i] = j;
										break;
									}
								}
								if(z1 == 1 && z2 == 0)
									CL[i] = SL[SJ[i]];
								if(z1 == 0 && z2 == 1)
									CL[i] = SL[SM[i]];
								if(z1 == 1 && z2 == 1)
								{
									if(SL[SJ[i]] < SL[SM[i]])
										CL[i] = SL[SJ[i]];
									else
										CL[i] = SL[SM[i]];
								}
								//CL[i] = min(SL[SJ[i]],SL[SM[i]]);
								SL[i] = CL[i] - Onps[l][i][3];
							}
						}
						//printf("\nOperations in critical paths");	
						k = 0;					
						for(i=0;i<toper;i++)
						{
							if(SE[i] == SL[i])
								co[k++] = i;
						}
						
						for(i=0;i<k;i++)
						{
							if(Onps[l][co[i]][1] == 0)
							{
								for(j=0;j<NM_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]];j++)
								{
									if(M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j] != Onps[l][co[i]][2])
									{
										if(oc[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]] == 0)
										{
											if((MST[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]] + p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]) < CE[co[i]])
											{
												Onps[l][co[i]][2] = M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];
												Onps[l][co[i]][3] = p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];

												goto outss;
											}
										}
										else
										{
											for(z=0;z<oc[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]];z++)
											{
												if(LS[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]][z][1] < CE[co[i]])
												{
													if((LS[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]][z][1]-LS[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]][z][0]) >= p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j])
	
													{
														Onps[l][co[i]][2] = M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];
														Onps[l][co[i]][3] = p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];
										
														goto outss11;
													}
												}
											}
										}
									}
								}
								continue;
							}
							for(j=0;j<NM_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]];j++)
							{
								if(M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j] != Onps[l][co[i]][2])
								{
									if(oc[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]] == 0)
									{
										if(CE[PJ[co[i]]] > MST[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]])
											z1 = CE[PJ[co[i]]];
										else
											z1 = MST[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]];

										if(((CE[co[i]] - CE[PJ[co[i]]]) > p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]) && ((z1 + p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]) < CE[co[i]]))		
										{
											Onps[l][co[i]][2] = M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];
											Onps[l][co[i]][3] = p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];
										
											goto outss22;
										}
									}
									else
									{
										for(z=0;z<oc[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]];z++)
										{
											if(LS[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]][z][0] <= CE[PJ[co[i]]] && LS[M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j]][z][1] >= CE[co[i]])
											{
												if(CE[PJ[co[i]]] + p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j] < CE[co[i]])
												{
													Onps[l][co[i]][2] = M_O[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];
													Onps[l][co[i]][3] = p[Onps[l][co[i]][0]][Onps[l][co[i]][1]][j];
										
													goto outss33;
												}
											}
										}
									}
								}
							}
						}
						outss:
						outss11:
						outss22:
						outss33:
				
						if(i == k)
							break;
						else
						{
							//Left-Shift operator
							for(i=1;i<=M;i++)
							{
								oc[i] = 0; //Op_counter
								MST[i] = 0; //Machine_Spantime
							}
							for(i=0;i<N;i++)
								FT[i] = 0; //Start time and Completion time
							for(i=0;i<toper;i++)
							{
								if(MST[Onps[l][i][2]] == 0 && FT[Onps[l][i][0]] == 0)
								{
									MST[Onps[l][i][2]] = Onps[l][i][3];
									FT[Onps[l][i][0]] = Onps[l][i][3];
									continue;
								}
								if(MST[Onps[l][i][2]] == 0 && FT[Onps[l][i][0]] != 0)
								{
									LS[Onps[l][i][2]][oc[Onps[l][i][2]]][0] = 0;
									LS[Onps[l][i][2]][oc[Onps[l][i][2]]][1] = FT[Onps[l][i][0]];
									LS[Onps[l][i][2]][oc[Onps[l][i][2]]][2] = Onps[l][i][0];
									LS[Onps[l][i][2]][oc[Onps[l][i][2]]++][3] = Onps[l][i][1];

									FT[Onps[l][i][0]] += Onps[l][i][3];
									MST[Onps[l][i][2]] = FT[Onps[l][i][0]];

									continue;
								}
								if(MST[Onps[l][i][2]] != 0 && FT[Onps[l][i][0]] == 0)
								{
									if(oc[Onps[l][i][2]] == 0)
									{
										MST[Onps[l][i][2]] += Onps[l][i][3];
										FT[Onps[l][i][0]] = MST[Onps[l][i][2]];
									}
									else
									{
										z1 = Onps[l][i][2];
										z2 = Onps[l][i][3];
										z3 = Onps[l][i][0];
										flag = 0;

										for(j=0;j<oc[Onps[l][i][2]];j++)
										{
											if(Onps[l][i][3]==(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0]))
											{
												FT[Onps[l][i][0]] += LS[Onps[l][i][2]][j][1];
												for(z=0;z<i;z++)
												{
													if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
														break;
												}
												for(k=j+1;k<oc[Onps[l][i][2]];k++)
												{
													LS[Onps[l][i][2]][k-1][0] = LS[Onps[l][i][2]][k][0];
													LS[Onps[l][i][2]][k-1][1] = LS[Onps[l][i][2]][k][1];
													LS[Onps[l][i][2]][k-1][2] = LS[Onps[l][i][2]][k][2];
													LS[Onps[l][i][2]][k-1][3] = LS[Onps[l][i][2]][k][3];
												}
												oc[Onps[l][i][2]]--;

												tmp[0] = Onps[l][i][0];
												tmp[1] = Onps[l][i][1];
												tmp[2] = Onps[l][i][2];
												tmp[3] = Onps[l][i][3];
												tmp[4] = Onps[l][i][4];

												for(k=i-1;k>=z;k--)
												{
													Onps[l][k+1][0] = Onps[l][k][0];
													Onps[l][k+1][1] = Onps[l][k][1];
													Onps[l][k+1][2] = Onps[l][k][2];
													Onps[l][k+1][3] = Onps[l][k][3];
													Onps[l][k+1][4] = Onps[l][k][4];
												}

												Onps[l][z][0] = tmp[0];
												Onps[l][z][1] = tmp[1];
												Onps[l][z][2] = tmp[2];
												Onps[l][z][3] = tmp[3];
												Onps[l][z][4] = tmp[4];
												//-----------------------------------------------
												flag = 1;
												break;
											}
											if(Onps[l][i][3] < (LS[Onps[l][i][2]][j][1] - LS[Onps[l][i][2]][j][0]))
											{	
												LS[Onps[l][i][2]][j][0] += Onps[l][i][3];		
												FT[Onps[l][i][0]] = LS[Onps[l][i][2]][j][0];

												for(z=0;z<i;z++)
												{
													if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
														break;
												}
						
												tmp[0] = Onps[l][i][0];
												tmp[1] = Onps[l][i][1];
												tmp[2] = Onps[l][i][2];
												tmp[3] = Onps[l][i][3];
												tmp[4] = Onps[l][i][4];

												for(k=i-1;k>=z;k--)
												{
													Onps[l][k+1][0] = Onps[l][k][0];
													Onps[l][k+1][1] = Onps[l][k][1];
													Onps[l][k+1][2] = Onps[l][k][2];
													Onps[l][k+1][3] = Onps[l][k][3];
													Onps[l][k+1][4] = Onps[l][k][4];
												}

												Onps[l][z][0] = tmp[0];
												Onps[l][z][1] = tmp[1];
												Onps[l][z][2] = tmp[2];
												Onps[l][z][3] = tmp[3];
												Onps[l][z][4] = tmp[4];

												flag = 1;
												break;
											}
										}
										if((j == oc[z1]) && (flag == 0))
										{
											MST[z1] += z2;
											FT[z3] = MST[z1];
										}
									}
									continue;
								}
								if(MST[Onps[l][i][2]] != 0 && FT[Onps[l][i][0]] != 0)
								{
									if(oc[Onps[l][i][2]] == 0)
									{
										if(MST[Onps[l][i][2]] >= FT[Onps[l][i][0]])
										{
											MST[Onps[l][i][2]] += Onps[l][i][3];
											FT[Onps[l][i][0]] = MST[Onps[l][i][2]];	
										}
										else
										{
											LS[Onps[l][i][2]][oc[Onps[l][i][2]]][0] = MST[Onps[l][i][2]];
											LS[Onps[l][i][2]][oc[Onps[l][i][2]]][1] = FT[Onps[l][i][0]];
											LS[Onps[l][i][2]][oc[Onps[l][i][2]]][2] = Onps[l][i][0];
											LS[Onps[l][i][2]][oc[Onps[l][i][2]]++][3] = Onps[l][i][1];

											MST[Onps[l][i][2]] = FT[Onps[l][i][0]] + Onps[l][i][3];
											FT[Onps[l][i][0]] = MST[Onps[l][i][2]];
										}
									}
									else
									{
										z1 = Onps[l][i][2];
										z2 = Onps[l][i][3];
										z3 = Onps[l][i][0];
										z4 = Onps[l][i][1];
										flag = 0;

										for(j=0;j<oc[Onps[l][i][2]];j++)
										{
											if((Onps[l][i][3]==(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && ((FT[Onps[l][i][0]]+Onps[l][i][3])==LS[Onps[l][i][2]][j][1]))
											{
												for(z=0;z<i;z++)
												{
													if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
														break;
												}
												for(k=z;k<i;k++)
												{
													if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
														break;
												}
												if(k != i)
													continue;
												else
												{
													FT[Onps[l][i][0]] += Onps[l][i][3];	

													for(k=j+1;k<oc[Onps[l][i][2]];k++)
													{
														LS[Onps[l][i][2]][k-1][0] = LS[Onps[l][i][2]][k][0];
														LS[Onps[l][i][2]][k-1][1] = LS[Onps[l][i][2]][k][1];
														LS[Onps[l][i][2]][k-1][2] = LS[Onps[l][i][2]][k][2];
														LS[Onps[l][i][2]][k-1][3] = LS[Onps[l][i][2]][k][3];
													}
													oc[Onps[l][i][2]]--;

													tmp[0] = Onps[l][i][0];
													tmp[1] = Onps[l][i][1];
													tmp[2] = Onps[l][i][2];
													tmp[3] = Onps[l][i][3];
													tmp[4] = Onps[l][i][4];

													for(k=i-1;k>=z;k--)
													{
														Onps[l][k+1][0] = Onps[l][k][0];
														Onps[l][k+1][1] = Onps[l][k][1];
														Onps[l][k+1][2] = Onps[l][k][2];
														Onps[l][k+1][3] = Onps[l][k][3];
														Onps[l][k+1][4] = Onps[l][k][4];
													}
													Onps[l][z][0] = tmp[0];
													Onps[l][z][1] = tmp[1];
													Onps[l][z][2] = tmp[2];
													Onps[l][z][3] = tmp[3];
													Onps[l][z][4] = tmp[4];
													//-----------------------------------------------
													flag = 1;
													break;
												}
											}	
											if((Onps[l][i][3]<(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && ((FT[Onps[l][i][0]]+Onps[l][i][3])==LS[Onps[l][i][2]][j][1]))
											{
												for(z=0;z<i;z++)
												{
													if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
														break;
												}
												for(k=z;k<i;k++)
												{
													if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
														break;
												}
												if(k != i)
													continue;
												else
												{
													LS[Onps[l][i][2]][j][1] = FT[Onps[l][i][0]];
													LS[Onps[l][i][2]][j][2] = Onps[l][i][0];
													LS[Onps[l][i][2]][j][3] = Onps[l][i][1];

													FT[Onps[l][i][0]] += Onps[l][i][3];

													tmp[0] = Onps[l][i][0];
													tmp[1] = Onps[l][i][1];
													tmp[2] = Onps[l][i][2];
													tmp[3] = Onps[l][i][3];
													tmp[4] = Onps[l][i][4];
													for(k=i-1;k>=z;k--)
													{
														Onps[l][k+1][0] = Onps[l][k][0];
														Onps[l][k+1][1] = Onps[l][k][1];
														Onps[l][k+1][2] = Onps[l][k][2];
														Onps[l][k+1][3] = Onps[l][k][3];
														Onps[l][k+1][4] = Onps[l][k][4];
													}
													Onps[l][z][0] = tmp[0];
													Onps[l][z][1] = tmp[1];
													Onps[l][z][2] = tmp[2];
													Onps[l][z][3] = tmp[3];
													Onps[l][z][4] = tmp[4];
													//-----------------------------------------------
													flag = 1;
													break;
												}
											}
											if((Onps[l][i][3]<(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && (LS[Onps[l][i][2]][j][0] < FT[Onps[l][i][0]]) && ((FT[Onps[l][i][0]]+Onps[l][i][3])<LS[Onps[l][i][2]][j][1]))
											{
												for(z=0;z<i;z++)
												{
													if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
													{
														break;
													}
												}
												for(k=z;k<i;k++)
												{
													if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
														break;
												}
												if(k != i)
													continue;
												else
												{
													k1 = LS[Onps[l][i][2]][j][1];
													k2 = LS[Onps[l][i][2]][j][2];
													k3 = LS[Onps[l][i][2]][j][3];

													LS[Onps[l][i][2]][j][1] = FT[Onps[l][i][0]];
													LS[Onps[l][i][2]][j][2] = Onps[l][i][0];
													LS[Onps[l][i][2]][j][3] = Onps[l][i][1];

													FT[Onps[l][i][0]] += Onps[l][i][3];

													for(k=oc[Onps[l][i][2]]-1;k>j;k--)
													{
														LS[Onps[l][i][2]][k+1][0] = LS[Onps[l][i][2]][k][0];
														LS[Onps[l][i][2]][k+1][1] = LS[Onps[l][i][2]][k][1];
														LS[Onps[l][i][2]][k+1][2] = LS[Onps[l][i][2]][k][2];
														LS[Onps[l][i][2]][k+1][3] = LS[Onps[l][i][2]][k][3];
													}
													oc[Onps[l][i][2]]++;

													LS[Onps[l][i][2]][j+1][0] = FT[Onps[l][i][0]];
													LS[Onps[l][i][2]][j+1][1] = k1;
													LS[Onps[l][i][2]][j+1][2] = k2;
													LS[Onps[l][i][2]][j+1][3] = k3;

													tmp[0] = Onps[l][i][0];
													tmp[1] = Onps[l][i][1];
													tmp[2] = Onps[l][i][2];
													tmp[3] = Onps[l][i][3];
													tmp[4] = Onps[l][i][4];

													for(k=i-1;k>=z;k--)
													{
														Onps[l][k+1][0] = Onps[l][k][0];
														Onps[l][k+1][1] = Onps[l][k][1];
														Onps[l][k+1][2] = Onps[l][k][2];
														Onps[l][k+1][3] = Onps[l][k][3];
														Onps[l][k+1][4] = Onps[l][k][4];
													}
													Onps[l][z][0] = tmp[0];
													Onps[l][z][1] = tmp[1];
													Onps[l][z][2] = tmp[2];
													Onps[l][z][3] = tmp[3];
													Onps[l][z][4] = tmp[4];
				
													flag = 1;
													break;
												}
											}
											if((Onps[l][i][3]<(LS[Onps[l][i][2]][j][1]-LS[Onps[l][i][2]][j][0])) && (LS[Onps[l][i][2]][j][0] >= FT[Onps[l][i][0]]) && ((FT[Onps[l][i][0]]+Onps[l][i][3])<LS[Onps[l][i][2]][j][1]))
											{
												for(z=0;z<i;z++)
												{
													if((Onps[l][z][0] == LS[Onps[l][i][2]][j][2]) && (Onps[l][z][1] == LS[Onps[l][i][2]][j][3]))
														break;
												}
												for(k=z;k<i;k++)
												{
													if((Onps[l][i][0] == Onps[l][k][0]) && (Onps[l][i][1] > Onps[l][k][1]))
														break;
												}
												if(k != i)
													continue;
												else
												{
													LS[Onps[l][i][2]][j][0] += Onps[l][i][3];

													FT[Onps[l][i][0]] = LS[Onps[l][i][2]][j][0];

													tmp[0] = Onps[l][i][0];
													tmp[1] = Onps[l][i][1];
													tmp[2] = Onps[l][i][2];
													tmp[3] = Onps[l][i][3];
													tmp[4] = Onps[l][i][4];

													for(k=i-1;k>=z;k--)
													{
														Onps[l][k+1][0] = Onps[l][k][0];
														Onps[l][k+1][1] = Onps[l][k][1];
														Onps[l][k+1][2] = Onps[l][k][2];
														Onps[l][k+1][3] = Onps[l][k][3];
														Onps[l][k+1][4] = Onps[l][k][4];
													}
													Onps[l][z][0] = tmp[0];
													Onps[l][z][1] = tmp[1];
													Onps[l][z][2] = tmp[2];
													Onps[l][z][3] = tmp[3];
													Onps[l][z][4] = tmp[4];
													//-----------------------------------------------
													flag = 1;
													break;

												}
											}
										}
										if((j == oc[z1]) && (flag == 0))
										{
											if(MST[z1] >= FT[z3])
											{
												MST[z1] += z2;
												FT[z3] = MST[z1];
											}
											else
											{
												LS[z1][oc[z1]][0] = MST[z1];
												LS[z1][oc[z1]][1] = FT[z3];			
												LS[z1][oc[z1]][2] = z3;
												LS[z1][oc[z1]++][3] = z4; 

												MST[z1] = FT[z3] + z2;
												FT[z3] = MST[z1];
											}
										}
									}	
								}
							}
							On_F[l] = 0;
							for(i=1;i<=M;i++)
							{
								if(On_F[l] < MST[i])
									On_F[l] = MST[i];
							}
							if(On_F[l] > val) // < >, < >, < >
							{
								On_F[l] = val;
								for(i=0;i<toper;i++)
								{
									Onps[l][i][0] = TMP[i][0];
									Onps[l][i][1] = TMP[i][1];
									Onps[l][i][2] = TMP[i][2];
									Onps[l][i][3] = TMP[i][3];
									Onps[l][i][4] = TMP[i][4];
								}
								break;
							}
							if(On_F[l] == val) // < >, < >, < >
							{
								if(count++ < COUNTER)
									break;
							}
						}
					}
				}
				}
			  //------------------ Local Search ----------------------------------------
			for(l=0;l<Onpop;l++)
                	{
	                	flag = 0;
                        	i = Onlook[l];
                        	if(F[i] > On_F[l])
                        	{
								F[i] = On_F[l];
					
								for(j=0;j<toper;j++)
								{
									Op[i][j][0] = Onps[l][j][0];
									Op[i][j][1] = Onps[l][j][1];
									Op[i][j][2] = Onps[l][j][2];
									Op[i][j][3] = Onps[l][j][3];
									Op[i][j][4] = Onps[l][j][4];
								}
								Scout[i] = Gen;
                        						flag = 1;
                        	}
                        	if(flag == 1)
                        	{
                        		if(Optimal > F[i])
                        	        {
                        	        	/*times(&buff);
                        	                butimer=buff.tms_utime;*/
	
                                        	Update_Optimal(i);
                                	}
                        	}
			}
			
			//fprintf(fp2,"%d \n",Optimal);
			//printf("\nCOUNT = %d Gen = %d Optimal = %d",COUNT,Gen,Optimal);
			//printf("\nBest known Schedule = \n");
			//for(i=0;i<toper;i++)
			//	printf("%d %d %d %d %d <=> ",Op_seq[i][0],Op_seq[i][1],Op_seq[i][2],Op_seq[i][3],Op_seq[i][4]);
		}
		int MSche[Mac][T_Oper][5],MScheNumber[Mac];
		int JE[NJb][Max_Op][2];
		for(i=0;i<M;i++)
		{
			MScheNumber[i]=0;
		}
		for(i=0;i<toper;i++)
		{
			/*if((Op_seq[i][0]==0)&&(Op_seq[i][1]==0))
			{
				system("pause");
			}*/
			/*if ((Op_seq[i][0]==3)&&(Op_seq[i][1]==0))
			{
				system("pause");
			}*/
			MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][0]=Op_seq[i][0]+1;
			MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][1]=Op_seq[i][1]+1;
			if((MScheNumber[Op_seq[i][2]-1]==0)&&(Op_seq[i][1]==0))
			{
				 if(ReFT[Op_seq[i][0]]>ReMST[Op_seq[i][2]])
				 {
					MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=ReFT[Op_seq[i][0]];
					MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
					MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3];
				 }
				 else
				 {
					MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=ReMST[Op_seq[i][2]];
					MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
					MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3];
				 }
			}
			else
			{
				if((MScheNumber[Op_seq[i][2]-1]==0)&&(Op_seq[i][1]>0))
				   {
					  if(JE[Op_seq[i][0]][Op_seq[i][1]-1][1]>ReMST[Op_seq[i][2]])
					  {
						 MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=JE[Op_seq[i][0]][Op_seq[i][1]-1][1];
						 MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
					  	 MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3];
					  }
					  else
					  {
						 MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=ReMST[Op_seq[i][2]];
						 MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
						 MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3];
					  }

				   }
				else
				{
				   if((MScheNumber[Op_seq[i][2]-1]>0)&&(Op_seq[i][1]==0))
					{

					   if(MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]-1][3]>ReFT[Op_seq[i][0]])
						 {
							MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]-1][3];
							MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
							MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3];
					   }
						 else
						 {
							MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=ReFT[Op_seq[i][0]];
							MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
							MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3]; 
					   }
					}
				   else
				   {
						if(JE[Op_seq[i][0]][Op_seq[i][1]-1][1]>MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]-1][3])
						{
						   MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=JE[Op_seq[i][0]][Op_seq[i][1]-1][1];
						   MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
						   MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3];
						}
						else
						{
						    MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]-1][3];
						    MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2]+Op_seq[i][3];
							MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4]=Op_seq[i][3];
						}
				   }
				   //left shift
				   for(int k=MScheNumber[Op_seq[i][2]-1];k>0;k--)
				   {
					   int aa,bb,cc;
					   if(MScheNumber[Op_seq[i][2]-1]==1)
					   {
						   aa=MSche[Op_seq[i][2]-1][k-1][2]-ReMST[Op_seq[i][2]];
					   }
					   else
					   {
						   aa=MSche[Op_seq[i][2]-1][k-1][2]-MSche[Op_seq[i][2]-1][k-2][3];
					   }
					   if(Op_seq[i][1]==0)
					   {
						 bb=MSche[Op_seq[i][2]-1][k-1][2]-ReFT[Op_seq[i][0]];
					   }
					   else
					   {
						 bb=MSche[Op_seq[i][2]-1][k-1][2]-JE[Op_seq[i][0]][Op_seq[i][1]-1][1];
					   }
						cc=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][4];
						if(((aa>cc)||(aa==cc))&&((bb>cc)||(bb==cc)))
						{
						   MSche[Op_seq[i][2]-1][k][0]=MSche[Op_seq[i][2]-1][k-1][0];
						   MSche[Op_seq[i][2]-1][k][1]=MSche[Op_seq[i][2]-1][k-1][1];
						   MSche[Op_seq[i][2]-1][k][2]=MSche[Op_seq[i][2]-1][k-1][2];
						   MSche[Op_seq[i][2]-1][k][3]=MSche[Op_seq[i][2]-1][k-1][3];
						   MSche[Op_seq[i][2]-1][k][4]=MSche[Op_seq[i][2]-1][k-1][4];

						   MSche[Op_seq[i][2]-1][k-1][0]=Op_seq[i][0]+1;
						   MSche[Op_seq[i][2]-1][k-1][1]=Op_seq[i][1]+1;
						   if(aa>bb)
						   {
							    if(Op_seq[i][1]==0)
								{
									MSche[Op_seq[i][2]-1][k-1][2]=ReFT[Op_seq[i][0]];
								}
								else
								{
									MSche[Op_seq[i][2]-1][k-1][2]=JE[Op_seq[i][0]][Op_seq[i][1]-1][1];
								}
						   }
						   else
						   {
								if(MScheNumber[Op_seq[i][2]-1]==1)
					            {
							       MSche[Op_seq[i][2]-1][k-1][2]=ReMST[Op_seq[i][2]];
								}
								else
								{
									MSche[Op_seq[i][2]-1][k-1][2]=MSche[Op_seq[i][2]-1][k-2][3];
								}
						   }
						   MSche[Op_seq[i][2]-1][k-1][3]=MSche[Op_seq[i][2]-1][k-1][2]+Op_seq[i][3];
						   MSche[Op_seq[i][2]-1][k-1][4]=Op_seq[i][3];
						}
						else
						{
							break;
						}
				   }
				 }
			}
			
			JE[Op_seq[i][0]][Op_seq[i][1]][0]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][2];
			JE[Op_seq[i][0]][Op_seq[i][1]][1]=MSche[Op_seq[i][2]-1][MScheNumber[Op_seq[i][2]-1]][3];
			MScheNumber[Op_seq[i][2]-1]++;
			//printf("%d %d %d \n",Op_seq[i][0],Op_seq[i][1],Op_seq[i][2],Op_seq[i][3]);
		}
		fp1 = fopen("MSche.txt","a");
		if(fp1 == NULL)
		{
			printf("\nError in opening file MSche.txt\n");
			system("pause");
		}
		for(i=0;i<M;i++)
		{
			for(j=0;j<MScheNumber[i];j++)
			{
			   fprintf(fp1,"%d %d %d %d %d",MSche[i][j][0],MSche[i][j][1],MSche[i][j][2],MSche[i][j][3],MSche[i][j][4]);
			   fprintf(fp1,"\n");
			}
			fprintf(fp1,"********************\n");
		}
		fprintf(fp1,"///////////////////////////////////\n");
		fp1 = fopen("job start and end time.txt","a");
		if(fp1 == NULL)
		{
			printf("\nError in opening file job start and end time.txt\n");
			system("pause");
		}
		for(i=0;i<N;i++)
		{
			for(j=0;j<Jb[i];j++)
			{
			   fprintf(fp1,"%d %d",JE[i][j][0],JE[i][j][1]);
			   fprintf(fp1,"\n");
			}
			fprintf(fp1,"********************\n");
		}
		fprintf(fp1,"///////////////////////////////////\n");
		/*fp1 = fopen("Best known Schedule.txt","a");
		if(fp1 == NULL)
		{
			printf("\nError in opening file Best known Schedule\n");
			system("pause");
		}
		for(i=0;i<toper;i++)
		{
			fprintf(fp1,"%d %d %d %d %d",Op_seq[i][0]+1,Op_seq[i][1]+1,Op_seq[i][2],Op_seq[i][3],Op_seq[i][4]);
			fprintf(fp1,"\n");
		}
		fprintf(fp1,"///////////////////////////////////\n");*/
		/*printf("\n\nBest known Schedule = \n");
		for(i=0;i<toper;i++)
		printf("%d %d %d %d %d <=> ",Op_seq[i][0]+1,Op_seq[i][1]+1,Op_seq[i][2],Op_seq[i][3],Op_seq[i][4]);*/
		/*fp1 = fopen("sequence.txt","a");
		if(fp1 == NULL)
		{
			printf("\nError in opening file\n");
			system("pause");
		}
		for(i=0;i<toper;i++)
		{
			fprintf(fp1,"%d %d %d %d",Op_seq[i][0]+1,Op_seq[i][1]+1,Op_seq[i][2],Op_seq[i][3]);
			fprintf(fp1,"\n");
		}*/
		OPT[it] = Optimal;
		
		fp2	= fopen("result.txt","a");
		if(fp2 == NULL)
		{
			printf("\nError in opening file\n");
			system("pause");
		}
		/*butime[it]=(butimer-utimer)/100.0;
                times(&buff);
                utime[it]=(buff.tms_utime-utimer)/100.0;*/

               // printf("\nseed[%d] = %d Gen = [%d] TTB = %.2f  TET = %.2f  OPT[%d] = %d\n",it,seed[it],Gen,butime[it],utime[it],it,OPT[it]);
				//printf("\nseed[%d] = %d Gen = [%d] OPT[%d] = %d\n",it,seed[it],Gen,it,OPT[it]);
				fprintf(fp2,"\nseed[%d] = %d Gen = [%d] OPT[%d] = %d\n",it,seed[it],Gen,it,OPT[it]);
                fflush(stdout);
		time_t end,time; /*注意计时所用的变量名称*/	
					end=clock();
		  time=end-start;
		fp2	= fopen("time.txt","a");
		if(fp2 == NULL)
		{
			printf("\nError in opening file time txt\n");
			system("pause");
		}
		fprintf(fp2,"%d\n",time);
		//for(i=0;i<toper;i++)
  //              {
	 //       	for(j=i-1;j>=0;j--)
  //                      {
	 //               	if((Op_seq[i][0] == Op_seq[j][0]) && ((Op_seq[i][1] < Op_seq[j][1])))
  //                              {
		//			printf("\nError in operation sequence\n");
		//			exit(0);
		//		}
		//	}
		//}
		//for(i=0;i<N;i++)
		//{
		//	FT[i] = ReFT[i]; //Start time and Completion time
		//}
		////Evaluate makespan
		//for(i=1;i<=M;i++)
		//{
		//	MST[i] = ReMST[i]; //Machine_Spantime
		//}
		//for(i=0;i<toper;i++)
		//{
		//	if(MST[Op_seq[i][2]] >= FT[Op_seq[i][0]])
		//		{
		//			MST[Op_seq[i][2]] += Op_seq[i][3];
		//			FT[Op_seq[i][0]] = MST[Op_seq[i][2]];
		//		}
		//	else
		//		{
		//			MST[Op_seq[i][2]] = FT[Op_seq[i][0]] + Op_seq[i][3];
		//			FT[Op_seq[i][0]] = MST[Op_seq[i][2]];
		//		}
		//}
		//val = 0;
		//for(i=1;i<=M;i++)
		//{
		//	if(val < MST[i])
		//		val = MST[i];
		//}
		//fprintf(fp2,"\nval[%d] = %d\n",val);
	}
	Sum = 0;
        To_btime=0;
        To_utime=0;
        for(it=0;it<RUN;it++)
        {
                Sum += OPT[it];
                //To_btime+= butime[it];
                //To_utime+= utime[it];
        }
        Avrg = (float)Sum/RUN;
        /*Av_btime = To_btime/RUN;
        Av_utime = To_utime/RUN;*/

        A_Sd = 0;
        for(it=0;it<RUN;it++)
                A_Sd += (OPT[it]-Avrg)*(OPT[it]-Avrg);
        
        Std_dev_Val = sqrt(A_Sd/RUN);
        
        /*printf("\n\nATTB = %.2f Avg = %.2f ATET = %.2f  SD = %.2f ",Av_btime,Avrg,Av_utime,Std_dev_Val);*/
		//printf("\n\nAvg = %.2f SD = %.2f ",Avrg,Std_dev_Val);
        
	//printf("\n\n*********************************************************************************\n");
        printf("\n");
		//system("pause");
		/*}*/
	   return 0;
}



