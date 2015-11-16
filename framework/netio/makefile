CPP=g++
BIN=../../run/netio/bin/netio
OBJ=main.o socket_api.o

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(OBJ) -o $(BIN) 

main.o:main.cc
	$(CPP) -g -c main.cc

socket_api.o:../libcommon/socket_api.cc ../../include/libcommon/socket_api.h
	$(CPP) -g -c socket_api.cc

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
