/* GENERATED FILE: DO NOTE EDIT */

#define NUM_GROUPS_WESTMERE 14

static PerfmonGroupMap westmere_group_map[NUM_GROUPS_WESTMERE] = {

    {"BRANCH",BRANCH,"Branch prediction miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,BR_INST_RETIRED_ALL_BRANCHES:PMC0,BR_MISP_RETIRED_ALL_BRANCHES:PMC1"},

    {"CACHE",CACHE,"Data cache miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L1D_REPL:PMC0"},

    {"CLOCK",CLOCK,"Clock of cores","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,CPU_CLK_UNHALTED_REF:FIXC2"},

    {"DATA",DATA,"Load to store ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,MEM_INST_RETIRED_LOADS:PMC0,MEM_INST_RETIRED_STORES:PMC1"},

    {"FLOPS_DP",FLOPS_DP,"Double Precision MFlops/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,CPU_CLK_UNHALTED_REF:FIXC2,FP_COMP_OPS_EXE_SSE_FP_PACKED:PMC0,FP_COMP_OPS_EXE_SSE_FP_SCALAR:PMC1,FP_COMP_OPS_EXE_SSE_SINGLE_PRECISION:PMC2,FP_COMP_OPS_EXE_SSE_DOUBLE_PRECISION:PMC3"},

    {"FLOPS_SP",FLOPS_SP,"Single Precision MFlops/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,CPU_CLK_UNHALTED_REF:FIXC2,FP_COMP_OPS_EXE_SSE_FP_PACKED:PMC0,FP_COMP_OPS_EXE_SSE_FP_SCALAR:PMC1,FP_COMP_OPS_EXE_SSE_SINGLE_PRECISION:PMC2,FP_COMP_OPS_EXE_SSE_DOUBLE_PRECISION:PMC3"},

    {"FLOPS_X87",FLOPS_X87,"X87 MFlops/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,INST_RETIRED_X87:PMC0"},

    {"L2",L2,"L2 cache bandwidth in MBytes/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L1D_REPL:PMC0,L1D_M_EVICT:PMC1"},

    {"L2CACHE",L2CACHE,"L2 cache miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L2_DATA_RQSTS_DEMAND_ANY:PMC0,L2_RQSTS_MISS:PMC1"},

    {"L3",L3,"L3 cache bandwidth in MBytes/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,L2_LINES_IN_ANY:PMC0,L2_LINES_OUT_DEMAND_DIRTY:PMC1"},

    {"L3CACHE",L3CACHE,"L3 cache miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,UNC_L3_HITS_ANY:UPMC0,UNC_L3_MISS_ANY:UPMC1,UNC_L3_LINES_IN_ANY:UPMC2,UNC_L3_LINES_OUT_ANY:UPMC3"},

    {"MEM",MEM,"Main memory bandwidth in MBytes/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,UNC_QMC_NORMAL_READS_ANY:UPMC0,UNC_QMC_WRITES_FULL_ANY:UPMC1,UNC_QHL_REQUESTS_REMOTE_READS:UPMC2,UNC_QHL_REQUESTS_LOCAL_READS:UPMC3,UNC_QHL_REQUESTS_REMOTE_WRITES:UPMC4"},

    {"TLB",TLB,"TLB miss rate/ratio","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,DTLB_MISSES_ANY:PMC0"},

    {"VIEW",VIEW,"Double Precision MFlops/s","INSTR_RETIRED_ANY:FIXC0,CPU_CLK_UNHALTED_CORE:FIXC1,CPU_CLK_UNHALTED_REF:FIXC2,FP_COMP_OPS_EXE_SSE_FP_PACKED:PMC0,FP_COMP_OPS_EXE_SSE_FP_SCALAR:PMC1,FP_COMP_OPS_EXE_SSE_SINGLE_PRECISION:PMC2,FP_COMP_OPS_EXE_SSE_DOUBLE_PRECISION:PMC3,UNC_QMC_NORMAL_READS_ANY:UPMC0,UNC_QMC_WRITES_FULL_ANY:UPMC1,UNC_QHL_REQUESTS_REMOTE_READS:UPMC2,UNC_QHL_REQUESTS_LOCAL_READS:UPMC3"},

};

