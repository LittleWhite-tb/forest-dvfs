#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Rest.h"
#include "ThreadedProfiler.h"

#ifdef MAIN
//single global variable for the linker to hook us in... internally used only
static rest_main_t rest_original_main; 
char ldPreload[256]="\0";

//wrapper for the original function
static int rest_main(int argc, char** argv, char** env)
{
     //fprintf(stdout,"[DEBUG] Je suis dans le rest_main\n");
     //LD PRELOAD Could work but we are not sure
    //char * tmp = NULL;
    setenv("LD_PRELOAD","",1);
    //tmp = getenv ("LD_PRELOAD");
      		
    /*if(tmp == NULL)
    {
	fprintf(stderr,"No LD_PRELOAD were defined, it will not be used");
        //return rest_original_main(argc, argv, env);
    }*/
    
    RestInit(REST_T_PROFILER, REST_NAIVE_DM, REST_FREQ_CHANGER);
    
    atexit(RestDestroy);
    return rest_original_main(argc, argv, env);
}


// we redefine libc_start_main because at this point main is an arguement so we can store it in our symbol table and hook in
int __libc_start_main(rest_main_t main, int argc, char** ubp_av,
        rest_lsm_t init, rest_lsm_t fini, rest_lsm_t rtld_fini,
        void* stack_end)
{
    fprintf(stdout,"[DEBUG] Je suis dans le libc_start_main\n");
    //reset main to our global variable so our wrapper can call it easily
    rest_original_main = main;
    //Initialisation :

    rest_libc_start_main_t real_start =
        (rest_libc_start_main_t)dlsym(RTLD_NEXT, "__libc_start_main");

    
    //call the wrapper with the real libc_start_main
    return real_start(rest_main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}
#endif

