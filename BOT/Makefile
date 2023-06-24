# MY MAKE FILE!!

# color setting
Color_Off = \033[0m		# Text Reset
Black = \033[0;30m		# Black
Green = \033[0;31m		# Green
Green = \033[0;32m		# Green
Yellow = \033[0;33m		# Yellow
Blue = \033[0;34m		# Blue
Purple = \033[0;35m		# Purple
Cyan = \033[0;36m		# Cyan
White = \033[0;37m		# White

# **************************************************************************** #

# Variables

NAME		= bot
DBNAME		= $(NAME)_debug

SRC_DIR		= src
INC_DIR		= include
OBJ_DIR		= obj
TEST_DIR	= test
BIN_DIR		= bin

CXX			= c++
W_FLAGS		= -Wall -Wextra -Werror
# W_FLAGS		= 
VER_FLAGS	= -std=c++98
# get all directory in src 
INC_FLAGS	= $(addprefix -I, $(shell find $(SRC_DIR) -type d)) -I $(INC_DIR)
CXXFLAGS	= -c $(W_FLAGS) $(VER_FLAGS) $(INC_FLAGS)
LDFLAGS		= 
DBGFLAGS	= -g3 $(VER_FLAGS) $(INC_FLAGS)
TESTFLAGS	= $(W_FLAGS) $(VER_FLAGS) $(INC_FLAGS) -g3

# SRCS should include all cpp files in src dircetory and exclude all *test.cpp files
SRCS	= $(shell find $(SRC_DIR) -name "*.cpp" ! -name "*Test.cpp")
OBJS	= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# **************************************************************************** #

# Rules

# Main target
all : make_directories $(BIN_DIR)/$(NAME)

# Compilation
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Linking 
$(BIN_DIR)/$(NAME) : $(OBJS)
	$(CXX) -o $@ $^
	@echo "$(Blue)\n[ Compile finished!! ]\n$(Color_Off)"

make_directories : 
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(BIN_DIR)

debug : $(BIN_DIR)/$(DBNAME)

$(BIN_DIR)/$(DBNAME) : $(SRCS)
	@mkdir -p $(@D)
	$(CXX) $(DBGFLAGS) -o $(BIN_DIR)/$(DBNAME) $^
	@echo "$(Blue)\n[ LETS START DEBUGGING! ]\n$(Color_Off)"

re : fclean all

# 'make run' will recompile and execute executable file with given arguments
#   add arguments if you need
BIN_ARGS = 

run : re
	./$(BIN_DIR)/$(NAME) $(BIN_ARGS)

# 'make test' will compile excluding main.cpp
#   and execute executable file with given arguments
#   add arguments if you need
TEST_ARGS = 
TEST_SRCS = $(shell find $(SRC_DIR) -name "*.cpp" ! -name "main.cpp")
TEST_BIN = $(BIN_DIR)/$(NAME)_test

test : fclean $(TEST_BIN)
	./$(TEST_BIN) $(TEST_ARGS)

$(TEST_BIN) : $(TEST_SRCS)
	@mkdir -p $(@D)
	$(CXX) $(TESTFLAGS) -o $(TEST_BIN) $^
	@echo "$(Blue)\n[ LETS START TESTING! ]\n$(Color_Off)"

.PHONY : all make_directories clean fclean debug re run test
