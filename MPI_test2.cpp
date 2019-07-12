#include"mpi.h"
#include<stdio.h>
#include<math.h>

double f(double a)
{
	return(4.0/(1.0+a*a));
}
int main(int argc,char *argv[])
{
	int done=0,n,myid,numprocs,i;//n是计算点数,myid是进程名,numprocs进程数量.i是循环变量
	double PI25DT=3.141592653589793238462643;
	double mypi,pi,h,sum,x;//mypi每个进程计算的值,
	double startwtime,endwtime;//记录时间
	int namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc,&argv);//初始化
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);//获取进程数
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);//获取进程名
	MPI_Get_processor_name(processor_name,&namelen);//核的名字
	fprintf(stderr,"Process %d on %s\n",myid,processor_name);//打印进程和对应核的名字
	n=0;//初始化计算点数
	while(!done){
		if(myid==0){
			printf("Enter the number of intervals:(0 quits)");
			scanf("%d",&n);
			startwtime=MPI_Wtime();
			}
			MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
			if(n==0)
				done=1;
			else{
				h=1.0/(double)n;
				sum=0.0;
				for(i=myid+1;i<=n;i+=numprocs){
				x=h*((double)i-0.5);
				sum+=f(x);
				}
			mypi=h*sum;
			MPI_Reduce(&mypi,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
			if(myid==0){
				printf("pi is approximately %.16f,Error is %.16f\n",pi,fabs(pi-PI25DT));
				endwtime=MPI_Wtime();
				printf("clock time=%f\n",endwtime-startwtime);
				}
			}
		}
		MPI_Finalize();
	}
