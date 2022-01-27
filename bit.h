#pragma once
#include "cam.h"
#include "map.h"
#include "rend.h"
#include "util.h"
//For return_max;

void bitwise_not(tile MAP[], int start);

void control_bitwise(tile MAP[]);

void make_friends(tile MAP[], camera cam);

void depression(tile MAP[]);
