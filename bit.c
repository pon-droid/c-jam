#include "bit.h"

void bitwise_not(tile MAP[], int start){
    int i;
    for( i = start; i < MAP_W * MAP_H; i++){
        if(MAP[i].friend >= 0){
            int tmp = MAP[i].state;
            MAP[i].state = MAP[MAP[i].friend].state;
            MAP[MAP[i].friend].state = tmp;
            //Ensure we don't do a tangled swap, no cheating
            bitwise_not(MAP,return_max(i,MAP[i].friend) + 1);
            return;
        }
    }
}

void control_bitwise(tile MAP[]){

    const Uint8* keys = SDL_GetKeyboardState( NULL );

	if(keys[SDL_SCANCODE_PAGEUP]){
        bitwise_not(MAP, 0);
        SDL_Delay(200);
        return;
    }
    
}


void make_friends(tile MAP[], camera cam){
  int x, y;
  Uint32 mouse = SDL_GetMouseState(&x,&y);

  x = (x + cam.x)/CELL_W;
  y = (y + cam.y)/CELL_H;
  
  if((mouse & SDL_BUTTON_LMASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].friend = WANT_FRIEND;
  }
  if((mouse & SDL_BUTTON_RMASK) != 0 && in_map(x,y)){
    int i;
    for( i = 0; i < MAP_W * MAP_H; i++){
      if(MAP[i].friend == WANT_FRIEND){
        MAP[y * MAP_W + x].friend = i;
        MAP[i].friend = y * MAP_W + x;
        return;
      }
    }
  }
}

void depression(tile MAP[]){
	int i;
	for( i = 0; i < MAP_W * MAP_H; i++){
		MAP[i].friend = NO_FRIEND;
	}
}
