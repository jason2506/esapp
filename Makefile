CXX=g++
CXXFLAGS=-Wall -O3 -std=c++11

SRC_PATH=./src
INC_PATH=./include
DEP_PATH=./dep
OBJ_PATH=./obj
LIB_PATH=./lib
EX_PATH=./example

SRCS=$(notdir $(wildcard $(SRC_PATH)/*.cpp))
OBJS=$(SRCS:.cpp=.o)
DEPS=$(SRCS:.cpp=.d)

LIB=esapp
TARGET=lib$(LIB).a
EXAMPLE=example

vpath %.cpp $(SRC_PATH)
vpath %.hpp $(SRC_PATH)
vpath %.o $(OBJ_PATH)
vpath %.a $(LIB_PATH)

all: build $(EXAMPLE)

build: $(TARGET) $(SRC_PATH)/esapp.hpp
	@mkdir -p $(INC_PATH)
	@cp $(SRC_PATH)/esapp.hpp $(INC_PATH)/

$(EXAMPLE): $(TARGET) $(EXAMPLE).o
	$(CXX) $(EX_PATH)/$@.cpp -I$(INC_PATH) -L$(LIB_PATH) -l$(LIB) $(CXXFLAGS) -o $(EX_PATH)/$@

$(EXAMPLE).o: $(EX_PATH)/$(EXAMPLE).cpp
	@mkdir -p $(OBJ_PATH)
	$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -c -o $(OBJ_PATH)/$@

$(TARGET): $(OBJS)
	@mkdir -p $(LIB_PATH)
	$(AR) rcs $(LIB_PATH)/$(TARGET) $(addprefix $(OBJ_PATH)/,$(OBJS))

%.o: %.cpp %.hpp
	@mkdir -p $(OBJ_PATH)
	$(CXX) $< $(CXXFLAGS) -c -o $(OBJ_PATH)/$@

$(DEP_PATH)/%.d: %.cpp %.hpp
	@mkdir -p $(DEP_PATH)
	@$(CXX) $< $(CXXFLAGS) -MM > $@

.PHONY: build clean
clean:
	$(RM) -r $(DEP_PATH)
	$(RM) -r $(OBJ_PATH)
	$(RM) -r $(LIB_PATH)
	$(RM) -r $(INC_PATH)
	$(RM) $(EX_PATH)/$(EXAMPLE)

ifneq ($(MAKECMDGOALS),clean)
-include $(addprefix $(DEP_PATH)/,$(DEPS))
endif
