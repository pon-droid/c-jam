#include<stdbool.h>
#include<SDL.h>

#define SCR_W 750
#define SCR_H 750

#define MAP_W 50
#define MAP_H 50

static unsigned char CELL_W = 64;
static unsigned char CELL_H = 64;

#define MAP_VW 50
#define MAP_VH 50

#define PL_W (CELL_W/2)
#define PL_H (CELL_H/2)

#define PL_S .01

#define GET_MAP(y, x) (MAP[y * MAP_W + x])

int max(int x, int y){ if(x > y) { return x; } else { return y; } }
int min(int x, int y){ if(x < y) { return x; } else { return y; } }

typedef struct neighbour {
  int x, y;
} neighbour;

//For other bitwise operations

typedef struct tile {
  bool block;
  bool binary;
} tile;

typedef struct person {
  float x, y;
  float vx, vy;
} person;

typedef struct point {
  int x, y;
} point;

typedef struct camera {
  float x, y;
} camera;

bool in_map(int x, int y){
  // return true;
  return x < MAP_W && x >= 0 && y < MAP_H && y >= 0;
}

bool quit (void){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT){
      return true;
    }
  }

  return false;
}

tile get_tile(int x, int y, tile MAP[]){
  if(x >= 0 && x < MAP_W && y >= 0 && y < MAP_H){
    return GET_MAP(y, x);
  } else {
    tile new;
    new.block = 0;
    new.binary = 0;
    return new;
  }
}

void scroll_draw_map(tile MAP[], SDL_Renderer *rend, point camera){

  int x, y, x1, y1, x2, y2;

  int mx, my;

  x1 = (camera.x % CELL_W) * -1;
  x2 = x1 + MAP_VW * CELL_W + ( x1 == 0 ? 0 : CELL_W);

  y1 = (camera.x % CELL_H) * -1;
  y2 = y1 + MAP_VH * CELL_H + (y1 == 0 ? 0 : CELL_H);

  mx = camera.x / CELL_W;
  my = camera.y / CELL_H;

  for ( y = y1; y < y2; y += CELL_H){
    for ( x = x1; x < x2; x+= CELL_W){
      if(mx >= 0 && my >= 0 && mx < MAP_W && my < MAP_H){
	SDL_Rect rect;
	tile tmp;
	rect.h = CELL_H;
	rect.w = CELL_W;
	tmp = GET_MAP(my, mx);
        if(tmp.block){ SDL_SetRenderDrawColor(rend, 0, 0, 255, 255); }
        if(tmp.binary && !tmp.block){ SDL_SetRenderDrawColor(rend, 0, 255, 0, 255); }
        else{ SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); }

	rect.x = x;
	rect.y = y;

	SDL_RenderFillRect(rend, &rect);
      }
      mx++;
    }
    mx = camera.x / CELL_W;
    my++;
  }

}

void clamp_cam(point *camera){
  camera->x = min(max(camera->x, 0), (MAP_W * SCR_W));
  camera->y = min(max(camera->y, 0), (MAP_H * SCR_H));
}
	
  

void draw_map(tile MAP[], SDL_Renderer *rend){
  SDL_Rect rect;
  rect.h = CELL_H;
  rect.w = CELL_W;

  tile tmp;

  int y, x;

  for( y = 0; y < MAP_VH; y++){
    for( x = 0; x < MAP_VW; x++){
      tmp = GET_MAP(y, x);

      if(tmp.block){ SDL_SetRenderDrawColor(rend, 0, 0, 255, 255); }
      if(tmp.binary && !tmp.block){ SDL_SetRenderDrawColor(rend, 0, 255, 0, 255); }
      if(!tmp.binary && !tmp.block){ SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); }
      rect.x = x * CELL_W;
      rect.y = y * CELL_H;
      
      SDL_RenderFillRect(rend, &rect);
    }
  }
}

void actual_draw_map(tile MAP[], SDL_Renderer *rend, camera cam){
  SDL_Rect rect;
  rect.h = CELL_H;
  rect.w = CELL_W;

  tile tmp;

  int y, x;

  for( y = 0; y < MAP_VH; y++){
    for( x = 0; x < MAP_VW; x++){
      tmp = GET_MAP(y, x);

      if(tmp.block){ SDL_SetRenderDrawColor(rend, 0, 0, 255, 255); }
      if(tmp.binary && !tmp.block){ SDL_SetRenderDrawColor(rend, 0, 255, 0, 255); }
      if(!tmp.binary && !tmp.block){ SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); }
      rect.x = (x * CELL_W) - cam.x;
      rect.y = (y * CELL_H) - cam.y;
      
      SDL_RenderFillRect(rend, &rect);
    }
  }
}

//Maps to array dimensions


bool in_map_bounds(int x, int y){
  //This assumes that coords have been scaled
  if(x < 0 || x > MAP_W || y < 0 || y > MAP_H) { return true; }
  else { return false; }

}

bool n_collide(tile MAP[], int x, int y){
  x = x/CELL_W;
  y = y/CELL_H;

}

bool p_collide(tile MAP[], int x, int y){
 
  x = x/CELL_W;
  y = y/CELL_H;
  

  if(x < 0 || x > MAP_W || y < 0 || y > MAP_H) { return true; }
  
  if(GET_MAP(y, x).binary || GET_MAP(y, x).block) { return true; }
  
  return false;
}

