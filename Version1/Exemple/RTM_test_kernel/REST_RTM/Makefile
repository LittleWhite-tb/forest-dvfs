PREP= -lpower_instru -lpthread
FC=gfortran
CC = gcc
INCLUDE = -I.. -I../.. -I../../..  -I../../../..
FFLAGS  = -g -O3

PRODUCTIVITY=../productivity_link
PRODUCTIVITY_SOURCE_FILES=$(PRODUCTIVITY).c $(PRODUCTIVITY)_helper.c
PRODUCTIVITY_OBJECT_FILES=$(patsubst %.c,%.o,$(PRODUCTIVITY_SOURCE_FILES))

CFLAGS_PRODUCTIVITY=-m64 -g -O0 -msse -Wall -fPIC -std=gnu99 -pthread
DFLAGS_PRODUCTIVITY=-D__PL_FILESYSTEM_LESS__ -D__PL_FILESYSTEM_LESS_CONNECTED__ -D__PL_PROFILE__ -D__PL_PROFILE_USE_CONSTANT_FREQUENCY__ -D__PL_LINUX__ -D__PL_GENERATE_INI__ -D__PL_GENERATE_INI_VERSION_TAGGING__ -D__PL_GENERATE_INI_BUILD_TAGGING__ -D__PL_GENERATE_INI_DATE_AND_TIME_TAGGING__ -D__PL_BLOCKING_COUNTER_FILE_LOCK__ -D__PL_EXTRA_INPUT_CHECKS__ -D_DEBUG -D__PL_DEBUG__ -D_SVID_SOURCE -D_REENTRANT -D_LIBC_REENTRANT
LFLAGS_PRODUCTIVITY=-lpthread -luuid -ldl -lrt -lrest
LFLAGS = -L.. -L../.. -L../../.. -L../../../..

ALL     = test_kernel 


test_kernel: test_kernel.o RestFortranWrapper.o
	$(FC) $^ $(FFLAGS) $(INCLUDE) $(LFLAGS_PRODUCTIVITY)  -o $@ $(LFLAGS)

%.o: %.f90
	$(FC) $(FFLAGS) -c $^ $(INCLUDE) -o $@

%.o: %.c
	$(CC) -c $^ $(INCLUDE) $(CFLAGS) $(DFLAGS_PRODUCTIVITY) $(CFLAGS_PRODUCTIVITY) -o $@

clean:
	rm -rf *.o *.mod *.MOD core* *~ *.inst.* *.pdb $(ALL)
