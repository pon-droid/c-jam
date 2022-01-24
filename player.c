#include "player.h"

person read_player_pos(void){
  FILE *in;

  in = fopen("player.dat","r");
  person player;
  if(in == NULL) { player.x = MAP_W/2*CELL_W; player.y = MAP_H/2*CELL_H; printf("No player data! \n"); return player;}

  fread(&player, sizeof(person), 1, in);
  fclose(in);
  return player;
}

void save_player_pos(person player){
  FILE *out;
  out = fopen("player.dat", "w");
  if(out == NULL) { printf("Could not save player data! \n"); return;}

  fwrite(&player,sizeof(person),1, out);
  fclose(out);

}

bool in_map(int x, int y){
  return x < MAP_W && x >= 0 && y < MAP_H && y >= 0;
}

bool collide(tile MAP[], int x, int y){

  x = x/CELL_W;
  y = y/CELL_H;


  if(in_map(x,y)){
    if(MAP[y * MAP_W + x].state == BLOCK || MAP[y * MAP_W + x].state == BINARY_ON){ return false; }
  } else {
    return true;
  }

  return true;

}


bool collide_man(tile MAP[], int x, int y, int w, int h){ 
  return collide(MAP,x,y) || collide(MAP,x+w,y) || collide(MAP,x+w,y+h) || collide(MAP,x,y+h);
}

bool on_ground(person *player, tile MAP[]){
  //Add one, because when on ground we are not colliding, so check if block underneath is collidable
  return collide(MAP,player->x,player->y+PL_H + 4) || collide(MAP,player->x+PL_W,player->y+PL_H + 4);
}

void manage_player(person *player, tile MAP[], int interval){

  const Uint8* keys = SDL_GetKeyboardState( NULL );
  //Dont predict next position
  if( keys[SDL_SCANCODE_RIGHT] ){
    player->vx = (PL_S * interval);
  }
  if( keys[SDL_SCANCODE_LEFT] ){
    player->vx = (PL_S * interval) * -1;
  }
  if( keys[SDL_SCANCODE_UP] ){
    player->vy = (PL_S * interval) * -1;
  }
  if( keys[SDL_SCANCODE_DOWN] ){
    player->vy = (PL_S * interval);
  }

  player->x += player->vx;
  player->y += player->vy;

  if(collide_man(MAP,player->x,player->y,PL_W,PL_H)) { player->x -= player->vx; player->y -= player->vy; }


  player->vx = 0;
  player->vy = 0;
  /*
  Snap to last block, better collision response then subtracting velocity
  if(collide_man(MAP,player->x,player->y)) {
    int tmp = player->y/CELL_H;
    int snap = player->y*CELL_H;
    player->y = snap;
    player->vy = 0;
    
  }  
  */
 
}
