#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <dlfcn.h>
#include "Rest.h"

#define REST

/*typedef double (*evalGet)(void *data);
typedef  void* (*evalInit)(void);
typedef int (*evalClose)(void *data);
void *dl;

evalGet evaluationStart;
evalGet evaluationStop;
evalInit evaluationInit;
evalClose evaluationClose;

typedef struct Energy
{
	double start;
	double stop;
	double elapsed;
}Energy;

Energy E;*/

void loadPowerlibraries()
{
	dlPower = dlopen("/opt/rest_modifications/power/timer.so",RTLD_LAZY);
	assert(dlPower != NULL);
	E.evaluationInit =(evalInit) dlsym(dlPower,"evaluationInit");
	assert(E.evaluationInit != NULL);
	E.evaluationStart =(evalGet) dlsym(dlPower,"evaluationStart");
	assert(E.evaluationStart != NULL);
	E.evaluationStop = (evalGet) dlsym(dlPower,"evaluationStop");
	assert(E.evaluationStop != NULL);
	E.evaluationClose = (evalClose) dlsym(dlPower,"evaluationClose");
	assert(E.evaluationClose != NULL);
}

void loadTimelibraries()
{
	dlTime = dlopen("/opt/rest_modifications/timer/timer.so",RTLD_LAZY);
	assert(dlTime != NULL);
	T.evaluationInit =(evalInit) dlsym(dlTime,"evaluationInit");
	assert(T.evaluationInit != NULL);
	T.evaluationStart =(evalGet) dlsym(dlTime,"evaluationStart");
	assert(T.evaluationStart != NULL);
	T.evaluationStop = (evalGet) dlsym(dlTime,"evaluationStop");
	assert(T.evaluationStop != NULL);
	T.evaluationClose = (evalClose) dlsym(dlTime,"evaluationClose");
	assert(T.evaluationClose != NULL);
}
/*
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int dest, int tag,MPI_Comm comm, MPI_Request *request)
{
	return PMPI_Irecv(buf,count,datatype,dest,tag,comm,request);
}
int mpi_irecv_(void *buf, long int *count, long int *datatype,long int *dest,long int *tag,long int *comm,MPI_Request *request,int *ierr)
{
	//fprintf(stdout,"CATCH MPI_RECV\n");
	*ierr = MPI_Irecv(buf,*count,*datatype,*dest,*tag,*comm,request);
	return *ierr;
	
}

int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,MPI_Comm comm, MPI_Request *request)
{
	return PMPI_Isend(buf,count,datatype,dest,tag,comm,request);
}
int mpi_isend_(void *buf,long int *count, long int *datatype,long int *dest,long int *tag,long int *comm,MPI_Request *request,int *ierr)
{
	//fprintf(stdout,"CATCH MPI_SEND\n");
	*ierr = MPI_Isend(buf,*count,*datatype,*dest,*tag,*comm,request);
	return *ierr;

}
*/
int MPI_Init(int *argc, char ***argv)
{
	E.start = -1;
	E.stop = -1;
	E.elapsed = -1;
	T.start = -1;
	T.stop = -1;
	T.elapsed = -1;
	int rc,rank;
	void * evaldata = NULL;
	rc = PMPI_Init(argc, argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	loadTimelibraries();
	if(rank == 0)
	{	
		loadPowerlibraries();
		tmpEvaluationInit = E.evaluationInit;
		(*tmpEvaluationInit) ();
		tmpEvaluationStart = E.evaluationStart;
		E.start  = (*tmpEvaluationStart)(evaldata);
	}
	tmpEvaluationInit = T.evaluationInit;
	(*tmpEvaluationInit)();
	tmpEvaluationStart = T.evaluationStart;
	T.start = (*tmpEvaluationStart)(evaldata);
	#ifdef REST
	RestInit(REST_T_PROFILER, REST_NAIVE_DM, REST_FREQ_CHANGER);
	#endif
	return rc;
}

int mpi_init_(int *ierr)
{
	return MPI_Init(NULL, NULL);
}

int MPI_Finalize()
{
	int rc, rank;
	void *evaldata = NULL;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	if(rank == 0)
	{
		tmpEvaluationStop = E.evaluationStop;
		E.stop = (*tmpEvaluationStop)(evaldata);
		tmpEvaluationClose = E.evaluationClose;
		(*tmpEvaluationClose)(evaldata);
		E.elapsed = E.stop - E.start;
	
	}
	tmpEvaluationStop = T.evaluationStop;
 	T.stop = (*tmpEvaluationStop)(evaldata);
	tmpEvaluationClose = T.evaluationClose;
	(*tmpEvaluationClose)(evaldata);
	T.elapsed = T.stop - T.start;


	char buff_tmp[200]="\0";
	char rankstring[5]="\0";
	
	assert(getenv("REST_OUTPUT") !=NULL);
	strcat(buff_tmp,getenv("REST_OUTPUT"));




	strcat(buff_tmp,"/RTM_mpi_performance");
	sprintf(rankstring,"%d",rank);
	strcat(buff_tmp,rankstring);


	FILE *outputPower = fopen(buff_tmp,"w");
	assert(outputPower != NULL);

	fprintf(outputPower,"Time: %f\n",T.elapsed);
	if(rank == 0)
	{
		fprintf(outputPower, "Energy : %f\n",E.elapsed);
	}
	fclose(outputPower);
		
	#ifdef REST
	RestDestroy();
	#endif

	rc = PMPI_Finalize();
	if(rank == 0)
	{
		dlclose(dlPower);
	}
	dlclose(dlTime);
	return rc;

}

int mpi_finalize_(int *ierr)
{
	return MPI_Finalize();
}

