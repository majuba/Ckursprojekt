//
//  gameObjs.h
//  sdltest
//
//  Created by Mark Backhaus on 04.04.18.
//  Copyright © 2018 Mark Backhaus. All rights reserved.
//

#ifndef gameObjs_h
#define gameObjs_h

#include <stdio.h>
#include <SDL2/SDL.h>

typedef enum objTypes {
    PLAYER,
    BULLET,
    CHICKEN
} objTypes;

typedef struct gameObj {
    SDL_Rect source;
    SDL_Rect position;
    double x, y;
    double dx, dy;
    int visible;
    objTypes type;
} gameObj;

void objList_write(gameObj *list, gameObj obj, int maxSize);
SDL_Texture* load_Texture(char *path, SDL_Renderer *renderer);

#endif /* gameObjs_h */
