#include <vector>
#include <algorithm> 
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include "M_data.h"
#include <numeric>
#include <boost/random.hpp>
using namespace std;
using namespace boost;

#define PI 3.141592
#define MAXFunc 6
#define MAXREP  30

// function for test
double abs_function_sphere(vector <double> x);
double abs_function_schwefel(vector <double> x);
double abs_function_step(vector <double> x);
double abs_function_Rosenbrock(vector <double> x);
double abs_function_Rotated_hyper_ellipsoid(vector <double> x);
double abs_function_Generalized_swefel(vector <double> x);
double abs_function_Rastrign(vector <double> x);
double abs_function_Ackley(vector <double> x);
double abs_function_Griewank(vector <double> x);
double abs_function_Six_Hump_Camel_Back(vector <double> x);

// function in CEC2005
double abs_function_shift_sphere(vector <double> x);
double abs_function_shift_schwefel(vector <double> x);
double abs_function_shift_rosenbrock(vector <double> x);
double abs_function_shift_rastrigin(vector <double> x);
double abs_function_shift_Rotated_Griewank(vector <double> x);
double abs_function_shifted_rotated_rastrign(vector <double> x);
// function for test
double function_sphere(vector <double> x);
double function_schwefel(vector <double> x);
double function_step(vector <double> x);
double function_Rosenbrock(vector <double> x);
double function_Rotated_hyper_ellipsoid(vector <double> x);
double function_Generalized_swefel(vector <double> x);
double function_Rastrign(vector <double> x);
double function_Ackley(vector <double> x);
double function_Griewank(vector <double> x);
double function_Six_Hump_Camel_Back(vector <double> x);

// function in CEC2005
double function_shift_sphere(vector <double> x);
double function_shift_schwefel(vector <double> x);
double function_shift_rosenbrock(vector <double> x);
double function_shift_rastrigin(vector <double> x);
double function_shift_Rotated_Griewank(vector <double> x);
double function_shifted_rotated_rastrign(vector <double> x);

// the pointer to each problem;
double (*func_Fitness)(vector <double> x);
double (*func)(vector <double> x);

//for normal distribution ---boost
boost::mt19937 generator_normal;
boost::mt19937 generator_uniform(42u);

// for HS algorithm
void InitialParms();
void Initialization();
void Evolve();
void CheckHSM();
void FitnessProblemSpecific();
void ProblemSpecific();
void Statistics();

// parameters of problems.
double UBi[]={100,10,30,100,100,500,5.12,32,600,5,100,100,100,5,100,5};
double LBi[]={-100,-10,-30,-100,-100,-500,-5.12,-32,-600,-5,-100,-100,-100,-5,-100,-5};
int Dimension[]={100,100,100,100,100,100,100,100,100,2,100,100,100,100,100,100};
int FuncNo=0;

// variables of Harmony search algorithm
vector <vector <double> > Harmonies;
vector <double> HarmonyValue;
int HMS,NI;
double HMCR, PAR, BW, bestsofar;
// records for statistics
double FitArray[MAXFunc][MAXREP];
double BWmax,BWmin;

void main(void)
{
	//for MGHS
	int AA[]={5,10,20,50};
	for(int i=0;i<4;i++)
	{
		HMS=AA[i];		
		for(FuncNo=5; FuncNo<MAXFunc; FuncNo++)
		{
			double avg=0,avgF=0;
			FitnessProblemSpecific();				
			ProblemSpecific();

			for(int rep=0;rep<MAXREP;rep++)
			{
				generator_normal.seed(static_cast <unsigned short> (100+rep));
				generator_uniform.seed(static_cast <unsigned short> (100+rep));
				InitialParms();				
				Initialization();
				Evolve(); //SGHS
				CheckHSM();
				bestsofar=*min_element(HarmonyValue.begin(),HarmonyValue.end());
				
				double minfit=HarmonyValue[0];
				int pt=0;
				for(int xx=1;xx<HarmonyValue.size();xx++)
				{
					if(HarmonyValue[xx]<minfit)
					{
						minfit=HarmonyValue[xx];
						pt=xx;
					}
				}
				
				double tempvalF=func_Fitness(Harmonies[pt]);
				printf("\n%f",tempvalF);
				if(tempvalF!=bestsofar) printf("\n---------erro!-----------%f,%f",tempvalF,bestsofar);
				double tempval=func(Harmonies[pt]);
				printf("\n%f",tempval);
				avgF+=tempvalF;
				avg+=tempval;	
				FitArray[FuncNo][rep]=tempval;
			}

			avg/=MAXREP;
			avgF/=MAXREP;
			printf("\n----Func=%2d, avgF=%f avg=%f---", FuncNo, avgF, avg);				

			FILE *f;
			fopen_s(&f,"avg.txt","a");
			fprintf(f,"\n%f,%f,", avgF,avg);
			fclose(f);

			printf("\n--------------\n");
		}
		Statistics();
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
	case 14:func_Fitness=abs_function_shift_Rotated_Griewank;break;
	case 15:func_Fitness=abs_function_shifted_rotated_rastrign;break;
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
	case 14:func=function_shift_Rotated_Griewank;break;
	case 15:func=function_shifted_rotated_rastrign;break;
	default:func=NULL;break;		
	}
}

void InitialParms()
{
	if(FuncNo!=9) 
		Dimension[FuncNo]=100;
	else
		Dimension[FuncNo]=2;
//	HMS=5;
	PAR=0.9;
	HMCR=0.98;
	NI=50000;
	BWmax=1.0/100*(UBi[FuncNo]-LBi[FuncNo]);
	BWmin=5.0/10000;
//	BWmin=1.0/1000;
}

