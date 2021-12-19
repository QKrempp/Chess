CXX		= g++
LD		= g++
CXXFLAG		= -O3 -W -Wall -ansi -pedantic -std=c++11
PROG_NAME	= Chess.xpp

SRC_DIR		= ./src
BUILD_DIR	= ./src
BIN_DIR		= ./
SRC_LIST	= $(wildcard $(SRC_DIR)/*.cpp)
OBJ_LIST	= $(addprefix $(BUILD_DIR)/, $(notdir $(SRC_LIST:.cpp=.o)))

WIN_CXX		= x86_64-w64-mingw32-g++
WIN_LD		= x86_64-w64-mingw32-g++
WIN_DIR		= ./win
WIN_PROG	= Chess.exe
WIN_OBJ		= $(addprefix $(WIN_DIR)/, $(notdir $(SRC_LIST:.cpp=.o)))

.PHONY: all clean

all: $(PROG_NAME) $(WIN_PROG)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) -c $(CXXFLAG) $^ -o $@

$(PROG_NAME) : $(OBJ_LIST)
	$(LD) $^ -o $(BIN_DIR)/$@

$(WIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(WIN_CXX) -c $(CXXFLAGS) $^ -o $@

$(WIN_PROG) : $(WIN_OBJ)
	$(WIN_LD) $^ -o $(WIN_DIR)/$@

clean:
	rm -f $(BUILD_DIR)/*.o $(WIN_DIR)/*.o

mrproper: clean
	rm -f $(BIN_DIR)/$(PROG_NAME) $(WIN_DIR)/$(WIN_PROG)
#
# test:
# 	@echo $(OBJ_LIST) $(WIN_OBJ) $(SRC_LIST)
