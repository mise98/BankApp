objects = server client
all: $(objects)

$(objects): %: %.c
		$(CC) $(CFLAGS) -o $@ $< -lpthread
clean :
			-rm *.o $(objects)        