/* GENERATED FILE: DO NOTE EDIT */

#define NUM_GROUPS_K10 15

static PerfmonGroupMap k10_group_map[NUM_GROUPS_K10] = {

    {"BRANCH",BRANCH,"Branch prediction miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,BRANCH_RETIRED:PMC1,BRANCH_MISPREDICT_RETIRED:PMC2,BRANCH_TAKEN_RETIRED:PMC3"},

    {"CACHE",CACHE,"Data cache miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,DATA_CACHE_ACCESSES:PMC1,DATA_CACHE_REFILLS_L2_ALL:PMC2,DATA_CACHE_REFILLS_NORTHBRIDGE_ALL:PMC3"},

    {"CPI",CPI,"Cycles per instruction","INSTRUCTIONS_RETIRED:PMC0,CPU_CLOCKS_UNHALTED:PMC1,UOPS_RETIRED:PMC2"},

    {"FLOPS_DP",FLOPS_DP,"Double Precision MFlops/s","SSE_RETIRED_ADD_DOUBLE_FLOPS:PMC0,SSE_RETIRED_MULT_DOUBLE_FLOPS:PMC1,CPU_CLOCKS_UNHALTED:PMC2"},

    {"FLOPS_SP",FLOPS_SP,"Single Precision MFlops/s","SSE_RETIRED_ADD_SINGLE_FLOPS:PMC0,SSE_RETIRED_MULT_SINGLE_FLOPS:PMC1,CPU_CLOCKS_UNHALTED:PMC2"},

    {"FLOPS_X87",FLOPS_X87,"X87 MFlops/s","X87_FLOPS_RETIRED_ADD:PMC0,X87_FLOPS_RETIRED_MULT:PMC1,X87_FLOPS_RETIRED_DIV:PMC2,CPU_CLOCKS_UNHALTED:PMC3"},

    {"FPU_EXCEPTION",FPU_EXCEPTION,"Floating point exceptions","INSTRUCTIONS_RETIRED:PMC0,FP_INSTRUCTIONS_RETIRED_ALL:PMC1,FPU_EXCEPTIONS_ALL:PMC2"},

    {"ICACHE",ICACHE,"Instruction cache miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,ICACHE_FETCHES:PMC1,ICACHE_REFILLS_L2:PMC2,ICACHE_REFILLS_MEM:PMC3"},

    {"L2",L2,"L2 cache bandwidth in MBytes/s","DATA_CACHE_REFILLS_L2_ALL:PMC0,DATA_CACHE_EVICTED_ALL:PMC1,CPU_CLOCKS_UNHALTED:PMC2"},

    {"L2CACHE",L2CACHE,"L2 cache miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,L2_REQUESTS_ALL:PMC1,L2_MISSES_ALL:PMC2,L2_FILL_ALL:PMC3"},

    {"L3CACHE",L3CACHE,"L3 cache miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,L3_READ_REQUEST_ALL_ALL_CORES:PMC1,L3_MISSES_ALL_ALL_CORES:PMC2"},

    {"MEM",MEM,"Main memory bandwidth in MBytes/s","NORTHBRIDGE_READ_RESPONSE_ALL:PMC0,OCTWORDS_WRITE_TRANSFERS:PMC1,DRAM_ACCESSES_DCTO_ALL:PMC2,DRAM_ACCESSES_DCT1_ALL:PMC3"},

    {"NUMA",NUMA,"Bandwidth on the Hypertransport links","CPU_TO_DRAM_LOCAL_TO_0:PMC0,CPU_TO_DRAM_LOCAL_TO_1:PMC1,CPU_TO_DRAM_LOCAL_TO_2:PMC2,CPU_TO_DRAM_LOCAL_TO_3:PMC3"},

    {"NUMA2",NUMA2,"Bandwidth on the Hypertransport links","CPU_TO_DRAM_LOCAL_TO_4:PMC0,CPU_TO_DRAM_LOCAL_TO_5:PMC1,CPU_TO_DRAM_LOCAL_TO_6:PMC2,CPU_TO_DRAM_LOCAL_TO_7:PMC3"},

    {"TLB",TLB,"TLB miss rate/ratio","INSTRUCTIONS_RETIRED:PMC0,DATA_CACHE_ACCESSES:PMC1,DTLB_L2_HIT_ALL:PMC2,DTLB_L2_MISS_ALL:PMC3"},

};

