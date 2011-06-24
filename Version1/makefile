EXE=rest

MAIN_OBJ := $(patsubst %.c,obj/%.o,$(wildcard *.c))
PROFILER_OBJ := $(patsubst Profiler/Src/%.c,obj/%.o,$(wildcard Profiler/Src/*.c))
DECISION_OBJ := $(patsubst Decision/Src/%.c,obj/%.o,$(wildcard Decision/Src/*.c))
FREQCHANGE_OBJ := $(patsubst FreqChange/Src/%.c,obj/%.o,$(wildcard FreqChange/Src/*.c))
LIBS = -lpthread -lpapi

OPT_INCLUDE = -IProfiler/Include -IDecision/Include -IFreqChange/Include -I/usr/local/include
OPT = -O3 -Wall -Wextra -g $(OPT_INCLUDE) 

FULL_OBJ = $(MAIN_OBJ) $(PROFILER_OBJ) $(DECISION_OBJ) $(FREQCHANGE_OBJ)

all: $(EXE)

$(EXE):	$(FULL_OBJ)
	gcc -o $(EXE) $(FULL_OBJ) $(OPT) $(LIBS)

$(PROFILER_OBJ):obj/%.o: Profiler/Src/%.c Profiler/Include/%.h 
	gcc -c $< -o $@ $(OPT)$(LIBS)

$(DECISION_OBJ):obj/%.o: Decision/Src/%.c Decision/Include/%.h 
	gcc -c $< -o $@ $(OPT) $(LIBS)

$(FREQCHANGE_OBJ):obj/%.o: FreqChange/Src/%.c FreqChange/Include/%.h 
	gcc -c $< -o $@ $(OPT) $(LIBS)

$(MAIN_OBJ):obj/%.o: %.c %.h
	gcc -c $< -o $@ $(OPT) $(LIBS)

clean:
	rm $(OBJ) $(EXE) obj/*o tmp Log.txt doxygen_warn `find . -name "*~"` output/* *out 2> /dev/null || echo "Done cleaning"

count:
	wc `find . -name "*\.c"|grep -v utils` `find . -name "*\.h"|grep -v utils` 

countfiles:
	@echo "c: " `find . -name "*c" |wc -l`; \
    echo "Headers:" `find . -name "*h" | wc -l`;\
    echo "Txt:" `find data -name "*txt" |wc -l`
    
