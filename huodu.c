
/***************************************************************************************************
关于编译的问题：gcc编译的时候添加链接库，可以解决掉exp函数未定义的问题
gcc xxx.c  -o  xxx -lm
关于环境的问题：在ubuntu18.04上无需cell初始化
		在ubuntu20.04上需要在原子制图处对cell数组初始化，原因未知
课堂作业，参考，算法还待完善，可以利用多线程来提高运算速度
***************************************************************************************************/

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>

void sort(long double * p, int len)
{
	for (int i = 0; i<len; i++)
	{
		for (int j = 0; j<len - i - 1; j++)
		{
			if (p[j]>p[j + 1])
			{
				long double temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
		}
	}
}


void sortp(long double *p, int len)
{
	printf("放射性活度排序:\n");
	for (int i = 0; i < len; i++)
	{
		printf("%.20Lf\n", p[i]);
	}
}

void sortprintf(long double *p ,int len)
{
	printf("模拟剩余原子数排序:\n");
	for(int i=0; i<len;i++)
	{
		printf("%.0Lf\n",p[i]);
        }
}


int main()
{
	system("clear");
	/* define the date we need */
	long double time_a=365*24*3600;                // A years to seconds
	long double time_800=time_a*800;
	long double n=(1e-15)/226*(6.023*1e23);        // number of 1e-15 gram Ra
	long double constant=1.37*1e-11;               //衰变常数 
	long double p800=expl(-constant*time_800);     //八百年剩余百分比
	long double pa=expl(-constant*time_a);         //一年剩余百分比
	long double pc;
	long double A[100];
	long double nleft;
	long double ndecay;
 	long double nav[100];
	long double Aav=0;
	srand((unsigned)time(NULL));

	printf("\n********************debug*********************\n");
	printf("一年有%Lf秒 \n",time_a);
	printf("原子数:%Lf \n",n);
	printf("800年有%Lf秒 \n",time_800);
	printf("800年的衰变的概率：%Lf \t一年的衰变的概率： %Lf\n",1-p800,1-pa);
	printf("800年后剩余原子理论期望值：%Lf\n",n*p800);
    	printf("************************************************\n\n");
	system(" echo debug信息显示三秒 && sleep 3s ");
	printf("\n");

	/**************800年剩余原子数************/
	printf("模拟剩余的原子数：\n");
	for(int i=0;i<100;i++)
	{
	  nleft=0;
	 	for(int j=0;j<n;j++)
		{
			pc=rand()/(RAND_MAX+1.0);
			if(pc<=p800)
			{
				nleft++;
			}
		}
	 //printf("%.0Lf\n",nleft);
	 nav[i]=nleft;

	}
	sort(nav,100);
	sortprintf(nav,100);                                                //剩余原子数进行排序并打印

	/**************一年的平均活度**************/

	for(int i=0;i<100;i++)
	{
	ndecay=0;
		for(int j=0;j<((nav[49]+nav[50])/2);j++)
		{
			pc=rand()/(RAND_MAX+1.0);
			if(pc>pa)
			{
				ndecay++;
			}
		}
	A[i]=ndecay/time_a;
	}

	sort(A,100);
	sortp(A,100);

	for(int i=0;i<100;i++)
	{
		Aav=Aav+A[i];
	}
	Aav=Aav/100;

	printf("\n**************************数据汇总**********************\n\n");
	printf("800年后模拟剩余原子数：%Lf\n",(nav[49]+nav[50])/2);
	printf("800年后理论剩余原子数：%Lf\n",n*p800);
	printf("一年模拟活度为：%.20Lf\n",Aav);
	printf("\n********************************************************\n\n");
	printf("\n\n\n\n");


	/************ 原子数制图 *************/
	int flag=0;                                                             //本来用来优化在for（int j）里面重复的比较
	long double card=(nav[99]-nav[0])/20;

	/*
	printf ("\ndebug info\n");
	printf("**************************************%Lf************************************\n",card);
	sleep(5);
	*/

	int cell[20];
	for(int i=0;i<20;i++)
	{
		cell[i]=0;                                          //unbuntu 20.04需要对数组进行初始化

		for(int j=0;j<100;j++)
			{
				if(		( 	nav[j]>= (nav[0]+i*card)	) 	 && 	( 	 nav[j] < (nav[0]+(i+1)*card)		)		)
				{
					cell[i]=cell[i]+1;
				}

			}
	}
	cell[19]=cell[19]+1; 
                                            //因为画的那个图里面第二十个点取得是空，最后一个端点就没算
	printf("剩余原子的分布：\n");
	for(int j=0;j<20;j++)
	{
		printf("%.2Lf--%.2Lf:|",nav[0]+j*card,nav[0]+(j+1)*card );
		for(int i=0;i<cell[j];i++)
		{
			printf("******|");
		}
		printf(" %d",cell[j]);
		printf("\n");
		cell[j]=0;                                               //卸磨杀驴,给下面用
	}
	printf("\n\n\n\n");

	/***************** 活度制图 ****************/
	long  double card_a=(A[99]-A[0])/20;
	for(int i=0;i<20;i++)
	{
		for(int j=0;j<100;j++)
		{
			if(  A[j]   >=    ( A[0]+ i * card_a )        &&        A[j]    <    ( A[0] + (i+1) * card_a  )     )
			cell[i]=cell[i]+1;
		}
	}
	cell[19]=cell[19]+1;

	printf("活度的分布：\n");
	for(int j=0;j<20;j++)
	{
		printf("%.10Lf--%.10Lf:|",A[0]+j*card_a,A[0]+(j+1)*card_a);
		for(int i=0;i<cell[j];i++)
		{
			printf("*****|");
		}
		printf(" %d\n",cell[j]);
	}

	printf("\n\n");
	return 0;
}

