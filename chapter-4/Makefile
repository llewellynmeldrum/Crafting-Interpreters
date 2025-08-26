SHELL:= /bin/zsh
CXX := g++
CXXFLAGS := -Iinclude -Wall -Wextra -std=c++17

SRC_DIR := src
OBJ_DIR := build
TARGET := main.out

all: $(TARGET) 
COMP_BANNER:= "1. COMPILE SRC -> OBJ  > "
LINK_BANNER:= "2. LINKING OBJ -> BIN  > "
DELO_BANNER:= "3. DELETED .OBJ FILES  > " 
RUN_BANNER:=  "4. EXECUTING BINARY:" 

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# make binary
$(TARGET): $(OBJS) 
	@greenbanner $(COMP_BANNER) 
	@printf " "
	$(CXX) $(OBJS) -o $@
	@printf "\n"

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@yellowbanner $(LINK_BANNER) 
	@printf " "
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "\n"

# make dirs
$(OBJ_DIR):
	chalk bgWhiteBright red "CREATING DIRECTORIES"
	@mkdir -p $@

clean:
	@chalk bgWhiteBright red "REMOVING EXECUTABLES AND OBJECT FILES"
	rm -rf $(OBJ_DIR)/*  $(TARGET)

.PHONY: clean
