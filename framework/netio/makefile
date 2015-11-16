CPP=g++
BIN=../../run/netio/bin/netio
OBJ=main.o

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(OBJ) -o $(BIN) 

main.o:main.cc
	$(CPP) -g -c main.cc

socket_api.o:socket_api.cc ../../include/libcommon/socket_api.h
	$(CPP) -g -c main.cc

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
