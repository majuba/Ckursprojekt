/*
 *  main.c
 *  sdltest
 *
 *  Created by Mark Backhaus on 03.04.18.
 *  Copyright © 2018 Mark Backhaus. All rights reserved.
 *
 *  TODO:
 *
 *      - change sprite for movement direction
 *      - render multiple objects preferably with a loop and a function
 *      - shooting
 *      - Background
 *      - scrolling background
 *
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

const int BULLET_SPEED = 20;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_BULLETS = 50;

typedef enum objTypes {
    PLAYER,
    BULLET
} objTypes;

struct gameObj {
    SDL_Rect source;
    SDL_Rect position;
    double dx, dy;
    int visible;
    objTypes type;
};

struct objList {
    struct gameObj *data;
    int free;
};

struct objList init_objList(int size){
    struct objList ret = {
        .data = malloc(sizeof(struct gameObj) * size),
        .free = 0,
    };
    
    return ret;
}

void append_objlist(struct objList *list, struct gameObj obj){
    if(list->free > MAX_BULLETS){
        list->free = 0;
    }
    list->data[list->free] = obj;
    list->free++;
    return;
}

SDL_Texture* load_Texture(char *path, SDL_Renderer *renderer){
    SDL_Surface *temp = NULL;
    temp = SDL_LoadBMP(path);
    SDL_Texture *res = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    return res;
}

void update_gameObj(struct gameObj *p)
{
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
            p->dx = p->dx < 0 ? floor(p->dx) : ceil(p->dx);
            if(p->position.x > SCREEN_WIDTH){
                p->visible = 0;
                break;
            }
            p->position.x += (int) p->dx;
            break;
    }
    return;
};

int main(int argc, const char * argv[]) {
    //initializing
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bmpTex = NULL;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not init, SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    
    window = SDL_CreateWindow("test", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP);
    
    if(window == NULL)
    {
        fprintf(stderr, "window could not be created! SDL ERR: %s\n", SDL_GetError());
        exit(1);
    }
    
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    bmpTex = load_Texture("./assets/plane.bmp", renderer);
    
    int quit = 0;
    double scale = 1.5;
    SDL_Event e;
    //struct objList enemies = init_objList(20);
    struct objList bullets = init_objList(MAX_BULLETS);
    
    struct gameObj player = {
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
    
    struct gameObj bullet = {
        .type = BULLET,
        .position = {
            .w = 20,
        }
    };
    
    while( !quit )
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        
        deltaTime = ((NOW - LAST) / (double)SDL_GetPerformanceFrequency() );
        
        bullet.dx = 10 * deltaTime;
        
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
                        bullet.position.x = player.position.x + player.position.w;
                        bullet.position.y = player.position.y + player.position.h;
                        bullet.visible = 1;
                        append_objlist(&bullets, bullet);
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
        for(size_t i = 0; i <= bullets.free; i++){
            update_gameObj(&bullets.data[i]);
        }
        //player.position.x += (int) player.dx;
        //player.position.y += (int) player.dy;
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); //white background
        SDL_RenderClear(renderer);
        //SDL_RendererFlip flip = SDL_FLIP_NONE;
        
        SDL_Point center;
        center.x = player.position.w / 2;
        center.y = player.position.h / 2;
        //SDL_RenderCopyEx(renderer, bmpTex, &player.source, &player.position, angle, &center, flip);
        SDL_RenderCopy(renderer, bmpTex, &player.source, &player.position);
        SDL_SetRenderDrawColor(renderer, 255, 200, 50, 255);
        for(size_t i = 0; i < bullets.free; i++){
            SDL_RenderDrawLine(renderer,
                               bullets.data[i].position.x,
                               bullets.data[i].position.y,
                               bullets.data[i].position.x + bullets.data[i].position.w,
                               bullets.data[i].position.y + 1);
        }
        
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyTexture(bmpTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

