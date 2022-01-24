#pragma once

#include <SDL2/SDL.h>
#include "map.h"
#include "player.h"
#include "cam.h"


void draw_map(tile MAP[], SDL_Renderer *rend, camera cam);

void set_cam(camera *cam, person *player);

void draw_player(SDL_Renderer *rend, person player, camera cam);

void draw_friends(SDL_Renderer *rend, tile MAP[], camera cam, int start);
