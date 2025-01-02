###############################################################################
# User settings
###############################################################################
CXX				 = clang++

SYSTEMC_HOME	 = $(shell echo $$SYSTEMC_HOME)

# CXXFLAGS		 = -pedantic-errors -Wall -Wextra -Werror
CXXFLAGS		 = -std=c++17 -pthread
LDFLAGS			 = -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm -pthread
BUILD			 = ./build
OBJ_DIR			 = $(BUILD)/objects
APP_DIR			 = $(BUILD)/apps
TARGET			 = program
INCLUDE			 = -I./src -I$(SYSTEMC_HOME)/include
SRC				 = $(wildcard src/*.cpp test/*.cpp)

OBJECTS			 = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES	 = $(OBJECTS:.o=.d)

###############################################################################
# Rules
###############################################################################
all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
