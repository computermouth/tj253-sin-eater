
#ifndef DEAL_SATAN_H
#define DEAL_SATAN_H

#include "raylib.h"

void satan_init();
void satan_draw();
void satan_update();
Vector2 satan_dir_from(float y, float x);
float satan_distance(float px, float py);
void satan_health_down(int m);
int satan_get_health();
void satan_kill();

#endif