

CC = gcc

#LDFLAGS = -Xlinker -L$(ROOT)

#LOADLIBES = -lm

#CFLAGS = -I$(ROOT)/include

% : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ \
        $? $(LOADLIBES) dynamixel.o