void Initialization()
{
	// produce HMS harmonies;
	Harmonies.clear();
	vector <double> temphm;
	boost::uniform_real<> uni_dist(0,1);
	boost::variate_generator<mt19937&,boost::uniform_real<>>uni(generator_uniform,uni_dist);

	for(int j=0;j<HMS;j++)
	{
		for(int i=0;i<Dimension[FuncNo];i++)
		{
		//	double tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*rand()/RAND_MAX;
			double tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*uni();
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

void Evolve()	// For GHS
{
	
		{
			double minfit=HarmonyValue[0];
			int pt=0;
			for(int xx=1;xx<HarmonyValue.size();xx++)
			{
				if(HarmonyValue[xx]<minfit)
				{
					minfit=HarmonyValue[xx];
					pt=xx;
				}
			}
			double sp=func(Harmonies[pt]);
			FILE *f;
			fopen_s(&f,"plot.txt","a");
			fprintf(f,"\n%8.6f,",sp);
			fclose(f);
		}

	vector <double> HMCRArray,PARArray,BWArray;	
	double TempHMCR=HMCR;
	double TempPAR=PAR;
	double TempBW=BW;
	boost::uniform_real<> uni_dist(0,1);
	boost::variate_generator<mt19937&,boost::uniform_real<>>uni_real(generator_uniform,uni_dist);
	boost::uniform_int <> uni_dist_int(0,HMS-1),uni_dist_flip(0,1);
	boost::variate_generator<mt19937&,boost::uniform_int<>>uni_int(generator_uniform,uni_dist_int),uni_flip(generator_uniform,uni_dist_flip);

	for(int Gen=0;Gen<NI/100;Gen++)
	{
		normal_distribution <> normal_HMCR(HMCR,0.05),normal_PAR(PAR,0.05);
		variate_generator<mt19937&,normal_distribution<>>ndist_HMCR(generator_normal,normal_HMCR), ndist_PAR(generator_normal,normal_PAR);

		bool bFlag=false;
		for(int divGen=0;divGen<100;divGen++)
		{
		//	if(Gen%5==0)
			{
				bFlag=false;
				do{
					TempHMCR=ndist_HMCR();
				}while(TempHMCR>1 || TempHMCR<0.9);
				do{
					TempPAR=ndist_PAR();
				}while(TempPAR>1 || TempPAR<0);
			}

			if(Gen*100+divGen<NI/2)
				TempBW=BWmax-(BWmax-BWmin)*(Gen*100+divGen)*2/NI;
			else
				TempBW=BWmin;

			//--------Find the best harmony in the memory
			int i, pt=0; 
			double tempval, minv=HarmonyValue[0];
			for(i=1;i<HMS;i++)
			{
				if(minv>HarmonyValue[i])
				{
					minv=HarmonyValue[i];
					pt=i;
				}
			}

			//-----------improvise a harmony---------
			vector <double> nhm;		
			for(i=0;i<Dimension[FuncNo];i++)
			{
				if(uni_real()<TempHMCR) //Memory consideration
				{
					if(uni_real()<TempPAR)
					{
						tempval=Harmonies[pt][i];					
					}
					else
					{
						if(uni_flip())
							tempval=Harmonies[uni_int()][i]+uni_real()*TempBW;
						else
							tempval=Harmonies[uni_int()][i]-uni_real()*TempBW;
					}

				}
				else //Randomly selection 
				{
					tempval=LBi[FuncNo]+(UBi[FuncNo]-LBi[FuncNo])*uni_real();
				}									
				nhm.push_back(tempval);
			}

			// Find the worst harmony in the memory
			double max=HarmonyValue[0];
			pt=0;
			for(i=1;i<HMS;i++)
			{
				if(HarmonyValue[i]>max)
				{
					max=HarmonyValue[i];
					pt=i;
				}
			}

			// update the harmony memory;
			double tv=(*func_Fitness)(nhm);
			if(tv<max)
			{
				HarmonyValue[pt]=tv;	
				copy(nhm.begin(),nhm.end(),Harmonies[pt].begin());
				bFlag=true;
			}

	//		if((Gen+1)%5==0)
			{
				if(bFlag)
				{
					HMCRArray.push_back(TempHMCR);
					PARArray.push_back(TempPAR);
				}
			}
		}


		if(HMCRArray.size())
		{
			HMCR=0;
			vector <double>::iterator y;
			for(y=HMCRArray.begin();y!=HMCRArray.end();y++)
			{
				HMCR+=*y;
			}
			HMCR/=HMCRArray.size();
			HMCRArray.clear();
		}

		if(PARArray.size())
		{
			PAR=0;
			vector <double>::iterator y;
			for(y=PARArray.begin();y!=PARArray.end();y++)
			{
				PAR+=*y;
			}
			PAR/=PARArray.size();
			PARArray.clear();
		}
	
	

			if((Gen+1)%10==0){
			double minfit=HarmonyValue[0];
			int pt=0;
			for(int xx=1;xx<HarmonyValue.size();xx++)
			{
				if(HarmonyValue[xx]<minfit)
				{
					minfit=HarmonyValue[xx];
					pt=xx;
				}
			}
			double sp=func(Harmonies[pt]);
			FILE *f;
			fopen_s(&f,"plot.txt","a");
			fprintf(f,"%8.6f,",sp);
			fclose(f);
		}
	}
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

		fprintf(fp,"\n%16.6f==%16.6f",avg,SD);
		
		meanmin+=min;
		meanavg+=avg;
		meanmax+=max;
		meanSD+=SD;
	}
	
	fclose(fp);

	fopen_s(&fp,"ztext.txt","a");
	fprintf(fp,"\n");
	for(FuncNo=0;FuncNo<MAXFunc; FuncNo++)
	{
		fprintf(fp,"\n");
		for(int i=0;i<MAXREP;i++)
		{
			fprintf(fp,"%16.6f,",FitArray[FuncNo][i]);
		}
	}
	fclose(fp);
}

double abs_function_sphere(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=(*i)*(*i);
	}
	return sum;
}

double abs_function_schwefel(vector <double> x)
{
	double sum=0,product=1;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=fabs(*i);
		product*=fabs(*i);
	}
	return sum+product;
}

double abs_function_step(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=int(*i+0.5)*int(*i+0.5);
	}
	return sum;
}

double abs_function_Rosenbrock(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end()-1;i++)
	{
		sum+=(*i-1)*(*i-1)+100*(*(i+1)-(*i)*(*i))*(*(i+1)-(*i)*(*i));
	}
	return fabs(sum);
}

double abs_function_Rotated_hyper_ellipsoid(vector <double> x)
{
	double sum=0,sum1;
	vector<double>::iterator i,j;
	for(i=x.begin();i!=x.end();i++)
	{
		sum1=0;
		for(j=x.begin();j<=i;j++)
			sum1+=*j;
		sum+=sum1*sum1;
	}
	return sum;
}

double abs_function_Generalized_swefel(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum-=(*i)*sin(sqrt(fabs(*i)));
	}
	sum+=418.9829*x.size();
	return fabs(sum);
}


double abs_function_Rastrign(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=10-10*cos(2*PI*(*i))+(*i)*(*i);
	}
	return fabs(sum);
}

double abs_function_Ackley(vector <double> x)
{
	double sum=0,sum1=0,sum2=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum1+=(*i)*(*i);
		sum2+=cos(2*PI*(*i));
	}

	sum=-20*exp(-0.2*sqrt(sum1/x.size ()))-exp(sum2/x.size())+20+2.718282;
	return fabs(sum);
}

