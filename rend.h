#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "player.h"
#include "cam.h"


#define BUFFER_COUNT 4


SDL_Texture* get_texture(SDL_Renderer *rend, char* img);

void populate_textures(SDL_Renderer *rend, SDL_Texture *arr[]);

void draw_map(tile MAP[], SDL_Renderer *rend, camera cam, SDL_Texture *text[]);

void set_cam(camera *cam, person *player);

void draw_player(SDL_Renderer *rend, person player, camera cam, SDL_Texture *buffer[]);

void draw_friends(SDL_Renderer *rend, tile MAP[], camera cam, int start);

void cleanup_textures(SDL_Texture *buffer[]);
