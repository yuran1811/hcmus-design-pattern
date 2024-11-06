CXX 		= g++
CXXFLAGS 	= -Wall -Wno-narrowing -Wno-enum-compare -Wno-sign-compare -Wl,--subsystem,windows

BIN			= bin
INCLUDE		= include
SRC			= src

SRCS 		= $(wildcard $(SRC)/core/*.cpp $(SRC)/components/*.cpp $(SRC)/interface/*.cpp $(SRC)/handler/*.cpp $(SRC)/shared/*.cpp $(SRC)/utils/*.cpp $(SRC)/*.cpp)
TARGET		= main

RAYLIB_VER	= -5.0
RAYLIB_INC 	= -I../raylib$(RAYLIB_VER)/src -I../raylib$(RAYLIB_VER)/src/external -I../raylib$(RAYLIB_VER)/src/extras
RAYLIB_LIB	= -L../raylib$(RAYLIB_VER)/src -lraylib
LDFLAGS 	= -L$(SRC) ../raylib$(RAYLIB_VER)/src/raylib.rc.data -static -lopengl32 -lgdi32 -lwinmm -lpthread -DPLATFORM_DESKTOP

$(BIN)/$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o ./$(BIN)/$(TARGET) $(RAYLIB_INC) $(RAYLIB_LIB) $(LDFLAGS) -I$(INCLUDE)

all: $(BIN)/$(TARGET)

exec:
	./$(BIN)/$(TARGET)

clean:
	rm -f ./$(BIN)/$(TARGET)

run: all
	make exec

.PHONY: all exec clean run