double abs_function_Griewank(vector <double> x)
{
	double sum=0,sum1=0,sum2=1;
	vector<double>::iterator i;
	int j=0;
	for(i=x.begin();i!=x.end();i++)
	{
		j++;
		sum1+=(*i)*(*i);
		sum2*=cos(*i/sqrt(double(j)));
	}

	sum=sum1/4000-sum2+1;
	return fabs(sum);
}

double abs_function_Six_Hump_Camel_Back(vector <double> x)
{
	double sum=4*x[0]*x[0]-2.1*x[0]*x[0]*x[0]*x[0]+x[0]*x[0]*x[0]*x[0]*x[0]*x[0]/3
		+x[0]*x[1]-4*x[1]*x[1]+4*x[1]*x[1]*x[1]*x[1];
	sum+=1.0316285;
	double aa=fabs(sum);
	return fabs(sum);
}

// for shift rotated composit problems

double abs_function_shift_sphere(vector <double> x)
{
	double O_shpere[100]={-3.9311900e+001,5.8899900e+001,-4.6322400e+001,-7.4651500e+001,-1.6799700e+001,-8.0544100e+001,-1.0593500e+001,2.4969400e+001,8.9838400e+001,9.1119000e+000,-1.0744300e+001,-2.7855800e+001,-1.2580600e+001,7.5930000e+000,7.4812700e+001,6.8495900e+001,-5.3429300e+001,7.8854400e+001,-6.8595700e+001,6.3743200e+001,3.1347000e+001,-3.7501600e+001,3.3892900e+001,-8.8804500e+001,-7.8771900e+001,-6.6494400e+001,4.4197200e+001,1.8383600e+001,2.6521200e+001,8.4472300e+001,3.9176900e+001,-6.1486300e+001,-2.5603800e+001,-8.1182900e+001,5.8695800e+001,-3.0838600e+001,-7.2672500e+001,8.9925700e+001,-1.5193400e+001,-4.3337000e+000,5.3430000e+000,1.0560300e+001,-7.7726800e+001,5.2085900e+001,4.0394400e+001,8.8332800e+001,-5.5830600e+001,1.3181000e+000,3.6025000e+001,-6.9927100e+001,-8.6279000e+000,-5.6894400e+001,8.5129600e+001,1.7673600e+001,6.1529000e+000,-1.7695700e+001,-5.8953700e+001,3.0356400e+001,1.5920700e+001,-1.8008200e+001,8.0641100e+001,-4.2391200e+001,7.6277600e+001,-5.0165200e+001,-7.3573600e+001,2.8336900e+001,-5.7990500e+001,-2.2732700e+001,5.2026900e+001,3.9259900e+001,1.0867900e+001,7.7820700e+001,6.6039500e+001,-5.0066700e+001,5.5706300e+001,7.3714100e+001,3.8529600e+001,-5.6786500e+001,-8.9647700e+001,3.7957600e+001,2.9472000e+001,-3.5464100e+001,-3.1786800e+001,7.7323500e+001,5.4790600e+001,-4.8279400e+001,7.4271400e+001,7.2610300e+001,6.2964000e+001,-1.4144600e+001,2.0492300e+001,4.6589700e+001,-8.3602100e+001,-4.6480900e+001,8.3737300e+001,-7.9661100e+001,2.4347900e+001,-1.7230300e+001,7.2340400e+001,-3.6402200e+001};
	double Bias_sphere=-4.5000000e+002;
	double sum=0;
	for(unsigned int i=0;i<x.size();i++)
	{
		sum+=(x[i]-O_shpere[i])*(x[i]-O_shpere[i]);
	}
	return sum;
}

double abs_function_shift_schwefel(vector <double> x)
{
	double O_schwefel[100]={3.5626700e+001,-8.2912300e+001,-1.0642300e+001,-8.3581500e+001,8.3155200e+001,4.7048000e+001,-8.9435900e+001,-2.7421900e+001,7.6144800e+001,-3.9059500e+001,4.8885700e+001,-3.9828000e+000,-7.1924300e+001,6.4194700e+001,-4.7733800e+001,-5.9896000e+000,-2.6282800e+001,-5.9181100e+001,1.4602800e+001,-8.5478000e+001,-5.0490100e+001,9.2400000e-001,3.2397800e+001,3.0238800e+001,-8.5094900e+001,6.0119700e+001,-3.6218300e+001,-8.5883000e+000,-5.1971000e+000,8.1553100e+001,-2.3431600e+001,-2.5350500e+001,-4.1248500e+001,8.8018000e+000,-2.4222200e+001,-8.7980700e+001,7.8047300e+001,-4.8052800e+001,1.4017700e+001,-3.6640500e+001,1.2216800e+001,1.8144900e+001,-6.4564700e+001,-8.4849300e+001,-7.6608800e+001,-1.7042000e+000,-3.6076100e+001,3.7033600e+001,1.8443100e+001,-6.4359000e+001,-3.9369200e+001,-1.7714000e+001,3.0198500e+001,-1.8548300e+001,9.6866000e+000,8.2600900e+001,-4.5525600e+001,5.1443000e+000,7.4204000e+001,6.6810300e+001,-6.3470400e+001,1.3032900e+001,-5.6878000e+000,2.9527100e+001,-4.3530000e-001,-2.6165200e+001,-6.6847000e+000,-8.0229100e+001,-2.9581500e+001,8.2042200e+001,7.7177000e+001,-1.1277000e+001,3.2075900e+001,-2.6858000e+000,8.1509600e+001,6.4077000e+001,-2.6129400e+001,-8.4782000e+001,-6.2876800e+001,-3.7635500e+001,7.6891600e+001,5.3417000e+001,-2.5331100e+001,-3.8070200e+001,-8.4173800e+001,-1.1224600e+001,-8.3461900e+001,-1.7550800e+001,-3.6528500e+001,8.9552800e+001,2.5879400e+001,6.8625200e+001,5.5796800e+001,-2.9597500e+001,-5.8097600e+001,6.5741300e+001,-8.8703000e+000,-5.3281000e+000,7.4066100e+001,4.0338000e+000};
	double Bias_schwefel=-4.5000000e+002;
	double sum=0;
	for(unsigned int i=0;i<x.size();i++)
	{
		double parsum=0;
		for(unsigned int j=0;j<i;j++)
		{
			parsum+=x[j]-O_schwefel[j];
		}
		sum+=parsum*parsum;
	}
	return sum;
}

