
#include "res.h"

#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>

// x -1, 0, 1
// y -1, 0, 1

// x 0, 1, 2
// y 0, 1, 2

float round_to_nearest(float original_number, float increment) {
    float rounded_down = increment * floorf(original_number / increment);
    float rounded_up = increment * ceilf(original_number / increment);

    return (fabsf(original_number - rounded_down) < fabsf(original_number - rounded_up)) ? rounded_down : rounded_up;
}

// x + 1
// y + 1

//      player_tex[X][Y][AN][FR]
typedef struct {
    Texture player_tex[3][3][ 4][ 5];
} pt_s;

typedef enum {
        IDLE,
        WALK,
        ATK1,
        DIE_,
} phase;

pt_s player_textures = { 0 };

static int last_xdir = 2;
static int last_ydir = 2;

void player_draw(){

    int xdir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    int ydir = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
    xdir++;
    ydir++;
    
    phase p = WALK;
    
    // IDLE
    if(xdir == 1 && ydir == 1){
		xdir = last_xdir;
		ydir = last_ydir;
		p = IDLE;
	} else {
		last_xdir = xdir;
		last_ydir = ydir;
	}
	
	if (IsKeyDown(KEY_J))
		p = ATK1;
	if (IsKeyDown(KEY_K))
		p = DIE_;

    DrawTexture(player_textures.player_tex[xdir][ydir][p][(unsigned int)fmodf(GetTime() * 10, 5)], 300, 300, WHITE);
}

