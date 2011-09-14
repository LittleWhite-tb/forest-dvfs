/* GENERATED FILE: DO NOTE EDIT */

#define NUM_GROUPS_CORE2 10

static PerfmonGroupMap core2_group_map[NUM_GROUPS_CORE2] = {

    {"BRANCH",BRANCH,"Branch prediction miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,BR_INST_RETIRED_ANY:PMC0,BR_INST_RETIRED_MISPRED:PMC1"},

    {"CACHE",CACHE,"Data cache miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L1D_REPL:PMC0,L1D_ALL_CACHE_REF:PMC1"},

    {"DATA",DATA,"Load to store ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,INST_RETIRED_LOADS:PMC0,INST_RETIRED_STORES:PMC1"},

    {"FLOPS_DP",FLOPS_DP,"Double Precision MFlops/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,SIMD_COMP_INST_RETIRED_PACKED_DOUBLE:PMC0,SIMD_COMP_INST_RETIRED_SCALAR_DOUBLE:PMC1"},

    {"FLOPS_SP",FLOPS_SP,"Single Precision MFlops/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,SIMD_COMP_INST_RETIRED_PACKED_SINGLE:PMC0,SIMD_COMP_INST_RETIRED_SCALAR_SINGLE:PMC1"},

    {"FLOPS_X87",FLOPS_X87,"X87 MFlops/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,X87_OPS_RETIRED_ANY:PMC0"},

    {"L2",L2,"L2 cache bandwidth in MBytes/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L1D_REPL:PMC0,L1D_M_EVICT:PMC1"},

    {"L2CACHE",L2CACHE,"L2 cache miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L2_RQSTS_THIS_CORE_ALL_MESI:PMC0,L2_RQSTS_SELF_I_STATE:PMC1"},

    {"MEM",MEM,"Main memory bandwidth in MBytes/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,BUS_TRANS_MEM_THIS_CORE_THIS_A:PMC0"},

    {"TLB",TLB,"TLB miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,DTLB_MISSES_ANY:PMC0,L1D_ALL_CACHE_REF:PMC1"},

};

