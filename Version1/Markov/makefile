#  This code builds a framework that helps model a program's behaviour. It is programmed
#  in such a way that is should seem to the user as being a black-box. Not a lot of functions
#  are needed to be able to make it run.

#  For any problem of installation or usage, please email me at: beyler@dpt-info.u-strasbg.fr

#  Copyright (C) 2006 Beyler Jean Christophe

#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#Simple makefile

CC = gcc
OPT = 	-O2 -Wall
EX_DIR = example
TYPE = _struct
SRC = simple
NBR = 1
MARKOV_VER = Markov$(TYPE).o
EXEC = test

SRC_F = $(EX_DIR)/$(SRC)$(TYPE)$(NBR).o

$(EXEC): $(MARKOV_VER) $(MARKOV_VER) $(SRC_F) 
	$(CC) $(OPT) -o $@ $(MARKOV_VER) $(SRC_F) 

$(MARKOV_VER): $(MARKOV_VER:%o=%c) $(MARKOV_VER:%o=%h) 
	$(CC) $(OPT) -c $<

$(SRC_F): $(SRC_F:%o=%c)
	$(CC) $(OPT) -c $< -o $@
 
clean:
	rm -f *.o *dot $(EX_DIR)/*.o $(EXEC)
