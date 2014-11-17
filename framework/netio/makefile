CPP=g++
BIN=../../run/netio/bin/netio
OBJ=main.o

all:$(BIN)

$(BIN):$(OBJ)
	$(CPP) $(OBJ) -o $(BIN) 

main.o:main.cpp
	$(CPP) -c main.cpp

clean:
	rm -f $(BIN) $(OBJ)
