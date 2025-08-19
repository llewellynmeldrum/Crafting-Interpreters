.PHONY: all clean run

SHELL:= /bin/zsh
DONTRUN?=
PROG:=tokenizer
CXX:=g++
CXXFLAGS:=-std=c++17
SRC:=main.cpp
OBJS:=$(SRC:.cpp=.o)
all: run 

COMP_BANNER:= "1. COMPILE SRC -> OBJ > "
LINK_BANNER:= "2. LINKING OBJ -> BIN > "
OREM_BANNER:= "3. CLEANED .O FILES   > " 


# build executable (linking)
$(PROG): $(OBJS) 
	@pbanner $(LINK_BANNER) 
	@printf " "
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROG)
	@printf "\n"

	@pbanner $(OREM_BANNER) 
	@printf " "
	rm -f $(OBJS)
	@printf "\n"

#compile .cpp into .o (compilation proper)
%.o : %.cpp
	@pbanner $(COMP_BANNER) 
	@printf " "
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "\n"

run: $(PROG)
ifdef ARG
	@echo "RUNNING FILE MODE:"
	@bat -A $(ARG) 
endif
	./$(PROG) $(ARG)

clean: 
	chalk bgWhiteBright red ">REMOVING EXECUTABLES AND OBJECT FILES<"
	rm -f $(PROG) $(OBJS)
