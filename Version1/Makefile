STATICLIB=rest.a
DYNAMICLIB=librest.so
CC=gcc



MAIN_OBJ := $(patsubst %.c,obj/%.o,$(wildcard *.c))
THREADED_PROFILER_OBJ := $(patsubst Profiler/ThreadProfiler/Src/%.c,obj/%.o,$(wildcard Profiler/ThreadProfiler/Src/*.c))
PAPI_COUNTERS_OBJ := $(patsubst Profiler/Papi/Src/%.c,obj/%.o,$(wildcard Profiler/Papi/Src/*.c))
COMMON_OBJ := $(patsubst Common/Src/%.c,obj/%.o,$(wildcard Common/Src/*.c))
MARKOV_OBJ := $(patsubst Markov/Src/%.c,obj/%.o,$(wildcard Markov/Src/*.c))
DECISION_PREDICTIVE_OBJ := $(patsubst Decision/PredictiveDM/Src/%.c,obj/%.o,$(wildcard Decision/PredictiveDM/Src/*.c))
DECISION_NAIVE_OBJ := $(patsubst Decision/NaiveDM/Src/%.c,obj/%.o,$(wildcard Decision/NaiveDM/Src/*.c))
DECISION_MARKOV_OBJ := $(patsubst Decision/MarkovDM/Src/%.c,obj/%.o,$(wildcard Decision/MarkovDM/Src/*.c))
FREQCHANGE_OBJ := $(patsubst FreqChange/Src/%.c,obj/%.o,$(wildcard FreqChange/Src/*.c))

DECISION_OBJ = $(DECISION_NAIVE_OBJ) $(DECISION_MARKOV_OBJ) $(DECISION_PREDICTIVE_OBJ)
PROFILER_OBJ = $(PAPI_COUNTERS_OBJ) $(THREADED_PROFILER_OBJ) 
FULL_OBJ = $(MAIN_OBJ) $(PROFILER_OBJ) $(DECISION_OBJ) $(FREQCHANGE_OBJ) $(COMMON_OBJ) $(MARKOV_OBJ)


LIBS = -lpthread -lpapi -ldl
REST_INCLUDE = -IProfiler/Include -IProfiler/VmadProfiler/Include -IProfiler/ThreadProfiler/Include -IProfiler/Papi/Include -IDecision/Include -IDecision/PredictiveDM/Include -IDecision/NaiveDM/Include -IDecision/MarkovDM/Include -IFreqChange/Include -I/usr/local/include -ICommon/Include -IMarkov/Include -I.

VMAD_INCLUDE = -I../../vmad/VMAD_version_under_development/camus_vm/include -I../../vmad/VMAD_version_under_development/camus_vm/ressource/ -I../../vmad/VMAD_version_under_development/camus_vm/modules/
OPT_INCLUDE = $(REST_INCLUDE) $(VMAD_INCLUDE)
OPT = -fPIC -O3 -Wall -Wextra -lm -g $(OPT_INCLUDE) -DX86


all: staticlib dynamiclib librest_vmad_bootstrap librest_vmad_decision

staticlib : $(FULL_OBJ)
	@ar -rv $(STATICLIB) $(FULL_OBJ)
dynamiclib : $(FULL_OBJ)
	@gcc -o $(DYNAMICLIB) -shared $(FULL_OBJ) $(OPT) $(CFLAGS) $(LIBS)
	
librest_vmad_bootstrap: rest_vmad_bootstrap
	@gcc -o librest_vmad_bootstrap.so -shared obj/rest_vmad_bootstrap.o $(OPT) $(CFLAGS) $(LIBS)

librest_vmad_decision: rest_vmad_decision
	@gcc -o librest_vmad_decision.so -shared obj/rest_vmad_decision.o $(OPT) $(CFLAGS) $(LIBS)

rest_vmad_bootstrap: Profiler/VmadProfiler/Src/rest_vmad.c Profiler/VmadProfiler/Include/rest_vmad.h
	@$(CC) -c $< -o obj/rest_vmad_bootstrap.o $(OPT) $(CFLAGS) $(LIBS)
	
rest_vmad_decision: Profiler/VmadProfiler/Src/rest_decision.c Profiler/VmadProfiler/Include/rest_decision.h
	@$(CC) -c $< -o obj/rest_vmad_decision.o $(OPT) $(CFLAGS) $(LIBS)

$(VMAD_PROFILER_OBJ):obj/%.o: Profiler/VmadProfiler/Src/%.c Profiler/VmadProfiler/Include/%.h 
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(THREADED_PROFILER_OBJ):obj/%.o: Profiler/ThreadProfiler/Src/%.c Profiler/ThreadProfiler/Include/%.h
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(PAPI_COUNTERS_OBJ):obj/%.o: Profiler/Papi/Src/%.c Profiler/Papi/Include/%.h
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(COMMON_OBJ):obj/%.o: Common/Src/%.c Common/Include/%.h 
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(DECISION_NAIVE_OBJ):obj/%.o: Decision/NaiveDM/Src/%.c Decision/NaiveDM/Include/%.h 
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)
	
$(DECISION_MARKOV_OBJ):obj/%.o: Decision/MarkovDM/Src/%.c Decision/MarkovDM/Include/%.h 
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(DECISION_PREDICTIVE_OBJ):obj/%.o: Decision/PredictiveDM/Src/%.c Decision/PredictiveDM/Include/%.h 
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(FREQCHANGE_OBJ):obj/%.o: FreqChange/Src/%.c FreqChange/Include/%.h 
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(MARKOV_OBJ):obj/%.o: Markov/Src/%.c Markov/Include/%.h 
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(MAIN_OBJ):obj/%.o: %.c %.h
	@$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

clean:
	@rm $(OBJ) $(EXE) obj/*o tmp Log.txt doxygen_warn librest.so rest.a `find . -name "*~"` output/* *out 2> /dev/null || echo "Done cleaning"

count:
	wc `find . -name "*\.c"|grep -v utils` `find . -name "*\.h"|grep -v utils` 

countfiles:
	@echo "c: " `find . -name "*c" |wc -l`; \
    echo "Headers:" `find . -name "*h" | wc -l`;\
    echo "Txt:" `find data -name "*txt" |wc -l`

