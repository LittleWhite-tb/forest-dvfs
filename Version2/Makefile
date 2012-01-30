CXX=g++
CXXFLAGS=-O3 -Wall -Wextra -g
LD=$(CXX)
LD_FLAGS=
LIBS=-lpthread -lm

COMM_HDR=$(shell find Core/Communicator/ -type f -name '*.h')
COMM_SRC=$(shell find Core/Communicator/ -type f -name '*.cpp')
COMM_OBJ=$(addprefix obj/, $(patsubst %.cpp, %_pdc.o, $(COMM_SRC)))
COMM_INC_FLAG=$(addprefix -I, $(sort $(dir $(COMM_HDR))))

COMMPIC_OBJ=$(addprefix obj/, $(patsubst %.cpp, %_pic.o, $(COMM_SRC)))

DECS_HDR=$(shell find Core/DecisionServer/ -type f -name '*.h')
DECS_SRC=$(shell find Core/DecisionServer/ -type f -name '*.cpp')
DECS_OBJ=$(addprefix obj/, $(patsubst %.cpp, %_pdc.o, $(DECS_SRC)))
DECS_INC_FLAG=$(addprefix -I, $(sort $(dir $(DECS_HDR))))

PROF_HDR=$(shell find Core/Profiler/ -type f -name '*.h')
PROF_SRC=$(shell find Core/Profiler/ -type f -name '*.cpp')
PROF_OBJ=$(addprefix obj/, $(patsubst %.cpp, %_pic.o, $(PROF_SRC)))
PROF_INC_FLAG=$(addprefix -I, $(sort $(dir $(PROF_HDR))))

LOG_HDR=$(shell find Core/Log/ -type f -name '*.h')
LOG_SRC=$(shell find Core/Log/ -type f -name '*.cpp')
LOG_OBJ=$(addprefix obj/, $(patsubst %.cpp, %_pdc.o, $(LOG_SRC)))
LOG_INC_FLAG=$(addprefix -I, $(sort $(dir $(LOG_HDR))))

LOGPIC_OBJ=$(addprefix obj/, $(patsubst %.cpp, %_pic.o, $(LOG_SRC)))


ALL_OBJ=$(COMM_OBJ) $(COMMPIC_OBJ) $(DECS_OBJ) $(PROF_OBJ) $(LOG_OBJ) $(LOGPIC_OBJ)
ALL_INC_FLAGS=-I. $(COMM_INC_FLAG) $(DECS_INC_FLAG) $(PROF_INC_FLAG) $(LOG_INC_FLAG)


all: decisionServer rest_prof.so

decisionServer: $(COMM_OBJ) $(DECS_OBJ) $(LOG_OBJ)
	$(LD) $(LD_FLAGS) $^ $(LIBS) -o decisionServer

rest_prof.so: $(COMMPIC_OBJ) $(PROF_OBJ) $(LOGPIC_OBJ)
	$(LD) $(LD_FLAGS) -shared $^ $(LIBS) -lpapi -ldl -o rest_prof.so

clean:
	@rm -rf obj decisionServer rest_prof.so

install: rest_prof.so
	@cp rest_prof.so /usr/lib

obj/%.d: %.cpp
	@mkdir -p $(@D)
	@$(CXX) -MM $(ALL_INC_FLAGS) $< > $@ 2> /dev/null

-include $(patsubst %.o, %.d, $(ALL_OBJ))

obj/%_pdc.o: %.cpp obj/%.d
	@echo '$(CXX) $(CXXFLAGS) $<'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(ALL_INC_FLAGS) -c $< -o $@

obj/%_pic.o: %.cpp obj/%.d
	@echo '$(CXX) $(CXXFLAGS) -fPIC $<'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -fPIC $(ALL_INC_FLAGS) -c $< -o $@
