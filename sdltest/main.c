//
//  main.c
//  sdltest
//
//  Created by Mark Backhaus on 03.04.18.
//  Copyright © 2018 Mark Backhaus. All rights reserved.
//

#include <stdio.h>
#include <inttypes.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct player {
    SDL_Rect source;
    SDL_Rect position;
};

int main(int argc, const char * argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bmpTex = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Surface *bmpSurface = NULL;
    
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
            screenSurface = SDL_GetWindowSurface(window);
            bmpSurface = SDL_LoadBMP("/Users/markbackhaus/Documents/Studium/SS19/ckurs/sdltest/assets/shipsheetparts.bmp");
            bmpTex = SDL_CreateTextureFromSurface(renderer, bmpSurface);
            SDL_FreeSurface(bmpSurface);
        
            int quit = 0;
            SDL_Event e;
            
            SDL_Rect source;
            source.x = 20;
            source.y = 12;
            source.w = 48;
            source.h = 35;
            
            SDL_Rect destination;
            destination.x = 50;
            destination.y = 50;
            destination.w = source.w;
            destination.h = source.h;
            
            double const angle = 90.0;
            
            while( !quit )
            {
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
                            case SDLK_UP:
                                destination.y -= 10;
                                break;
                            case SDLK_DOWN:
                                destination.y += 10;
                                break;
                            case SDLK_LEFT:
                                destination.x -= 10;
                                break;
                            case SDLK_RIGHT:
                                destination.x += 10;
                                break;
                        }
                    }
                }
                
                
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);
                SDL_RendererFlip flip = SDL_FLIP_NONE;
                
                SDL_Point center;
                center.x = destination.w / 2;
                center.y = destination.h / 2;
                SDL_RenderCopyEx(renderer, bmpTex, &source, &destination, angle, &center, flip);
                //SDL_RenderCopy(renderer, bmpTex, &source, &destination);
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
