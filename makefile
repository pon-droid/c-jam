FILES=main.c map.c player.c rend.c
PKGS=sdl2
CFLAGS=-Wall -Wextra -pedantic -std=c99 `pkg-config --cflags $(PKGS)`
CRFLAGS=-Wall -Wextra -pedantic -O2 -std=c99  `pkg-config --cflags --static $(PKGS)`
LIBS=`pkg-config --libs $(PKGS)`

bit: $(FILES)
	$(CC) $(CFLAGS) -o bit $(FILES) $(LIBS)
bitter: $(FILES)
	$(CC) $(CRFLAGS) -o bitter $(FILES) $(LIBS)
clean:
	rm -f *.dat bit bitter