#
# FoREST - Reactive DVFS Control for Multicore Processors
# Copyright (C) 2013 Universite de Versailles
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CC=gcc
CFLAGS=-O3 -march=native -Wextra -Wall
LDFLAGS=

.PHONY: run clean

all:
	$(CC) $(CFLAGS) $(LDFLAGS) add.c -o ./add

run:
	@echo "=== Generate config files for FoREST ==="
	echo "A prompt (sudo) will ask you, your admin password to run modprobe and our CPU tester"
	sudo modprobe msr
	sudo python ./usefulFreqs.py 0

clean:
	rm -rf ./add