double abs_function_shift_rosenbrock(vector <double> x)
{
	double O_rosenbrock[100]={8.1023200e+001,-4.8395000e+001,1.9231600e+001,-2.5231000e+000,7.0433800e+001,4.7177400e+001,-7.8358000e+000,-8.6669300e+001,5.7853200e+001,-9.9533000e+000,2.0777800e+001,5.2548600e+001,7.5926300e+001,4.2877300e+001,-5.8272000e+001,-1.6972800e+001,7.8384500e+001,7.5042700e+001,-1.6151300e+001,7.0856900e+001,-7.9579500e+001,-2.6483700e+001,5.6369900e+001,-8.8224900e+001,-6.4999600e+001,-5.3502200e+001,-5.4230000e+001,1.8682600e+001,-4.1006100e+001,-5.4213400e+001,-8.7250600e+001,4.4421400e+001,-9.8826000e+000,7.7726600e+001,-6.1210000e+000,-1.4643000e+001,6.2319800e+001,4.5274000e+000,-5.3523400e+001,3.0984700e+001,6.0861300e+001,-8.6464800e+001,3.2629800e+001,-2.1693400e+001,5.9723200e+001,5.0630000e-001,3.7704800e+001,-1.2799300e+001,-3.5168800e+001,-5.5862300e+001,-5.5182300e+001,3.2800100e+001,-3.5502400e+001,7.5012000e+000,-6.2842800e+001,3.5621700e+001,-2.1892800e+001,6.4802000e+001,6.3657900e+001,1.6841300e+001,-6.2050000e-001,7.1958400e+001,5.7893200e+001,2.6083800e+001,5.7235300e+001,2.8840900e+001,-2.8445200e+001,-3.7849300e+001,-2.8585100e+001,6.1342000e+000,4.0880300e+001,-3.4327700e+001,6.0929200e+001,1.2253000e+001,-2.3325500e+001,3.6493100e+001,8.3828000e+000,-9.9215000e+000,3.5022100e+001,2.1835800e+001,5.3067700e+001,8.2231800e+001,4.0662000e+000,6.8425500e+001,-5.8867800e+001,8.6354400e+001,-4.1139400e+001,-4.4580700e+001,6.7633500e+001,4.2715000e+001,-6.5426600e+001,-8.7883700e+001,7.0901600e+001,-5.4155100e+001,-3.6229800e+001,2.9059600e+001,-3.8806400e+001,-5.5396000e+000,-7.8339300e+001,8.7900200e+001};
	double Bias_rosenbrock=390;
	double sum=0;
	for(unsigned int i=0;i<x.size()-1;i++)
	{
		sum+=100*((x[i]-O_rosenbrock[i]+1)*(x[i]-O_rosenbrock[i]+1)-(x[i+1]-O_rosenbrock[i+1]+1))
			    *((x[i]-O_rosenbrock[i]+1)*(x[i]-O_rosenbrock[i]+1)-(x[i+1]-O_rosenbrock[i+1]+1))
			    +(x[i]-O_rosenbrock[i]+1-1)*(x[i]-O_rosenbrock[i]+1-1);
	}
	return sum;
}

double abs_function_shift_rastrigin(vector <double> x)
{
	double O_rastrigin[100]={1.9005000e+000,-1.5644000e+000,-9.7880000e-001,-2.2536000e+000,2.4990000e+000,-3.2853000e+000,9.7590000e-001,-3.6661000e+000,9.8500000e-002,-3.2465000e+000,3.8060000e+000,-2.6834000e+000,-1.3701000e+000,4.1821000e+000,2.4856000e+000,-4.2237000e+000,3.3653000e+000,2.1532000e+000,-3.0929000e+000,4.3105000e+000,-2.9861000e+000,3.4936000e+000,-2.7289000e+000,-4.1266000e+000,-2.5900000e+000,1.3124000e+000,-1.7990000e+000,-1.1890000e+000,-1.0530000e-001,-3.1074000e+000,-3.9641000e+000,-4.3387000e+000,3.0705000e+000,3.3205000e+000,-3.8178000e+000,-1.4980000e+000,-4.3807000e+000,2.7110000e+000,-3.7956000e+000,-2.3627000e+000,4.0086000e+000,-1.3728000e+000,-4.4362000e+000,-2.9183000e+000,-2.2457000e+000,3.0650000e-001,-8.9240000e-001,-3.2364000e+000,-1.2521000e+000,2.7198000e+000,-3.9787000e+000,3.0678000e+000,-4.2400000e+000,-3.9580000e+000,3.9479000e+000,2.2030000e-001,-2.6124000e+000,2.6498000e+000,-2.3256000e+000,-1.5383000e+000,3.4760000e+000,2.4462000e+000,2.4575000e+000,3.7409000e+000,-2.4887000e+000,3.8555000e+000,-1.1426000e+000,1.3389000e+000,2.2323000e+000,2.3137000e+000,-4.3370000e+000,3.9260000e+000,3.5905000e+000,-1.2858000e+000,-2.0113000e+000,2.9087000e+000,3.9278000e+000,1.0812000e+000,-7.4610000e-001,3.4740000e+000,2.3036000e+000,-3.3781000e+000,-4.4910000e-001,9.4020000e-001,-3.0583000e+000,-4.2165000e+000,-2.3604000e+000,8.3640000e-001,1.4773000e+000,-2.7292000e+000,-1.5904000e+000,7.6960000e-001,5.1640000e-001,2.6576000e+000,-5.4270000e-001,1.0358000e+000,6.9260000e-001,-4.2775000e+000,-1.5911000e+000,-3.5775000e+000};
	double Bias_rastrigin=-330;
	double sum=0;
	for(unsigned int i=0;i<x.size();i++)
	{
		sum+=10-10*cos(2*PI*(x[i]-O_rastrigin[i]))+(x[i]-O_rastrigin[i])*(x[i]-O_rastrigin[i]);
	}
	return fabs(sum);
}



double function_sphere(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=(*i)*(*i);
	}
	return sum;
}

double function_schwefel(vector <double> x)
{
	double sum=0,product=1;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=fabs(*i);
		product*=fabs(*i);
	}
	return sum+product;
}

double function_step(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=int(*i+0.5)*int(*i+0.5);
	}
	return sum;
}

double function_Rosenbrock(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end()-1;i++)
	{
		sum+=(*i-1)*(*i-1)+100*(*(i+1)-(*i)*(*i))*(*(i+1)-(*i)*(*i));
	}
	return sum;
}

double function_Rotated_hyper_ellipsoid(vector <double> x)
{
	double sum=0,sum1;
	vector<double>::iterator i,j;
	for(i=x.begin();i!=x.end();i++)
	{
		sum1=0;
		for(j=x.begin();j<=i;j++)
			sum1+=*j;
		sum+=sum1*sum1;
	}
	return sum;
}

