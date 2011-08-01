CC=gcc

MAIN_OBJ := $(patsubst Src/%.c,obj/%.o,$(wildcard Src/*.c))
EXE := $(patsubst Src/%.c,%,$(wildcard Src/*.c))
LIBS = -L.. -lpthread -lpapi -ldl -lrest

OPT_INCLUDE = -I../ -I./Include -I../FreqChange/Include/ -I../Profiler/Include/ -I../Common/Include/
CFLAGS = -DX86 -g

all: $(EXE)

$(EXE): $(MAIN_OBJ)
	$(CC)  $< -o $@ $(OPT_INCLUDE) $(CFLAGS) $(LIBS)

$(MAIN_OBJ):obj/%.o: Src/%.c Include/%.h
	$(CC) -c $< -o $@ $(OPT_INCLUDE) $(CFLAGS) $(LIBS)


clean:
	rm $(OBJ) $(EXE) obj/*o tmp Log.txt doxygen_warn `find . -name "*~"` output/* *out 2> /dev/null || echo "Done cleaning"

