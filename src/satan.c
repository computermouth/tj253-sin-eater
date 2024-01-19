
#include "res.h"

#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>

// x + 1
// y + 1

//          satan_tex[X][Y][AN][FR]
typedef struct {
    Texture satan_tex[3][3][ 4][ 5];
} st_s;

typedef enum {
        IDLE,
        WALK,
        ATK1,
        DIE_,
} phase;

st_s satan_textures = { 0 };

static int last_xdir = 2;
static int last_ydir = 2;

void satan_draw(){

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

    DrawTexture(satan_textures.satan_tex[xdir][ydir][p][(unsigned int)fmodf(GetTime() * 10, 5)], 600, 300, WHITE);
}

void satan_init_tex(){
    st_s p = {
        .satan_tex = {
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

    satan_textures = p;
};