double function_Generalized_swefel(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum-=(*i)*sin(sqrt(fabs(*i)));
	}
	sum+=x.size()*418.9829;
//	sum+=12569.5;
	return sum;
}


double function_Rastrign(vector <double> x)
{
	double sum=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum+=10-10*cos(2*PI*(*i))+(*i)*(*i);
	}
	return sum;
}

double function_Ackley(vector <double> x)
{
	double sum=0,sum1=0,sum2=0;
	vector<double>::iterator i;
	for(i=x.begin();i!=x.end();i++)
	{
		sum1+=(*i)*(*i);
		sum2+=cos(2*PI*(*i));
	}

	sum=-20*exp(-0.2*sqrt(sum1/x.size()))-exp(sum2/x.size())+20+2.718282;
	return sum;
}

double function_Griewank(vector <double> x)
{
	double sum=0,sum1=0,sum2=1;
	vector<double>::iterator i;
	int j=0;
	for(i=x.begin();i!=x.end();i++)
	{
		j++;
		sum1+=(*i)*(*i);
		sum2*=cos(*i/sqrt(double(j)));
	}

	sum=sum1/4000-sum2+1;
	return sum;
}

double function_Six_Hump_Camel_Back(vector <double> x)
{
	double sum=4*x[0]*x[0]-2.1*x[0]*x[0]*x[0]*x[0]+x[0]*x[0]*x[0]*x[0]*x[0]*x[0]/3
		+x[0]*x[1]-4*x[1]*x[1]+4*x[1]*x[1]*x[1]*x[1];
//	sum+=1.0316285;
	return sum;
}

// for shift rotated composit problems
double function_shift_sphere(vector <double> x)
{
	double O_shpere[100]={-3.9311900e+001,5.8899900e+001,-4.6322400e+001,-7.4651500e+001,-1.6799700e+001,-8.0544100e+001,-1.0593500e+001,2.4969400e+001,8.9838400e+001,9.1119000e+000,-1.0744300e+001,-2.7855800e+001,-1.2580600e+001,7.5930000e+000,7.4812700e+001,6.8495900e+001,-5.3429300e+001,7.8854400e+001,-6.8595700e+001,6.3743200e+001,3.1347000e+001,-3.7501600e+001,3.3892900e+001,-8.8804500e+001,-7.8771900e+001,-6.6494400e+001,4.4197200e+001,1.8383600e+001,2.6521200e+001,8.4472300e+001,3.9176900e+001,-6.1486300e+001,-2.5603800e+001,-8.1182900e+001,5.8695800e+001,-3.0838600e+001,-7.2672500e+001,8.9925700e+001,-1.5193400e+001,-4.3337000e+000,5.3430000e+000,1.0560300e+001,-7.7726800e+001,5.2085900e+001,4.0394400e+001,8.8332800e+001,-5.5830600e+001,1.3181000e+000,3.6025000e+001,-6.9927100e+001,-8.6279000e+000,-5.6894400e+001,8.5129600e+001,1.7673600e+001,6.1529000e+000,-1.7695700e+001,-5.8953700e+001,3.0356400e+001,1.5920700e+001,-1.8008200e+001,8.0641100e+001,-4.2391200e+001,7.6277600e+001,-5.0165200e+001,-7.3573600e+001,2.8336900e+001,-5.7990500e+001,-2.2732700e+001,5.2026900e+001,3.9259900e+001,1.0867900e+001,7.7820700e+001,6.6039500e+001,-5.0066700e+001,5.5706300e+001,7.3714100e+001,3.8529600e+001,-5.6786500e+001,-8.9647700e+001,3.7957600e+001,2.9472000e+001,-3.5464100e+001,-3.1786800e+001,7.7323500e+001,5.4790600e+001,-4.8279400e+001,7.4271400e+001,7.2610300e+001,6.2964000e+001,-1.4144600e+001,2.0492300e+001,4.6589700e+001,-8.3602100e+001,-4.6480900e+001,8.3737300e+001,-7.9661100e+001,2.4347900e+001,-1.7230300e+001,7.2340400e+001,-3.6402200e+001};
	double Bias_sphere=-4.5000000e+002;
	double sum=0;
	for(unsigned int i=0;i<x.size();i++)
	{
		sum+=(x[i]-O_shpere[i])*(x[i]-O_shpere[i]);
	}
	return sum+Bias_sphere;
}

double function_shift_schwefel(vector <double> x)
{
	double O_schwefel[100]={3.5626700e+001,-8.2912300e+001,-1.0642300e+001,-8.3581500e+001,8.3155200e+001,4.7048000e+001,-8.9435900e+001,-2.7421900e+001,7.6144800e+001,-3.9059500e+001,4.8885700e+001,-3.9828000e+000,-7.1924300e+001,6.4194700e+001,-4.7733800e+001,-5.9896000e+000,-2.6282800e+001,-5.9181100e+001,1.4602800e+001,-8.5478000e+001,-5.0490100e+001,9.2400000e-001,3.2397800e+001,3.0238800e+001,-8.5094900e+001,6.0119700e+001,-3.6218300e+001,-8.5883000e+000,-5.1971000e+000,8.1553100e+001,-2.3431600e+001,-2.5350500e+001,-4.1248500e+001,8.8018000e+000,-2.4222200e+001,-8.7980700e+001,7.8047300e+001,-4.8052800e+001,1.4017700e+001,-3.6640500e+001,1.2216800e+001,1.8144900e+001,-6.4564700e+001,-8.4849300e+001,-7.6608800e+001,-1.7042000e+000,-3.6076100e+001,3.7033600e+001,1.8443100e+001,-6.4359000e+001,-3.9369200e+001,-1.7714000e+001,3.0198500e+001,-1.8548300e+001,9.6866000e+000,8.2600900e+001,-4.5525600e+001,5.1443000e+000,7.4204000e+001,6.6810300e+001,-6.3470400e+001,1.3032900e+001,-5.6878000e+000,2.9527100e+001,-4.3530000e-001,-2.6165200e+001,-6.6847000e+000,-8.0229100e+001,-2.9581500e+001,8.2042200e+001,7.7177000e+001,-1.1277000e+001,3.2075900e+001,-2.6858000e+000,8.1509600e+001,6.4077000e+001,-2.6129400e+001,-8.4782000e+001,-6.2876800e+001,-3.7635500e+001,7.6891600e+001,5.3417000e+001,-2.5331100e+001,-3.8070200e+001,-8.4173800e+001,-1.1224600e+001,-8.3461900e+001,-1.7550800e+001,-3.6528500e+001,8.9552800e+001,2.5879400e+001,6.8625200e+001,5.5796800e+001,-2.9597500e+001,-5.8097600e+001,6.5741300e+001,-8.8703000e+000,-5.3281000e+000,7.4066100e+001,4.0338000e+000};
	double Bias_schwefel=-4.5000000e+002;
	double sum=0;
	for(unsigned int i=0;i<x.size();i++)
	{
		double parsum=0;
		for(unsigned int j=0;j<i;j++)
		{
			parsum+=x[j]-O_schwefel[j];
		}
		sum+=parsum*parsum;
	}
	return sum+Bias_schwefel;
}

