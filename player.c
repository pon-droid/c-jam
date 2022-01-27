#include "player.h"




float PL_S;
float GRAV;
float FRICT;
float PL_JUMP;

void save_physics(void){
	FILE *out;

	out = fopen("config.txt", "w");
	
	if(out == NULL) { printf("Cannot save config file\n"); return;}
	
	fprintf(out,"Speed: %f\n",PL_S);
	fprintf(out,"Gravity: %f\n",GRAV);
	fprintf(out,"Friction: %f\n",FRICT);
	fprintf(out,"Jump: %f\n",PL_JUMP);
	
	fclose(out);
}

void read_physics(void){
	FILE *in;

	in = fopen("config.txt", "r");

	if(in == NULL){
	  printf("Cannot find config file, using defaults\n");
      PL_S = .8;
      GRAV = .00004;
      FRICT = .2;
      PL_JUMP = .02;
      return;
	 }

    fscanf(in,"Speed: %f\n",&PL_S);
	fscanf(in,"Gravity: %f\n",&GRAV);
	fscanf(in,"Friction: %f\n",&FRICT);
	fscanf(in,"Jump: %f\n",&PL_JUMP);

	fclose(in);
}

person read_player_pos(void){
  FILE *in;

  in = fopen("player.dat","r");
  
  if(in == NULL) { person player = {MAP_W/2*CELL_W,MAP_H/2*CELL_H,0,0}; printf("No player data! \n"); return player;}
  
  person player;
  
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
    if(MAP[y * MAP_W + x].state == BINARY_OFF){ return false; }
  } else {
    return true;
  }

  return true;

}


bool collide_man(tile MAP[], int x, int y, int w, int h){ 
  return collide(MAP,x,y) || collide(MAP,x+w,y) || collide(MAP,x+w,y+h) || collide(MAP,x,y+h);
}

bool on_ground(person player, tile MAP[]){
  //Add four, for generous jump frame
  return (collide(MAP,player.x,player.y+PL_H + 4) || collide(MAP,player.x+PL_W,player.y+PL_H + 4)) && !(collide(MAP,player.x,player.y) && (collide(MAP,player.x+PL_W,player.y)));
}

bool hit_head(person player, tile MAP[]){
	return (collide(MAP,player.x,player.y - 4) || collide(MAP,player.x+PL_W,player.y - 4)) && !(on_ground(player,MAP));
}

void manage_player(person *player, tile MAP[], int interval){

  float x = player->x;
  float y = player->y;

  const Uint8* keys = SDL_GetKeyboardState( NULL );
  //Dont predict next position
  if( keys[SDL_SCANCODE_RIGHT] ){
    player->vx += (PL_S * interval);
  }
  if( keys[SDL_SCANCODE_LEFT] ){
    player->vx -= (PL_S * interval);
  }
  if( keys[SDL_SCANCODE_UP] && on_ground(*player,MAP) ){
    player->vy = (PL_JUMP * interval) * -2;
  }
  /*
  if( keys[SDL_SCANCODE_DOWN] ){
    player->vy = (PL_S * interval);
  }
  */
  
  player->vy += GRAV * interval;
  if(on_ground(*player,MAP)) { player->vy -= GRAV * interval;}
  player->vx *= (FRICT * interval);

  player->x += player->vx;
  if(collide_man(MAP,player->x,player->y,PL_W,PL_H)) { player->x = x;}
  player->y += player->vy;
  if(hit_head(*player,MAP)){ player->vy = .001; player->y += player->vy; }
  if(collide_man(MAP,player->x,player->y,PL_W,PL_H)) { player->y = y;}
  
 // if(collide_man(MAP,player->x,player->y,PL_W,PL_H)) { player->x -= player->vx; player->y -= player->vy; } 
 
}
