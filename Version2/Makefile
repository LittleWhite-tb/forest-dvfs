CXX=g++
CXXFLAGS=-O3 -Wall -Wextra -g
LD=$(CXX)
LD_FLAGS=
LIBS=-lpthread -lm

COMM_HDR=$(shell find Core/Communicator/ -type f -name '*.h')
COMM_SRC=$(shell find Core/Communicator/ -type f -name '*.cpp')
COMM_OBJ=$(addprefix obj/, $(patsubst %.cpp, %.o, $(COMM_SRC)))
COMM_INC_FLAG=$(addprefix -I, $(sort $(dir $(COMM_HDR))))

DECS_HDR=$(shell find Core/DecisionServer/ -type f -name '*.h')
DECS_SRC=$(shell find Core/DecisionServer/ -type f -name '*.cpp')
DECS_OBJ=$(addprefix obj/, $(patsubst %.cpp, %.o, $(DECS_SRC)))
DECS_INC_FLAG=$(addprefix -I, $(sort $(dir $(DECS_HDR))))

PROF_HDR=$(shell find Core/Profiler/ -type f -name '*.h')
PROF_SRC=$(shell find Core/Profiler/ -type f -name '*.cpp')
PROF_OBJ=$(addprefix obj/, $(patsubst %.cpp, %.o, $(PROF_SRC)))
PROF_INC_FLAG=$(addprefix -I, $(sort $(dir $(PROF_HDR))))

ALL_OBJ=$(COMM_OBJ) $(DECS_OBJ) $(PROF_OBJ)
ALL_INC_FLAGS=-I. $(COMM_INC_FLAG) $(DECS_INC_FLAG) $(PROF_INC_FLAG)


all: decisionServer profiler

decisionServer: $(COMM_OBJ) $(DECS_OBJ)
	$(LD) $(LD_FLAGS) $^ $(LIBS) -o decisionServer

profiler: $(COMM_OBJ) $(PROF_OBJ)
	$(LD) $(LD_FLAGS) $^ $(LIBS) -lpapi -o profiler

clean:
	@rm -rf obj

obj/%.d: %.cpp
	@mkdir -p $(@D)
	@$(CXX) -MM $(ALL_INC_FLAGS) $< > $@ 2> /dev/null

-include $(patsubst %.o, %.d, $(ALL_OBJ))

obj/%.o: %.cpp obj/%.d
	@echo '$(CXX) $(CXXFLAGS) $<'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(ALL_INC_FLAGS) -c $< -o $@
