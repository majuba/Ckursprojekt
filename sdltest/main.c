/*
 *  main.c
 *  sdltest
 *
 *  Created by Mark Backhaus on 03.04.18. ✈️✈️✈️
 *  Copyright © 2018 Mark Backhaus. All rights reserved.
 *
 *  TODO:
 *
 *      - change sprite for movement direction
 *      - render multiple objects preferably with a loop and a function
 *      - bullet sprites
 *      - Background
 *      - scrolling background
 *
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include "gameObjs.h"

const int BULLET_SPEED = 20;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_BULLETS = 50;
const int MAX_CHICKENS = 20;

void update_gameObj(gameObj *p)
{
    //updates positions of gameobjects, checking for boundaries as well as destroying bullets
    switch(p->type)
    {
        case PLAYER:
            if(p->position.x + p->dx > SCREEN_WIDTH - p->position.w || p->position.x + p->dx < 0){
                p->dx = 0;
                return;
            }
            if(p->position.y + p->dy >= SCREEN_HEIGHT - p->position.h || p->position.y + p->dy <= 0){
                p->dy = 0;
                return;
            }
            p->position.x += (int) p->dx;
            p->position.y += (int) p->dy;
            break;
        case BULLET:
            if(p->position.x > SCREEN_WIDTH){
                p->visible = 0;
                break;
            }
            p->position.x += (int) p->dx;
            break;
        case CHICKEN:
            if(p->position.x < - p->position.w){
                p->visible = 0;
            }
            p->position.x += (int) p->dx;
    }
    return;
};

int main(int argc, const char * argv[]) {
    //initializing
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *spriteSheet = NULL;
    SDL_Texture *chicken_cooked = NULL;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not init, SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    
    window = SDL_CreateWindow("Absolutely not Gradius",
                              100, /*pos x*/
                              100, /*pos y*/
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_ALWAYS_ON_TOP);
    
    if(window == NULL)
    {
        fprintf(stderr, "window could not be created! SDL ERR: %s\n", SDL_GetError());
        exit(1);
    }
    
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    
    if(renderer == NULL){
        fprintf(stderr, "error initializing renderer! SDL ERR: %s \n", SDL_GetError());
        exit(1);
    }
    
    spriteSheet = load_Texture("./assets/plane.bmp", renderer);
    chicken_cooked = load_Texture("./assets/tinychicken_cooked.bmp", renderer);
    
    if(spriteSheet == NULL){
        fprintf(stderr, "error loading spritesheet! SDL ERR: %s \n", SDL_GetError());
        exit(1);
    }
    
    int quit = 0;
    double scale = 1.5;
    SDL_Event e;
    //struct objList enemies = init_objList(20);
    gameObj *bullets = malloc(sizeof(gameObj) * MAX_BULLETS);
    gameObj *chickens = malloc(sizeof(gameObj) * MAX_CHICKENS);
    
    //temporary "0-bullet"
    gameObj tempBullets = {
        .visible = 0
    };
    
    //initializing objList with "0-bullets"
    for(size_t i = 0; i < MAX_BULLETS; i++){
        bullets[i] = tempBullets;
    }
    
    //initializing chickenlist
    
    gameObj temp_chicken = {
        .visible = 0,
    };
    
    for(size_t i = 0; i < MAX_CHICKENS; i++){
        chickens[i] = temp_chicken;
    }
    
    gameObj player = {
        .source = {
            .x = 16,
            .y = 47,
            .w = 51,
            .h = 25,
        },
        .position = {
            .x = 50,
            .y = 50,
            .w = player.source.w * scale,
            .h = player.source.h * scale,
        },
        .dx = 0,
        .dy = 0,
        .visible = 1,
        .type = PLAYER,
    };
    
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    int vel = 1;
    
    gameObj bullet = {
        .type = BULLET,
        .position = {
            .w = 20,
        }
    };
    
    gameObj chicken = {
        .type = CHICKEN,
        .source = {
            .x = 3,
            .y = 10,
            .w = 100,
            .h = 100,
        },
        .position = {
            .x = SCREEN_WIDTH + chicken.source.w,
            .w = 50,
            .h = 40,
        }
    };
    
    srand(time(NULL));
    
    while( !quit )
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        
        deltaTime = ((NOW - LAST) / (double)SDL_GetPerformanceFrequency() );
        
        bullet.dx = 2;
        
        while( SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_w:
                        player.dy = - (deltaTime * vel);
                        break;
                    case SDLK_s:
                        player.dy = (deltaTime * vel);
                        break;
                    case SDLK_a:
                        player.dx = - (deltaTime * vel);
                        break;
                    case SDLK_d:
                        player.dx = (deltaTime * vel);
                        break;
                    case SDLK_SPACE:
                        bullet.position.x = player.position.x + player.position.w - 5;
                        bullet.position.y = player.position.y + player.position.h - 2;
                        bullet.visible = 1;
                        objList_write(bullets, bullet, MAX_BULLETS);
                }
            }
            else if (e.type == SDL_KEYUP)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_w:
                    case SDLK_s:
                        player.dy = 0;
                        break;
                    case SDLK_a:
                    case SDLK_d:
                        player.dx = 0;
                        break;
                        
                }
            }
        }
        
        //rounding speed away from 0 so the negative values dont become 0
        
        player.dx = (player.dx) < 0 ? floor(player.dx) : ceil(player.dx);
        player.dy = (player.dy) < 0 ? floor(player.dy) : ceil(player.dy);
        
        //updating player position
        update_gameObj(&player);
        
        //updating bullet positions
        for(size_t i = 0; i <= MAX_BULLETS; i++){
            if(bullets[i].visible)
                update_gameObj(&bullets[i]);
        }
        
        chicken.position.y = rand() % SCREEN_HEIGHT;
        chicken.dx = -1 * deltaTime;
        chicken.visible = 1;
        
        if(rand() % 100 == 0)
            objList_write(chickens, chicken, MAX_CHICKENS);
        
        for(size_t i = 0; i <= MAX_CHICKENS; i++){
            if(chickens[i].visible){
                chickens[i].dx = chickens[i].dx < 0 ? floor(chickens[i].dx) : ceil(chickens[i].dx);
                update_gameObj(&chickens[i]);
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); //white background
        SDL_RenderClear(renderer);

        //bullet color
        SDL_SetRenderDrawColor(renderer, 255, 200, 50, 255);
        //rendering bullets as lines :: TODO: sprites
        SDL_Rect pos;
        for(size_t i = 0; i < MAX_BULLETS; i++){
            if(bullets[i].visible){
                pos = bullets[i].position;
                SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + pos.w, pos.y);
            }
        }
        
        for(size_t i = 0; i < MAX_CHICKENS; i++){
            if(chickens[i].visible)
                SDL_RenderCopy(renderer, chicken_cooked, &chickens[i].source, &chickens[i].position);
        }
        
        SDL_RenderCopy(renderer, spriteSheet, &player.source, &player.position);
        SDL_RenderPresent(renderer);
    }
    //destroying and quitting
    free(bullets);
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

