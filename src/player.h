
#ifndef DEAL_PLAYER_H
#define DEAL_PLAYER_H

#include "raylib.h"

void player_init();
void player_update();
void player_draw();
Vector2 player_dir_from(float x, float y);
float player_distance(float px, float py);
void player_health_down(int m);
int player_get_health();

void player_kill();

#endif