CC = clang++

# Set:
# 	* All standard warnings enable
# 	* All warnings are errors
# 	* Extra warnings
# 	* Set standard to c++20
# 	* set the include directory to ./include
# 	* compile the file without linking.
CXXFLAGS = -Wall -Werror -Wextra -std=c++2a -Iinclude -c
SRC = src/main.cpp src/matrix.cpp src/stat_util.cpp src/linear_model.cpp

OBJ_DIR = obj
BIN_DIR = bin
SRC_DIR = src
INC_DIR = include

PROG = prog

all: $(OBJ_DIR) $(BIN_DIR) $(BIN_DIR)/$(PROG)

$(OBJ_DIR):
	mkdir obj

$(BIN_DIR):
	mkdir bin

$(BIN_DIR)/$(PROG): obj/main.o obj/matrix.o obj/stat_util.o obj/linear_model.o obj/tests.o obj/LinearRegression.o
	$(CC) $^ -o $@
	./bin/prog

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CC) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/main.o: src/main.cpp include/tests.h
	$(CC) $(CXXFLAGS) $< -o $@

# Now need to have management of build include dependencies.

clean:
	rm -rf obj
	rm -rf bin
