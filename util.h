#pragma once

#include "rend.h"
#include "map.h"
#include "player.h"
#include "bit.h"
#include "cam.h"

int return_max(int x, int y);

void reload_textures(SDL_Renderer *rend, SDL_Texture *arr[]);

void mouse_edit(tile MAP[], camera cam);

void dev_mode(tile MAP[],camera cam, person *player);

void mouse_cam(camera *cam);
