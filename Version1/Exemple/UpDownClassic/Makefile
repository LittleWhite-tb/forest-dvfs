CC=gcc

MAIN_OBJ := $(patsubst Src/%.c,obj/%.o,$(wildcard Src/*.c))
EXE := $(patsubst Src/%.c,%,$(wildcard Src/*.c))
LIBS = -L../.. -lpthread -lpapi -ldl -lm -lrest


OPT_INCLUDE = -I../../ -I./Include -I../../FreqChange/Include/ -I../../Profiler/Include/ -I../../Decision/Include -I../../Common/Include/
OPT_FLAGS = -DX86 -g -O3

all: $(EXE)

$(EXE): $(MAIN_OBJ)
	$(CC)  $< -o $@ $(OPT_INCLUDE) $(CFLAGS) $(OPT_FLAGS) $(LIBS)

$(MAIN_OBJ):obj/%.o: Src/%.c Include/%.h
	$(CC) -c $< -o $@ $(OPT_INCLUDE) $(CFLAGS) $(OPT_FLAGS) $(LIBS)


clean:
	rm $(OBJ) $(EXE) obj/*o tmp Log.txt doxygen_warn `find . -name "*~"` output/* *out 2> /dev/null || echo "Done cleaning"