bool t_collide(tile MAP[], int x, int y){

  x = x/CELL_W;
  y = y/CELL_H;


  if(!in_map(x,y)){ return false; }

  if(MAP[y * MAP_W + x].binary || MAP[y * MAP_W + x].block){ return false; }

  return true;

}

  

bool collide_man(tile MAP[], int x, int y){

  
  
  return t_collide(MAP,x,y) || t_collide(MAP,x+PL_W,y) || t_collide(MAP,x+PL_W,y+PL_H) || t_collide(MAP,x,y+PL_H);

}

void draw_corn(person player, SDL_Renderer *rend, camera cam){
  SDL_SetRenderDrawColor(rend,255,255,0,255);
  SDL_Rect rct;
  rct.h = 5;
  rct.w = 5;

  rct.x = player.x - cam.x;
  rct.y = player.y - cam.y;

  SDL_RenderFillRect(rend, &rct);

}

void update_player(person *player, SDL_Renderer *rend, tile MAP[], camera cam, int interval){
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_Rect rect;
  rect.h = PL_H;
  rect.w = PL_W;
  
  const Uint8* keys = SDL_GetKeyboardState( NULL );
  int up = player->y - (PL_S * interval);
  int down = player->y + (PL_S * interval);
  int right = player->x + (PL_S * interval);
  int left = player->x - (PL_S * interval);

  if( keys[SDL_SCANCODE_RIGHT] && (collide_man(MAP,right,player->y))){// && !(p_collide(map,right,player->y))){
    player->x = right;
  }
  if( keys[SDL_SCANCODE_LEFT] && (collide_man(MAP,left,player->y))){// && !(p_collide(map,left,player->y))){
    player->x = left;
  }
  if( keys[SDL_SCANCODE_UP] && (collide_man(MAP,player->x,up))){// && !(p_collide(map,player->x,up))){
    player->y = up;
  }
  if( keys[SDL_SCANCODE_DOWN] && (collide_man(MAP,player->x,down))){// && !(p_collide(map,player->x,down))){
    player->y = down;
  }
  
  
  rect.x = player->x - cam.x;
  rect.y = player->y - cam.y;
  
  SDL_RenderFillRect(rend, &rect);
}

void manage_player(person *player, tile MAP[], camera cam, SDL_Renderer *rend, int interval){
  SDL_SetRenderDrawColor(rend,0,0,0,255);
  SDL_Rect rect;
  rect.h = PL_H;
  rect.w = PL_W;

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

  if(collide_man(MAP,player->x,player->y)) { player->x -= player->vx; player->y -= player->vy; }

  rect.x = player->x - cam.x;
  rect.y = player->y - cam.y;

  player->vx = 0;
  player->vy = 0;

  SDL_RenderFillRect(rend, &rect);
}
  

int x_zoom = 0;
int y_zoom = 0;

void zoom(SDL_Renderer *rend){
  const Uint8* keys = SDL_GetKeyboardState( NULL );

  if(keys[SDL_SCANCODE_0]){
    x_zoom--;
    y_zoom--;
  }
  if(keys[SDL_SCANCODE_1]){
    x_zoom++;
    y_zoom++;
  }


  SDL_RenderSetLogicalSize(rend,x_zoom,y_zoom);
 
}
    
void print_coord(person player){
  printf("X: %f, Y: %f\n",player.x/CELL_W, player.y/CELL_H);
}




void mouse_edit(tile MAP[], camera cam, person player){
  int x, y;
  Uint32 mouse = SDL_GetMouseState(&x, &y);
  //Translate screen position to array position
  x = (x + cam.x)/CELL_W;
  y = (y + cam.y)/CELL_H;
  
  if((mouse & SDL_BUTTON_LMASK) != 0 && in_map(x,y)){

    MAP[y * MAP_W + x].binary = true;
  }
  if((mouse & SDL_BUTTON_RMASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].binary = false;
  }
  if((mouse & SDL_BUTTON_X2MASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].block = true;
  }
  if((mouse & SDL_BUTTON_X1MASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].block = false;
  }

}

int main (void){
  SDL_Window *win = SDL_CreateWindow("bitunwise", 0, 0, SCR_W, SCR_H, SDL_WINDOW_RESIZABLE);
  SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

  tile MAP[MAP_W * MAP_H];
  person player;
  player.x = SCR_W/2;
  player.y = SCR_H/2;
  camera cam;
  

  int i;

  for( i = 0; i < MAP_W * MAP_H; i++){
    tile ntile;
    ntile.block = false;
    if(i % 3 == 0){
      ntile.binary = true;
    } else {
      ntile.binary = false;
    }
    MAP[i] = ntile;
  }

  int last_time = 0;

  while(!quit()){
    int elapsed = SDL_GetTicks();
    
    cam.x = player.x - SCR_W/2;
    cam.y = player.y - SCR_H/2;
    
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderClear(rend);
    actual_draw_map(MAP, rend, cam);
    //draw_map(MAP, rend);
    // scroll_draw_map(MAP, rend, camera);
    // clamp_cam(&camera);
    int interval = elapsed - last_time;
    //update_player(&player, rend, MAP, cam, interval);
    manage_player(&player, MAP, cam, rend, interval);
    draw_corn(player,rend,cam);
    mouse_edit(MAP,cam,player);
    //cam_handler(&cam);
    SDL_RenderPresent(rend);
    
    last_time = elapsed;
  }


  return 0;
}

  

  
