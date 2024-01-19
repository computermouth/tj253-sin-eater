
#include "res.h"
#include "satan.h"

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
        ATK2, // non-anim
        DASH, // non-anim
} phase;

typedef struct {
    Texture textures[3][3][ 4][ 5];
    int last_xdir;
    int last_ydir;
    int curr_xdir;
    int curr_ydir;
    int health;
    float pos_x;
    float pos_y;
    phase animation;
    float anim_start;
    float atk1_last;
    float atk1_cool;
    float atk2_last;
    float atk2_cool;
    float dash_last;
    float dash_cool;
} pd_t;

static pd_t player_data = { 0 };

int player_get_health(){
    return player_data.health;
}

void player_kill(){
    player_data.animation = DIE_;
}

Vector2 player_dir_from(float px, float py){
    Vector2 v =  Vector2Normalize(Vector2Subtract((Vector2){px + 128, py + 128}, (Vector2){player_data.pos_x + 128, player_data.pos_y + 128}));
    v.x = roundf(v.x);
    v.y = roundf(v.y);
    return Vector2Negate(v);
}

float player_distance(float px, float py){
    return Vector2Distance((Vector2){px, py}, (Vector2){player_data.pos_x, player_data.pos_y});
}

void player_health_down(int m){
    player_data.health -= m;
}

void player_update(){

    // movement
    int xdir = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    int ydir = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

    // if attacking, return
    if (player_data.animation == ATK1 && (GetTime() - player_data.anim_start) <= 0.5)
        return;
    if (player_data.animation == ATK2 && (GetTime() - player_data.anim_start) <= 1.0)
        return;

    player_data.pos_x += xdir * 200 * GetFrameTime();
    player_data.pos_y += ydir * 200 * GetFrameTime();

    player_data.pos_x = Clamp(player_data.pos_x, 0, 1280 - 256);
    player_data.pos_y = Clamp(player_data.pos_y, 0,  720 - 256);

    if (player_data.animation == DASH && (GetTime() - player_data.anim_start) <= 0.25){
        player_data.pos_x += xdir * 500 * GetFrameTime();
        player_data.pos_y += ydir * 500 * GetFrameTime();

        player_data.pos_x = Clamp(player_data.pos_x, 0, 1280 - 256);
        player_data.pos_y = Clamp(player_data.pos_y, 0,  720 - 256);
        return;
    }

    // anims
    if(xdir == 0 && ydir == 0){
		player_data.curr_xdir = player_data.last_xdir;
		player_data.curr_ydir = player_data.last_ydir;
		player_data.animation = IDLE;
	} else {
		player_data.animation = WALK;
	}

	if (IsKeyDown(KEY_J) && (player_data.animation == WALK || player_data.animation == IDLE) && (player_data.atk1_last + player_data.atk1_cool) < GetTime()){
		player_data.animation = ATK1;
        player_data.anim_start = GetTime();
        player_data.atk1_last = GetTime();

        if(satan_distance(player_data.pos_x, player_data.pos_y) < 128)
            satan_health_down(2);
    }

	if (IsKeyDown(KEY_L) && (player_data.animation == WALK || player_data.animation == IDLE) && (player_data.dash_last + player_data.dash_cool) < GetTime()){
		player_data.animation = DASH;
        player_data.anim_start = GetTime();
        player_data.dash_last = GetTime();
    }

	if (IsKeyDown(KEY_K) && (player_data.animation == WALK || player_data.animation == IDLE) && (player_data.atk2_last + player_data.atk2_cool) < GetTime()){
		player_data.animation = ATK2;
        player_data.anim_start = GetTime();
        player_data.atk2_last = GetTime();

        if(satan_distance(player_data.pos_x, player_data.pos_y) < 256)
            satan_health_down(5);
    }

    // direction to face
    Vector2 dir = satan_dir_from(player_data.pos_x, player_data.pos_y);
    player_data.curr_xdir = dir.x + 1;
    player_data.curr_ydir = dir.y + 1;

}

void player_draw(){

    int xd = player_data.curr_xdir;
    int yd = player_data.curr_ydir;
    int ap = player_data.animation;

    int xp = player_data.pos_x;
    int yp = player_data.pos_y;

    float at = player_data.anim_start;
    int frame = fmodf((GetTime() - at) * 10, 5); // half second from start to finish

    Color c = WHITE;
    if (ap == DASH){
        ap = WALK;
        c = BLUE;
    } else if (ap == ATK2){
        ap = IDLE;
        c = RED;
        //                        player.w / 2 - fire.w / 2
        Color firecolor = WHITE;
        // todo, fade
        // DrawTexture(fire__tex, player_data.pos_x - 128 - 128, player_data.pos_y - 128 - 64, firecolor);
        DrawTexture(fire__tex, player_data.pos_x - 128, player_data.pos_y, firecolor);
        frame = 0;
    }


    typedef enum {
        CLOCK,
        DASH,
        SUPER,
        SWORD,
    } img;

    Texture * t[4] = {&clock_tex, &dash__tex, &super_tex, &sword_tex};

    img atk1 = CLOCK;
    img atk2 = CLOCK;
    img dash = CLOCK;

    if((player_data.atk1_last + player_data.atk1_cool) <= GetTime())
        atk1 = SWORD;
    if((player_data.atk2_last + player_data.atk2_cool) <= GetTime())
        atk2 = SUPER;
    if((player_data.dash_last + player_data.dash_cool) <= GetTime())
        dash = DASH;

    DrawTexture(player_data.textures[xd][yd][ap][frame], xp, yp, c);

    DrawText("J", 960 - 100 + 32, 550, 30, BLACK);
    DrawTexturePro(*t[atk1], (Rectangle){0, 0, 16, 16}, (Rectangle){960 - 100, 600, 64, 64}, (Vector2){0}, 0, WHITE);
    DrawText("K", 960 - 000 + 32, 550, 30, BLACK);
    DrawTexturePro(*t[atk2], (Rectangle){0, 0, 16, 16}, (Rectangle){960 - 000, 600, 64, 64}, (Vector2){0}, 0, WHITE);
    DrawText("L", 960 + 100 + 32, 550, 30, BLACK);
    DrawTexturePro(*t[dash], (Rectangle){0, 0, 16, 16}, (Rectangle){960 + 100, 600, 64, 64}, (Vector2){0}, 0, WHITE);

    char h[20];
    sprintf(h, "health: %d", player_data.health);
    DrawText(h, 20, 20, 20, BLACK);

}

void player_init(){
    pd_t p = {
        .last_xdir = 2,
        .last_ydir = 0,
        .curr_xdir = 2,
        .curr_ydir = 0,
        .pos_x = 100,
        .pos_y = 400,
        .health = 20,
        .animation = IDLE,
        .anim_start = GetTime(),
        .atk1_last = GetTime(),
        .atk1_cool = 2,
        .atk2_last = GetTime(),
        .atk2_cool = 7,
        .dash_last = GetTime(),
        .dash_cool = 3,
        .textures = {
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

    player_data = p;
};
