CXX=g++
CXXFLAGS=-O3 -msse3 -Wall -Wextra -Werror -DARCH_SNB -DREST_LOG
CXXFLAGS_DEBUG=-g
LD=$(CXX)
LD_FLAGS=
LIBS=-lpthread -lrt -lm
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


all: debug

release: $(LIBPFM_BIN) $(LOCAL_OBJ_RELEASE)
	$(LD) $(LD_FLAGS) $^ $(LIBS) $(LIBPFM_BIN) -o localRest

debug: $(LIBPFM_BIN) $(LOCAL_OBJ_DEBUG)
	$(LD) $(LD_FLAGS) $^ $(LIBS) $(LIBPFM_BIN) $(CXXFLAGS_DEBUG) -o localRest

$(LIBPFM_BIN):
	@$(MAKE) -C libpfm-$(LIBPFM_VER)

clean:
	@rm -rf obj localRest

mrproper: clean
	@$(MAKE) -C libpfm-$(LIBPFM_VER) clean

install: localRest
	@cp localRest /usr/bin
	
uninstall:
	@rm -f /usr/bin/localRest

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

