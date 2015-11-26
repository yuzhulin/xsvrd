CPP=g++
BIN=../../run/mainsvrd/bin/mainsvrd
OBJ=main.o mainsvrd_control.o 
INC=../../include/libcommon
C_ARGS=-DXDBG

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(C_ARGS) $(OBJ) -o $(BIN) -lpthread

main.o:main.cc
	$(CPP) $(C_ARGS) -g -c main.cc

mainsvrd_control.o:mainsvrd_control.cc mainsvrd_control.h
	$(CPP) $(C_ARGS) -g -c mainsvrd_control.cc

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
