CXX := g++
CXXFLAGS := -Wall -std=c++11

BUILD := build
BIN_DIR := $(BUILD)/bin
OBJ_DIR := $(BUILD)/obj

CPT := cpt
CPT_INCLUDE := -I$(CPT)/include
CPT_SRC := $(wildcard $(CPT)/src/*.cpp)
CPT_OBJ := $(CPT_SRC:%.cpp=$(OBJ_DIR)/%.o)

CPT_LIB := $(BIN_DIR)/lib$(CPT).a

CPTC := cptc
CPTC_SRC := cpt.cpp
CPTC_APP := $(BIN_DIR)/$(CPTC)

all: build $(CPTC)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPT_INCLUDE) -o $@ -c $<

$(CPT_LIB): $(CPT_OBJ)
	@mkdir -p $(@D)
	ar rcs $(CPT_LIB) $^

$(CPT): $(CPT_LIB)

$(CPTC): $(CPTC_APP)

$(CPTC_APP): $(CPT_LIB) $(CPTC_SRC)
	$(CXX) $(CXXFLAGS) $(CPT_INCLUDE) $(CPTC_SRC) -L$(BIN_DIR) -l$(CPT) -o $@

.PHONY: all build clean $(CPT) $(CPTC)

build:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rvf $(OBJ_DIR)/*
	@rm -rvf $(BIN_DIR)/*

