

all:
	gcc t.c ser.c -o server
	gcc t.c cli.c -o client

clean:
	rm server client
