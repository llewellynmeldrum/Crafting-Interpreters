# testing subtree commits
SHELL:= /bin/zsh
C := gcc
CFLAGS := -Iinclude -Wall -Wextra -std=c2x

SRC_DIR := src
OBJ_DIR := build
TARGET := clox 

all: $(TARGET) 
COMP_BANNER:= "1. COMPILE SRC -> OBJ  > "
LINK_BANNER:= "2. LINKING OBJ -> BIN  > "
DELO_BANNER:= "3. DELETED .OBJ FILES  > " 
RUN_BANNER:=  "4. EXECUTING BINARY:" 

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# make binary
$(TARGET): $(OBJS) 
	@greenbanner $(COMP_BANNER) 
	@printf " "
	$(C) $(OBJS) -o $@
	@printf "\n"

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@yellowbanner $(LINK_BANNER) 
	@printf " "
	$(C) $(CFLAGS) -c $< -o $@
	@printf "\n"

# make dirs
$(OBJ_DIR):
	chalk bgWhiteBright red "CREATING DIRECTORIES"
	@mkdir -p $@
run: $(TARGET)
	@./$(TARGET)

clean:
	@chalk bgWhiteBright red "REMOVING EXECUTABLES AND OBJECT FILES"
	rm -rf $(OBJ_DIR)/*  $(TARGET)

.PHONY: clean