void
perfmon_printDerivedMetricsWestmere(PerfmonGroup group)
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

            bstrListAdd(fc,2,CPI );

            bstrListAdd(fc,3,Data cache misses);

            bstrListAdd(fc,4,Data cache miss rate);

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

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

            }
            break;

        case CLOCK:
            numRows = 1;
            stat = (double**) malloc(numRows * sizeof(double*));
            for (int i=0; i<numRows; i++)
            {
                stat[i] = (double*) malloc(4 * sizeof(double));
                stat[i][0] = 0;
                stat[i][1] = 0;
                stat[i][2] = DBL_MAX;
            }
            INIT_BASIC;

            bstrListAdd(fc,1,Clock [MHz] );

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = 1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

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
            numRows = 8;
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

            bstrListAdd(fc,2,Clock [MHz] );

            bstrListAdd(fc,3,CPI );

            bstrListAdd(fc,4,DP MFlops/s (DP assumed));

            bstrListAdd(fc,5,Packed MUOPS/s  );

            bstrListAdd(fc,6,Scalar MUOPS/s);

            bstrListAdd(fc,7,SP MUOPS/s);

            bstrListAdd(fc,8,DP MUOPS/s);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")*2.0+perfmon_getResult(threadId,"PMC1"))/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")/time;
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

                tableData.rows[6].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC2")/time;
                stat[6][0] += (double) tableData.rows[6].value[threadId];
                stat[6][1] =  MAX(stat[6][1],(double) tableData.rows[6].value[threadId]);
                stat[6][2] =  MIN(stat[6][2],(double) tableData.rows[6].value[threadId]);

                tableData.rows[7].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC3")/time;
                stat[7][0] += (double) tableData.rows[7].value[threadId];
                stat[7][1] =  MAX(stat[7][1],(double) tableData.rows[7].value[threadId]);
                stat[7][2] =  MIN(stat[7][2],(double) tableData.rows[7].value[threadId]);

            }
            break;

        case FLOPS_SP:
            numRows = 8;
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

            bstrListAdd(fc,2,Clock [MHz] );

            bstrListAdd(fc,3,CPI );

            bstrListAdd(fc,4,SP MFlops/s (SP assumed));

            bstrListAdd(fc,5,Packed MUOPS/s  );

            bstrListAdd(fc,6,Scalar MUOPS/s);

            bstrListAdd(fc,7,SP MUOPS/s);

            bstrListAdd(fc,8,DP MUOPS/s);

            initResultTable(&tableData, fc, numRows, numColumns);

            for(threadId=0; threadId < perfmon_numThreads; threadId++)
            {

                tableData.rows[0].value[threadId] = perfmon_getResult(threadId,"FIXC1")*inverseClock;
                stat[0][0] += (double) tableData.rows[0].value[threadId];
                stat[0][1] =  MAX(stat[0][1],(double) tableData.rows[0].value[threadId]);
                stat[0][2] =  MIN(stat[0][2],(double) tableData.rows[0].value[threadId]);

                tableData.rows[1].value[threadId] = 1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock;
                stat[1][0] += (double) tableData.rows[1].value[threadId];
                stat[1][1] =  MAX(stat[1][1],(double) tableData.rows[1].value[threadId]);
                stat[1][2] =  MIN(stat[1][2],(double) tableData.rows[1].value[threadId]);

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")*4.0+perfmon_getResult(threadId,"PMC1"))/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")/time;
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

                tableData.rows[6].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC2")/time;
                stat[6][0] += (double) tableData.rows[6].value[threadId];
                stat[6][1] =  MAX(stat[6][1],(double) tableData.rows[6].value[threadId]);
                stat[6][2] =  MIN(stat[6][2],(double) tableData.rows[6].value[threadId]);

                tableData.rows[7].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC3")/time;
                stat[7][0] += (double) tableData.rows[7].value[threadId];
                stat[7][1] =  MAX(stat[7][1],(double) tableData.rows[7].value[threadId]);
                stat[7][2] =  MIN(stat[7][2],(double) tableData.rows[7].value[threadId]);

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

        case L3:
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

            bstrListAdd(fc,3,L3 Load [MBytes/s] );

            bstrListAdd(fc,4,L3 Evict [MBytes/s] );

            bstrListAdd(fc,5,L3 bandwidth [MBytes/s]);

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

        case L3CACHE:
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

            bstrListAdd(fc,3,L3 request rate  );

            bstrListAdd(fc,4,L3 miss rate  );

            bstrListAdd(fc,5,L3 miss ratio );

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

                tableData.rows[2].value[threadId] = perfmon_getResult(threadId,"UPMC0")/perfmon_getResult(threadId,"FIXC0");
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = perfmon_getResult(threadId,"UPMC1")/perfmon_getResult(threadId,"FIXC0");
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = perfmon_getResult(threadId,"UPMC1")/perfmon_getResult(threadId,"UPMC0");
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

            }
            break;

        case MEM:
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

            bstrListAdd(fc,3,Memory bandwidth [MBytes/s]);

            bstrListAdd(fc,4,Remote Read BW [MBytes/s]);

            bstrListAdd(fc,5,Remote Write BW [MBytes/s]);

            bstrListAdd(fc,6,Remote BW [MBytes/s]);

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

                tableData.rows[2].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"UPMC0")+perfmon_getResult(threadId,"UPMC1"))*64/time;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"UPMC2"))*64/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"UPMC4"))*64/time;
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"UPMC2")+perfmon_getResult(threadId,"UPMC4"))*64/time;
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

            }
            break;

        case TLB:
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

            bstrListAdd(fc,3,L1 DTLB miss rate );

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

            }
            break;

        case VIEW:
            numRows = 10;
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

            bstrListAdd(fc,3,Clock [MHz] );

            bstrListAdd(fc,4,DP MFlops/s (DP assumed));

            bstrListAdd(fc,5,Packed MUOPS/s  );

            bstrListAdd(fc,6,Scalar MUOPS/s);

            bstrListAdd(fc,7,SP MUOPS/s);

            bstrListAdd(fc,8,DP MUOPS/s);

            bstrListAdd(fc,9,Memory bandwidth [MBytes/s]);

            bstrListAdd(fc,10,Remote Read BW [MBytes/s]);

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

                tableData.rows[2].value[threadId] = 1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock;
                stat[2][0] += (double) tableData.rows[2].value[threadId];
                stat[2][1] =  MAX(stat[2][1],(double) tableData.rows[2].value[threadId]);
                stat[2][2] =  MIN(stat[2][2],(double) tableData.rows[2].value[threadId]);

                tableData.rows[3].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"PMC0")*2.0+perfmon_getResult(threadId,"PMC1"))/time;
                stat[3][0] += (double) tableData.rows[3].value[threadId];
                stat[3][1] =  MAX(stat[3][1],(double) tableData.rows[3].value[threadId]);
                stat[3][2] =  MIN(stat[3][2],(double) tableData.rows[3].value[threadId]);

                tableData.rows[4].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC0")/time;
                stat[4][0] += (double) tableData.rows[4].value[threadId];
                stat[4][1] =  MAX(stat[4][1],(double) tableData.rows[4].value[threadId]);
                stat[4][2] =  MIN(stat[4][2],(double) tableData.rows[4].value[threadId]);

                tableData.rows[5].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC1")/time;
                stat[5][0] += (double) tableData.rows[5].value[threadId];
                stat[5][1] =  MAX(stat[5][1],(double) tableData.rows[5].value[threadId]);
                stat[5][2] =  MIN(stat[5][2],(double) tableData.rows[5].value[threadId]);

                tableData.rows[6].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC2")/time;
                stat[6][0] += (double) tableData.rows[6].value[threadId];
                stat[6][1] =  MAX(stat[6][1],(double) tableData.rows[6].value[threadId]);
                stat[6][2] =  MIN(stat[6][2],(double) tableData.rows[6].value[threadId]);

                tableData.rows[7].value[threadId] = 1.0E-06*perfmon_getResult(threadId,"PMC3")/time;
                stat[7][0] += (double) tableData.rows[7].value[threadId];
                stat[7][1] =  MAX(stat[7][1],(double) tableData.rows[7].value[threadId]);
                stat[7][2] =  MIN(stat[7][2],(double) tableData.rows[7].value[threadId]);

                tableData.rows[8].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"UPMC0")+perfmon_getResult(threadId,"UPMC1"))*64/time;
                stat[8][0] += (double) tableData.rows[8].value[threadId];
                stat[8][1] =  MAX(stat[8][1],(double) tableData.rows[8].value[threadId]);
                stat[8][2] =  MIN(stat[8][2],(double) tableData.rows[8].value[threadId]);

                tableData.rows[9].value[threadId] = 1.0E-06*(perfmon_getResult(threadId,"UPMC2"))*64/time;
                stat[9][0] += (double) tableData.rows[9].value[threadId];
                stat[9][1] =  MAX(stat[9][1],(double) tableData.rows[9].value[threadId]);
                stat[9][2] =  MIN(stat[9][2],(double) tableData.rows[9].value[threadId]);

            }
            break;


        default:
            fprintf (stderr, "perfmon_printDerivedMetricswestmere: Unknown group! Exiting!\n" );
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
perfmon_logDerivedMetricsWestmere(PerfmonGroup group, double time,double timeStamp)
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
                    
                        printf("Data cache miss rate %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
            break;
            
        case CLOCK:

                    
                        printf("Clock [MHz]  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock);
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
                    
                        printf("Clock [MHz]  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("DP MFlops/s (DP assumed) %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")*2.0+perfmon_getResult(threadId,"PMC1"))/time);
                        }
                        printf("\n");
                    
                        printf("Packed MUOPS/s   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
                        printf("Scalar MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")/time);
                        }
                        printf("\n");
                    
                        printf("SP MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC2")/time);
                        }
                        printf("\n");
                    
                        printf("DP MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC3")/time);
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
                    
                        printf("Clock [MHz]  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock);
                        }
                        printf("\n");
                    
                        printf("CPI  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("SP MFlops/s (SP assumed) %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")*4.0+perfmon_getResult(threadId,"PMC1"))/time);
                        }
                        printf("\n");
                    
                        printf("Packed MUOPS/s   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
                        printf("Scalar MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")/time);
                        }
                        printf("\n");
                    
                        printf("SP MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC2")/time);
                        }
                        printf("\n");
                    
                        printf("DP MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC3")/time);
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
            
        case L3:

                    
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
                    
                        printf("L3 Load [MBytes/s]  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")*64.0/time);
                        }
                        printf("\n");
                    
                        printf("L3 Evict [MBytes/s]  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")*64.0/time);
                        }
                        printf("\n");
                    
                        printf("L3 bandwidth [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")+perfmon_getResult(threadId,"PMC1"))*64.0/time);
                        }
                        printf("\n");
                    
            break;
            
        case L3CACHE:

                    
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
                    
                        printf("L3 request rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"UPMC0")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L3 miss rate   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"UPMC1")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
                        printf("L3 miss ratio  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"UPMC1")/perfmon_getResult(threadId,"UPMC0"));
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
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"UPMC0")+perfmon_getResult(threadId,"UPMC1"))*64/time);
                        }
                        printf("\n");
                    
                        printf("Remote Read BW [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"UPMC2"))*64/time);
                        }
                        printf("\n");
                    
                        printf("Remote Write BW [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"UPMC4"))*64/time);
                        }
                        printf("\n");
                    
                        printf("Remote BW [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"UPMC2")+perfmon_getResult(threadId,"UPMC4"))*64/time);
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
                    
                        printf("L1 DTLB miss rate  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",perfmon_getResult(threadId,"PMC0")/perfmon_getResult(threadId,"FIXC0"));
                        }
                        printf("\n");
                    
            break;
            
        case VIEW:

                    
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
                    
                        printf("Clock [MHz]  %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.E-06*(perfmon_getResult(threadId,"FIXC1")/perfmon_getResult(threadId,"FIXC2"))/inverseClock);
                        }
                        printf("\n");
                    
                        printf("DP MFlops/s (DP assumed) %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"PMC0")*2.0+perfmon_getResult(threadId,"PMC1"))/time);
                        }
                        printf("\n");
                    
                        printf("Packed MUOPS/s   %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC0")/time);
                        }
                        printf("\n");
                    
                        printf("Scalar MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC1")/time);
                        }
                        printf("\n");
                    
                        printf("SP MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC2")/time);
                        }
                        printf("\n");
                    
                        printf("DP MUOPS/s %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*perfmon_getResult(threadId,"PMC3")/time);
                        }
                        printf("\n");
                    
                        printf("Memory bandwidth [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"UPMC0")+perfmon_getResult(threadId,"UPMC1"))*64/time);
                        }
                        printf("\n");
                    
                        printf("Remote Read BW [MBytes/s] %e ",timeStamp);
                        for(threadId=0; threadId < perfmon_numThreads; threadId++)
                        {
                            printf(" %e  ",1.0E-06*(perfmon_getResult(threadId,"UPMC2"))*64/time);
                        }
                        printf("\n");
                    
            break;
            

        default:
                fprintf (stderr, "perfmon_printDerivedMetricswestmere: Unknown group! Exiting!\n" );
                exit (EXIT_FAILURE);
                break;
    }
}



