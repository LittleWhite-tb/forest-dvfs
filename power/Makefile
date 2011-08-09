OPT = -O3 -Wall -Wextra -g -DX86
CC=gcc


timer.so : %.so: %.c %.h
	$(CC) $< -o $@ -fPIC -shared $(OPT) -I. -D__PL_LINUX__ esrv/esrv_lib.so
