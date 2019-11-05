CXX := g++
CXXFLAGS := -Wall -std=c++11 -O3

BUILD := build
BIN_DIR := $(BUILD)/bin
OBJ_DIR := $(BUILD)/obj

CPT := cpt
CPT_INCLUDE := -I$(CPT)/include
CPT_HEADERS := $(wildcard $(CPT)/include/cpt/*.hpp)
CPT_SRC := $(wildcard $(CPT)/src/*.cpp)
CPT_OBJ := $(CPT_SRC:%.cpp=$(OBJ_DIR)/%.o)

CPT_LIB := $(BIN_DIR)/lib$(CPT).a

CPTC := cptc
CPTC_INCLUDE := $(CPT_INCLUDE) -I$(CPTC)/include -I$(CPTC)/deps/CLI11/include
CPTC_HEADERS := $(wildcard $(CPTC)/include/cptc/*.hpp)
CPTC_SRC := $(wildcard $(CPTC)/src/*.cpp)
CPTC_APP := $(BIN_DIR)/$(CPTC)

all: build $(CPTC)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CPT_INCLUDE) -o $@ -c $<

$(CPT_LIB): $(CPT_OBJ) $(CPT_HEADERS)
	@mkdir -p $(@D)
	ar rcs $(CPT_LIB) $^

$(CPT): $(CPT_LIB)

$(CPTC): $(CPTC_APP)

$(CPTC_APP): $(CPT_LIB) $(CPTC_SRC) $(CPTC_HEADERS)
	$(CXX) $(CXXFLAGS) $(CPTC_INCLUDE) $(CPTC_SRC) -L$(BIN_DIR) -l$(CPT) -o $@

.PHONY: all build clean debug release $(CPT) $(CPTC)

build:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	@rm -rvf $(OBJ_DIR)/*
	@rm -rvf $(BIN_DIR)/*

