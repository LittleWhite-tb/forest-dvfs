FC = mpiifort -O3 -xHost -ftz
CC = mpicc
#FC = mpif90 
#CC = gcc
CFLAGS	= -g -O2 -DX86
INCLUDES = -I/opt/rest_modifications/Version1 -I/opt/rest_modifications/power/
LFLAGS =  -lrest -ldl 

ALL = fd_iso

fd_iso: test_mpi.o parser.o m_var_kernel.o m_fd_iso_3d_lx4_opt.o m_fd_iso_3d_comm.o test_kernel.o
	$(FC) $(INCLUDES) $(LFLAGS) $^ -o $@

%.o: %.f90 
	$(FC) $(INCLUDES) -c $^ -o $@

parser.o: parser.F90
	$(FC) -c -o $@ $<

test_mpi.o: test_mpi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $^  -o $@

clean:
	rm -rf *.o *.mod $(ALL)
