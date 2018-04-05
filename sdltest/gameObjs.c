//
//  gameObjs.c
//  sdltest
//
//  Created by Mark Backhaus on 04.04.18.
//  Copyright © 2018 Mark Backhaus. All rights reserved.
//

#include "gameObjs.h"

void objList_write(gameObj *list, gameObj obj, int maxSize){
    for(size_t i = 0; i < maxSize; i++){
        if(! list[i].visible){
            list[i] = obj;
            return;
        }
    }
    return;
}

SDL_Texture* load_Texture(char *path, SDL_Renderer *renderer){
    SDL_Surface *temp = NULL;
    temp = SDL_LoadBMP(path);
    SDL_Texture *res = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    return res;
}

int check_collsion(SDL_Rect bullet, SDL_Rect chicken){
    if((bullet.x + bullet.w > chicken.x
       && bullet.x + bullet.w < chicken.x + chicken.w
       && bullet.y > chicken.y && bullet.y < chicken.y + chicken.h)
       || (bullet.x > chicken.x && bullet.x < chicken.x + chicken.w
       && bullet.y > chicken.y && bullet.y < chicken.y + chicken.h))
    {
        return 1;
    }
    return 0;
}


