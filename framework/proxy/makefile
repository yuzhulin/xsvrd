CPP=g++
BIN=../../run/proxy/bin/proxy
OBJ=main.o handlerthread.o x_thread.o controller.o
INC=../../include/libcommon

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(OBJ) -o $(BIN) -lpthread 

main.o:main.cc
	$(CPP) -g -c main.cc

handlerthread.o:handlerthread.cc handlerthread.h
	$(CPP) -g -c handlerthread.cc

x_thread.o:../libcommon/x_thread.cc $(INC)/x_thread.h
	$(CPP) -g -c ../libcommon/x_thread.cc

controller.o:controller.cc controller.h
	$(CPP) -g -c controller.cc

clean:
	rm -f $(BIN) $(OBJ)

run:
	$(BIN)
