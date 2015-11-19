CPP=g++
BIN=../../run/agent/bin/agent
OBJ=main.o handlerthread.o x_thread.o maincontrol.o
INC=../../include/libcommon

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(OBJ) -o $(BIN) 

main.o:main.cc
	$(CPP) -g -c main.cc

handlerthread.o:handlerthread.cc handlerthread.h
	$(CPP) -g -c handlerthread.h

x_thread.o:../libcommon/x_thread.cc $(INC)/x_thread.h
	$(CPP) -g -c ../libcommon/x_thread.cc

maincontrol.o:maincontrol.cc maincontrol.h
	$(CPP) -g -c maincontrol.h

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
