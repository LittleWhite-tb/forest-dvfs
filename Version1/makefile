EXE=rest
STATICLIB=rest.a
DYNAMICLIB=librest.so
CC=gcc

MAIN_OBJ := $(patsubst %.c,obj/%.o,$(wildcard *.c))
PROFILER_OBJ := $(patsubst Profiler/Src/%.c,obj/%.o,$(wildcard Profiler/Src/*.c))
COMMON_OBJ := $(patsubst Common/Src/%.c,obj/%.o,$(wildcard Common/Src/*.c))
DECISION_OBJ := $(patsubst Decision/Src/%.c,obj/%.o,$(wildcard Decision/Src/*.c))
FREQCHANGE_OBJ := $(patsubst FreqChange/Src/%.c,obj/%.o,$(wildcard FreqChange/Src/*.c))
LIBS = -lpthread -lpapi -ldl

OPT_INCLUDE = -IProfiler/Include -IDecision/Include -IFreqChange/Include -I/usr/local/include -ICommon/Include
OPT = -fPIC -O3 -Wall -Wextra -lm -g $(OPT_INCLUDE) -DX86

FULL_OBJ = $(MAIN_OBJ) $(PROFILER_OBJ) $(DECISION_OBJ) $(FREQCHANGE_OBJ) $(COMMON_OBJ)

all: $(EXE)
staticlib : $(PROFILER_OBJ) $(DECISION_OBJ) $(FREQCHANGE_OBJ) $(COMMON_OBJ)
	ar -rv $(STATICLIB) $(PROFILER_OBJ) $(DECISION_OBJ) $(FREQCHANGE_OBJ) $(COMMON_OBJ)
dynamiclib : $(PROFILER_OBJ) $(DECISION_OBJ) $(FREQCHANGE_OBJ) $(COMMON_OBJ)
	gcc -o $(DYNAMICLIB) -shared $(PROFILER_OBJ) $(DECISION_OBJ) $(FREQCHANGE_OBJ) $(COMMON_OBJ) $(OPT) $(CFLAGS) $(LIBS)

$(EXE):	$(FULL_OBJ)
	$(CC) -o $(EXE) $(FULL_OBJ) $(OPT) $(CFLAGS) $(LIBS)

$(PROFILER_OBJ):obj/%.o: Profiler/Src/%.c Profiler/Include/%.h 
	$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(COMMON_OBJ):obj/%.o: Common/Src/%.c Common/Include/%.h 
	$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(DECISION_OBJ):obj/%.o: Decision/Src/%.c Decision/Include/%.h 
	$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(FREQCHANGE_OBJ):obj/%.o: FreqChange/Src/%.c FreqChange/Include/%.h 
	$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

$(MAIN_OBJ):obj/%.o: %.c %.h
	$(CC) -c $< -o $@ $(OPT) $(CFLAGS) $(LIBS)

clean:
	rm $(OBJ) $(EXE) obj/*o tmp Log.txt doxygen_warn `find . -name "*~"` output/* *out 2> /dev/null || echo "Done cleaning"

count:
	wc `find . -name "*\.c"|grep -v utils` `find . -name "*\.h"|grep -v utils` 

countfiles:
	@echo "c: " `find . -name "*c" |wc -l`; \
    echo "Headers:" `find . -name "*h" | wc -l`;\
    echo "Txt:" `find data -name "*txt" |wc -l`
