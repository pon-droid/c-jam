#include<stdlib.h>

#define MAP_W 50
#define MAP_H 50

typedef struct tile {
  bool block;
  bool binary;
} tile;

void save_map(tile MAP[]){
  FILE *out;

  out = fopen("map.dat", "w");

  if(out == NULL) { printf("Could not save map!\n"); }

  int i;
  for( i = 0; i < MAP_W * MAP_H; i++){
    fwrite(&MAP[i], sizeof(tile), 1, out);
  }

  fclose(out);

}

void read_map(tile MAP[]){
  FILE *in;

  in = fopen("map.dat","r");

  if(in == NULL) { printf("Could not read map!\n"); }

  int i;
  for( i = 0; i < MAP_W * MAP_H; i++){
    fread(&MAP[i], sizeof(tile), 1, in);
  }

  fclose(in):

}
