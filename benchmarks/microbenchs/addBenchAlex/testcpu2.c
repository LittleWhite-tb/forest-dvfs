#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/time.h>

#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

void pinCPU(int cpu)
{
   cpu_set_t cpuset;
   pid_t myself = getpid();
 
   CPU_ZERO(&cpuset);
   CPU_SET(cpu,&cpuset);
 
   int ret = sched_setaffinity(myself,sizeof(cpu_set_t),&cpuset);
   if(ret != 0)
   {
      perror("sched_setaffinity");
      exit(0);
   }
}


long unsigned int getMSTime()
{
   struct timeval tv;
   long unsigned int msTime = 0;
   
   gettimeofday(&tv, NULL);
   
   msTime = tv.tv_sec * 1000 + tv.tv_usec/1000;
   
   return msTime;
}



int main()
{
	int i = 0;

int newNice = 0;
   do
   {
      newNice = nice(-1);
      if ( newNice == -1 && errno == EPERM )
      {
         fprintf(stderr,"Fail to set the nice to -20 because you are not root\n");
         break;
      }
   }while ( newNice != -20 );

	//pinCPU(0);

	for ( i = 0 ; i < 1024*1024*128 ; i++ )
	{
		asm volatile(
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      :
      :
      :"%eax"
   );
	}

	return 0;
}
