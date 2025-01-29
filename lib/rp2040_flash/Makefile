CC=gcc
CFLAGS=-c -Wall -ggdb -DHAS_LIBUSB
LDFALGS=
SOURCES=main.c picoboot_connection.c
OBJECTS=$(SOURCES:.c=.o)
LIBS=`pkg-config libusb-1.0 --libs`
INCLUDE_DIRS+=-I../pico-sdk/ `pkg-config libusb-1.0 --cflags`

EXECUTABLE=rp2040_flash

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
