/* GENERATED FILE: DO NOTE EDIT */

#define NUM_GROUPS_K8 5

static PerfmonGroupMap k8_group_map[NUM_GROUPS_K8] = {

    {"BRANCH",BRANCH,"Branch prediction miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,BRANCH_RETIRED:PMC1,BRANCH_MISPREDICT_RETIRED:PMC2,BRANCH_TAKEN_RETIRED:PMC3"},

    {"CACHE",CACHE,"Data cache miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,DATA_CACHE_ACCESSES:PMC1,DATA_CACHE_REFILLS_L2_ALL:PMC2,DATA_CACHE_REFILLS_NORTHBRIDGE_ALL:PMC3"},

    {"CPI",CPI,"Cycles per instruction","INSTRUCTIONS_RETIRED:PMC0,CPU_CLOCKS_UNHALTED:PMC1,UOPS_RETIRED:PMC2"},

    {"ICACHE",ICACHE,"Instruction cache miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,ICACHE_FETCHES:PMC1,ICACHE_REFILLS_L2:PMC2,ICACHE_REFILLS_MEM:PMC3"},

    {"L2",L2,"L2 cache bandwidth in MBytes/s","DATA_CACHE_REFILLS_L2_ALL:PMC0,DATA_CACHE_EVICTED_ALL:PMC1,CPU_CLOCKS_UNHALTED:PMC2"},

};

void
perfmon_printDerivedMetricsK8(PerfmonGroup group)
{
    int threadId;
    double time = rdtscTime;
    double inverseClock = 1.0 /(double) timer_getCpuClock();
    PerfmonResultTable tableData;
    int numRows;
    int numColumns = perfmon_numThreads;
    bstrList* fc;
    bstring label;
    double** stat;

    switch ( group ) 
    {

        case BRANCH:
            numRows = 6;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Branch rate  );

            bstrListAdd(fc,2,Branch misprediction rate );

            bstrListAdd(fc,3,Branch misprediction ratio );

            bstrListAdd(fc,4,Branch taken rate );

            bstrListAdd(fc,5,Branch taken ratio );

            bstrListAdd(fc,6,Instructions per branch );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0");
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC1");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"PMC3")/perfmon_getResult(threadId,"PMC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = perfmon_getResult(threadId,"PMC3")/perfmon_getResult(threadId,"PMC1");
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1");
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

            }
            break;

        case CACHE:
            numRows = 4;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Data cache misses);

            bstrListAdd(fc,2,Data cache request rate);

            bstrListAdd(fc,3,Data cache miss rate);

            bstrListAdd(fc,4,Data cache miss ratio);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3");
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = (perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = (perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC1");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case CPI:
            numRows = 4;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Runtime [s] );

            bstrListAdd(fc,2,CPI  );

            bstrListAdd(fc,3,CPI (based on uops)  );

            bstrListAdd(fc,4,IPC  );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC2");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case ICACHE:
            numRows = 4;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Instruction cache misses );

            bstrListAdd(fc,2,Instruction cache request rate  );

            bstrListAdd(fc,3,Instruction cache miss rate   );

            bstrListAdd(fc,4,Instruction cache miss ratio  );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3");
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = (perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = (perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC1");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case L2:
            numRows = 4;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Runtime [s]  );

            bstrListAdd(fc,2,L2 bandwidth MBytes/s  );

            bstrListAdd(fc,3,L2 refill bandwidth MBytes/s  );

            bstrListAdd(fc,4,L2 evict MBytes/s   );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC2")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))*64.0/time;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")*64.0/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;


        default:
            fprintf (stderr, "perfmon_printDerivedMetricsk8: Unknown group! Exiting!\n" );
            exit (EXIT_FAILURE);
            break;
    }

    printResultTable(&tableData);
    freeResultTable(&tableData);



    /* for threaded results print sum, max, min and avg */
    if (perfmon_numThreads > 1)
    {
        initStatisticTable(&tableData, fc, numRows);
        for (int i=0; i<numRows; i++)
        {
            stat[i][3] =  stat[i][0]/perfmon_numThreads;
            for (int j=0; j<4; j++)
            {
                tableData.rows[i].value[j] = stat[i][j];
            }
        }
        printResultTable(&tableData);
        freeResultTable(&tableData);
    }

    for (int i=0; i<numRows; i++)
    {
        free(stat[i]);
    }
    free(stat);
    bdestroy(label);
    bstrListDestroy(fc);
}

void
perfmon_logDerivedMetricsK8(PerfmonGroup group, double time,double timeStamp)
{
    int threadId;
    double inverseClock = 1.0 /(double) timer_getCpuClock();

    switch ( group ) 
    {
        
        case BRANCH:

                    
                        printf("Branch rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Branch misprediction rate  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Branch misprediction ratio  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
                        printf("Branch taken rate  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC3")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Branch taken ratio  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC3")/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
                        printf("Instructions per branch  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
            break;
            
        case CACHE:

                    
                        printf("Data cache misses %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"));
                        }
                        printf("\n");
                    
                        printf("Data cache request rate %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Data cache miss rate %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Data cache miss ratio %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
            break;
            
        case CPI:

                    
                        printf("Runtime [s]  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("CPI (based on uops)   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC2"));
                        }
                        printf("\n");
                    
                        printf("IPC   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
            break;
            
        case ICACHE:

                    
                        printf("Instruction cache misses  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"));
                        }
                        printf("\n");
                    
                        printf("Instruction cache request rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Instruction cache miss rate    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Instruction cache miss ratio   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
            break;
            
        case L2:

                    
                        printf("Runtime [s]   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("L2 bandwidth MBytes/s   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))*64.0/time);
                        }
                        printf("\n");
                    
                        printf("L2 refill bandwidth MBytes/s   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time);
                        }
                        printf("\n");
                    
                        printf("L2 evict MBytes/s    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")*64.0/time);
                        }
                        printf("\n");
                    
            break;
            

        default:
                fprintf (stderr, "perfmon_printDerivedMetricsk8: Unknown group! Exiting!\n" );
                exit (EXIT_FAILURE);
                break;
    }
}