double function_shift_rosenbrock(vector <double> x)
{
	double O_rosenbrock[100]={8.1023200e+001,-4.8395000e+001,1.9231600e+001,-2.5231000e+000,7.0433800e+001,4.7177400e+001,-7.8358000e+000,-8.6669300e+001,5.7853200e+001,-9.9533000e+000,2.0777800e+001,5.2548600e+001,7.5926300e+001,4.2877300e+001,-5.8272000e+001,-1.6972800e+001,7.8384500e+001,7.5042700e+001,-1.6151300e+001,7.0856900e+001,-7.9579500e+001,-2.6483700e+001,5.6369900e+001,-8.8224900e+001,-6.4999600e+001,-5.3502200e+001,-5.4230000e+001,1.8682600e+001,-4.1006100e+001,-5.4213400e+001,-8.7250600e+001,4.4421400e+001,-9.8826000e+000,7.7726600e+001,-6.1210000e+000,-1.4643000e+001,6.2319800e+001,4.5274000e+000,-5.3523400e+001,3.0984700e+001,6.0861300e+001,-8.6464800e+001,3.2629800e+001,-2.1693400e+001,5.9723200e+001,5.0630000e-001,3.7704800e+001,-1.2799300e+001,-3.5168800e+001,-5.5862300e+001,-5.5182300e+001,3.2800100e+001,-3.5502400e+001,7.5012000e+000,-6.2842800e+001,3.5621700e+001,-2.1892800e+001,6.4802000e+001,6.3657900e+001,1.6841300e+001,-6.2050000e-001,7.1958400e+001,5.7893200e+001,2.6083800e+001,5.7235300e+001,2.8840900e+001,-2.8445200e+001,-3.7849300e+001,-2.8585100e+001,6.1342000e+000,4.0880300e+001,-3.4327700e+001,6.0929200e+001,1.2253000e+001,-2.3325500e+001,3.6493100e+001,8.3828000e+000,-9.9215000e+000,3.5022100e+001,2.1835800e+001,5.3067700e+001,8.2231800e+001,4.0662000e+000,6.8425500e+001,-5.8867800e+001,8.6354400e+001,-4.1139400e+001,-4.4580700e+001,6.7633500e+001,4.2715000e+001,-6.5426600e+001,-8.7883700e+001,7.0901600e+001,-5.4155100e+001,-3.6229800e+001,2.9059600e+001,-3.8806400e+001,-5.5396000e+000,-7.8339300e+001,8.7900200e+001};
	double Bias_rosenbrock=390;
	double sum=0;
	for(unsigned int i=0;i<x.size()-1;i++)
	{
		sum+=100*((x[i]-O_rosenbrock[i]+1)*(x[i]-O_rosenbrock[i]+1)-(x[i+1]-O_rosenbrock[i+1]+1))
			    *((x[i]-O_rosenbrock[i]+1)*(x[i]-O_rosenbrock[i]+1)-(x[i+1]-O_rosenbrock[i+1]+1))
			    +(x[i]-O_rosenbrock[i]+1-1)*(x[i]-O_rosenbrock[i]+1-1);
	}
	return sum+Bias_rosenbrock;
}

double function_shift_rastrigin(vector <double> x)
{
	double O_rastrigin[100]={1.9005000e+000,-1.5644000e+000,-9.7880000e-001,-2.2536000e+000,2.4990000e+000,-3.2853000e+000,9.7590000e-001,-3.6661000e+000,9.8500000e-002,-3.2465000e+000,3.8060000e+000,-2.6834000e+000,-1.3701000e+000,4.1821000e+000,2.4856000e+000,-4.2237000e+000,3.3653000e+000,2.1532000e+000,-3.0929000e+000,4.3105000e+000,-2.9861000e+000,3.4936000e+000,-2.7289000e+000,-4.1266000e+000,-2.5900000e+000,1.3124000e+000,-1.7990000e+000,-1.1890000e+000,-1.0530000e-001,-3.1074000e+000,-3.9641000e+000,-4.3387000e+000,3.0705000e+000,3.3205000e+000,-3.8178000e+000,-1.4980000e+000,-4.3807000e+000,2.7110000e+000,-3.7956000e+000,-2.3627000e+000,4.0086000e+000,-1.3728000e+000,-4.4362000e+000,-2.9183000e+000,-2.2457000e+000,3.0650000e-001,-8.9240000e-001,-3.2364000e+000,-1.2521000e+000,2.7198000e+000,-3.9787000e+000,3.0678000e+000,-4.2400000e+000,-3.9580000e+000,3.9479000e+000,2.2030000e-001,-2.6124000e+000,2.6498000e+000,-2.3256000e+000,-1.5383000e+000,3.4760000e+000,2.4462000e+000,2.4575000e+000,3.7409000e+000,-2.4887000e+000,3.8555000e+000,-1.1426000e+000,1.3389000e+000,2.2323000e+000,2.3137000e+000,-4.3370000e+000,3.9260000e+000,3.5905000e+000,-1.2858000e+000,-2.0113000e+000,2.9087000e+000,3.9278000e+000,1.0812000e+000,-7.4610000e-001,3.4740000e+000,2.3036000e+000,-3.3781000e+000,-4.4910000e-001,9.4020000e-001,-3.0583000e+000,-4.2165000e+000,-2.3604000e+000,8.3640000e-001,1.4773000e+000,-2.7292000e+000,-1.5904000e+000,7.6960000e-001,5.1640000e-001,2.6576000e+000,-5.4270000e-001,1.0358000e+000,6.9260000e-001,-4.2775000e+000,-1.5911000e+000,-3.5775000e+000};
	double Bias_rastrigin=-330;
	double sum=0;
	for(unsigned int i=0;i<x.size();i++)
	{
		sum+=10-10*cos(2*PI*(x[i]-O_rastrigin[i]))+(x[i]-O_rastrigin[i])*(x[i]-O_rastrigin[i]);
	}
	return sum+Bias_rastrigin;
}

