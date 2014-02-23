CXX=g++
CXXFLAGS=-Wall -O3 -std=c++11

SRC_PATH=./src
INC_PATH=./include
DEP_PATH=./dep
OBJ_PATH=./obj
LIB_PATH=./lib

SRCS=$(notdir $(wildcard $(SRC_PATH)/*.cpp))
OBJS=$(SRCS:.cpp=.o)
DEPS=$(SRCS:.cpp=.d)

LIB=esa
TARGET=lib$(LIB).a

vpath %.cpp $(SRC_PATH)
vpath %.hpp $(INC_PATH)
vpath %.o $(OBJ_PATH)
vpath %.a $(LIB_PATH)

all: $(TARGET) example

example: $(TARGET) example.o
	$(CXX) example/$@.cpp -I$(INC_PATH) -L$(LIB_PATH) -l$(LIB) -o example/$@ -std=c++11

example.o: example/example.cpp
	@mkdir -p $(OBJ_PATH)
	$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -c -o $(OBJ_PATH)/$@

$(TARGET): $(OBJS)
	@mkdir -p $(LIB_PATH)
	$(AR) rcs $(LIB_PATH)/$(TARGET) $(addprefix $(OBJ_PATH)/,$(OBJS))

%.o: %.cpp %.hpp
	@mkdir -p $(OBJ_PATH)
	$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -c -o $(OBJ_PATH)/$@

$(DEP_PATH)/%.d: %.cpp %.hpp
	@mkdir -p $(DEP_PATH)
	$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -MM > $@

.PHONY: clean
clean:
	$(RM) $(DEP_PATH)/*.d
	$(RM) $(OBJ_PATH)/*.o
	$(RM) $(LIB_PATH)/$(TARGET)
	$(RM) example/example

ifneq ($(MAKECMDGOALS),clean)
-include $(addprefix $(DEP_PATH)/,$(DEPS))
endif