void
perfmon_printDerivedMetricsCore2(PerfmonGroup group)
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,Branch rate  );

            bstrListAdd(fc,4,Branch misprediction rate );

            bstrListAdd(fc,5,Branch misprediction ratio );

            bstrListAdd(fc,6,Instructions per branch );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0");
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = perfmon_getResult(threadId,"FIXC0")/perfmon_getResult(threadId,"PMC0");
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

            }
            break;

        case CACHE:
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,Data cache misses);

            bstrListAdd(fc,4,Data cache request rate);

            bstrListAdd(fc,5,Data cache miss rate);

            bstrListAdd(fc,6,Data cache miss ratio);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0");
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1");
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

            }
            break;

        case DATA:
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,Load to Store ratio);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

            }
            break;

        case FLOPS_DP:
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,DP MFlops/s   );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")*2.0+perfmon_getResult(threadId,"PMC1"))/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

            }
            break;

        case FLOPS_SP:
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,SP MFlops/s);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")*4.0+perfmon_getResult(threadId,"PMC1"))/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

            }
            break;

        case FLOPS_X87:
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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,X87 MFlops/s );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

            }
            break;

        case L2:
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

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,L2 Load [MBytes/s]);

            bstrListAdd(fc,4,L2 Evict [MBytes/s]);

            bstrListAdd(fc,5,L2 bandwidth [MBytes/s]);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
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

                tableData.rows[4].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))*64.0/time;
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

            }
            break;

        case L2CACHE:
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

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,L2 request rate);

            bstrListAdd(fc,4,L2 miss rate);

            bstrListAdd(fc,5,L2 miss ratio);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0");
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

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

            bstrListAdd(fc,1,Runtime [s]);

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,Memory bandwidth [MBytes/s]);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

            }
            break;

        case TLB:
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

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,L1 DTLB request rate   );

            bstrListAdd(fc,4,DTLB miss rate   );

            bstrListAdd(fc,5,L1 DTLB miss ratio  );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1");
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

            }
            break;


        default:
            fprintf (stderr, "perfmon_printDerivedMetricscore2: Unknown group! Exiting!\n" );
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
perfmon_logDerivedMetricsCore2(PerfmonGroup group, double time,double timeStamp)
{
    int threadId;
    double inverseClock = 1.0 /(double) timer_getCpuClock();

    switch ( group ) 
    {
        
        case BRANCH:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Branch rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Branch misprediction rate  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Branch misprediction ratio  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Instructions per branch  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC0")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
            break;
            
        case CACHE:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Data cache misses %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
                        printf("Data cache request rate %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Data cache miss rate %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Data cache miss ratio %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
            break;
            
        case DATA:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Load to Store ratio %e ",timeStamp);
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
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("DP MFlops/s    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")*2.0+perfmon_getResult(threadId,"PMC1"))/time);
                        }
                        printf("\n");
                    
            break;
            
        case FLOPS_SP:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("SP MFlops/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")*4.0+perfmon_getResult(threadId,"PMC1"))/time);
                        }
                        printf("\n");
                    
            break;
            
        case FLOPS_X87:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("X87 MFlops/s  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
            break;
            
        case L2:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L2 Load [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time);
                        }
                        printf("\n");
                    
                        printf("L2 Evict [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")*64.0/time);
                        }
                        printf("\n");
                    
                        printf("L2 bandwidth [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))*64.0/time);
                        }
                        printf("\n");
                    
            break;
            
        case L2CACHE:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L2 request rate %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L2 miss rate %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L2 miss ratio %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"PMC0"));
                        }
                        printf("\n");
                    
            break;
            
        case MEM:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("Memory bandwidth [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time);
                        }
                        printf("\n");
                    
            break;
            
        case TLB:

                    
                        printf("Runtime [s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")*inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L1 DTLB request rate    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("DTLB miss rate    %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L1 DTLB miss ratio   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"PMC1"));
                        }
                        printf("\n");
                    
            break;
            

        default:
                fprintf (stderr, "perfmon_printDerivedMetricscore2: Unknown group! Exiting!\n" );
                exit (EXIT_FAILURE);
                break;
    }
}