void
perfmon_printDerivedMetricsK10(PerfmonGroup group)
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

        case FLOPS_DP:
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,DP MFlops/s   );

            bstrListAdd(fc,3,DP Add MFlops/s   );

            bstrListAdd(fc,4,DP Mult MFlops/s   );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC2")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))/time;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case FLOPS_SP:
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,SP MFlops/s );

            bstrListAdd(fc,3,SP Add MFlops/s );

            bstrListAdd(fc,4,SP Mult MFlops/s  );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC2")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))/time;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case FLOPS_X87:
            numRows = 5;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,X87 MFlops/s      );

            bstrListAdd(fc,3,X87 Add MFlops/s   );

            bstrListAdd(fc,4,X87 Mult MFlops/s  );

            bstrListAdd(fc,5,X87 Div MFlops/s   );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC3")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1")+perfmon_getResult(threadId,"PMC2"))/time;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC2")/time;
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

            }
            break;

        case FPU_EXCEPTION:
            numRows = 2;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Overall FP exception rate );

            bstrListAdd(fc,2,FP exception rate   );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC0");
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC1");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

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

        case L2CACHE:
            numRows = 3;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,L2 request rate  );

            bstrListAdd(fc,2,L2 miss rate  );

            bstrListAdd(fc,3,L2 miss ratio  );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = (perfmon_getResult(threadId,"PMC1")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0");
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC2")/(perfmon_getResult(threadId,"PMC1")+perfmon_getResult(threadId,"PMC3"));
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

            }
            break;

        case L3CACHE:
            numRows = 3;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,L3 request rate );

            bstrListAdd(fc,2,L3 miss rate  );

            bstrListAdd(fc,3,L3 miss ratio );

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

            }
            break;

        case MEM:
            numRows = 3;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Read data bandwidth (MBytes/s) );

            bstrListAdd(fc,2,Write data bandwidth (MBytes/s) );

            bstrListAdd(fc,3,DRAM bandwidth (MBytes/s)  );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")*8.0/time;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))*64.0/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

            }
            break;

        case NUMA:
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

            bstrListAdd(fc,1,Mega requests per second to Node 0  );

            bstrListAdd(fc,2,Mega requests per second to Node 1  );

            bstrListAdd(fc,3,Mega requests per second to Node 2  );

            bstrListAdd(fc,4,Mega requests per second to Node 3  );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")/time;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC2")/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC3")/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case NUMA2:
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

            bstrListAdd(fc,1,Hyper Transport link0 bandwidth (MBytes/s) );

            bstrListAdd(fc,2,Hyper Transport link1 bandwidth (MBytes/s) );

            bstrListAdd(fc,3,Hyper Transport link2 bandwidth (MBytes/s) );

            bstrListAdd(fc,4,Hyper Transport link3 bandwidth (MBytes/s) );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")*4.0/time;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")*4.0/time;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC2")*4.0/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC3")*4.0/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case TLB:
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

            bstrListAdd(fc,1,L1 DTLB request rate );

            bstrListAdd(fc,2,L1 DTLB miss rate  );

            bstrListAdd(fc,3,L1 DTLB miss ratio  );

            bstrListAdd(fc,4,L2 DTLB request rate  );

            bstrListAdd(fc,5,L2 DTLB miss rate   );

            bstrListAdd(fc,6,L2 DTLB miss ratio   );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0");
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = (perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = (perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC1");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = (perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = perfmon_getResult(threadId,"PMC3")/perfmon_getResult(threadId,"PMC0");
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = perfmon_getResult(threadId,"PMC3")/(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"));
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

            }
            break;


        default:
            fprintf (stderr, "perfmon_printDerivedMetricsk10: Unknown group! Exiting!\n" );
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
perfmon_logDerivedMetricsK10(PerfmonGroup group, double time,double timeStamp)
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
            
        case FLOPS_DP:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("DP MFlops/s    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))/time);
                        }
                        printf("\n");
                    
                        printf("DP Add MFlops/s    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
                        printf("DP Mult MFlops/s    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")/time);
                        }
                        printf("\n");
                    
            break;
            
        case FLOPS_SP:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("SP MFlops/s  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))/time);
                        }
                        printf("\n");
                    
                        printf("SP Add MFlops/s  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
                        printf("SP Mult MFlops/s   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")/time);
                        }
                        printf("\n");
                    
            break;
            
        case FLOPS_X87:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC3")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("X87 MFlops/s       %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1")+perfmon_getResult(threadId,"PMC2"))/time);
                        }
                        printf("\n");
                    
                        printf("X87 Add MFlops/s    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
                        printf("X87 Mult MFlops/s   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")/time);
                        }
                        printf("\n");
                    
                        printf("X87 Div MFlops/s    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC2")/time);
                        }
                        printf("\n");
                    
            break;
            
        case FPU_EXCEPTION:

                    
                        printf("Overall FP exception rate  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("FP exception rate    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC1"));
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
            
        case L2CACHE:

                    
                        printf("L2 request rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC1")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L2 miss rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L2 miss ratio   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/(perfmon_getResult(threadId,"PMC1")+perfmon_getResult(threadId,"PMC3")));
                        }
                        printf("\n");
                    
            break;
            
        case L3CACHE:

                    
                        printf("L3 request rate  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L3 miss rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L3 miss ratio  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC2")/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
            break;
            
        case MEM:

                    
                        printf("Read data bandwidth (MBytes/s)  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time);
                        }
                        printf("\n");
                    
                        printf("Write data bandwidth (MBytes/s)  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")*8.0/time);
                        }
                        printf("\n");
                    
                        printf("DRAM bandwidth (MBytes/s)   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))*64.0/time);
                        }
                        printf("\n");
                    
            break;
            
        case NUMA:

                    
                        printf("Mega requests per second to Node 0   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
                        printf("Mega requests per second to Node 1   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")/time);
                        }
                        printf("\n");
                    
                        printf("Mega requests per second to Node 2   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC2")/time);
                        }
                        printf("\n");
                    
                        printf("Mega requests per second to Node 3   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC3")/time);
                        }
                        printf("\n");
                    
            break;
            
        case NUMA2:

                    
                        printf("Hyper Transport link0 bandwidth (MBytes/s)  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")*4.0/time);
                        }
                        printf("\n");
                    
                        printf("Hyper Transport link1 bandwidth (MBytes/s)  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")*4.0/time);
                        }
                        printf("\n");
                    
                        printf("Hyper Transport link2 bandwidth (MBytes/s)  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC2")*4.0/time);
                        }
                        printf("\n");
                    
                        printf("Hyper Transport link3 bandwidth (MBytes/s)  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC3")*4.0/time);
                        }
                        printf("\n");
                    
            break;
            
        case TLB:

                    
                        printf("L1 DTLB request rate  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L1 DTLB miss rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L1 DTLB miss ratio   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
                        printf("L2 DTLB request rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3"))/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L2 DTLB miss rate    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC3")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("L2 DTLB miss ratio    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC3")/(perfmon_getResult(threadId,"PMC2")+perfmon_getResult(threadId,"PMC3")));
                        }
                        printf("\n");
                    
            break;
            

        default:
                fprintf (stderr, "perfmon_printDerivedMetricsk10: Unknown group! Exiting!\n" );
                exit (EXIT_FAILURE);
                break;
    }
}