static PerfmonGroupHelp k8_group_help[NUM_GROUPS_K8] = {

    {"BRANCH","Formulas:\n\
Branch rate = BRANCH_RETIRED / INSTRUCTIONS_RETIRED\n\
Branch misprediction rate = BRANCH_MISPREDICT_RETIRED / INSTRUCTIONS_RETIRED\n\
Branch misprediction ratio = BRANCH_MISPREDICT_RETIRED / BRANCH_RETIRED\n\
Branch taken rate = BRANCH_TAKEN_RETIRED / INSTRUCTIONS_RETIRED\n\
Branch taken ratio = BRANCH_TAKEN_RETIRED / BRANCH_RETIRED\n\
Instructions per branch = INSTRUCTIONS_RETIRED / BRANCH_RETIRED\n\
-\n\
The rates state how often in average a branch or a mispredicted branch occured\n\
per instruction retired in total. The Branch misprediction ratio sets directly\n\
into relation what ration of all branch instruction where mispredicted.\n\
Instructions per branch is 1/Branch rate. The same applies for the branches\n\
taken metrics.\n\
"},

    {"CACHE","Formulas:\n\
Data cache misses = DATA_CACHE_REFILLS_L2_AL + DATA_CACHE_REFILLS_NORTHBRIDGE_ALL\n\
Data cache request rate = DATA_CACHE_ACCESSES / INSTRUCTIONS_RETIRED\n\
Data cache miss rate = (DATA_CACHE_REFILLS_L2_AL + DATA_CACHE_REFILLS_NORTHBRIDGE_ALL)/INSTRUCTIONS_RETIRED\n\
Data cache miss ratio = (DATA_CACHE_REFILLS_L2_AL + DATA_CACHE_REFILLS_NORTHBRIDGE_ALL)/DATA_CACHE_ACCESSES\n\
-\n\
This group measures the locality of your data accesses with regard to the\n\
L1 Cache. Data cache request rate tells you how data intensive your code is\n\
or how many Data accesses you have in average per instruction.\n\
The Data cache miss rate gives a measure how often it was necessary to get\n\
cachelines from higher levels of the memory hierarchy. And finally \n\
Data cache miss ratio tells you how many of your memory references required\n\
a cacheline to be loaded from a higher level. While the Data cache miss rate \n\
might be given by your algorithm you should try to get Data cache miss ratio\n\
as low as possible by increasing your cache reuse.\n\
This group was taken from the whitepaper -Basic Performance Measurements for AMD Athlon 64,\n\
AMD Opteron and AMD Phenom Processors- from Paul J. Drongowski.\n\
"},

    {"CPI","This group measures how efficient the processor works with\n\
regard to instruction throughput. Also important as a standalone\n\
metric is INSTRUCTIONS_RETIRED as it tells you how many instruction\n\
you need to execute for a task. An optimization might show very\n\
low CPI values but execute many more instruction for it.\n\
\n\
"},

    {"ICACHE","Formulas:\n\
Instruction cache misses ICACHE_REFILLS_L2 + ICACHE_REFILLS_MEM\n\
Instruction cache request rate ICACHE_FETCHES / INSTRUCTIONS_RETIRED\n\
Instruction cache miss rate  (ICACHE_REFILLS_L2+ICACHE_REFILLS_MEM)/INSTRUCTIONS_RETIRED\n\
Instruction cache miss ratio (ICACHE_REFILLS_L2+ICACHE_REFILLS_MEM)/ICACHE_FETCHES\n\
-\n\
This group measures the locality of your instruction code with regard to the\n\
L1 I-Cache. \n\
\n\
"},

    {"L2","Formulas:\n\
L2 bandwidth MBytes/s   1.0E-06*(DATA_CACHE_REFILLS_L2_ALL+DATA_CACHE_EVICTED_ALL)*64/time\n\
L2 refill bandwidth MBytes/s   1.0E-06*DATA_CACHE_REFILLS_L2_ALL*64/time\n\
L2 evict MBytes/s    1.0E-06*DATA_CACHE_EVICTED_ALL*64/time\n\
-\n\
Profiling group to measure L2 cache bandwidth. The bandwidth is\n\
computed by the number of cacheline loaded from L2 to L1 and the\n\
number of modified cachelines evicted from the L1. \n\
Note that this bandwidth also includes data transfers due to a\n\
write allocate load on a store miss in L1 and copy back transfers if\n\
originated from L2.\n\
\n\
\n\
\n\
"},

};