static PerfmonGroupHelp core2_group_help[NUM_GROUPS_CORE2] = {

    {"BRANCH","Formulas:\n\
Branch rate = BR_INST_RETIRED_ANY / INSTR_RETIRED_ANY\n\
Branch misprediction rate = BR_INST_RETIRED_MISPRED / INSTR_RETIRED_ANY\n\
Branch misprediction ratio = BR_INST_RETIRED_MISPRED / BR_INST_RETIRED_ANY\n\
Instructions per branch = INSTR_RETIRED_ANY / BR_INST_RETIRED_ANY\n\
-\n\
The rates state how often in average a branch or a mispredicted branch occured\n\
per instruction retired in total. The Branch misprediction ratio sets directly\n\
into relation what ration of all branch instruction where mispredicted.\n\
Instructions per branch is 1/Branch rate.\n\
"},

    {"CACHE","Formulas:\n\
Data cache request rate =  L1D_ALL_CACHE_REF / INSTR_RETIRED_ANY\n\
Data cache miss rate = L1D_REPL / INSTR_RETIRED_ANY\n\
Data cache miss ratio =  L1D_REPL / L1D_ALL_CACHE_REF\n\
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
\n\
"},

    {"DATA","Formulas:\n\
Load to Store ratio = INST_RETIRED_LOADS / INST_RETIRED_STORES\n\
-\n\
This is a simple metric to determine your Load to store ratio.\n\
\n\
"},

    {"FLOPS_DP","Formulas:\n\
DP MFlops/s = 1.0E-06*(SIMD_COMP_INST_RETIRED_PACKED_DOUBLE*2+SIMD_COMP_INST_RETIRED_SCALAR_DOUBLE)/time\n\
-\n\
Profiling group to measure double SSE flops. Dont forget that your code might also execute X87 flops.\n\
On the number of SIMD_COMP_INST_RETIRED_PACKED_DOUBLE you can see how well your code was vectorized.\n\
\n\
\n\
"},

    {"FLOPS_SP","Formulas:\n\
SP MFlops/s = 1.0E-06*(SIMD_COMP_INST_RETIRED_PACKED_SINGLE*4+SIMD_COMP_INST_RETIRED_SCALAR_SINGLE)/time\n\
-\n\
Profiling group to measure single precision SSE flops. Dont forget that your code might also execute X87 flops.\n\
On the number of SIMD_COMP_INST_RETIRED_PACKED_SINGLE you can see how well your code was vectorized.\n\
\n\
\n\
"},

    {"FLOPS_X87","Formulas:\n\
X87 MFlops/s = 1.0E-06*X87_OPS_RETIRED_ANY/time\n\
-\n\
Profiling group to measure X87 flops. Note that also non computational operations\n\
are measured by this event.\n\
\n\
"},

    {"L2","Formulas:\n\
L2 Load [MBytes/s] = 1.0E-06*L1D_REPL*64/time\n\
L2 Evict [MBytes/s] = 1.0E-06*L1D_M_EVICT*64/time\n\
L2 bandwidth [MBytes/s] = 1.0E-06*(L1D_REPL+L1D_M_EVICT)*64/time\n\
-\n\
Profiling group to measure L2 cache bandwidth. The bandwidth is\n\
computed by the number of cacheline allocated in the L1 and the \n\
number of modified cachelines evicted from the L1. \n\
Note that this bandwidth also includes data transfers due to a\n\
write allocate load on a store miss in L1.\n\
\n\
"},

    {"L2CACHE","Formulas:\n\
L2 request rate =  L2_RQSTS_THIS_CORE_ALL_MESI / INSTR_RETIRED_ANY\n\
L2 miss rate  = L2_RQSTS_SELF_I_STATE / INSTR_RETIRED_ANY\n\
L2 miss ratio = L2_RQSTS_SELF_I_STATE / L2_RQSTS_THIS_CORE_ALL_MESI\n\
-\n\
This group measures the locality of your data accesses with regard to the\n\
L2 Cache. L2 request rate tells you how data intensive your code is\n\
or how many Data accesses you have in average per instruction.\n\
The L2 miss rate gives a measure how often it was necessary to get\n\
cachelines from memory. And finally L2 miss ratio tells you how many of your\n\
memory references required a cacheline to be loaded from a higher level.\n\
While the Data cache miss rate might be given by your algorithm you should\n\
try to get Data cache miss ratio as low as possible by increasing your cache reuse.\n\
Note: This group might need to be revised!\n\
\n\
\n\
"},

    {"MEM","Formulas:\n\
Memory bandwidth [MBytes/s] = 1.0E-06*BUS_TRANS_MEM_THIS_CORE_THIS_A*64/time\n\
-\n\
Profiling group to measure memory bandwidth drawn by this core. \n\
"},

    {"TLB","Formulas:\n\
L1 DTLB request rate =  L1D_ALL_CACHE_REF / INSTR_RETIRED_ANY\n\
DTLB miss rate  = DTLB_MISSES_ANY / INSTR_RETIRED_ANY\n\
L1 DTLB miss ratio  =  DTLB_MISSES_ANY / L1D_ALL_CACHE_REF\n\
-\n\
L1 DTLB request  rate tells you how data intensive your code is\n\
or how many Data accesses you have in average per instruction.\n\
The DTLB miss  rate gives a measure how often a TLB miss occured\n\
per instruction. And finally L1 DTLB  miss ratio tells you how many\n\
of your memory references required caused a TLB miss in average.\n\
\n\
"},

};

