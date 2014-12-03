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

BUILD_DIR=../..

COMM_DIR =../lib
OI_LIB = 
#BINARY = testlib
SVROBJS = base.o dynamiclibloader.o codequeue.o lock.o objmng.o queue.o shm.o tcpsocket.o tcpconn.o thread.o tlib_cfg.o tlib_log.o sockstream.o stringmng.o
#PACKAGE = QQT_RESSVRD_INS_DEBUG_`date +%Y%m%d`.tgz
PACKAGE = QQT_RESSVRD_INS_`date +%Y%m%d`.tgz

# Comment the following line if you are not using the gnu c compiler
# C_ARGS = -Wall
C_ARGS = -I. -I$(BUILD_DIR)/include -I$(BUILD_DIR)/include/libcommon -I$(BUILD_DIR)/include/interface $(DBINC)  -g -shared -fPIC
#C_ARGS = -I. -O3 -g

# You might have to change this if your c compiler is not cc
CC = cc
CPP = g++ -std=c++0x



INTERFACELIB = $(BUILD_DIR)/libs/libcommon.a 
all:$(INTERFACELIB)

$(INTERFACELIB): $(SVROBJS)
	rm -f $@
	ar cqs $@ $(SVROBJS)

base.o: base.cc $(BUILD_DIR)/include/libcommon/base.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ base.cc
dynamiclibloader.o: dynamiclibloader.cc $(BUILD_DIR)/include/libcommon/base.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ base.cc
codequeue.o: codequeue.cc $(BUILD_DIR)/include/libcommon/codequeue.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ codequeue.cc
lock.o: lock.cc $(BUILD_DIR)/include/libcommon/lock.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ lock.cc
queue.o: queue.cc $(BUILD_DIR)/include/libcommon/queue.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ queue.cc
objmng.o: objmng.cc $(BUILD_DIR)/include/libcommon/objmng.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ objmng.cc
shm.o: shm.cc $(BUILD_DIR)/include/libcommon/shm.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ shm.cc
tcpconn.o: tcpconn.cc $(BUILD_DIR)/include/libcommon/tcpconn.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tcpconn.cc
tcpsocket.o: tcpsocket.cc $(BUILD_DIR)/include/libcommon/tcpsocket.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tcpsocket.cc
thread.o: thread.cc $(BUILD_DIR)/include/libcommon/thread.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ thread.cc
tlib_cfg.o: tlib_cfg.cc $(BUILD_DIR)/include/libcommon/tlib_cfg.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tlib_cfg.cc
tlib_log.o: tlib_log.cc $(BUILD_DIR)/include/libcommon/tlib_log.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ tlib_log.cc
sockstream.o: sockstream.cc $(BUILD_DIR)/include/libcommon/sockstream.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ sockstream.cc
stringmng.o: stringmng.cc $(BUILD_DIR)/include/libcommon/stringmng.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ stringmng.cc

package:
	rm -f ../$(PACKAGE)
	rm -rf ../log/*
	cd ..;tar zcvf $(PACKAGE) bin log config 

clean:
	rm -f $(SVROBJS) $(BINARY)
