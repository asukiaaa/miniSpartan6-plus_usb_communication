#
# makefile for ubuntu
#

CC = g++
LIBS = -lusb-1.0

TARGET = serial_communication

ALL: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c $(LIBS)

clean:
	$(RM) $(TARGET)
