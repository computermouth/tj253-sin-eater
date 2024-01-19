
#include "res.h"
#include "player.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
        IDLE,
        WALK,
        ATK1,
        DIE_,
} phase;

typedef struct {
    Texture textures[3][3][ 4][ 5];
    int last_xdir;
    int last_ydir;
    int curr_xdir;
    int curr_ydir;
    float pos_x;
    float pos_y;
    int health;
    phase animation;
    float anim_start;
    float atk1_last;
    float atk1_cool;
    float atk2_last;
    float atk2_cool;
    float dash_last;
    float dash_cool;
} sd_t;

static sd_t satan_data = { 0 };

void satan_health_down(int m){
    satan_data.health -= m;
}
int satan_get_health(){
    return satan_data.health;
}

void satan_kill(){
    satan_data.animation = DIE_;
}

Vector2 satan_dir_from(float px, float py){
    Vector2 v =  Vector2Normalize(Vector2Subtract((Vector2){px + 128, py + 128}, (Vector2){satan_data.pos_x + 128, satan_data.pos_y + 128}));
    v.x = roundf(v.x);
    v.y = roundf(v.y);
    return Vector2Negate(v);
}

float satan_distance(float px, float py){
    return Vector2Distance((Vector2){px, py}, (Vector2){satan_data.pos_x, satan_data.pos_y});
}

void satan_update(){

    // direction to face
    Vector2 dir = player_dir_from(satan_data.pos_x, satan_data.pos_y);

    satan_data.curr_xdir = dir.x + 1;
    satan_data.curr_ydir = dir.y + 1;

    // if attacking, return
    if (satan_data.animation == ATK1 && (GetTime() - satan_data.anim_start) <= 0.5){
        return;
    } else if (satan_data.animation == ATK1 && (GetTime() - satan_data.anim_start) > 0.5) {
        if(player_distance(satan_data.pos_x, satan_data.pos_y) < 256)
            player_health_down(2);
    }

    satan_data.animation = WALK;

    // attack
    if (player_distance(satan_data.pos_x, satan_data.pos_y) < 64 && (satan_data.atk1_last + satan_data.atk1_cool) < GetTime()) {
        satan_data.animation = ATK1;
        satan_data.anim_start = GetTime();
        satan_data.atk1_last = GetTime();
        return;
    }
    
    // back off
    if (player_distance(satan_data.pos_x, satan_data.pos_y) < 200 && (satan_data.atk1_last + satan_data.atk1_cool) > GetTime()) {
        satan_data.pos_x -= dir.x * 100 * GetFrameTime();
        satan_data.pos_y -= dir.y * 100 * GetFrameTime();
        satan_data.pos_x = Clamp(satan_data.pos_x, 0, 1280 - 256);
        satan_data.pos_y = Clamp(satan_data.pos_y, 0,  720 - 256);
        return;
    }

    // approach
    if (player_distance(satan_data.pos_x, satan_data.pos_y) >= 64){
        satan_data.pos_x += dir.x * 100 * GetFrameTime();
        satan_data.pos_y += dir.y * 100 * GetFrameTime();
        satan_data.pos_x = Clamp(satan_data.pos_x, 0, 1280 - 256);
        satan_data.pos_y = Clamp(satan_data.pos_y, 0,  720 - 256);
        return;
    }

    // // anims
    // if(xdir == 0 && ydir == 0){
	// 	player_data.curr_xdir = player_data.last_xdir;
	// 	player_data.curr_ydir = player_data.last_ydir;
	// 	player_data.animation = IDLE;
    //     player_data.anim_start = GetTime();
	// } else {
	// 	player_data.animation = WALK;
    //     player_data.anim_start = GetTime();
	// }

	// if (IsKeyDown(KEY_J)){
	// 	player_data.animation = ATK1;
    //     player_data.anim_start = GetTime();
    // }

	// if (IsKeyDown(KEY_K)){
	// 	player_data.animation = DIE_;
    //     player_data.anim_start = GetTime();
    // }

}

