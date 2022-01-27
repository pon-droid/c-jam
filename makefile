

CC=$(CROSS)gcc
LD=$(CROSS)ld
AR=$(CROSS)ar
PKG_CONFIG=$(CROSS)pkg-config

FILES=main.c map.c player.c rend.c bit.c util.c

PKGS=sdl2 SDL2_image

CFLAGS=-Wall -Wextra -pedantic -std=c99 `$(PKG_CONFIG) --cflags $(PKGS)`
CRFLAGS=-Wall -Wextra -pedantic -O2 -std=c99  `$(PKG_CONFIG) --cflags --static $(PKGS)`
LIBS=`$(PKG_CONFIG) --libs $(PKGS)`

bit: $(FILES)
	$(CC) $(CFLAGS) -o bit $(FILES) $(LIBS)
bitter: $(FILES)
	$(CC) $(CRFLAGS) -o bitter $(FILES) $(LIBS)
clean:
	rm -f *.dat config.txt bit bitter
