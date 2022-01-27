#include "map.h"



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

void diag(tile MAP[]){
  int i;
  
    for( i = 0; i < MAP_W * MAP_H; i++){
    tile tmp;
    if(i % 4 == 0){
      tmp.state = BINARY_ON;
    } else {
      tmp.state = BINARY_OFF;
    }
    tmp.friend = NO_FRIEND;
    MAP[i] = tmp;
  }
}

void read_map(tile MAP[]){
  FILE *in;

  in = fopen("map.dat","r");

  if(in == NULL) { printf("Could not read map!\n"); diag(MAP); return; }

  int i;
  for( i = 0; i < MAP_W * MAP_H; i++){
    fread(&MAP[i], sizeof(tile), 1, in);
  }

  fclose(in);

}



