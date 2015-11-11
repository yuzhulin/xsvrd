CPP=g++
BIN=../../run/netio/bin/netio
OBJ=main.o

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(OBJ) -o $(BIN) 

main.o:main.cc
	$(CPP) -c main.cc

clean:
	rm -f $(BIN) $(OBJ)
