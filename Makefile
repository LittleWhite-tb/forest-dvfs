#
# FoREST - Reactive DVFS Control for Multicore Processors
# Copyright (C) 2013 Universite de Versailles
# Copyright (C) 2011-2012 Exascale Research Center
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

CXX=g++
CXXFLAGS=-O3 -msse3 -Wall -Wextra -Werror -DREST_LOG -DARCH_SNB
CXXFLAGS_DEBUG=-g
LD=$(CXX)
LD_FLAGS=
LIBS=-lpthread -lrt
CXXFLAGS_RELEASE=-DNDEBUG

LIBPFM_VER=4.3.0
LIBPFM_BIN=libpfm-$(LIBPFM_VER)/lib/libpfm.a
LIBPFM_INC=libpfm-$(LIBPFM_VER)/include/

LOCAL_HDR=$(shell find LocalRest/ -type f -name '*.h')
LOCAL_SRC=$(shell find LocalRest/ -type f -name '*.cpp')
LOCAL_OBJ_RELEASE=$(addprefix obj/, $(patsubst %.cpp, %.o, $(LOCAL_SRC)))
LOCAL_OBJ_DEBUG=$(addprefix obj/, $(patsubst %.cpp, %_debug.o, $(LOCAL_SRC)))
LOCAL_INC_FLAG=$(addprefix -I, $(sort $(dir $(LOCAL_HDR)))) -I$(LIBPFM_INC)


ALL_OBJ=$(LOCAL_OBJ)
ALL_INC_FLAGS=$(LOCAL_INC_FLAG)

.PHONY: check-arch offline clean mrproper

all: check-arch offline debug

check-arch:
	@echo "=== Testing CPU architecture ==="
	make -C ./offline/archTester
	#./offline/archTester/archTester

offline:
	make -C ./offline/lPowerProbe/
	make -C ./offline
	make -C ./offline run
	cd ..

release: $(LIBPFM_BIN) $(LOCAL_OBJ_RELEASE)
	$(LD) $(LD_FLAGS) $^ $(LIBS) $(LIBPFM_BIN) -o localRest

debug: $(LIBPFM_BIN) $(LOCAL_OBJ_DEBUG)
	$(LD) $(LD_FLAGS) $^ $(LIBS) $(LIBPFM_BIN) $(CXXFLAGS_DEBUG) -o localRest

$(LIBPFM_BIN):
	@$(MAKE) -C libpfm-$(LIBPFM_VER)

clean:
	@rm -rf obj localRest
	@make -C ./offline/lPowerProbe/ clean
	@make -C ./offline clean
	@make -C ./offline/archTester clean

mrproper: clean
	@$(MAKE) -C libpfm-$(LIBPFM_VER) clean

obj/%.d: %.cpp
	@mkdir -p $(@D)
	@$(CXX) -MM $(ALL_INC_FLAGS) $< > $@ 2> /dev/null

-include $(patsubst %.o, %.d, $(ALL_OBJ))

obj/%_debug.o: %.cpp obj/%.d
	@echo '$(CXX) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $<'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $(ALL_INC_FLAGS) -c $< -o $@

obj/%.o: %.cpp obj/%.d
	@echo '$(CXX) $(CXXFLAGS) $(CXXFLAGS_RELEASE) $<'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(CXXFLAGS_RELEASE) $(ALL_INC_FLAGS) -c $< -o $@

