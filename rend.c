#include "rend.h"


void set_cam(camera *cam, person *player){
  cam->x = player->x - SCR_W/2;
  cam->y = player->y - SCR_H/2;
}

SDL_Texture* get_texture(SDL_Renderer *rend, char* img){
	SDL_Surface *surf = IMG_Load(img);
	if(surf == NULL) { printf("Cannot load image, %s", IMG_GetError()); return NULL;}

	SDL_Texture *text = SDL_CreateTextureFromSurface(rend,surf);
	SDL_FreeSurface(surf);

	return text;
}

void populate_textures(SDL_Renderer *rend, SDL_Texture *arr[]){
	arr[0] = get_texture(rend,"bin_off.png");
	arr[1] = get_texture(rend, "bin_on.png");
	arr[2] = get_texture(rend, "block.png");
	arr[3] = get_texture(rend, "player.png");
}


void draw_map(tile MAP[], SDL_Renderer *rend, camera cam, SDL_Texture *text[]){
  SDL_Rect rect;
  rect.h = CELL_H;
  rect.w = CELL_W;

  tile tmp;

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 16;
  src.h = 16;

  int y, x;

  for( y = 0; y < MAP_H; y++){
    for( x = 0; x < MAP_W; x++){
      tmp = GET_MAP(y, x);
      GET_MAP(y,x).x = x;
      GET_MAP(y,x).y = y;

      
      rect.x = (x * CELL_W) - cam.x;
      rect.y = (y * CELL_H) - cam.y;

      if(tmp.state == BLOCK){ SDL_RenderCopy(rend,text[2],&src,&rect); continue; }
      if(tmp.state == BINARY_ON){ SDL_RenderCopy(rend,text[1],&src,&rect); continue; }
      if(tmp.state == BINARY_OFF){ SDL_RenderCopy(rend,text[0],&src,&rect); continue; }
      //if(tmp.friend != NO_FRIEND) { SDL_SetRenderDrawColor(rend,255,255,255,255);}
      //if(tmp.friend == WANT_FRIEND ) { SDL_SetRenderDrawColor(rend,0,0,0,255); }

     // SDL_RenderCopy(rend,texture,&src, &rect);
      // SDL_RenderFillRect(rend, &rect);
    }
  }
}


void draw_friends(SDL_Renderer *rend, tile MAP[], camera cam, int start){

  SDL_SetRenderDrawColor(rend,255,255,255,255);
  int i;
  for( i = start; i < MAP_W * MAP_H; i++){
    if(MAP[i].friend != NO_FRIEND && MAP[i].friend != WANT_FRIEND){
      int x1,x2,y1,y2;
      //Center coords
      x1 = MAP[i].x * CELL_W + CELL_W/2;
      x2 = MAP[MAP[i].friend].x * CELL_W + CELL_W/2;
      y1 = MAP[i].y * CELL_H + CELL_H/2;
      y2 = MAP[MAP[i].friend].y * CELL_H + CELL_H/2;

      SDL_RenderDrawLine(rend,x1 - cam.x,y1 - cam.y,x2 - cam.x,y2 - cam.y);
      draw_friends(rend,MAP,cam,return_max(i,MAP[i].friend) + 1);
      return;

    }
  }
}

void draw_player(SDL_Renderer *rend, person player, camera cam, SDL_Texture *buffer[]){

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = 16;
    src.h = 16;
    
    SDL_Rect tmp;
    tmp.h = PL_H;
    tmp.w = PL_W;
    tmp.x = player.x - cam.x;
    tmp.y = player.y - cam.y;

    SDL_RenderCopy(rend,buffer[3],&src,&tmp);
    //SDL_SetRenderDrawColor(rend,0,50,58,255);

    //SDL_RenderFillRect(rend, &tmp);

}

void cleanup_textures(SDL_Texture *buffer[]){
	int i;
	for( i = 0; i < BUFFER_COUNT; i++){
		SDL_DestroyTexture(buffer[i]);
	}
}
