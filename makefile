# CC=/usr/bin/g++
CC=g++
CFLAGS = -O3 -std=c++11 -lpthread -flto -march=native
LDFLAGS=-flto
EXEC=qd_test
all: $(EXEC)

ifeq ($(prof),1)
 CFLAGS+= -pg
endif
ifeq ($(deb),1)
 CFLAGS+= -O0 -DASSERTS -g
endif


all: $(EXEC)

qd_test:  src/qd_tester.cpp
	$(CC)  $^ $(CFLAGS) -o $(EXEC)

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -rf *.o
	rm $(EXEC)