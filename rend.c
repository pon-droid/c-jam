#include "rend.h"


void set_cam(camera *cam, person *player){
  cam->x = player->x - SCR_W/2;
  cam->y = player->y - SCR_H/2;
}

void draw_map(tile MAP[], SDL_Renderer *rend, camera cam){
  SDL_Rect rect;
  rect.h = CELL_H;
  rect.w = CELL_W;

  tile tmp;

  int y, x;

  for( y = 0; y < MAP_H; y++){
    for( x = 0; x < MAP_W; x++){
      tmp = GET_MAP(y, x);

      if(tmp.state == BLOCK){ SDL_SetRenderDrawColor(rend, 0, 0, 255, 255); }
      if(tmp.state == BINARY_ON){ SDL_SetRenderDrawColor(rend, 0, 255, 0, 255); }
      if(tmp.state == BINARY_OFF){ SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); }
      if(tmp.friend != NO_FRIEND) { SDL_SetRenderDrawColor(rend,255,255,255,255);}
      if(tmp.friend == WANT_FRIEND ) { SDL_SetRenderDrawColor(rend,0,0,0,255); }
      rect.x = (x * CELL_W) - cam.x;
      rect.y = (y * CELL_H) - cam.y;
      
      SDL_RenderFillRect(rend, &rect);
    }
  }
}

SDL_Point derive_coords(int relation){
	int x, y;

	for ( y = 0; y < MAP_H; y++){
		for( x = 0; x < MAP_W; x++){
			if((y*MAP_W+x) == relation){
				SDL_Point tmp;
				tmp.x = x;
				tmp.y = y;
				return tmp;
			}
		}
	}
}

void draw_friends(SDL_Renderer *rend, tile MAP[], camera cam){
  int x,y;

  for( y = 0; y < MAP_H; y++){
    for( x = 0; x < MAP_W; x++){
      int relation = MAP[y * MAP_W + x].friend;
      if(MAP[y * MAP_W + x].friend != NO_FRIEND && MAP[y * MAP_W + x].friend != WANT_FRIEND){
        // solve this algebra y * MAP_W + x = something or record x and y coords
        SDL_Point point = derive_coords(relation);
        int dx1, dx2, dy1, dy2;
        dx1 = x;
        dx2 = point.x;
        dy1 = y;
        dy2 = point.y;

        SDL_SetRenderDrawColor(rend,255,0,0,255);

        SDL_RenderDrawLine(rend,dx1 - cam.x,dy1 - cam.y,dx2 - cam.x,dy2 - cam.y);
      }
    }
  }
}

void draw_player(SDL_Renderer *rend, person player, camera cam){
    SDL_Rect tmp;
    tmp.h = PL_H;
    tmp.w = PL_W;
    tmp.x = player.x - cam.x;
    tmp.y = player.y - cam.y;

    SDL_SetRenderDrawColor(rend,0,50,58,255);

    SDL_RenderFillRect(rend, &tmp);

}
