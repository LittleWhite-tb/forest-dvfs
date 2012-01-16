EXE=decisionServer profiler rest
CC=g++

MAIN_OBJ := $(patsubst %.cpp,obj/%.o,$(wildcard *.cpp))
CORE_OBJ := $(patsubst Core/Src/%.cpp,obj/%.o,$(wildcard Core/Src/*.cpp))
LIBS = -lpthread

PROFILER_OBJ :=$(patsubst Core/Profiler/Src/%.cpp,obj/%.o,$(wildcard Core/Profiler/Src/*.cpp))
PROFILER_THREADEDPROFILER_PROFSERVER_OBJ := $(patsubst Core/Profiler/ProfServer/Src/%.cpp,obj/%.o,$(wildcard Core/Profiler/ProfServer/Src/*.cpp))
PROFILER_THREADEDPROFILER_PAPI_OBJ := $(patsubst Core/Profiler/ThreadedProfiler/Papi/Src/%.cpp,obj/%.o,$(wildcard Core/Profiler/ThreadedProfiler/Papi/Src/*.cpp))
PROFILER_THREADEDPROFILER_LIKWID_OBJ := $(patsubst Core/Profiler/ThreadedProfiler/Likwid/Src/%.cpp,obj/%.o,$(wildcard Core/Profiler/ThreadedProfiler/Likwid/Src/*.cpp))

DECISIONSERVER_OBJ := $(patsubst Core/DecisionServer/Src/%.cpp,obj/%.o,$(wildcard Core/DecisionServer/Src/*.cpp))
DECISIONSERVER_DECISION_CORESINFOS_OBJ := $(patsubst Core/DecisionServer/CoresInfos/Src/%.cpp,obj/%.o,$(wildcard Core/DecisionServer/CoresInfos/Src/*.cpp))
DECISIONSERVER_DECISION_OBJ := $(patsubst Core/DecisionServer/DecisionMaker/Src/%.cpp,obj/%.o,$(wildcard Core/DecisionServer/DecisionMaker/Src/*.cpp))

DECISIONSERVER_DECISION_PREDICTIVE_OBJ := $(patsubst Core/DecisionServer/DecisionMaker/Predictive/Src/%.cpp,obj/%.o,$(wildcard DecisionServer/Core/DecisionMaker/Predictive/Src/*.cpp))
DECISIONSERVER_DECISION_NAIVE_OBJ := $(patsubst Core/DecisionServer/DecisionMaker/NaiveDecisions/Src/%.cpp,obj/%.o,$(wildcard Core/DecisionServer/DecisionMaker/NaiveDecisions/Src/*.cpp))
DECISIONSERVER_DECISION_MARKOV_OBJ := $(patsubst Core/DecisionServer/DecisionMaker/Markov/Src/%.cpp,obj/%.o,$(wildcard Core/DecisionServer/Decision/Markov/Src/*.cpp))
DECISIONSERVER_FREQCHANGER_OBJ := $(patsubst Core/DecisionServer/FreqChanger/Src/%.cpp,obj/%.o,$(wildcard Core/DecisionServer/FreqChanger/Src/*.cpp))

COMMUNICATOR_OBJ := $(patsubst Core/Communicator/Src/%.cpp,obj/%.o,$(wildcard Core/Communicator/Src/*.cpp))
COMMUNICATOR_MSGREADER_OBJ := $(patsubst Core/Communicator/MsgReader/Src/%.cpp,obj/%.o,$(wildcard Core/Communicator/MsgReader/Src/*.cpp))
COMMUNICATOR_MESSAGE_OBJ := $(patsubst Core/Communicator/Message/Src/%.cpp,obj/%.o,$(wildcard Core/Communicator/Message/Src/*.cpp))
COMMUNICATOR_YELLOWPAGES_OBJ := $(patsubst Core/Communicator/YellowPages/Src/%.cpp,obj/%.o,$(wildcard Core/Communicator/YellowPages/Src/*.cpp))

LIBS =

OPT_INCLUDE_PROFILER = -ICore/Profiler/Include -ICore/Profiler/ThreadedProfiler/ProfServer/Include -ICore/Profiler/ThreadedProfiler/Include -ICore/Profiler/ThreadedProfiler/Papi/Include -ICore/Profiler/ThreadedProfiler/Likwid/Include 

OPT_INCLUDE_DECISION = -ICore/DecisionServer/Include -ICore/DecisionServer/CoresInfos/Include -ICore/DecisionServer/FreqChanger/Include -ICore/DecisionServer/DecisionMaker/Include -ICore/DecisionServer/DecisionMaker/Predictive/Include -ICore/DecisionServer/DecisionMaker/NaiveDecisions/Include -ICore/DecisionServer/DecisionMaker/Markov/Include

OPT_INCLUDE_COMMUNICATOR = -ICore/Communicator/Include -ICore/Communicator/MsgReader/Include -ICore/Communicator/YellowPages/Include -ICore/Communicator/Message/Include

OPT_INCLUDE = $(OPT_INCLUDE_DECISION) $(OPT_INCLUDE_PROFILER) $(OPT_INCLUDE_COMMUNICATOR)

OPT_ALL = -fPIC -O3 -Wall -Wextra -lm -g $(OPT_INCLUDE) -DX86
OPT_DECISION = -fPIC -O3 -Wall -Wextra -lm -g $(OPT_INCLUDE_DECISION) -DX86
OPT_PROFILER = -fPIC -O3 -Wall -Wextra -lm -g $(OPT_INCLUDE_PROFILER) -DX86
OPT_COMMUNICATOR = -fPIC -O3 -Wall -Wextra -lm -g $(OPT_INCLUDE_COMMUNICATOR) -DX86

ALLDECISION_OBJ=$(DECISIONSERVER_OBJ) $(DECISIONSERVER_DECISION_OBJ) $(DECISIONSERVER_DECISION_CORESINFOS_OBJ)  $(DECISIONSERVER_DECISION_NAIVE_OBJ) $(DECISIONSERVER_DECISION_MARKOV_OBJ) $(DECISIONSERVER_DECISION_PREDICTIVE_OBJ) $(DECISIONSERVER_FREQCHANGER_OBJ)
ALLCOMMUNICATOR_OBJ=$(COMMUNICATOR_OBJ) $(COMMUNICATOR_MSGREADER_OBJ) $(COMMUNICATOR_YELLOWPAGES_OBJ) $(COMMUICATOR_MESSAGE)
ALLPROFILER_OBJ=$(PROFILER_OBJ) $(PROFILER_PROFSERVER_OBJ) $(PROFILER_THREADEDPROFILER_OBJ) 
FULL_OBJ=$(MAIN_OBJ) $(ALLPROFILER_OBJ) $(ALLDECISION_OBJ) $(ALLCOMMUNICATOR)

all: $(EXE)

decisionServer:	$(ALLDECISION_OBJ)
	$(CC) $(ALLDECISION_OBJ) $(OPT_DECISION) $(LIBS) -o $@ 
	
profiler:	$(ALLPROFILER_OBJ)
	$(CC) $(ALLPROFILER_OBJ) $(OPT_PROFILER) $(LIBS) -o $@ 

rest:	$(REST_OBJ)
	$(CC) $(REST_OBJ) $(OPT_ALL) $(LIBS) -o $@
	
$(PROFILER_OBJ):obj/%.o: Core/Profiler/Src/%.cpp Core/Profiler/Include/%.h
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)
	
$(PROFILER_PROFSERVER_OBJ):obj/%.o: Core/Profiler/ProfServer/Src/%.cpp Core/Profiler/ProfServer/Include/%.h
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)

$(PROFILER_THREADEDPROFILER_PAPI_OBJ):obj/%.o: Core/Profiler/ThreadedProfiler/Papi/Src/%.cpp Core/Profiler/ThreadedProfiler/Papi/Include/%.h
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)

$(PROFILER_THREADEDPROFILER_LIKWID_OBJ):obj/%.o: Core/Profiler/ThreadedProfiler/Likwid/Src/%.cpp Core/Profiler/ThreadedProfiler/Likwid/Include/%.h
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)
	
$(DECISIONSERVER_DECISION_OBJ):obj/%.o: Core/DecisionServer/DecisionMaker/Src/%.cpp Core/DecisionServer/DecisionMaker/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)
	
$(DECISIONSERVER_DECISION_CORESINFOS_OBJ):obj/%.o: Core/DecisionServer/CoresInfos/Src/%.cpp Core/DecisionServer/CoresInfos/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)

$(DECISIONSERVER_DECISION_NAIVE_OBJ):obj/%.o: Core/DecisionServer/DecisionMaker/NaiveDecisions/Src/%.cpp Core/DecisionServer/DecisionMaker/NaiveDecisions/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)
	
$(DECISIONSERVER_DECISION_MARKOV_OBJ):obj/%.o: Core/DecisionServer/DecisionMaker/Markov/Src/%.cpp Core/DecisionServer/DecisionMaker/Markov/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)

$(DECISIONSERVER_DECISION_PREDICTIVE_OBJ):obj/%.o: Core/DecisionServer/DecisionMaker/Predictive/Src/%.cpp Core/DecisionServer/DecisionMaker/Predictive/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)

$(DECISIONSERVER_FREQCHANGER_OBJ):obj/%.o: Core/DecisionServer/FreqChanger/Src/%.cpp Core/DecisionServer/FreqChanger/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)

$(DECISIONSERVER_OBJ):obj/%.o: Core/DecisionServer/Src/%.cpp Core/DecisionServer/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)
	
$(COMMUNICATOR_OBJ):obj/%.o: Core/Communicator/Src/%.cpp Core/Communicator/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_DECISION) $(CFLAGS) $(LIBS)

$(COMMUNICATOR_MSGREADER_OBJ):obj/%.o: Core/Communicator/MsgReader/Src/%.cpp Core/Communicator/MsgReader/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_DECISION) $(CFLAGS) $(LIBS)
	
$(COMMUNICATOR_MESSAGE_OBJ):obj/%.o: Core/Communicator/Message/Src/%.cpp Core/Communicator/Message/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_DECISION) $(CFLAGS) $(LIBS)
	
$(COMMUNICATOR_YELLOWPAGES_OBJ):obj/%.o: Core/Communicator/YellowPages/Src/%.cpp Core/Communicator/YellowPages/Include/%.h 
	$(CC) -c $< -o $@ $(OPT_COMMUNICATOR) $(CFLAGS) $(LIBS)

$(REST_OBJ):obj/%.o: %.cpp %.h
	$(CC) -c $< -o $@ $(OPT_ALL) $(CFLAGS) $(LIBS)


clean:
	@rm -f $(OBJ) $(EXE) obj/*o `find . -name "*~"` || echo "Done cleaning"
	
cleandoc:
	@rm -R doc/* || echo "Done cleaning"

count:
	wc `find . -name "*\.cpp"|grep -v utils` `find . -name "*\.h"|grep -v utils`

countfiles:
	@echo "CPP: " `find . -name "*cpp" |wc -l`; \
    echo "Headers:" `find . -name "*h" | wc -l`;\
    echo "Txt:" `find data -name "*txt" |wc -l`
    
