CC = gcc
CPP = g++
COMMON = ./

BUILD_DIR=../..

HEADFILE = -I$(BUILD_DIR)/include -I$(BUILD_DIR)/include/base -I$(BUILD_DIR)/include/interface
CFLAGS = -g $(HEADFILE) -fpic
OBJ = logger.o

#LDFLAGS= ../../../../../framework/lib/ -lcommon
OI_LIB = $(BUILD_DIR)/libs/libcommon.a

BIN = $(BUILD_DIR)/libs/liblog.so

all : $(BIN)

$(BIN) : $(OBJ)
	rm -f $@
	$(CPP) -o $@ $(CFLAGS) $(OBJ) $(OI_LIB)   -shared

logger.o : logger.cc logger.h
	$(CPP)  -c -o $@ $(CFLAGS) logger.cc

clean:
	rm -f $(BIN) $(OBJ)



