CXX=g++
CXXFLAGS=-Wall -O3 -std=c++11

SRC_PATH=./src
INC_PATH=./include
OBJ_PATH=./obj
LIB_PATH=./lib

OBJS=conv.o trie.o seg.o
LIB=esa
TARGET=lib$(LIB).a

vpath %.cpp $(SRC_PATH)
vpath %.hpp $(INC_PATH)
vpath %.o $(OBJ_PATH)
vpath %.a $(LIB_PATH)

all: $(TARGET) example

example: example.o
	$(CXX) example/$@.cpp -I$(INC_PATH) -L$(LIB_PATH) -l$(LIB) -o example/$@ -std=c++11

example.o: example/example.cpp
	@mkdir -p $(OBJ_PATH)
	$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -c -o $(OBJ_PATH)/$@

$(TARGET): $(OBJS)
	@mkdir -p $(LIB_PATH)
	$(AR) rcs $(LIB_PATH)/$(TARGET) $(OBJ_PATH)/*.o

%.o: %.cpp %.hpp
	@mkdir -p $(OBJ_PATH)
	$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -c -o $(OBJ_PATH)/$@

.PHONY: clean
clean:
	$(RM) $(OBJ_PATH)/*.o
	$(RM) $(LIB_PATH)/$(TARGET)
	$(RM) example/example