void player_init_tex(){
    pt_s p = {
        .player_tex = {
            [1][0] = { // x == 0, y == -1
                [IDLE] = {
                    res_img_orc_idle_000_Idle_Armed_Body_000_0002_png,
                    res_img_orc_idle_000_Idle_Armed_Body_000_0006_png,
                    res_img_orc_idle_000_Idle_Armed_Body_000_0010_png,
                    res_img_orc_idle_000_Idle_Armed_Body_000_0014_png,
                    res_img_orc_idle_000_Idle_Armed_Body_000_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_000_Walk_Armed_Body_000_0002_png,
                    res_img_orc_walk_000_Walk_Armed_Body_000_0006_png,
                    res_img_orc_walk_000_Walk_Armed_Body_000_0010_png,
                    res_img_orc_walk_000_Walk_Armed_Body_000_0014_png,
                    res_img_orc_walk_000_Walk_Armed_Body_000_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_000_Attack_01_Body_000_0002_png,
                    res_img_orc_attack_000_Attack_01_Body_000_0006_png,
                    res_img_orc_attack_000_Attack_01_Body_000_0010_png,
                    res_img_orc_attack_000_Attack_01_Body_000_0014_png,
                    res_img_orc_attack_000_Attack_01_Body_000_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_000_Death_Armed_Body_000_0002_png,
                    res_img_orc_death_000_Death_Armed_Body_000_0006_png,
                    res_img_orc_death_000_Death_Armed_Body_000_0010_png,
                    res_img_orc_death_000_Death_Armed_Body_000_0014_png,
                    res_img_orc_death_000_Death_Armed_Body_000_0018_png,
                }
            },
            [2][0] = { // x == 1, y == -1
                [IDLE] = {
                    res_img_orc_idle_045_Idle_Armed_Body_045_0002_png,
                    res_img_orc_idle_045_Idle_Armed_Body_045_0006_png,
                    res_img_orc_idle_045_Idle_Armed_Body_045_0010_png,
                    res_img_orc_idle_045_Idle_Armed_Body_045_0014_png,
                    res_img_orc_idle_045_Idle_Armed_Body_045_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_045_Walk_Armed_Body_045_0002_png,
                    res_img_orc_walk_045_Walk_Armed_Body_045_0006_png,
                    res_img_orc_walk_045_Walk_Armed_Body_045_0010_png,
                    res_img_orc_walk_045_Walk_Armed_Body_045_0014_png,
                    res_img_orc_walk_045_Walk_Armed_Body_045_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_045_Attack_01_Body_045_0002_png,
                    res_img_orc_attack_045_Attack_01_Body_045_0006_png,
                    res_img_orc_attack_045_Attack_01_Body_045_0010_png,
                    res_img_orc_attack_045_Attack_01_Body_045_0014_png,
                    res_img_orc_attack_045_Attack_01_Body_045_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_045_Death_Armed_Body_045_0002_png,
                    res_img_orc_death_045_Death_Armed_Body_045_0006_png,
                    res_img_orc_death_045_Death_Armed_Body_045_0010_png,
                    res_img_orc_death_045_Death_Armed_Body_045_0014_png,
                    res_img_orc_death_045_Death_Armed_Body_045_0018_png,
                }
            },
            [2][1] = { // x == 1, y == 0
                [IDLE] = {
                    res_img_orc_idle_090_Idle_Armed_Body_090_0002_png,
                    res_img_orc_idle_090_Idle_Armed_Body_090_0006_png,
                    res_img_orc_idle_090_Idle_Armed_Body_090_0010_png,
                    res_img_orc_idle_090_Idle_Armed_Body_090_0014_png,
                    res_img_orc_idle_090_Idle_Armed_Body_090_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_090_Walk_Armed_Body_090_0002_png,
                    res_img_orc_walk_090_Walk_Armed_Body_090_0006_png,
                    res_img_orc_walk_090_Walk_Armed_Body_090_0010_png,
                    res_img_orc_walk_090_Walk_Armed_Body_090_0014_png,
                    res_img_orc_walk_090_Walk_Armed_Body_090_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_090_Attack_01_Body_090_0002_png,
                    res_img_orc_attack_090_Attack_01_Body_090_0006_png,
                    res_img_orc_attack_090_Attack_01_Body_090_0010_png,
                    res_img_orc_attack_090_Attack_01_Body_090_0014_png,
                    res_img_orc_attack_090_Attack_01_Body_090_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_090_Death_Armed_Body_090_0002_png,
                    res_img_orc_death_090_Death_Armed_Body_090_0006_png,
                    res_img_orc_death_090_Death_Armed_Body_090_0010_png,
                    res_img_orc_death_090_Death_Armed_Body_090_0014_png,
                    res_img_orc_death_090_Death_Armed_Body_090_0018_png,
                }
            },
            [2][2] = { // x == 1, y == 1
                [IDLE] = {
                    res_img_orc_idle_135_Idle_Armed_Body_135_0002_png,
                    res_img_orc_idle_135_Idle_Armed_Body_135_0006_png,
                    res_img_orc_idle_135_Idle_Armed_Body_135_0010_png,
                    res_img_orc_idle_135_Idle_Armed_Body_135_0014_png,
                    res_img_orc_idle_135_Idle_Armed_Body_135_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_135_Walk_Armed_Body_135_0002_png,
                    res_img_orc_walk_135_Walk_Armed_Body_135_0006_png,
                    res_img_orc_walk_135_Walk_Armed_Body_135_0010_png,
                    res_img_orc_walk_135_Walk_Armed_Body_135_0014_png,
                    res_img_orc_walk_135_Walk_Armed_Body_135_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_135_Attack_01_Body_135_0002_png,
                    res_img_orc_attack_135_Attack_01_Body_135_0006_png,
                    res_img_orc_attack_135_Attack_01_Body_135_0010_png,
                    res_img_orc_attack_135_Attack_01_Body_135_0014_png,
                    res_img_orc_attack_135_Attack_01_Body_135_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_135_Death_Armed_Body_135_0002_png,
                    res_img_orc_death_135_Death_Armed_Body_135_0006_png,
                    res_img_orc_death_135_Death_Armed_Body_135_0010_png,
                    res_img_orc_death_135_Death_Armed_Body_135_0014_png,
                    res_img_orc_death_135_Death_Armed_Body_135_0018_png,
                }
            },
            [1][2] = { // x == 0, y == 1
                [IDLE] = {
                    res_img_orc_idle_180_Idle_Armed_Body_180_0002_png,
                    res_img_orc_idle_180_Idle_Armed_Body_180_0006_png,
                    res_img_orc_idle_180_Idle_Armed_Body_180_0010_png,
                    res_img_orc_idle_180_Idle_Armed_Body_180_0014_png,
                    res_img_orc_idle_180_Idle_Armed_Body_180_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_180_Walk_Armed_Body_180_0002_png,
                    res_img_orc_walk_180_Walk_Armed_Body_180_0006_png,
                    res_img_orc_walk_180_Walk_Armed_Body_180_0010_png,
                    res_img_orc_walk_180_Walk_Armed_Body_180_0014_png,
                    res_img_orc_walk_180_Walk_Armed_Body_180_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_180_Attack_01_Body_180_0002_png,
                    res_img_orc_attack_180_Attack_01_Body_180_0006_png,
                    res_img_orc_attack_180_Attack_01_Body_180_0010_png,
                    res_img_orc_attack_180_Attack_01_Body_180_0014_png,
                    res_img_orc_attack_180_Attack_01_Body_180_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_180_Death_Armed_Body_180_0002_png,
                    res_img_orc_death_180_Death_Armed_Body_180_0006_png,
                    res_img_orc_death_180_Death_Armed_Body_180_0010_png,
                    res_img_orc_death_180_Death_Armed_Body_180_0014_png,
                    res_img_orc_death_180_Death_Armed_Body_180_0018_png,
                }
            },
            [0][2] = { // x == -1, y == 1
                [IDLE] = {
                    res_img_orc_idle_225_Idle_Armed_Body_225_0002_png,
                    res_img_orc_idle_225_Idle_Armed_Body_225_0006_png,
                    res_img_orc_idle_225_Idle_Armed_Body_225_0010_png,
                    res_img_orc_idle_225_Idle_Armed_Body_225_0014_png,
                    res_img_orc_idle_225_Idle_Armed_Body_225_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_225_Walk_Armed_Body_225_0002_png,
                    res_img_orc_walk_225_Walk_Armed_Body_225_0006_png,
                    res_img_orc_walk_225_Walk_Armed_Body_225_0010_png,
                    res_img_orc_walk_225_Walk_Armed_Body_225_0014_png,
                    res_img_orc_walk_225_Walk_Armed_Body_225_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_225_Attack_01_Body_225_0002_png,
                    res_img_orc_attack_225_Attack_01_Body_225_0006_png,
                    res_img_orc_attack_225_Attack_01_Body_225_0010_png,
                    res_img_orc_attack_225_Attack_01_Body_225_0014_png,
                    res_img_orc_attack_225_Attack_01_Body_225_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_225_Death_Armed_Body_225_0002_png,
                    res_img_orc_death_225_Death_Armed_Body_225_0006_png,
                    res_img_orc_death_225_Death_Armed_Body_225_0010_png,
                    res_img_orc_death_225_Death_Armed_Body_225_0014_png,
                    res_img_orc_death_225_Death_Armed_Body_225_0018_png,
                }
            },
            [0][1] = { // x == -1, y == 0
                [IDLE] = {
                    res_img_orc_idle_270_Idle_Armed_Body_270_0002_png,
                    res_img_orc_idle_270_Idle_Armed_Body_270_0006_png,
                    res_img_orc_idle_270_Idle_Armed_Body_270_0010_png,
                    res_img_orc_idle_270_Idle_Armed_Body_270_0014_png,
                    res_img_orc_idle_270_Idle_Armed_Body_270_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_270_Walk_Armed_Body_270_0002_png,
                    res_img_orc_walk_270_Walk_Armed_Body_270_0006_png,
                    res_img_orc_walk_270_Walk_Armed_Body_270_0010_png,
                    res_img_orc_walk_270_Walk_Armed_Body_270_0014_png,
                    res_img_orc_walk_270_Walk_Armed_Body_270_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_270_Attack_01_Body_270_0002_png,
                    res_img_orc_attack_270_Attack_01_Body_270_0006_png,
                    res_img_orc_attack_270_Attack_01_Body_270_0010_png,
                    res_img_orc_attack_270_Attack_01_Body_270_0014_png,
                    res_img_orc_attack_270_Attack_01_Body_270_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_270_Death_Armed_Body_270_0002_png,
                    res_img_orc_death_270_Death_Armed_Body_270_0006_png,
                    res_img_orc_death_270_Death_Armed_Body_270_0010_png,
                    res_img_orc_death_270_Death_Armed_Body_270_0014_png,
                    res_img_orc_death_270_Death_Armed_Body_270_0018_png,
                }
            },
            [0][0] = { // x == -1, y == -1
                [IDLE] = {
                    res_img_orc_idle_315_Idle_Armed_Body_315_0002_png,
                    res_img_orc_idle_315_Idle_Armed_Body_315_0006_png,
                    res_img_orc_idle_315_Idle_Armed_Body_315_0010_png,
                    res_img_orc_idle_315_Idle_Armed_Body_315_0014_png,
                    res_img_orc_idle_315_Idle_Armed_Body_315_0018_png,
                },
                [WALK] = {
                    res_img_orc_walk_315_Walk_Armed_Body_315_0002_png,
                    res_img_orc_walk_315_Walk_Armed_Body_315_0006_png,
                    res_img_orc_walk_315_Walk_Armed_Body_315_0010_png,
                    res_img_orc_walk_315_Walk_Armed_Body_315_0014_png,
                    res_img_orc_walk_315_Walk_Armed_Body_315_0018_png,
                },
                [ATK1] = {
                    res_img_orc_attack_315_Attack_01_Body_315_0002_png,
                    res_img_orc_attack_315_Attack_01_Body_315_0006_png,
                    res_img_orc_attack_315_Attack_01_Body_315_0010_png,
                    res_img_orc_attack_315_Attack_01_Body_315_0014_png,
                    res_img_orc_attack_315_Attack_01_Body_315_0018_png,
                },
                [DIE_] = {
                    res_img_orc_death_315_Death_Armed_Body_315_0002_png,
                    res_img_orc_death_315_Death_Armed_Body_315_0006_png,
                    res_img_orc_death_315_Death_Armed_Body_315_0010_png,
                    res_img_orc_death_315_Death_Armed_Body_315_0014_png,
                    res_img_orc_death_315_Death_Armed_Body_315_0018_png,
                }
            },
        }
    };

    player_textures = p;
};
