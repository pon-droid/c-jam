#include "util.h"

void reload_textures(SDL_Renderer *rend, SDL_Texture *arr[]){
    const Uint8 *keys = SDL_GetKeyboardState( NULL );

    if(keys[SDL_SCANCODE_R]){
	 populate_textures(rend,arr);
	}
}

void mouse_edit(tile MAP[], camera cam){
  int x, y;
  Uint32 mouse = SDL_GetMouseState(&x, &y);
  //Translate screen position to array position
  x = (x + cam.x)/CELL_W;
  y = (y + cam.y)/CELL_H;


  
  if((mouse & SDL_BUTTON_LMASK) != 0 && in_map(x,y)){

    MAP[y * MAP_W + x].state = BINARY_ON;
  }
  if((mouse & SDL_BUTTON_RMASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].state = BINARY_OFF;
  }
  if((mouse & SDL_BUTTON_MMASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].state = BLOCK;
  }

}


int return_max(int x, int y){
  if(x > y){
    return x;
  } else {
    return y;
  }
}

void mouse_cam(camera *cam){
    const Uint8 *keys = SDL_GetKeyboardState( NULL );
    if(keys[SDL_SCANCODE_L]){
	int x,y;
	SDL_GetMouseState(&x,&y);
	cam->x = x;
	cam->y = y;
	}
}

void dev_mode(tile MAP[], camera cam, person *player){
  const Uint8 *keys = SDL_GetKeyboardState( NULL );
  if(keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_S]){
  	save_player_pos(*player);
  }


  
 // if(keys[SDL_SCANCODE_KP_9]){ GRAV++ }
  if(keys[SDL_SCANCODE_Q]) { *player = read_player_pos(); }
  if(keys[SDL_SCANCODE_LSHIFT]){
    make_friends(MAP,cam);
    if(keys[SDL_SCANCODE_F1]){
    	depression(MAP);
    }	
  } else {
  	mouse_edit(MAP, cam);
  }
}
