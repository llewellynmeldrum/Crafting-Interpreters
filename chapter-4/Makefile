.PHONY: all clean run

DONTRUN?=
PROG:=tokenizer
CXX:=g++
CXXFLAGS:=-std=c++17
SRC:=main.cpp
OBJS:=$(SRC:.cpp=.o)
all: run 

# build executable (linking)
$(PROG): $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROG)
	rm -f $(OBJS)

#compile .cpp into .o (compilation proper)
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(PROG)
ifdef ARG
	@echo "RUNNING FILE MODE:"
	@bat $(ARG)
endif
	./$(PROG) $(ARG)

clean: 
	rm -f $(PROG) $(OBJS)
