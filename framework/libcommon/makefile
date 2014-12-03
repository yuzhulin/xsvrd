# Uncomment the following line for Solaris # pony add "-lm" 990221
# C_LINK = -lsocket -lnsl -lm

# Uncomment this for SCO.  (Note, this has only been reported to work with
# Revision 3.2.4 with the "SCO TCP/IP Development System" package installed.
# Please let me know if you have any other SCO success stories.
# C_LINK = -lsocket

# mysql flags
NCURSES= 
DBINC = -I/usr/local/mysql/include/mysql -I/usr/include/mysql
DBLIB =
DEBUG= -g

BUILD_DIR=../../..

COMM_DIR =../lib
OI_LIB = 
#BINARY = testlib
SVROBJS = base.o codequeue.o lock.o objmng.o queue.o shm.o tcpsocket.o tcpconn.o thread.o tlib_cfg.o tlib_log.o sockstream.o stringmng.o
#PACKAGE = QQT_RESSVRD_INS_DEBUG_`date +%Y%m%d`.tgz
PACKAGE = QQT_RESSVRD_INS_`date +%Y%m%d`.tgz

# Comment the following line if you are not using the gnu c compiler
# C_ARGS = -Wall

C_ARGS = -I. -I$(BUILD_DIR)/include/base -I$(BUILD_DIR)/include/interface $(DBINC)  -g -shared -fPIC
#C_ARGS = -I. -O3 -g

# You might have to change this if your c compiler is not cc
CC = cc
CPP = g++



INTERFACELIB = $(BUILD_DIR)/lib/libcommon.a 
all:$(INTERFACELIB)

$(INTERFACELIB): $(SVROBJS)
	rm -f $@
	ar cqs $@ $(SVROBJS)

base.o: base.cpp $(BUILD_DIR)/include/base/base.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ base.cpp
codequeue.o: codequeue.cpp $(BUILD_DIR)/include/base/codequeue.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ codequeue.cpp
lock.o: lock.cpp $(BUILD_DIR)/include/base/lock.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ lock.cpp
queue.o: queue.cpp $(BUILD_DIR)/include/base/queue.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ queue.cpp
objmng.o: objmng.cpp $(BUILD_DIR)/include/base/objmng.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ objmng.cpp
shm.o: shm.cpp $(BUILD_DIR)/include/base/shm.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ shm.cpp
tcpconn.o: tcpconn.cpp $(BUILD_DIR)/include/base/tcpconn.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tcpconn.cpp
tcpsocket.o: tcpsocket.cpp $(BUILD_DIR)/include/base/tcpsocket.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tcpsocket.cpp
thread.o: thread.cpp $(BUILD_DIR)/include/base/thread.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ thread.cpp
tlib_cfg.o: tlib_cfg.cpp $(BUILD_DIR)/include/base/tlib_cfg.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tlib_cfg.cpp
tlib_log.o: tlib_log.cpp $(BUILD_DIR)/include/base/tlib_log.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tlib_log.cpp
sockstream.o: sockstream.cpp $(BUILD_DIR)/include/base/sockstream.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ sockstream.cpp
stringmng.o: stringmng.cpp $(BUILD_DIR)/include/base/stringmng.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ stringmng.cpp

package:
	rm -f ../$(PACKAGE)
	rm -rf ../log/*
	cd ..;tar zcvf $(PACKAGE) bin log config 

clean:
	rm -f $(SVROBJS) $(BINARY)
