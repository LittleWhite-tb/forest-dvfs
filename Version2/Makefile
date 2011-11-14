EXE=rest

MAIN_OBJ := $(patsubst %.cpp,obj/%.o,$(wildcard *.cpp))
CORE_OBJ := $(patsubst Core/Src/%.cpp,obj/%.o,$(wildcard Core/Src/*.cpp))
LIBS =``

OPT_INCLUDE = -ICore/Include
OPT = -O3 -Wall -Wextra -g $(OPT_INCLUDE) 

FULL_OBJ = $(MAIN_OBJ) $(CORE_OBJ)

all: $(EXE)

$(EXE):	$(FULL_OBJ)
	g++ -o $(EXE) $(FULL_OBJ) $(OPT) $(LIBS) -Wl,-E

$(CORE_OBJ):obj/%.o: Core/Src/%.cpp Core/Include/%.h 
	g++ -c $< -o $@ $(OPT)$(LIBS)

$(MAIN_OBJ):obj/%.o: %.cpp %.h
	g++ -c $< -o $@ $(OPT) $(LIBS)

clean:
	@rm $(OBJ) $(EXE) obj/*o `find . -name "*~"` `rm -R doc/*` || echo "Done cleaning"
count:
	wc `find . -name "*\.cpp"|grep -v utils` `find . -name "*\.h"|grep -v utils` 

countfiles:
	@echo "CPP: " `find . -name "*cpp" |wc -l`; \
    echo "Headers:" `find . -name "*h" | wc -l`;\
    echo "Txt:" `find data -name "*txt" |wc -l`
    
