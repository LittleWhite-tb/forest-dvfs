CXX=g++
CXXFLAGS=-O3 -g -Wall -Wextra -DARCH_SNB
LD=$(CXX)
LD_FLAGS=
LIBS=-lpthread -lm

LIBPFM_VER=4.2.0
LIBPFM_BIN=libpfm-$(LIBPFM_VER)/lib/libpfm.a
LIBPFM_INC=libpfm-$(LIBPFM_VER)/include/

LOCAL_HDR=$(shell find LocalRest/ -type f -name '*.h')
LOCAL_SRC=$(shell find LocalRest/ -type f -name '*.cpp')
LOCAL_OBJ=$(addprefix obj/, $(patsubst %.cpp, %.o, $(LOCAL_SRC)))
LOCAL_INC_FLAG=$(addprefix -I, $(sort $(dir $(LOCAL_HDR)))) -I$(LIBPFM_INC)


ALL_OBJ=$(LOCAL_OBJ)
ALL_INC_FLAGS=$(LOCAL_INC_FLAG)


all: localRest

localRest: $(LIBPFM_BIN) $(LOCAL_OBJ)
	$(LD) $(LD_FLAGS) $^ $(LIBS) $(LIBPFM_BIN) -o $@

$(LIBPFM_BIN):
	@$(MAKE) -C libpfm-$(LIBPFM_VER)

clean:
	@rm -rf obj localRest
	@$(MAKE) -C libpfm-$(LIBPFM_VER) clean

install: localRest
	@cp localRest /usr/bin
	
uninstall:
	@rm -f /usr/bin/localRest

obj/%.d: %.cpp
	@mkdir -p $(@D)
	@$(CXX) -MM $(ALL_INC_FLAGS) $< > $@ 2> /dev/null

-include $(patsubst %.o, %.d, $(ALL_OBJ))

obj/%.o: %.cpp obj/%.d
	@echo '$(CXX) $(CXXFLAGS) $<'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(ALL_INC_FLAGS) -c $< -o $@

