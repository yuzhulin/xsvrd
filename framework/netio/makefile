CPP=g++
BIN=../../run/netio/bin/netio
OBJ=main.o socket_api.o
INC=../../include/libcommon

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(OBJ) -o $(BIN) 

main.o:main.cc
	$(CPP) -g -c main.cc

socket_api.o:../libcommon/socket_api.cc $(INC)/socket_api.h
	$(CPP) -g -c ../libcommon/socket_api.cc

x_thread.o:../libcommon/x_thread.cc $(INC)/x_thread.h
	$(CPP) -g -c ../libcommon/x_thread.cc

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
