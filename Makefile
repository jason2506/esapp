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
vpath %.hpp $(INC_PATH)
vpath %.o $(OBJ_PATH)
vpath %.a $(LIB_PATH)

all: $(TARGET) $(EXAMPLE)

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
	$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -c -o $(OBJ_PATH)/$@

$(DEP_PATH)/%.d: %.cpp %.hpp
	@mkdir -p $(DEP_PATH)
	@$(CXX) $< -I$(INC_PATH) $(CXXFLAGS) -MM > $@

.PHONY: clean
clean:
	$(RM) $(DEP_PATH)/*.d
	$(RM) $(OBJ_PATH)/*.o
	$(RM) $(LIB_PATH)/$(TARGET)
	$(RM) $(EX_PATH)/$(EXAMPLE)

ifneq ($(MAKECMDGOALS),clean)
-include $(addprefix $(DEP_PATH)/,$(DEPS))
endif
