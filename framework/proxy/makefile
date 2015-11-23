CPP=g++
BIN=../../run/proxy/bin/proxy
OBJ=main.o handlerthread.o x_thread.o controller.o x_tcpsocket.o socket_api.o
INC=../../include/libcommon
C_ARGS=-DXDBG

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(C_ARGS) $(OBJ) -o $(BIN) -lpthread

main.o:main.cc
	$(CPP) $(C_ARGS) -g -c main.cc

handlerthread.o:handlerthread.cc handlerthread.h
	$(CPP) $(C_ARGS) -g -c handlerthread.cc

x_thread.o:../libcommon/x_thread.cc $(INC)/x_thread.h
	$(CPP) $(C_ARGS) -g -c ../libcommon/x_thread.cc

controller.o:controller.cc controller.h
	$(CPP) $(C_ARGS) -g -c controller.cc

x_tcpsocket.o:../libcommon/x_tcpsocket.cc $(INC)/x_tcpsocket.h
	$(CPP) $(C_ARGS) -g -c ../libcommon/x_tcpsocket.cc

socket_api.o:../libcommon/socket_api.cc $(INC)/socket_api.h
	$(CPP) $(C_ARGS) -g -c ../libcommon/socket_api.cc

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
