CXX 		= g++
CXXFLAGS 	= -O2 -mavx -march=x86-64-v3 -Wall -Wno-narrowing -Wno-enum-compare -Wno-sign-compare -Wl,--subsystem,windows

BIN			= bin
INCLUDE		= include
SRC			= src

SRCS 		= $(wildcard $(INCLUDE)/*.cpp $(SRC)/components/*.cpp $(SRC)/core/*.cpp $(SRC)/shared/*.cpp $(SRC)/utils/*.cpp)
SRCS_MAIN 	= $(SRCS) $(SRC)/main.cpp
SRCS_TEST 	= $(SRCS) $(SRC)/main.test.cpp
OBJS_MAIN 	= $(SRCS_MAIN:.cpp=.o)
OBJS_TEST 	= $(SRCS_TEST:.cpp=.o)
TARGET		= main

RAYLIB_VER	= -5.0
RAYLIB_INC 	= -I../raylib$(RAYLIB_VER)/src -I../raylib$(RAYLIB_VER)/src/external -I../raylib$(RAYLIB_VER)/src/extras
RAYLIB_LIB	= -L../raylib$(RAYLIB_VER)/src -lraylib
LDFLAGS 	= -L$(SRC) ../raylib$(RAYLIB_VER)/src/raylib.rc.data -static -lopengl32 -lgdi32 -lwinmm -lpthread -DPLATFORM_DESKTOP

$(BIN)/$(TARGET): $(OBJS_MAIN)
	$(CXX) $(CXXFLAGS) $(OBJS_MAIN) -o ./$(BIN)/$(TARGET) $(RAYLIB_INC) $(RAYLIB_LIB) $(LDFLAGS) -I$(INCLUDE)

$(BIN)/$(TARGET)_test: $(OBJS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_TEST) -o ./$(BIN)/$(TARGET)_test $(RAYLIB_INC) $(RAYLIB_LIB) $(LDFLAGS) -I$(INCLUDE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -I$(INCLUDE) -c $< -o $@

all: $(BIN)/$(TARGET)

exec:
	./$(BIN)/$(TARGET)

clean:
	rm -f ./$(BIN)/$(TARGET) ./$(BIN)/$(TARGET)_test $(OBJS_MAIN) $(OBJS_TEST)

run: all
	make exec

test: $(BIN)/$(TARGET)_test
	./$(BIN)/$(TARGET)_test

.PHONY: all exec clean run test