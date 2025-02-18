CXX 		= g++
CXXFLAGS 	= -O2 -mavx -march=x86-64-v3 -Wall -Wno-narrowing -Wno-enum-compare -Wno-sign-compare -Wl,--subsystem,windows

BIN			= bin
INCLUDE		= include
SRC			= src

SRCS 		= $(wildcard $(INCLUDE)/*.cpp $(SRC)/components/*.cpp $(SRC)/core/*.cpp $(SRC)/gui/*.cpp $(SRC)/shared/*.cpp $(SRC)/utils/*.cpp)
SRCS_MAIN 	= $(SRCS) $(SRC)/main.cpp
SRCS_TEST 	= $(SRCS) $(SRC)/main.test.cpp
OBJS_MAIN 	= $(SRCS_MAIN:.cpp=.o)
OBJS_TEST 	= $(SRCS_TEST:.cpp=.o)
DEPFILES 	= $(SRCS_MAIN:.cpp=.d) $(SRCS_TEST:.cpp=.d)
TARGET		= main
TARGET_TEST	= main_test

RAYLIB_VER	= -5.0
RAYLIB_INC 	= -I../raylib$(RAYLIB_VER)/src -I../raylib$(RAYLIB_VER)/src/external -I../raylib$(RAYLIB_VER)/src/extras
RAYLIB_LIB	= -L../raylib$(RAYLIB_VER)/src -lraylib
LDFLAGS 	= -L$(SRC) ../raylib$(RAYLIB_VER)/src/raylib.rc.data -static -lopengl32 -lgdi32 -lwinmm -lpthread -DPLATFORM_DESKTOP

# Targets
$(BIN)/$(TARGET): $(OBJS_MAIN)
	$(CXX) $(CXXFLAGS) $(OBJS_MAIN) -o ./$(BIN)/$(TARGET) $(RAYLIB_INC) $(RAYLIB_LIB) $(LDFLAGS) -I$(INCLUDE)

$(BIN)/$(TARGET_TEST): $(OBJS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_TEST) -o ./$(BIN)/$(TARGET_TEST) $(RAYLIB_INC) $(RAYLIB_LIB) $(LDFLAGS) -I$(INCLUDE)

# Pattern rules for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP $(RAYLIB_INC) -I$(INCLUDE) -c $< -o $@

# Include dependency files
-include $(DEPFILES)

all: $(BIN)/$(TARGET)

exec:
	./$(BIN)/$(TARGET)

clean:
	rm -f ./$(BIN)/$(TARGET)* ./src/**/*.o $(DEPFILES)

cleanall: clean
	rm -f $(OBJS_MAIN) $(OBJS_TEST)

run: all
	make exec

test: $(BIN)/$(TARGET_TEST)
	./$(BIN)/$(TARGET_TEST)

pack: all
	rm -rf ./release
	mkdir ./release
	cp -r ./bin/* ./public/ ./release/
	zip -9 -r ecommerce.zip . -i "release/*"

.PHONY: all exec clean cleanall run test pack
