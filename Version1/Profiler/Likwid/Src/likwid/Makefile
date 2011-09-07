# This is libperfmon Makefile <jeanphilippe.halimi@gmail.com>
# Exascale Computing Research Center 2011

CFLAGS=-Wall -Wextra -O3 -std=c99 -fPIC
INCLUDES=-Iincludes/
CLIBS=
DEFINES=-DMAX_NUM_THREADS=128 -DMAX_NUM_SOCKETS=4 -D_GNU_SOURCE -DMSR_DIRECT
CC=gcc
SRC=perfmon.c asciiTable.c bstrlib.c cpuid.c msr.c strUtil.c affinity.c numa.c tree.c timer.c
OBJ=$(SRC:.c=.o)
STATICLIB=libperfmon.a

all:$(STATICLIB)

$(STATICLIB): $(SRC)
	gcc $^ -c $(CFLAGS) $(INCLUDES) $(CLIBS) $(DEFINES)
	ar rcs $@ $(OBJ)
	rm -rf $(OBJ)

clean:
	rm -rf $(OBJ)
	rm -rf $(STATICLIB)
