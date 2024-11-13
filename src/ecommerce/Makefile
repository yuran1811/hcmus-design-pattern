CXX 		= g++
CXXFLAGS 	= -Wall -Wno-unused-variable -Wno-narrowing -Wno-enum-compare -Wno-sign-compare -Wl,--subsystem,windows

BIN			= bin
INCLUDE		= include
SRC			= src

SRCS 		= $(wildcard $(INCLUDE)/*.cpp $(SRC)/core/*.cpp $(SRC)/components/*.cpp $(SRC)/shared/*.cpp $(SRC)/utils/*.cpp $(SRC)/*.cpp)
OBJS 		= $(SRCS:.cpp=.o)
TARGET		= main

RAYLIB_VER	= -5.0
RAYLIB_INC 	= -I../raylib$(RAYLIB_VER)/src -I../raylib$(RAYLIB_VER)/src/external -I../raylib$(RAYLIB_VER)/src/extras
RAYLIB_LIB	= -L../raylib$(RAYLIB_VER)/src -lraylib
LDFLAGS 	= -L$(SRC) ../raylib$(RAYLIB_VER)/src/raylib.rc.data -static -lopengl32 -lgdi32 -lwinmm -lpthread -DPLATFORM_DESKTOP

$(BIN)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o ./$(BIN)/$(TARGET) $(RAYLIB_INC) $(RAYLIB_LIB) $(LDFLAGS) -I$(INCLUDE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(RAYLIB_INC) -I$(INCLUDE) -c $< -o $@

all: $(BIN)/$(TARGET)

exec:
	./$(BIN)/$(TARGET)

clean:
	rm -f ./$(BIN)/$(TARGET) $(OBJS)

run: all
	make exec

.PHONY: all exec clean run