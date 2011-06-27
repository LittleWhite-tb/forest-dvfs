EXE = Microbench1


LIBS = -ldl
CC = gcc
OPT = -O3 -Wall -Wextra -g -DX86

TIMER_LIB = ../timer/timer.so
POWER_LIB = ../power/timer.so


all: $(TIMER_LIB) $(POWER_LIB) $(EXE)

$(EXE):	$(FULL_OBJ)
	$(CC) -o $(EXE) Microbench1.c $(OPT) $(LIBS)

$(TIMER_LIB):%.so: %.c %.h
	$(CC) $< -o $@ -fPIC -shared $(OPT) -I.
	
$(POWER_LIB):%.so: %.c %.h
	$(CC) $< -o $@ -fPIC -shared $(OPT) -I. -D__PL_LINUX__ ../power/esrv/esrv_lib.so

clean:
	rm $(POWER_LIB) $(TIMER_LIB) $(FULL_OBJ) $(EXE) tmp Log.txt `find . -name "*~"` 2> /dev/null || echo "Done cleaning"

count:
	wc `find . -name "*\.c"` `find . -name "*\.h"`

countfiles:
	@echo "CPP: " `find . -name "*c" |wc -l`; \
    echo "Headers:" `find . -name "*h" | wc -l`;\
    echo "Txt:" `find data -name "*txt" |wc -l`