double function_shifted_rotated_rastrign(vector <double> x)
{
	double f_bias=-330;
	double O_rastrigin[100]={1.9005000e+000,-1.5644000e+000,-9.7880000e-001,-2.2536000e+000,2.4990000e+000,-3.2853000e+000,9.7590000e-001,-3.6661000e+000,9.8500000e-002,-3.2465000e+000,3.8060000e+000,-2.6834000e+000,-1.3701000e+000,4.1821000e+000,2.4856000e+000,-4.2237000e+000,3.3653000e+000,2.1532000e+000,-3.0929000e+000,4.3105000e+000,-2.9861000e+000,3.4936000e+000,-2.7289000e+000,-4.1266000e+000,-2.5900000e+000,1.3124000e+000,-1.7990000e+000,-1.1890000e+000,-1.0530000e-001,-3.1074000e+000,-3.9641000e+000,-4.3387000e+000,3.0705000e+000,3.3205000e+000,-3.8178000e+000,-1.4980000e+000,-4.3807000e+000,2.7110000e+000,-3.7956000e+000,-2.3627000e+000,4.0086000e+000,-1.3728000e+000,-4.4362000e+000,-2.9183000e+000,-2.2457000e+000,3.0650000e-001,-8.9240000e-001,-3.2364000e+000,-1.2521000e+000,2.7198000e+000,-3.9787000e+000,3.0678000e+000,-4.2400000e+000,-3.9580000e+000,3.9479000e+000,2.2030000e-001,-2.6124000e+000,2.6498000e+000,-2.3256000e+000,-1.5383000e+000,3.4760000e+000,2.4462000e+000,2.4575000e+000,3.7409000e+000,-2.4887000e+000,3.8555000e+000,-1.1426000e+000,1.3389000e+000,2.2323000e+000,2.3137000e+000,-4.3370000e+000,3.9260000e+000,3.5905000e+000,-1.2858000e+000,-2.0113000e+000,2.9087000e+000,3.9278000e+000,1.0812000e+000,-7.4610000e-001,3.4740000e+000,2.3036000e+000,-3.3781000e+000,-4.4910000e-001,9.4020000e-001,-3.0583000e+000,-4.2165000e+000,-2.3604000e+000,8.3640000e-001,1.4773000e+000,-2.7292000e+000,-1.5904000e+000,7.6960000e-001,5.1640000e-001,2.6576000e+000,-5.4270000e-001,1.0358000e+000,6.9260000e-001,-4.2775000e+000,-1.5911000e+000,-3.5775000e+000};


	vector <double> y,z;

	for(int i=0;i<x.size();i++)
	{
		y.push_back(x[i]-O_rastrigin[i]);
	}
	
	for(int i=0;i<x.size();i++)
	{
		double sum=0;
		for(int j=0;j<y.size();j++)
		{
			if(y.size()==30)
				sum+=y[j]*M_30[i][j];
			if(y.size()==100)
				sum+=y[j]*M_100[i][j];
		}
		z.push_back(sum);
	}

	double sum=0;
	for(int i=0;i<z.size();i++)
	{
		sum+=10-10*cos(2*PI*z[i])+z[i]*z[i];		
	}
	sum+=f_bias;
	return sum;	
}

double abs_function_shifted_rotated_rastrign(vector <double> x)
{
	double f_bias=-330;
	double O_rastrigin[100]={1.9005000e+000,-1.5644000e+000,-9.7880000e-001,-2.2536000e+000,2.4990000e+000,-3.2853000e+000,9.7590000e-001,-3.6661000e+000,9.8500000e-002,-3.2465000e+000,3.8060000e+000,-2.6834000e+000,-1.3701000e+000,4.1821000e+000,2.4856000e+000,-4.2237000e+000,3.3653000e+000,2.1532000e+000,-3.0929000e+000,4.3105000e+000,-2.9861000e+000,3.4936000e+000,-2.7289000e+000,-4.1266000e+000,-2.5900000e+000,1.3124000e+000,-1.7990000e+000,-1.1890000e+000,-1.0530000e-001,-3.1074000e+000,-3.9641000e+000,-4.3387000e+000,3.0705000e+000,3.3205000e+000,-3.8178000e+000,-1.4980000e+000,-4.3807000e+000,2.7110000e+000,-3.7956000e+000,-2.3627000e+000,4.0086000e+000,-1.3728000e+000,-4.4362000e+000,-2.9183000e+000,-2.2457000e+000,3.0650000e-001,-8.9240000e-001,-3.2364000e+000,-1.2521000e+000,2.7198000e+000,-3.9787000e+000,3.0678000e+000,-4.2400000e+000,-3.9580000e+000,3.9479000e+000,2.2030000e-001,-2.6124000e+000,2.6498000e+000,-2.3256000e+000,-1.5383000e+000,3.4760000e+000,2.4462000e+000,2.4575000e+000,3.7409000e+000,-2.4887000e+000,3.8555000e+000,-1.1426000e+000,1.3389000e+000,2.2323000e+000,2.3137000e+000,-4.3370000e+000,3.9260000e+000,3.5905000e+000,-1.2858000e+000,-2.0113000e+000,2.9087000e+000,3.9278000e+000,1.0812000e+000,-7.4610000e-001,3.4740000e+000,2.3036000e+000,-3.3781000e+000,-4.4910000e-001,9.4020000e-001,-3.0583000e+000,-4.2165000e+000,-2.3604000e+000,8.3640000e-001,1.4773000e+000,-2.7292000e+000,-1.5904000e+000,7.6960000e-001,5.1640000e-001,2.6576000e+000,-5.4270000e-001,1.0358000e+000,6.9260000e-001,-4.2775000e+000,-1.5911000e+000,-3.5775000e+000};


	vector <double> y,z;

	for(int i=0;i<x.size();i++)
	{
		y.push_back(x[i]-O_rastrigin[i]);
	}
	
	for(int i=0;i<x.size();i++)
	{
		double sum=0;
		for(int j=0;j<y.size();j++)
		{
			if(y.size()==30)
				sum+=y[j]*M_30[i][j];
			if(y.size()==100)
				sum+=y[j]*M_100[i][j];
		}
		z.push_back(sum);
	}

	double sum=0;
	for(int i=0;i<z.size();i++)
	{
		sum+=10-10*cos(2*PI*z[i])+z[i]*z[i];		
	}
	return fabs(sum);	
}