static PerfmonGroupHelp westmere_group_help[NUM_GROUPS_WESTMERE] = {

    {"BRANCH","Formulas:\n\
Branch rate = BR_INST_RETIRED_ALL_BRANCHES / INSTR_RETIRED_ANY\n\
Branch misprediction rate =  BR_MISP_RETIRED_ALL_BRANCHES / INSTR_RETIRED_ANY\n\
Branch misprediction ratio = BR_MISP_RETIRED_ALL_BRANCHES / BR_INST_RETIRED_ALL_BRANCHES\n\
Instructions per branch = INSTR_RETIRED_ANY / BR_INST_RETIRED_ALL_BRANCHES\n\
-\n\
The rates state how often in average a branch or a mispredicted branch occured\n\
per instruction retired in total. The Branch misprediction ratio sets directly\n\
into relation what ration of all branch instruction where mispredicted.\n\
Instructions per branch is 1/Branch rate.\n\
\n\
"},

    {"CACHE","Formulas:\n\
Data cache miss rate = L1D_REPL / INSTR_RETIRED_ANY\n\
-\n\
This group measures the locality of your data accesses with regard to the\n\
L1 Cache. \n\
The Data cache miss rate gives a measure how often it was necessary to get\n\
cachelines from higher levels of the memory hierarchy.\n\
\n\
"},

    {"CLOCK","Formulas:\n\
Clock [MHz]  1.E-06*(CPU_CLK_UNHALTED_CORE/CPU_CLK_UNHALTED_REF)/inverseClock\n\
-\n\
This group gives you the actual core frequency in the measured time period. This is \n\
important because due to Speedstep and Turbo mode the frequency can vary significantly.\n\
\n\
\n\
"},

    {"DATA","Formulas:\n\
Load to Store ratio = MEM_INST_RETIRED_LOADS / MEM_INST_RETIRED_STORES\n\
-\n\
This is a simple metric to determine your Load to store ratio.\n\
\n\
"},

    {"FLOPS_DP","Formula:\n\
DP MFlops/s =  (FP_COMP_OPS_EXE_SSE_FP_PACKED*2 +  FP_COMP_OPS_EXE_SSE_FP_SCALAR)/ runtime\n\
-\n\
The Nehalem has not possibility to measure MFlops if mixed precision calculations are done.\n\
Therefore both Single as well as Double precision are measured to ensure the correctness\n\
of the measurements. You can check if your code was vectorized on the number of\n\
FP_COMP_OPS_EXE_SSE_FP_PACKED versus the  FP_COMP_OPS_EXE_SSE_FP_SCALAR.\n\
\n\
"},

    {"FLOPS_SP","Formula:\n\
SP MFlops/s =  (FP_COMP_OPS_EXE_SSE_FP_PACKED*4 +  FP_COMP_OPS_EXE_SSE_FP_SCALAR)/ runtime\n\
-\n\
The Nehalem has not possibility to measure MFlops if mixed precision calculations are done.\n\
Therefore both Single as well as Double precision are measured to ensure the correctness\n\
of the measurements. You can check if your code was vectorized on the number of\n\
FP_COMP_OPS_EXE_SSE_FP_PACKED versus the  FP_COMP_OPS_EXE_SSE_FP_SCALAR.\n\
\n\
"},

    {"FLOPS_X87","Profiling group to measure X87 flop rate.\n\
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
L2 request rate = L2_DATA_RQSTS_DEMAND_ANY / INSTR_RETIRED_ANY\n\
L2 miss rate  = L2_RQSTS_MISS / INSTR_RETIRED_ANY\n\
L2 miss ratio = L2_RQSTS_MISS / L2_DATA_RQSTS_DEMAND_ANY\n\
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

    {"L3","Formulas:\n\
L3 Load [MBytes/s]  1.0E-06*L2_LINES_IN_ANY*64/time\n\
L3 Evict [MBytes/s]  1.0E-06*L2_LINES_OUT_DEMAND_DIRTY*64/time\n\
L3 bandwidth [MBytes/s] 1.0E-06*(L2_LINES_IN_ANY+L2_LINES_OUT_DEMAND_DIRTY)*64/time\n\
-\n\
Profiling group to measure L3 cache bandwidth. The bandwidth is\n\
computed by the number of cacheline allocated in the L2 and the \n\
number of modified cachelines evicted from the L2. \n\
Note that this bandwidth also includes data transfers due to a\n\
write allocate load on a store miss in L2.\n\
Note: This group might need to be revised!\n\
\n\
"},

    {"L3CACHE","Formulas:\n\
L3 request rate  UNC_L3_HITS_ANY / INSTR_RETIRED_ANY \n\
L3 miss rate   UNC_L3_MISS_ANY / INSTR_RETIRED_ANY\n\
L3 miss ratio  UNC_L3_MISS_ANY / UNC_L3_HITS_ANY\n\
-\n\
This group measures the locality of your data accesses with regard to the\n\
L3 Cache. L3 request rate tells you how data intensive your code is\n\
or how many Data accesses you have in average per instruction.\n\
The L3 miss rate gives a measure how often it was necessary to get\n\
cachelines from memory. And finally L3 miss ratio tells you how many of your\n\
memory references required a cacheline to be loaded from a higher level.\n\
While the Data cache miss rate might be given by your algorithm you should\n\
try to get Data cache miss ratio as low as possible by increasing your cache reuse.\n\
Note: This group might need to be revised!\n\
\n\
\n\
"},

    {"MEM","Formulas:\n\
Memory bandwidth [MBytes/s] = 1.0E-06*(UNC_QMC_NORMAL_READS_ANY+UNC_QMC_WRITES_FULL_ANY)*64/time\n\
Remote Read BW [MBytes/s] =  1.0E-06*(UNC_QHL_REQUESTS_REMOTE_READS)*64/time;\n\
Remote Write BW [MBytes/s] 1.0E-06*(UNC_QHL_REQUESTS_REMOTE_WRITES)*64/time\n\
Remote Write BW [MBytes/s] 1.0E-06*(UNC_QHL_REQUESTS_REMOTE_READS+UNC_QHL_REQUESTS_REMOTE_WRITES)*64/time\n\
-\n\
Profiling group to measure memory bandwidth drawn by all cores of a socket.\n\
This group will be measured by one core per socket. The Remote  Read BW  tells you if\n\
cachelines are transfered between sockets, meaning that cores access data owned by a remote\n\
NUMA domain.\n\
\n\
"},

    {"TLB","Formulas:\n\
DTLB miss rate  = DTLB_MISSES_ANY / INSTR_RETIRED_ANY\n\
-\n\
The DTLB miss  rate gives a measure how often a TLB miss occured\n\
per instruction. \n\
\n\
"},

    {"VIEW","Formula:\n\
DP MFlops/s =  (FP_COMP_OPS_EXE_SSE_FP_PACKED*2 +  FP_COMP_OPS_EXE_SSE_FP_SCALAR)/ runtime\n\
Clock [MHz]  1.E-06*(CPU_CLK_UNHALTED_CORE/CPU_CLK_UNHALTED_REF)/inverseClock\n\
Packed MUOPS/s   1.0E-06*FP_COMP_OPS_EXE_SSE_FP_PACKED/time\n\
Scalar MUOPS/s 1.0E-06*FP_COMP_OPS_EXE_SSE_FP_SCALAR/time\n\
SP MUOPS/s 1.0E-06*FP_COMP_OPS_EXE_SSE_SINGLE_PRECISION/time\n\
DP MUOPS/s 1.0E-06*FP_COMP_OPS_EXE_SSE_DOUBLE_PRECISION/time\n\
Memory bandwidth [MBytes/s] 1.0E-06*(UNC_QMC_NORMAL_READS_ANY+UNC_QMC_WRITES_FULL_ANY)*64/time\n\
Remote Read BW [MBytes/s] 1.0E-06*(UNC_QHL_REQUESTS_REMOTE_READS)*64/time\n\
-\n\
This is a overview group using the acpabilities of westmere to measure multiple events at\n\
the same time.\n\
\n\
"},

};

