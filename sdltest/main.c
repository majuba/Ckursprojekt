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

typedef enum objTypes {
    PLAYER,
    ROCK,
    ENEMY
} objTypes;

struct gameObj {
    SDL_Rect source;
    SDL_Rect position;
    double dx, dy;
    int visible;
    objTypes type;
};



SDL_Texture* load_Texture(char *path, SDL_Renderer *renderer){
    SDL_Surface *temp = NULL;
    temp = SDL_LoadBMP(path);
    SDL_Texture *res = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    return res;
}

void update_gameObj(struct gameObj p, SDL_Event e); //TODO

int main(int argc, const char * argv[]) {
    //initializing
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bmpTex = NULL;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not init, SDL ERROR: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("test", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP);
        if(window == NULL)
        {
            printf("window could not be created! SDL ERR: %s\n", SDL_GetError());
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            bmpTex = load_Texture("./assets/shipsheetparts.bmp", renderer);
        
            int quit = 0;
            SDL_Event e;
            struct gameObj *objects = malloc(sizeof(struct gameObj) * 20); //handling up to 20 elements on screen
            
            struct gameObj player = {
                .source = {
                    .x = 20,
                    .y = 12,
                    .w = 45,
                    .h = 35,
                },
                .position = {
                    .x = 50,
                    .y = 50,
                    .w = 45,
                    .h = 35,
                },
                .dx = 0,
                .dy = 0,
                .visible = 1,
                .type = PLAYER
            };
            
            objects[0] = player;
            
            double const angle = 90.0;
            
            Uint64 NOW = SDL_GetPerformanceCounter();
            Uint64 LAST = 0;
            double deltaTime = 0;
            int vel = 1;
            
            while( !quit )
            {
                LAST = NOW;
                NOW = SDL_GetPerformanceCounter();
                
                deltaTime = ((NOW - LAST) / (double)SDL_GetPerformanceFrequency() );
                
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
                
                player.position.x += (int) player.dx;
                player.position.y += (int) player.dy;
                
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
                SDL_RendererFlip flip = SDL_FLIP_NONE;
                
                SDL_Point center;
                center.x = player.position.w / 2;
                center.y = player.position.h / 2;
                SDL_RenderCopyEx(renderer, bmpTex, &player.source, &player.position, angle, &center, flip);
                SDL_RenderPresent(renderer);
            }
            SDL_DestroyTexture(bmpTex);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 0;
        }
    }
}