double abs_function_shift_Rotated_Griewank(vector <double> x)
{
	double f_bias=-180;
	double O_rastrigin[100]={-2.7626840e+002,-1.1911000e+001,-5.7878840e+002,-2.8764860e+002,-8.4385800e+001,-2.2867530e+002,-4.5815160e+002,-2.0221450e+002,-1.0586420e+002,-9.6489800e+001,-3.9574680e+002,-5.7294980e+002,-2.7036410e+002,-5.6685430e+002,-1.5242040e+002,-5.8838190e+002,-2.8288920e+002,-4.8888650e+002,-3.4698170e+002,-4.5304470e+002,-5.0658570e+002,-4.7599870e+002,-3.6204920e+002,-2.3323670e+002,-4.9198640e+002,-5.4408980e+002,-7.3445600e+001,-5.2690110e+002,-5.0225610e+002,-5.3723530e+002,-2.6314870e+002,-2.0592600e+002,-4.6582320e+002,-9.7565000e+000,-1.6914000e+001,-4.5293970e+002,-4.3061070e+002,-1.6231370e+002,-6.9020400e+001,-3.0240820e+002,-5.2789320e+002,-4.8942740e+002,-7.4256200e+001,-6.8285000e+000,-5.4636310e+002,-4.1518800e+002,-3.2214990e+002,-1.4538220e+002,-5.2700190e+002,-4.2217790e+002,-2.5425420e+002,-4.9572890e+002,-2.8610230e+002,-1.9616440e+002,-1.0493830e+002,-3.8844900e+002,-5.7932200e+001,-5.3995800e+001,-1.1438820e+002,-5.0943400e+001,-5.0809610e+002,-7.7027800e+001,-2.4296420e+002,-1.1586500e+001,-5.0694900e+002,-2.2216620e+002,-3.0383250e+002,-3.5940410e+002,-1.5607150e+002,-2.5297110e+002,-3.4570620e+002,-1.3306540e+002,-3.0187400e+001,-3.0661620e+002,-1.6066730e+002,-2.1051130e+002,-1.8625180e+002,-5.5031270e+002,-5.1903050e+002,-3.2077600e+002,-4.7110670e+002,-1.5917240e+002,-4.4075100e+001,-8.4125100e+001,-5.3803890e+002,-4.4419930e+002,-5.9093380e+002,-2.1680070e+002,-5.2327310e+002,-3.1720000e+002,-3.9366710e+002,-5.4843390e+002,-4.1604030e+002,-3.8038260e+002,-1.0954200e+002,-1.6776100e+002,-3.4551270e+002,-5.4486310e+002,-3.5782180e+002,-5.9821170e+002};
	vector <double> y,z;

		for(unsigned int i=0;i<x.size();i++)
		{
			y.push_back(x[i]-O_rastrigin[i]);
		}

		for(unsigned int i=0;i<x.size();i++)
		{
			double sum=0;
			for(unsigned int j=0;j<y.size();j++)
			{
				if(y.size()==30)
					sum+=y[j]*M_30[i][j];
				if(y.size()==100)
					sum+=y[j]*M_100[i][j];
			}
			z.push_back(sum);
		}
		
		double sum=0,product=1;
		for(unsigned int i=0;i<x.size();i++)
		{
			sum+=z[i]*z[i];
			product*=cos(z[i]/sqrtf(i+1.0));
		}
		sum=sum/4000-product+1;
		return fabs(sum);
}

double function_shift_Rotated_Griewank(vector <double> x)
{

	double f_bias=-180;
	double O_rastrigin[100]={-2.7626840e+002,-1.1911000e+001,-5.7878840e+002,-2.8764860e+002,-8.4385800e+001,-2.2867530e+002,-4.5815160e+002,-2.0221450e+002,-1.0586420e+002,-9.6489800e+001,-3.9574680e+002,-5.7294980e+002,-2.7036410e+002,-5.6685430e+002,-1.5242040e+002,-5.8838190e+002,-2.8288920e+002,-4.8888650e+002,-3.4698170e+002,-4.5304470e+002,-5.0658570e+002,-4.7599870e+002,-3.6204920e+002,-2.3323670e+002,-4.9198640e+002,-5.4408980e+002,-7.3445600e+001,-5.2690110e+002,-5.0225610e+002,-5.3723530e+002,-2.6314870e+002,-2.0592600e+002,-4.6582320e+002,-9.7565000e+000,-1.6914000e+001,-4.5293970e+002,-4.3061070e+002,-1.6231370e+002,-6.9020400e+001,-3.0240820e+002,-5.2789320e+002,-4.8942740e+002,-7.4256200e+001,-6.8285000e+000,-5.4636310e+002,-4.1518800e+002,-3.2214990e+002,-1.4538220e+002,-5.2700190e+002,-4.2217790e+002,-2.5425420e+002,-4.9572890e+002,-2.8610230e+002,-1.9616440e+002,-1.0493830e+002,-3.8844900e+002,-5.7932200e+001,-5.3995800e+001,-1.1438820e+002,-5.0943400e+001,-5.0809610e+002,-7.7027800e+001,-2.4296420e+002,-1.1586500e+001,-5.0694900e+002,-2.2216620e+002,-3.0383250e+002,-3.5940410e+002,-1.5607150e+002,-2.5297110e+002,-3.4570620e+002,-1.3306540e+002,-3.0187400e+001,-3.0661620e+002,-1.6066730e+002,-2.1051130e+002,-1.8625180e+002,-5.5031270e+002,-5.1903050e+002,-3.2077600e+002,-4.7110670e+002,-1.5917240e+002,-4.4075100e+001,-8.4125100e+001,-5.3803890e+002,-4.4419930e+002,-5.9093380e+002,-2.1680070e+002,-5.2327310e+002,-3.1720000e+002,-3.9366710e+002,-5.4843390e+002,-4.1604030e+002,-3.8038260e+002,-1.0954200e+002,-1.6776100e+002,-3.4551270e+002,-5.4486310e+002,-3.5782180e+002,-5.9821170e+002};
		vector <double> y,z;

		for(unsigned int i=0;i<x.size();i++)
		{
			y.push_back(x[i]-O_rastrigin[i]);
		}

		for(unsigned int i=0;i<x.size();i++)
		{
			double sum=0;
			for(unsigned int j=0;j<y.size();j++)
			{
				if(y.size()==30)
					sum+=y[j]*M_30[i][j];
				if(y.size()==100)
					sum+=y[j]*M_100[i][j];
			}
			z.push_back(sum);
		}	

		double sum=0,product=1;
		for(unsigned int i=0;i<x.size();i++)
		{
			sum+=z[i]*z[i];
			product*=cos(z[i]/sqrtf(i+1.0));
		}
		sum=sum/4000-product+1+f_bias;
		return sum;
}
