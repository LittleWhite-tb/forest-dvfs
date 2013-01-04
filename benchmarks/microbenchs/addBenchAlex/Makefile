NBCORES=4

all: sleep testcpu testcpu2 testcpu_mpi
	

sleep:
	@gcc -Wall -Wextra sleep.c -o $@

testcpu:
	@gcc -Wall -Wextra testcpu.c -o $@

testcpu2:
	@gcc -Wall -Wextra -O3 testcpu2.c -o $@

testcpu_mpi:
	@mpicc -Wall -Wextra testcpu_mpi.c -o $@.$(NBCORES).C

clean:
	rm sleep testcpu testcpu2 testcpu_mpi.$(NBCORES).C