void satan_draw(){

    int xd = satan_data.curr_xdir;
    int yd = satan_data.curr_ydir;
    int ap = satan_data.animation;

    int xp = satan_data.pos_x;
    int yp = satan_data.pos_y;

    float at = satan_data.anim_start;
    int frame = fmodf((GetTime() - at) * 10, 5); // half second from start to finish

    DrawTexture(satan_data.textures[xd][yd][ap][frame], xp, yp, WHITE);

    char h[20];
    sprintf(h, "enemy health: %d", satan_data.health);
    DrawText(h, 1090, 20, 20, BLACK);
}

void satan_init(){
    sd_t p = {
        .last_xdir = 0,
        .last_ydir = 2,
        .curr_xdir = 0,
        .curr_ydir = 2,
        .pos_x = 1000,
        .pos_y = 50,
        .health = 50,
        .animation = IDLE,
        .anim_start = 0,
        .atk1_last = 0,
        .atk1_cool = 2,
        .textures = {
            [1][0] = { // x == 0, y == -1
                [IDLE] = {
                    res_img_satan_idle_000_Idle_Body_000_0003_png,
                    res_img_satan_idle_000_Idle_Body_000_0008_png,
                    res_img_satan_idle_000_Idle_Body_000_0013_png,
                    res_img_satan_idle_000_Idle_Body_000_0018_png,
                    res_img_satan_idle_000_Idle_Body_000_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_000_Walk_Body_000_0002_png,
                    res_img_satan_walk_000_Walk_Body_000_0006_png,
                    res_img_satan_walk_000_Walk_Body_000_0010_png,
                    res_img_satan_walk_000_Walk_Body_000_0014_png,
                    res_img_satan_walk_000_Walk_Body_000_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_000_Attack1_Body_000_0002_png,
                    res_img_satan_attack_000_Attack1_Body_000_0006_png,
                    res_img_satan_attack_000_Attack1_Body_000_0010_png,
                    res_img_satan_attack_000_Attack1_Body_000_0014_png,
                    res_img_satan_attack_000_Attack1_Body_000_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_000_Death_Body_000_0001_png,
                    res_img_satan_death_000_Death_Body_000_0007_png,
                    res_img_satan_death_000_Death_Body_000_0013_png,
                    res_img_satan_death_000_Death_Body_000_0019_png,
                    res_img_satan_death_000_Death_Body_000_0025_png,
                }
            },
            [2][0] = { // x == 1, y == -1
                [IDLE] = {
                    res_img_satan_idle_045_Idle_Body_045_0003_png,
                    res_img_satan_idle_045_Idle_Body_045_0008_png,
                    res_img_satan_idle_045_Idle_Body_045_0013_png,
                    res_img_satan_idle_045_Idle_Body_045_0018_png,
                    res_img_satan_idle_045_Idle_Body_045_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_045_Walk_Body_045_0002_png,
                    res_img_satan_walk_045_Walk_Body_045_0006_png,
                    res_img_satan_walk_045_Walk_Body_045_0010_png,
                    res_img_satan_walk_045_Walk_Body_045_0014_png,
                    res_img_satan_walk_045_Walk_Body_045_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_045_Attack1_Body_045_0002_png,
                    res_img_satan_attack_045_Attack1_Body_045_0006_png,
                    res_img_satan_attack_045_Attack1_Body_045_0010_png,
                    res_img_satan_attack_045_Attack1_Body_045_0014_png,
                    res_img_satan_attack_045_Attack1_Body_045_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_045_Death_Body_045_0001_png,
                    res_img_satan_death_045_Death_Body_045_0007_png,
                    res_img_satan_death_045_Death_Body_045_0013_png,
                    res_img_satan_death_045_Death_Body_045_0019_png,
                    res_img_satan_death_045_Death_Body_045_0025_png,
                }
            },
            [2][1] = { // x == 1, y == 0
                [IDLE] = {
                    res_img_satan_idle_090_Idle_Body_090_0003_png,
                    res_img_satan_idle_090_Idle_Body_090_0008_png,
                    res_img_satan_idle_090_Idle_Body_090_0013_png,
                    res_img_satan_idle_090_Idle_Body_090_0018_png,
                    res_img_satan_idle_090_Idle_Body_090_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_090_Walk_Body_090_0002_png,
                    res_img_satan_walk_090_Walk_Body_090_0006_png,
                    res_img_satan_walk_090_Walk_Body_090_0010_png,
                    res_img_satan_walk_090_Walk_Body_090_0014_png,
                    res_img_satan_walk_090_Walk_Body_090_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_090_Attack1_Body_090_0002_png,
                    res_img_satan_attack_090_Attack1_Body_090_0006_png,
                    res_img_satan_attack_090_Attack1_Body_090_0010_png,
                    res_img_satan_attack_090_Attack1_Body_090_0014_png,
                    res_img_satan_attack_090_Attack1_Body_090_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_090_Death_Body_090_0001_png,
                    res_img_satan_death_090_Death_Body_090_0007_png,
                    res_img_satan_death_090_Death_Body_090_0013_png,
                    res_img_satan_death_090_Death_Body_090_0019_png,
                    res_img_satan_death_090_Death_Body_090_0025_png,
                }
            },
            [2][2] = { // x == 1, y == 1
                [IDLE] = {
                    res_img_satan_idle_135_Idle_Body_135_0003_png,
                    res_img_satan_idle_135_Idle_Body_135_0008_png,
                    res_img_satan_idle_135_Idle_Body_135_0013_png,
                    res_img_satan_idle_135_Idle_Body_135_0018_png,
                    res_img_satan_idle_135_Idle_Body_135_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_135_Walk_Body_135_0002_png,
                    res_img_satan_walk_135_Walk_Body_135_0006_png,
                    res_img_satan_walk_135_Walk_Body_135_0010_png,
                    res_img_satan_walk_135_Walk_Body_135_0014_png,
                    res_img_satan_walk_135_Walk_Body_135_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_135_Attack1_Body_135_0002_png,
                    res_img_satan_attack_135_Attack1_Body_135_0006_png,
                    res_img_satan_attack_135_Attack1_Body_135_0010_png,
                    res_img_satan_attack_135_Attack1_Body_135_0014_png,
                    res_img_satan_attack_135_Attack1_Body_135_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_135_Death_Body_135_0001_png,
                    res_img_satan_death_135_Death_Body_135_0007_png,
                    res_img_satan_death_135_Death_Body_135_0013_png,
                    res_img_satan_death_135_Death_Body_135_0019_png,
                    res_img_satan_death_135_Death_Body_135_0025_png,
                }
            },
            [1][2] = { // x == 0, y == 1
                [IDLE] = {
                    res_img_satan_idle_180_Idle_Body_180_0003_png,
                    res_img_satan_idle_180_Idle_Body_180_0008_png,
                    res_img_satan_idle_180_Idle_Body_180_0013_png,
                    res_img_satan_idle_180_Idle_Body_180_0018_png,
                    res_img_satan_idle_180_Idle_Body_180_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_180_Walk_Body_180_0002_png,
                    res_img_satan_walk_180_Walk_Body_180_0006_png,
                    res_img_satan_walk_180_Walk_Body_180_0010_png,
                    res_img_satan_walk_180_Walk_Body_180_0014_png,
                    res_img_satan_walk_180_Walk_Body_180_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_180_Attack1_Body_180_0002_png,
                    res_img_satan_attack_180_Attack1_Body_180_0006_png,
                    res_img_satan_attack_180_Attack1_Body_180_0010_png,
                    res_img_satan_attack_180_Attack1_Body_180_0014_png,
                    res_img_satan_attack_180_Attack1_Body_180_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_180_Death_Body_180_0001_png,
                    res_img_satan_death_180_Death_Body_180_0007_png,
                    res_img_satan_death_180_Death_Body_180_0013_png,
                    res_img_satan_death_180_Death_Body_180_0019_png,
                    res_img_satan_death_180_Death_Body_180_0025_png,
                }
            },
            [0][2] = { // x == -1, y == 1
                [IDLE] = {
                    res_img_satan_idle_225_Idle_Body_225_0003_png,
                    res_img_satan_idle_225_Idle_Body_225_0008_png,
                    res_img_satan_idle_225_Idle_Body_225_0013_png,
                    res_img_satan_idle_225_Idle_Body_225_0018_png,
                    res_img_satan_idle_225_Idle_Body_225_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_225_Walk_Body_225_0002_png,
                    res_img_satan_walk_225_Walk_Body_225_0006_png,
                    res_img_satan_walk_225_Walk_Body_225_0010_png,
                    res_img_satan_walk_225_Walk_Body_225_0014_png,
                    res_img_satan_walk_225_Walk_Body_225_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_225_Attack1_Body_225_0002_png,
                    res_img_satan_attack_225_Attack1_Body_225_0006_png,
                    res_img_satan_attack_225_Attack1_Body_225_0010_png,
                    res_img_satan_attack_225_Attack1_Body_225_0014_png,
                    res_img_satan_attack_225_Attack1_Body_225_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_225_Death_Body_225_0001_png,
                    res_img_satan_death_225_Death_Body_225_0007_png,
                    res_img_satan_death_225_Death_Body_225_0013_png,
                    res_img_satan_death_225_Death_Body_225_0019_png,
                    res_img_satan_death_225_Death_Body_225_0025_png,
                }
            },
            [0][1] = { // x == -1, y == 0
                [IDLE] = {
                    res_img_satan_idle_270_Idle_Body_270_0003_png,
                    res_img_satan_idle_270_Idle_Body_270_0008_png,
                    res_img_satan_idle_270_Idle_Body_270_0013_png,
                    res_img_satan_idle_270_Idle_Body_270_0018_png,
                    res_img_satan_idle_270_Idle_Body_270_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_270_Walk_Body_270_0002_png,
                    res_img_satan_walk_270_Walk_Body_270_0006_png,
                    res_img_satan_walk_270_Walk_Body_270_0010_png,
                    res_img_satan_walk_270_Walk_Body_270_0014_png,
                    res_img_satan_walk_270_Walk_Body_270_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_270_Attack1_Body_270_0002_png,
                    res_img_satan_attack_270_Attack1_Body_270_0006_png,
                    res_img_satan_attack_270_Attack1_Body_270_0010_png,
                    res_img_satan_attack_270_Attack1_Body_270_0014_png,
                    res_img_satan_attack_270_Attack1_Body_270_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_270_Death_Body_270_0001_png,
                    res_img_satan_death_270_Death_Body_270_0007_png,
                    res_img_satan_death_270_Death_Body_270_0013_png,
                    res_img_satan_death_270_Death_Body_270_0019_png,
                    res_img_satan_death_270_Death_Body_270_0025_png,
                }
            },
            [0][0] = { // x == -1, y == -1
                [IDLE] = {
                    res_img_satan_idle_315_Idle_Body_315_0003_png,
                    res_img_satan_idle_315_Idle_Body_315_0008_png,
                    res_img_satan_idle_315_Idle_Body_315_0013_png,
                    res_img_satan_idle_315_Idle_Body_315_0018_png,
                    res_img_satan_idle_315_Idle_Body_315_0023_png,
                },
                [WALK] = {
                    res_img_satan_walk_315_Walk_Body_315_0002_png,
                    res_img_satan_walk_315_Walk_Body_315_0006_png,
                    res_img_satan_walk_315_Walk_Body_315_0010_png,
                    res_img_satan_walk_315_Walk_Body_315_0014_png,
                    res_img_satan_walk_315_Walk_Body_315_0018_png,
                },
                [ATK1] = {
                    res_img_satan_attack_315_Attack1_Body_315_0002_png,
                    res_img_satan_attack_315_Attack1_Body_315_0006_png,
                    res_img_satan_attack_315_Attack1_Body_315_0010_png,
                    res_img_satan_attack_315_Attack1_Body_315_0014_png,
                    res_img_satan_attack_315_Attack1_Body_315_0018_png,
                },
                [DIE_] = {
                    res_img_satan_death_315_Death_Body_315_0001_png,
                    res_img_satan_death_315_Death_Body_315_0007_png,
                    res_img_satan_death_315_Death_Body_315_0013_png,
                    res_img_satan_death_315_Death_Body_315_0019_png,
                    res_img_satan_death_315_Death_Body_315_0025_png,
                }
            },
        }
    };

    satan_data = p;
};
