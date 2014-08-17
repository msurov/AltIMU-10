

CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=
SOURCES = \
	main.cpp \
	compass_lib.cpp \
	gyro_lib.cpp \
	i2c_lib.cpp \
	lps_lib.cpp \
	sysclock_lib.cpp \
	udp_sender.cpp

OBJECTS=$(SOURCES:.cpp=.o)
	EXECUTABLE=gyro

all: \
	$(SOURCES) $(EXECUTABLE)
	    
$(EXECUTABLE): $(OBJECTS) 
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	    $(CC) $(CFLAGS) $< -o $@

clean:
	    rm -rf *.o gyro



