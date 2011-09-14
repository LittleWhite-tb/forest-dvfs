#include <stdio.h>

// Define rdtscll for x86_64 arch
#ifdef __x86_64__
	#define rdtscll(val) do { \
			unsigned int __a,__d; \
			asm volatile("rdtsc" : "=a" (__a), "=d" (__d)); \
			(val) = ((unsigned long)__a) | (((unsigned long)__d)<<32); \
	} while(0)
#endif

// Define rdtscll for i386 arch
#ifdef __i386__
	#define rdtscll(val) { \
		asm volatile ("rdtsc" : "=A"(val)); \
		}
#endif


static __inline__ unsigned long long getticks(void)
{
   unsigned long long ret;
   rdtscll(ret);
   return ret;
}

int main()
{
	unsigned long long start, stop;
	printf("This test takes 30 seconds, please wait patiently\n");
	start=getticks();
	sleep(30);
	stop=getticks();
	long freq=(stop-start)/30;
	printf("Clock frequency of rdtsc is %ld\n", freq);
}
