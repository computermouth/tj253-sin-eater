/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "res.h"
#include "player.h"
#include "satan.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;

static RenderTexture2D target = { 0 };  // Render texture to render our game

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

Material cube_mat = { 0 };

typedef enum {
    MENU,
    PLAY,
    OVER,
} state;

state gs = MENU;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#define _DEBUG 1
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "trijam253");

    resource_init();
    satan_init();
    player_init();
    
    Texture cube_tex = LoadTexture("res/grass_tile.png");
    cube_mat = LoadMaterialDefault();
    SetMaterialTexture(&cube_mat, MATERIAL_MAP_DIFFUSE, cube_tex);
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    SetRandomSeed(GetTime());

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    
    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------

    if (gs == PLAY){
        satan_update();
        player_update();
    }

    if (player_get_health() <= 0){
        gs = OVER;
        player_kill();
    }
    if (satan_get_health() <= 0){
        gs = OVER;
        satan_kill();
    }

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture, 
    // it could be useful for scaling or further sahder postprocessing
    BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        
        // TODO: Draw your game screen here
        DrawRectangle(10, 10, screenWidth - 20, screenHeight - 20, LIGHTGRAY);
        player_draw();
        satan_draw();

        if (gs == MENU) {
            // hack
            player_init();
            satan_init();
            DrawText("Satan: Fight me, loser buys lunch", 1280 / 2 - MeasureText("Satan: Fight me, loser buys lunch", 30) / 2, 100, 30, BLACK);
            if(GuiButton((Rectangle){1280. / 2 - 64, 720. / 2 - 64, 128, 128}, "IT'S A DEAL, SATAN!")){
                gs = PLAY;
            }
        }

        if (gs == OVER) {
            // hack
            if (player_get_health() <= 0) {
                DrawText("YOU LOSE, BUY LUNCH", 1280 / 2 - MeasureText("YOU LOSE, BUY LUNCH", 50) / 2, 100, 50, RED);
            } else {
                DrawText("YOU WIN, SATAN BUYS LUNCH", 1280 / 2 - MeasureText("YOU WIN, SATAN BUYS LUNCH", 50) / 2, 100, 50, GREEN);
            }
            if(GuiButton((Rectangle){1280. / 2 - 32, 720. / 2 - 32, 64, 64}, "AGAIN")){
                gs = PLAY;
                player_init();
                satan_init();
            }
        }
        
    EndTextureMode();
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        // TODO: Draw everything that requires to be drawn at this point, maybe UI?

    EndDrawing();
    //----------------------------------------------------------------------------------  
}
