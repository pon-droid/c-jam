PKGS=sdl2
CFLAGS=-Wall -Wextra -pedantic -std=c99 `pkg-config --cflags $(PKGS)`
CRFLAGS=-Wall -Wextra -pedantic -O2 -std=c99  `pkg-config --cflags --static $(PKGS)`
LIBS=`pkg-config --libs $(PKGS)`

bit: main.c
	$(CC) $(CFLAGS) -o bit main.c $(LIBS)
bitter: main.c
	$(CC) $(CRFLAGS) -o bitter main.c $(LIBS)
