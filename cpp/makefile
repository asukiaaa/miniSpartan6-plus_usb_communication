#
# makefile for ubuntu
#

CC  = gcc
CXX = g++

LIBUSB_FLAGS  = -lusb-1.0
LIBFTDI_FLAGS = `pkg-config --cflags --libs libftdi1`

SOURCES := $(wildcard src/*.cpp)
SOURCES += $(wildcard src/*.c)
SOURCES := $(addprefix bin/, $(notdir $(basename $(SOURCES))))

ALL: prepare $(SOURCES)

clean:
	rm -r bin

prepare: bin

bin:
	mkdir -p bin

bin/%: src/%.c
	$(CC) $< -o $@ $(LIBUSB_FLAGS) $(LIBFTDI_FLAGS)

bin/%: src/%.cpp
	$(CXX) $< -o $@ $(LIBUSB_FLAGS) $(LIBFTDI_FLAGS)
