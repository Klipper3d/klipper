CC = gcc
CFLAGS := -Wall -g -O2 -fPIC -flto -fwhole-program -fno-use-linker-plugin

OBJECTS = pyhelper.o serialqueue.o stepcompress.o itersolve.o trapq.o \
		  pollreactor.o msgblock.o trdispatch.o \
		  kin_cartesian.o kin_corexy.o kin_corexz.o kin_delta.o \
		  kin_deltesian.o kin_polar.o kin_rotary_delta.o kin_winch.o \
		  kin_extruder.o kin_shaper.o

DIST_LIB = c_helper.so

all: $(DIST_LIB)

clean:
	@echo "Cleaning up directory."
	rm -f *.a *.o  $(DIST_LIB) *~

$(DIST_LIB):$(OBJECTS)
	$(CC) -shared -fPIC -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

