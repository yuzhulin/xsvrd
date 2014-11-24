CC = gcc
CPP = g++
COMMON = ./

BUILD_DIR=../../..

HEADFILE = -I$(BUILD_DIR)/include/base -I$(BUILD_DIR)/include/interface
CFLAGS = -g $(HEADFILE) -D_LOGITEM_ -fpic
OBJ = configfile.o 

BIN = $(BUILD_DIR)/dll/configfile.so

all : $(BIN)

$(BIN) : $(OBJ)
	rm -f $@
	$(CPP) -o $@ $(CFLAGS) $(OBJ) -shared

configfile.o : configfile.cpp configfile.h
	$(CPP)  -c -o $@ $(CFLAGS) configfile.cpp


clean:
	rm -f $(BIN) $(OBJ)

