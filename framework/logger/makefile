CC = gcc
CPP = g++
COMMON = ./

BUILD_DIR=../..

HEADFILE = -I$(BUILD_DIR)/include/base -I$(BUILD_DIR)/include/interface
CFLAGS = -g $(HEADFILE) -fpic
OBJ = logfile.o

#LDFLAGS= ../../../../../framework/lib/ -lcommon
OI_LIB = $(BUILD_DIR)/lib/libcommon.a

BIN = $(BUILD_DIR)/dll/logfile.so

all : $(BIN)

$(BIN) : $(OBJ)
	rm -f $@
	$(CPP) -o $@ $(CFLAGS) $(OBJ) $(OI_LIB)   -shared

logfile.o : logfile.cpp logfile.h
	$(CPP)  -c -o $@ $(CFLAGS) logfile.cpp

clean:
	rm -f $(BIN) $(OBJ)



