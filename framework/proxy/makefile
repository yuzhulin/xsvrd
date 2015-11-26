CPP=g++
BIN=../../run/proxy/bin/proxy
OBJ=main.o handler.o x_thread.o proxy_control.o x_tcpsocket.o socket_api.o tcpconnection.o 
INC=../../include/libcommon
C_ARGS=-DXDBG

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(C_ARGS) $(OBJ) -o $(BIN) -lpthread

main.o:main.cc
	$(CPP) $(C_ARGS) -g -c main.cc

handler.o:handler.cc handler.h
	$(CPP) $(C_ARGS) -g -c handler.cc

x_thread.o:../libcommon/x_thread.cc $(INC)/x_thread.h
	$(CPP) $(C_ARGS) -g -c ../libcommon/x_thread.cc

proxy_control.o:proxy_control.cc proxy_control.h
	$(CPP) $(C_ARGS) -g -c proxy_control.cc

x_tcpsocket.o:../libcommon/x_tcpsocket.cc $(INC)/x_tcpsocket.h
	$(CPP) $(C_ARGS) -g -c ../libcommon/x_tcpsocket.cc

socket_api.o:../libcommon/socket_api.cc $(INC)/socket_api.h
	$(CPP) $(C_ARGS) -g -c ../libcommon/socket_api.cc

tcpconnection.o:../libcommon/tcpconnection.cc $(INC)/tcpconnection.h
	$(CPP) $(C_ARGS) -g -c ../libcommon/tcpconnection.cc

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