static PerfmonGroupHelp k10_group_help[NUM_GROUPS_K10] = {

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
\n\
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
\n\
"},

    {"CPI","This group measures how efficient the processor works with\n\
regard to instruction throughput. Also important as a standalone\n\
metric is INSTRUCTIONS_RETIRED as it tells you how many instruction\n\
you need to execute for a task. An optimization might show very\n\
low CPI values but execute many more instruction for it.\n\
\n\
"},

    {"FLOPS_DP","Formulas:\n\
DP MFlops/s = 1.0E-06*(SSE_RETIRED_ADD_DOUBLE_FLOPS+SSE_RETIRED_MULT_DOUBLE_FLOPS)/time\n\
-\n\
Profiling group to measure double SSE flops.\n\
Dont forget that your code might also execute X87 flops.\n\
\n\
\n\
"},

    {"FLOPS_SP","Formulas:\n\
SP MFlops/s = 1.0E-06*(SSE_RETIRED_ADD_SINGLE_FLOPS+SSE_RETIRED_MULT_SINGLE_FLOPS)/time\n\
-\n\
Profiling group to measure single precision SSE flops.\n\
Dont forget that your code might also execute X87 flops.\n\
\n\
\n\
"},

    {"FLOPS_X87","Profiling group to measure X87 flop rates.\n\
\n\
"},

    {"FPU_EXCEPTION","Formulas:\n\
Overall FP exception rate = FPU_EXCEPTIONS_ALL / INSTRUCTIONS_RETIRED\n\
FP exception rate = FPU_EXCEPTIONS_ALL / FP_INSTRUCTIONS_RETIRED_ALL\n\
-\n\
Floating point exceptions occur e.g. on the treatment of Denormals.\n\
There might be a large penalty if there are too many floating point\n\
exceptions.\n\
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
"},

    {"L2CACHE","Formulas:\n\
L2 request rate = (L2_REQUESTS_ALL+L2_FILL_ALL)/INSTRUCTIONS_RETIRED\n\
L2 miss rate  = L2_MISSES_ALL/INSTRUCTIONS_RETIRED\n\
L2 miss ratio = L2_MISSES_ALL/(L2_REQUESTS_ALL+L2_FILL_ALL)\n\
-\n\
This group measures the locality of your data accesses with regard to the\n\
L2 Cache. L2 request rate tells you how data intensive your code is\n\
or how many Data accesses you have in average per instruction.\n\
The L2 miss rate gives a measure how often it was necessary to get\n\
cachelines from memory. And finally L2 miss ratio tells you how many of your\n\
memory references required a cacheline to be loaded from a higher level.\n\
While the Data cache miss rate might be given by your algorithm you should\n\
try to get Data cache miss ratio as low as possible by increasing your cache reuse.\n\
This group was taken from the whitepaper -Basic Performance Measurements for AMD Athlon 64,\n\
AMD Opteron and AMD Phenom Processors- from Paul J. Drongowski.\n\
\n\
\n\
"},

    {"L3CACHE","Formulas:\n\
L3 request rate =  L3_READ_REQUEST_ALL_ALL_CORES / INSTRUCTIONS_RETIRED\n\
L3 miss rate  = L3_MISSES_ALL_ALL_CORES / INSTRUCTIONS_RETIRED\n\
L3 miss ratio =  L3_MISSES_ALL_ALL_CORES / L3_READ_REQUEST_ALL_ALL_CORES\n\
-\n\
This group measures the locality of your data accesses with regard to the\n\
L3 Cache. L3 request rate tells you how data intensive your code is\n\
or how many Data accesses you have in average per instruction.\n\
The L3 miss rate gives a measure how often it was necessary to get\n\
cachelines from memory. And finally L3 miss ratio tells you how many of your\n\
memory references required a cacheline to be loaded from a higher level.\n\
While the Data cache miss rate might be given by your algorithm you should\n\
try to get Data cache miss ratio as low as possible by increasing your cache reuse.\n\
Note: As this group measures the accesses from all cores it only makes sense\n\
to measure with one core per socket, similiar as with the Intel Nehalem Uncore events.\n\
This group was taken from the whitepaper -Basic Performance Measurements for AMD Athlon 64,\n\
AMD Opteron and AMD Phenom Processors- from Paul J. Drongowski.\n\
\n\
\n\
"},

    {"MEM","Formulas:\n\
Read data bandwidth (MBytes/s)  1.0E-06*NORTHBRIDGE_READ_RESPONSE_ALL*64/time\n\
Write data bandwidth (MBytes/s)  1.0E-06*OCTWORDS_WRITE_TRANSFERS*8/time\n\
Memory bandwidth [MBytes/s] = 1.0E-06*(DRAM_ACCESSES_DCTO_ALL+DRAM_ACCESSES_DCT1_ALL)*64/time\n\
-\n\
Profiling group to measure memory bandwidth drawn by all cores of a socket.\n\
Note: As this group measures the accesses from all cores it only makes sense\n\
to measure with one core per socket, similiar as with the Intel Nehalem Uncore events.\n\
\n\
"},

    {"NUMA","Formulas:\n\
Mega requests per second to Node X   1.0E-06*PMCX/time\n\
-\n\
Profiling group to measure the traffic from local CPU to the different \n\
DRAM NUMA nodes. This group allows to detect NUMA problems in a threaded \n\
code. You must first determine on which memory domains your code is running.\n\
A code should only have significant traffic to its own memory domain.\n\
\n\
\n\
"},

    {"NUMA2","Formulas:\n\
Hyper Transport linkn bandwidth (MBytes/s)  1.0E-06*HYPERTRANSPORT_LINK0_ALL_SENT*4.0/time\n\
-\n\
Profiling group to measure the bandwidth over the Hypertransport links. Can be used\n\
to detect NUMA problems. Usually there should be only limited traffic over the QPI \n\
links for optimal performance.\n\
\n\
\n\
"},

    {"TLB","Formulas:\n\
L1 DTLB request rate  DATA_CACHE_ACCESSES / INSTRUCTIONS_RETIRED\n\
L1 DTLB miss rate  (DTLB_L2_HIT_ALL+DTLB_L2_MISS_ALL)/INSTRUCTIONS_RETIRED\n\
L1 DTLB miss ratio  (DTLB_L2_HIT_ALL+DTLB_L2_MISS_ALL)/DATA_CACHE_ACCESSES\n\
L2 DTLB request rate  (DTLB_L2_HIT_ALL+DTLB_L2_MISS_ALL)/INSTRUCTIONS_RETIRED\n\
L2 DTLB miss rate  DTLB_L2_MISS_ALL / INSTRUCTIONS_RETIRED\n\
L2 DTLB miss ratio DTLB_L2_MISS_ALL / (DTLB_L2_HIT_ALL+DTLB_L2_MISS_ALL)\n\
-\n\
L1 DTLB request  rate tells you how data intensive your code is\n\
or how many Data accesses you have in average per instruction.\n\
The DTLB miss  rate gives a measure how often a TLB miss occured\n\
per instruction. And finally L1 DTLB  miss ratio tells you how many\n\
of your memory references required caused a TLB miss in average.\n\
NOTE: The L2 metrics are only relevant if L2 DTLB request rate is equal to the L1 DTLB miss rate!\n\
This group was taken from the whitepaper Basic -Performance Measurements for AMD Athlon 64,\n\
AMD Opteron and AMD Phenom Processors- from Paul J. Drongowski.\n\
"},

};

