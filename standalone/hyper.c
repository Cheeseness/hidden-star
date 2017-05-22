// gcc -std=c99 -o tunnel tunnel.c -O2 -lSDL2_image `sdl2-config --cflags --libs` -lm

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"

SDL_Event event;
SDL_Surface *screen;
SDL_Window *window;
SDL_Renderer *main_renderer;

int scale=8;
int resX=64;
int resY=64;
int direction=0;
float speed=0;
float loc_x;
float loc_y;
int found_exit=0;

/*        y,  x
int blank[10][10] = {  
   {0, 0, 0, 0, 5, 5, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5} ,
   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 5, 5, 0, 0, 0, 0}
};
*/

int cross1[10][10] = {  
   {0, 0, 0, 7, 5, 5, 6, 0, 0, 0} ,
   {8, 8, 0, 1, 5, 3, 0, 0, 8, 8} ,
   {5, 5, 6, 4, 5, 9, 0, 4, 5, 5} ,
   {5, 5, 9, 7, 5, 5, 9, 7, 5, 5} ,
   {1, 5, 5, 5, 5, 5, 5, 5, 5, 3} ,
   {0, 2, 1, 5, 5, 5, 3, 2, 2, 0} ,
   {0, 0, 0, 1, 5, 5, 9, 0, 0, 0} ,
   {0, 0, 0, 0, 1, 5, 5, 6, 0, 0} ,
   {0, 0, 0, 0, 7, 5, 3, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0}
};

int cross2[10][10] = {  
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {8, 8, 0, 4, 5, 5, 9, 8, 8, 8} ,
   {5, 5, 6, 0, 1, 5, 5, 5, 5, 5} ,
   {5, 5, 9, 0, 0, 1, 5, 5, 5, 5} ,
   {1, 5, 5, 6, 0, 0, 1, 5, 5, 3} ,
   {4, 5, 5, 6, 0, 0, 7, 5, 3, 0} ,
   {0, 1, 5, 9, 8, 7, 5, 5, 6, 0} ,
   {0, 4, 5, 5, 5, 5, 5, 5, 6, 0} ,
   {0, 0, 1, 5, 5, 5, 5, 3, 0, 0} ,
   {0, 0, 0, 1, 5, 5, 3, 0, 0, 0}
};

int cross3[10][10] = {  
   {0, 8, 8, 7, 5, 5, 9, 8, 8, 0} ,
   {7, 5, 5, 5, 5, 5, 5, 5, 5, 9} ,
   {5, 5, 3, 2, 2, 2, 2, 1, 5, 5} ,
   {5, 5, 6, 0, 0, 8, 0, 4, 5, 5} ,
   {1, 5, 6, 0, 4, 5, 6, 4, 5, 3} ,
   {4, 5, 9, 8, 7, 5, 6, 4, 5, 6} ,
   {4, 5, 5, 5, 5, 3, 0, 7, 5, 6} ,
   {0, 2, 2, 2, 2, 8, 7, 5, 5, 6} ,
   {0, 0, 0, 0, 7, 5, 5, 5, 3, 0} ,
   {0, 0, 0, 4, 5, 5, 3, 2, 0, 0}
};

int cross4[10][10] = {  
   {0, 0, 8, 7, 5, 5, 6, 0, 0, 0} ,
   {8, 7, 5, 5, 5, 5, 9, 8, 8, 8} ,
   {5, 5, 5, 3, 1, 5, 5, 5, 5, 5} ,
   {5, 5, 5, 6, 4, 5, 5, 5, 5, 5} ,
   {2, 1, 5, 9, 4, 5, 5, 3, 2, 2} ,
   {0, 0, 1, 5, 9, 2, 2, 0, 0, 0} ,
   {0, 0, 4, 5, 5, 9, 0, 0, 0, 0} ,
   {0, 8, 7, 5, 5, 5, 6, 0, 0, 0} ,
   {4, 5, 5, 5, 5, 5, 6, 0, 0, 0} ,
   {0, 2, 2, 1, 5, 5, 6, 0, 0, 0}
};

int cross5[10][10] = {  
   {0, 0, 0, 7, 5, 5, 9, 0, 0, 0} ,
   {8, 0, 4, 5, 5, 5, 5, 6, 8, 8} ,
   {5, 9, 7, 5, 3, 2, 2, 7, 5, 5} ,
   {5, 5, 5, 3, 0, 8, 7, 5, 5, 5} ,
   {1, 5, 5, 6, 4, 5, 5, 5, 3, 2} ,
   {4, 5, 5, 6, 0, 2, 1, 5, 6, 0} ,
   {4, 5, 5, 9, 8, 8, 7, 5, 6, 0} ,
   {0, 1, 5, 5, 5, 5, 5, 5, 6, 0} ,
   {0, 0, 1, 5, 5, 5, 3, 2, 0, 0} ,
   {0, 0, 0, 1, 5, 5, 6, 0, 0, 0}
};

int east1[10][10] = {  
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {8, 8, 8, 7, 5, 5, 9, 8, 0, 0} ,
   {5, 5, 5, 5, 5, 5, 5, 5, 9, 0} ,
   {5, 5, 5, 5, 3, 2, 1, 5,11, 6} ,
   {2, 2, 2, 2, 0, 8, 7, 5,11, 6} ,
   {0, 0, 0, 0, 4, 5, 5, 5,11, 6} ,
   {0, 0, 0, 0, 7, 5, 5, 5,11, 6} ,
   {0, 0, 0, 4, 5, 5, 3, 2, 2, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0}
};

int east2[10][10] = {  
   {0, 0, 0, 0, 8, 8, 8, 0, 0, 0} ,
   {8, 8, 8, 7, 5, 5, 5, 9, 0, 0} ,
   {5, 5, 5, 5, 5, 5, 5, 5, 6, 0} ,
   {5, 5, 5, 3, 1, 5, 5, 5, 6, 0} ,
   {2, 2, 2, 0, 4, 5,12, 5, 6, 0} ,
   {0, 0, 0, 0, 4, 5, 5, 5, 6, 0} ,
   {0, 0, 0, 0, 4, 5, 3, 2, 0, 0} ,
   {0, 0, 0, 0, 0, 2, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int north1[10][10] = {  
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {8, 8, 8, 8, 8, 0, 0, 8, 8, 8} ,
   {5, 5,11,11,11, 6, 4, 5, 5, 5} ,
   {5, 5,11,11, 5, 9, 7, 5, 5, 5} ,
   {2, 2, 2, 1, 5, 5, 5, 3, 2, 2} ,
   {0, 0, 0, 4, 5, 3, 2, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 9, 0, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0}
};

int north2[10][10] = {  
   {0, 0, 0, 8, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 7, 5, 9, 8, 7, 9, 8, 0} ,
   {0, 4,11, 5, 5, 5, 5, 5,11, 6} ,
   {0, 4,11, 3, 1,12, 3, 1,11, 6} ,
   {0, 4,11, 6, 0, 2, 0, 4,11, 6} ,
   {0, 4,11, 9, 8, 8, 8, 7,11, 6} ,
   {0, 4,11, 5, 5, 5, 5, 5,11, 6} ,
   {0, 0, 2, 1, 5, 5, 3, 2, 2, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0}
};

int west1[10][10] = {  
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 9, 8, 8, 8} ,
   {0, 0, 0, 7, 5, 5, 5, 5, 5, 5} ,
   {0, 0, 4, 5, 5, 3, 1,12, 5, 5} ,
   {0, 0, 4, 5, 5, 6, 4, 5, 3, 2} ,
   {0, 0, 4, 5, 5, 6, 4, 5, 6, 0} ,
   {0, 0, 4, 5, 5, 9, 7, 5, 6, 0} ,
   {0, 0, 0, 1, 5, 5,12, 5, 6, 0} ,
   {0, 0, 0, 4, 5, 5, 3, 2, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0}
};

int west2[10][10] = {  
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 8, 8, 8, 8} ,
   {0, 0, 0, 0, 8, 7, 5, 5, 5, 5} ,
   {0, 0, 0, 7, 5, 5, 5, 5, 5, 5} ,
   {0, 0, 7,12, 5, 5, 3, 2, 2, 2} ,
   {0, 4,12,12, 3, 2, 0, 0, 0, 0} ,
   {0, 4,12, 3, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 2, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int south1[10][10] = {  
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {8, 8, 0, 4, 5, 5, 9, 8, 8, 8} ,
   {5, 5, 6, 0, 1, 5, 5, 5, 5, 5} ,
   {5, 5, 9, 0, 0, 1, 5, 5, 5, 5} ,
   {1, 5, 5, 6, 0, 0, 2, 1, 5, 3} ,
   {4, 5, 5, 6, 0, 0, 0, 4, 5, 6} ,
   {4,11,11, 6, 0, 0, 8, 7, 5, 6} ,
   {4,11,11, 9, 8, 7, 5, 5, 5, 6} ,
   {4,11,11, 5, 5, 5, 5, 5, 5, 6} ,
   {0, 2, 2, 1, 3, 1, 3, 2, 2, 0}
};

int south2[10][10] = {  
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 8, 0, 4, 5, 5, 6, 0, 8, 0} ,
   {4,12, 9, 7, 5, 5, 9, 7,11, 6} ,
   {0, 1, 5, 5, 5, 5, 5, 5, 3, 0} ,
   {0, 0, 1, 3, 2, 2, 2, 2, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int nw1[10][10] = {  
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 8, 7, 9, 8, 8, 8, 8} ,
   {0, 0, 4, 5, 5, 5, 5, 5, 5, 5} ,
   {0, 0, 7, 5, 3, 2, 1, 5, 5, 5} ,
   {0, 4, 5, 5, 6, 0, 4, 5, 3, 2} ,
   {0, 4, 5, 5, 9, 0, 4,12, 6, 0} ,
   {0, 0, 1,11,11, 6, 0, 2, 0, 0} ,
   {0, 0, 4,11,11, 9, 0, 0, 0, 0} ,
   {0, 0, 0, 1, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0}
};

int ne1[10][10] = {  
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {8, 8, 8, 8, 0, 0, 8, 8, 0, 0} ,
   {5, 5,11,11, 9, 7, 5, 5, 6, 0} ,
   {5, 5,11,11, 5, 5, 3, 2, 0, 0} ,
   {2, 2, 2, 2, 1, 5, 9, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 1, 5, 9, 0, 0} ,
   {0, 0, 0, 0, 0, 7, 5, 5, 6, 0} ,
   {0, 0, 0, 0, 4, 5, 5, 3, 0, 0} ,
   {0, 0, 0, 0, 7, 5, 3, 0, 0, 0} ,
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0}
};

int sw1[10][10] = {  
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {0, 0, 8, 7, 5, 5, 6, 0, 8, 8} ,
   {0, 4, 5, 5, 5, 5, 6, 4, 5, 5} ,
   {0, 0, 2, 2, 1, 5, 6, 4, 5, 5} ,
   {0, 0, 0, 0, 4, 5, 9, 7, 5, 3} ,
   {0, 0, 0, 0, 4, 5, 5, 5, 5, 6} ,
   {0, 0, 0, 0, 0, 1,12,12, 3, 0} ,
   {0, 0, 0, 0, 0, 0, 2, 2, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int se1[10][10] = {  
   {0, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {8, 0, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {5, 9, 0, 4, 5, 5, 6, 0, 0, 0} ,
   {5, 5, 6, 4, 5, 5, 9, 0, 0, 0} ,
   {1, 5, 9, 7, 5,11,11, 6, 0, 0} ,
   {4, 5, 5, 3, 1,11,11, 6, 0, 0} ,
   {0, 2, 2, 0, 0, 1,11, 6, 0, 0} ,
   {0, 0, 0, 0, 0, 4,11, 6, 0, 0} ,
   {0, 0, 0, 0, 0, 4,12, 6, 0, 0} ,
   {0, 0, 0, 0, 0, 0, 2, 0, 0, 0}
};

int map1[30][30];
int ent1[30][30];
int exp1[30][30];

SDL_Texture *ship1;
SDL_Texture *capsule1;
SDL_Texture *ships[6];
SDL_Texture *paths[13];
SDL_Texture *t_stars1;
SDL_Texture *t_stars2;
SDL_Texture *t_stars3;

SDL_Texture* Load_tex(char *filename){
        SDL_Texture* texture = IMG_LoadTexture(main_renderer, filename);
        if (!texture){
                printf("Load image error: %s, %s \n",filename, IMG_GetError());
                exit(0);
        }
        return texture;
}

void main_blit(SDL_Texture *tex, int x, int y, int MODE){
        int w, h, r;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        SDL_Rect rect;
        rect.x=x;
        rect.y=y;
        rect.w=w;
        rect.h=h;
	r=direction+90;
	if(r>=360){r-=360;}
	if(MODE==0){
		r=0;
	}
	SDL_RenderCopyEx(main_renderer, tex, NULL, &rect, r, 0, 0);
}

void draw_pixel(int x, int y, int r, int g, int b){
    SDL_Rect rect;
	rect.x=x;
	rect.y=y;
	rect.w=1;
	rect.h=1;
	SDL_SetRenderDrawColor(main_renderer, r, g, b, 255);
	SDL_RenderFillRect(main_renderer, &rect);
}

void draw_minimap(int tx, int ty){
	SDL_SetRenderDrawBlendMode(main_renderer,SDL_BLENDMODE_BLEND);
    SDL_Rect rect;
        rect.x=0;
        rect.y=0;
        rect.w=64;
        rect.h=64;
	SDL_SetRenderDrawColor(main_renderer, 64, 0, 0, 192);
	SDL_RenderFillRect(main_renderer, &rect);
	rect.w=1;
	rect.h=1;
	for(int x=0;x<30;x++){
		for(int y=0;y<30;y++){
			int z=exp1[y][x];
			int ix=x+(16-tx);
			int iy=y+(16-ty);
			switch (z){
				case 0:
					draw_pixel(ix*2, iy*2, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2, 0, 0, 0);
					draw_pixel(ix*2, iy*2+1, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2+1, 0, 0, 0);
				break;
				case 1:
					draw_pixel(ix*2, iy*2, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2, 64, 64, 255);
					draw_pixel(ix*2, iy*2+1, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 255);
				break;
				case 2:
					draw_pixel(ix*2, iy*2, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2, 64, 64, 255);
					draw_pixel(ix*2, iy*2+1, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2+1, 0, 0, 0);
				break;
				case 3:
					draw_pixel(ix*2, iy*2, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2, 64, 64, 255);
					draw_pixel(ix*2, iy*2+1, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2+1, 0, 0, 0);
				break;
				case 4:
					draw_pixel(ix*2, iy*2, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2, 64, 64, 255);
					draw_pixel(ix*2, iy*2+1, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 255);
				break;
				case 5:
					draw_pixel(ix*2, iy*2, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2, 64, 64, 255);
					draw_pixel(ix*2, iy*2+1, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 255);
				break;
				case 6:
					draw_pixel(ix*2, iy*2, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2, 0, 0, 0);
					draw_pixel(ix*2, iy*2+1, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2+1, 0, 0, 0);
				break;
				case 7:
					draw_pixel(ix*2, iy*2, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2, 64, 64, 255);
					draw_pixel(ix*2, iy*2+1, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 255);
				break;
				case 8:
					draw_pixel(ix*2, iy*2, 0, 0, 0);
					draw_pixel(ix*2+1, iy*2, 0, 0, 0);
					draw_pixel(ix*2, iy*2+1, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 255);
				break;
				case 9:
					draw_pixel(ix*2, iy*2, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2, 0, 0, 0);
					draw_pixel(ix*2, iy*2+1, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 255);
				break;
				case 10:
					draw_pixel(ix*2, iy*2, 64, 128, 64);
					draw_pixel(ix*2+1, iy*2, 64, 255, 64);
					draw_pixel(ix*2, iy*2+1, 64, 255, 64);
					draw_pixel(ix*2+1, iy*2+1, 64, 128, 64);
				break;
				case 11:
					draw_pixel(ix*2, iy*2, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2, 128, 64, 64);
					draw_pixel(ix*2, iy*2+1, 128, 64, 64);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 255);
				break;
				case 12:
					draw_pixel(ix*2, iy*2, 64, 64, 64);
					draw_pixel(ix*2+1, iy*2, 64, 64, 255);
					draw_pixel(ix*2, iy*2+1, 64, 64, 255);
					draw_pixel(ix*2+1, iy*2+1, 64, 64, 64);
				break;
				default:
				break;
			}
		}
	}
					
	draw_pixel(32, 32, 255, 255, 255);
	draw_pixel(33, 32, 255, 255, 255);
	draw_pixel(32, 33, 255, 255, 255);
	draw_pixel(33, 33, 255, 255, 255);
}

void draw_hyperspace(){
	SDL_SetRenderDrawBlendMode(main_renderer,SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	SDL_RenderClear(main_renderer);
	int tx=floor(loc_x/32);
	int ty=floor(loc_y/32);
//	printf("tx %d, ty %d\n",tx, ty);
	for(int ix=-2;ix<3;ix++){
		for(int iy=-2;iy<3;iy++){
			if(ty+iy>=0 && ty+iy<30 && tx+ix>=0 && tx+ix<30){
				int z=map1[ty+iy][tx+ix];
				int e=ent1[ty+iy][tx+ix];
				if(ix > -1 && iy > -1 && ix < 3 && iy <3){
					exp1[ty+iy][tx+ix]=z;
				}
	//			printf("x %d, y %d, z %d\n",tx+ix, ty+iy,z);
				int bx=(((tx*32)-loc_x)+32*ix);
				int by=(((ty*32)-loc_y)+32*iy);
				if(z>0){
					main_blit(paths[(z-1)],bx,by,0);
				}
				if(e>0){
					if(e<6){
						main_blit(ships[(e-1)],bx+16,by+16,0);
					}
					if(e==10){
						main_blit(capsule1,bx+16,by+16,0);
					}
				}
			}
		}
	}
	main_blit(t_stars1,-32,-32,0);
	main_blit(t_stars2,loc_x/-4,loc_y/-4,0);
	main_blit(t_stars3,loc_x/-8,loc_y/-8,0);
	main_blit(ship1,26,26,1);

}


 /* Init and start */
int setup(){
	if (SDL_Init(SDL_INIT_VIDEO) <0){
		 SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	if (SDL_CreateWindowAndRenderer(resX*scale, resY*scale, SDL_WINDOW_RESIZABLE, &window, &main_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return 3;
    }

    SDL_RenderSetLogicalSize(main_renderer, resX, resY);
	SDL_SetWindowTitle(window, "Hyperspace");
	atexit(SDL_Quit);
	srand(time(NULL)); 
	t_stars1=Load_tex("../sprites/stars/stars1_256.png");
	t_stars2=Load_tex("../sprites/stars/stars2_256.png");
	t_stars3=Load_tex("../sprites/stars/stars3_256.png");
	ship1=Load_tex("../sprites/ship1.png");
	capsule1=Load_tex("../sprites/hyperspace/capsule1.png");
	for(int i=0;i<3;i++){
		char filename[64];
		sprintf(filename, "../sprites/ships/ring%d_front.png",(i+1));
		ships[i]=Load_tex(filename);
	}
	for(int i=0;i<3;i++){
		char filename[64];
		sprintf(filename, "../sprites/ships/tri%d_front.png",(i+1));
		ships[i+3]=Load_tex(filename);
	}
	for(int i=0;i<9;i++){
		char filename[64];
		sprintf(filename, "../sprites/hyperspace/32-path%d.png",(i+1));
		paths[i]=Load_tex(filename);
	}
	paths[9]=Load_tex("../sprites/hyperspace/32-exit1.png");
	paths[10]=Load_tex("../sprites/hyperspace/32-nebula.png");
	paths[11]=Load_tex("../sprites/hyperspace/32-debri1.png");
	for(int ix=0;ix<30;ix++){
		for(int iy=0;iy<30;iy++){
			map1[ix][iy]=0;
			ent1[ix][iy]=0;
			exp1[ix][iy]=-1;
		}
	}

	int ran;
	int j=0;
	int k=0;
	int c=0;

	int center=rand()%5+1;
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if(center==1){
				map1[iy+10][ix+10]=cross1[iy][ix];
			}
			if(center==2){
				map1[iy+10][ix+10]=cross2[iy][ix];
			}
			if(center==3){
				map1[iy+10][ix+10]=cross3[iy][ix];
			}
			if(center==4){
				map1[iy+10][ix+10]=cross4[iy][ix];
			}
			if(center==5){
				map1[iy+10][ix+10]=cross5[iy][ix];
			}
		}
	}

	int west=rand()%4+1;
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if(west==1){
				map1[iy+10][ix]=west1[iy][ix];
			}
			if(west==2){
				map1[iy+10][ix]=west2[iy][ix];
			}
			if(west==3){
				map1[iy+10][ix]=nw1[iy][ix];
			}
			if(west==4){
				map1[iy+10][ix]=sw1[iy][ix];
			}
		}
	}

	int north=rand()%4+1;
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if(north==1){
				map1[iy][ix+10]=north1[iy][ix];
			}
			if(north==2){
				map1[iy][ix+10]=north2[iy][ix];
			}
			if(north==3){
				map1[iy][ix+10]=nw1[iy][ix];
			}
			if(north==4){
				map1[iy][ix+10]=ne1[iy][ix];
			}
		}
	}

	int east=rand()%4+1;
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if(east==1){
				map1[iy+10][ix+20]=east1[iy][ix];
			}
			if(east==2){
				map1[iy+10][ix+20]=east2[iy][ix];
			}
			if(east==3){
				map1[iy+10][ix+20]=ne1[iy][ix];
			}
			if(east==4){
				map1[iy+10][ix+20]=se1[iy][ix];
			}
		}
	}

	int south=rand()%4+1;
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if(south==1){
				map1[iy+20][ix+10]=south1[iy][ix];
			}
			if(south==2){
				map1[iy+20][ix+10]=south2[iy][ix];
			}
			if(south==3){
				map1[iy+20][ix+10]=se1[iy][ix];
			}
			if(south==4){
				map1[iy+20][ix+10]=sw1[iy][ix];
			}
		}
	}

	//northwest
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if((west==1 || west==4) && (north==1 || north==4)){
				map1[iy][ix]=nw1[iy][ix];
			}else if(west==1 || west==4){
				map1[iy][ix]=north2[iy][ix];
			}else if(north==1 || north==4){
				map1[iy][ix]=west2[iy][ix];
			}
		}
	}

	//northeast
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if((east==1 || east==4) && (north==1 || north==3)){
				map1[iy][ix+20]=ne1[iy][ix];
			}else if(east==1 || east==4){
				map1[iy][ix+20]=north2[iy][ix];
			}else if(north==1 || north==3){
				map1[iy][ix+20]=east2[iy][ix];
			}
		}
	}

	//southeast
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if((east==1 || east==3) && (south==1 || south==4)){
				map1[iy+20][ix+20]=se1[iy][ix];
			}else if(east==1 || east==3){
				map1[iy+20][ix+20]=south2[iy][ix];
			}else if(south==1 || south==4){
				map1[iy+20][ix+20]=east2[iy][ix];
			}
		}
	}

	//southwest
	for(int ix=0;ix<10;ix++){
		for(int iy=0;iy<10;iy++){
			if((west==1 || west==3) && (south==1 || south==3)){
				map1[iy+20][ix]=sw1[iy][ix];
			}else if(west==1 || west==3){
				map1[iy+20][ix]=south2[iy][ix];
			}else if(south==1 || south==3){
				map1[iy+20][ix]=west2[iy][ix];
			}
		}
	}

	//place exit
	ran=rand()%300+2;
	while(c<ran){
		j++;
		if(j==29){k++;j=0;}
		if(k==29){k=0;}
		if(map1[k][j]==5){
			c++;
		}
	}
	map1[k][j]=10;
	//place player
	c=0; j=0; k=0;
	ran=rand()%300+2;
	while(c<ran){
		j++;
		if(j==29){k++;j=0;}
		if(k==29){k=0;}
		if(map1[k][j]==5){
			c++;
		}
	}
	loc_x=(j*32)-16;
	loc_y=(k*32)-16;

	//place ships
	for(int i=0;i<4;i++){
		c=0; j=0; k=0;
		ran=rand()%300+2;
		while(c<ran){
			j++;
			if(j==29){k++;j=0;}
			if(k==29){k=0;}
			if((map1[k][j]==5 || map1[k][j]==11 || map1[k][j]==12) && ent1[k][j]==0){
				c++;
			}
		}
		ent1[k][j]=rand()%6+1;
	}

	//place pods
	for(int i=0;i<8;i++){
		c=0; j=0; k=0;
		ran=rand()%300+2;
		while(c<ran){
			j++;
			if(j==29){k++;j=0;}
			if(k==29){k=0;}
			if((map1[k][j]==5 || map1[k][j]==11 || map1[k][j]==12) && ent1[k][j]==0){
				c++;
			}
		}
		ent1[k][j]=10;
	}

//print map
/*
	for(int ix=0;ix<30;ix++){
		for(int iy=0;iy<30;iy++){
			if(map1[ix][iy]<10){printf(" ");}
			printf("%d,", map1[ix][iy]);
		}
		printf("\n");
	}
*/
	return 0;
}

int main(int argc, char *argv[]){
	int s=setup();
	if(s > 0){
		return s;
	}

//int time_pos=0;
int current_time;
int last_time = SDL_GetTicks();
int mouseX; int mouseY;
int turn=0;
int speed_toggle=0;
int minimap_toggle=0;

while(event.type != SDL_QUIT){
	SDL_GetMouseState(&mouseX, &mouseY);
	if(SDL_PollEvent(&event)<1){SDL_Delay(10);}else{
		if(event.key.repeat == 0){
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_w:
						speed_toggle=1;
					break;
					case SDLK_s:
						speed_toggle=-1;
					break;
					case SDLK_a:
						turn=-1;
					break;
					case SDLK_d:
						turn=1;
					break;
					case SDLK_e:
						minimap_toggle=1;
					break;
					default:
					break;
				}
			case SDL_MOUSEBUTTONDOWN:
				break;
		}
	}
	}

	current_time=SDL_GetTicks();
	if((current_time-last_time) > 64){
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if(!(state[SDL_SCANCODE_E])){
			minimap_toggle=0;
		}
		if(!(state[SDL_SCANCODE_D] || state[SDL_SCANCODE_A])){
			turn=0;
		}
		if(turn<0){
			direction-=15;
		}
		if(turn>0){
			direction+=15;
		}
		if(direction<0){direction+=360;}
		if(direction>=360){direction-=360;}
		if(!(state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S])){
			speed_toggle=0;
		}
		if(speed_toggle==0){
			speed-=0.1;
		}
		if(speed_toggle<0){
			speed-=2;
		}
		if(speed_toggle>0){
			if(speed<1.5){
				speed=1.5;
			}else{
				speed+=0.1;
			}
		}
		if(speed<=0){speed=0;}
		if(speed>4){speed=4;}
		float d_x=(speed*cos(direction*(3.14159/180)));
		float d_y=(speed*sin(direction*(3.14159/180)));
		//fix for poor velocity calc at low speed before int casts
		if(d_x > 0.4 && d_x < 1){d_x=1;}
		if(d_x < -0.4 && d_x > -1){d_x=-1;}
		if(d_y > 0.4 && d_y < 1){d_y=1;}
		if(d_y < -0.4 && d_y > -1){d_y=-1;}
		int tx=floor((loc_x+32+d_x)/32);
		int ty=floor((loc_y+32+d_y)/32);
		int bx=((tx*32)-(loc_x+d_x))-16;
		int by=((ty*32)-(loc_y+d_y))-16;
		int z=map1[ty][tx];
		int e=ent1[ty][tx];
		int bounce=0;
		int q;
		switch(e){
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				if(bx > -16 && bx < 2 && by > -16 && by <2){
					ent1[ty][tx]=0;
					printf("ohi buddy, wanna fight?\n");
					speed=0;
				}
				break;
			case 10:
				if(bx > -16 && bx < 2 && by > -16 && by <2){
					ent1[ty][tx]=0;
					printf("found cookies!\n");
					speed=0;
				}
			break;
			default:
			break;
		}
		switch(z){
			case 0:
				speed=0;
				bounce=1;
			break;
			case 2:
				direction=270;
				bounce=1;
			break;
			case 4:
				direction=0;
				bounce=1;
			break;
			case 6:
				direction=180;
				bounce=1;
			break;
			case 8:
				direction=90;
				bounce=1;
			break;
			case 1:
					//d=(x-x1)(y2-y1)-(y-y1)(x2-x1)
				q=((bx - -16)*(16 - -16) - (by - -16)*(16 - -16));
				if(q>=0){bounce=1;direction=315;}
			break;
			case 3:
				q=((bx - -16)*(-16 - 16) - (by - 16)*(16 - -16));
				if(q>=0){bounce=1;direction=225;}
			break;
			case 7:
				q=((bx - -16)*(-16 - 16) - (by - 16)*(16 - -16));
				if(q<=0){bounce=1;direction=45;}
			break;
			case 9:
				q=((bx - -16)*(16 - -16) - (by - -16)*(16 - -16));
				if(q<=0){bounce=1;direction=135;}
			break;
			case 10:
				if(found_exit==0){
					loc_x+=d_x;
					loc_y+=d_y;
					printf("you made it!\n");
					found_exit=1;
				}
			break;
			default:
			break;
		}

		if(bounce==1){
			speed=1;
			d_x=(speed*cos(direction*(3.14159/180)));
			d_y=(speed*sin(direction*(3.14159/180)));
		}
		if(found_exit==0){
			loc_x+=(int)d_x;
			loc_y+=(int)d_y;
		}else{
			d_x=0;
			d_y=0;
			if(bx > 0){loc_x++;}
			if(bx < 0){loc_x--;}
			if(by > 0){loc_y++;}
			if(by < 0){loc_y--;}
			direction+=20;
			if(direction>=360){direction-=360;}
		}
		last_time=current_time;
		draw_hyperspace();
		if(minimap_toggle==1){
			draw_minimap(tx, ty);
		}
		SDL_RenderPresent(main_renderer);
	}
}
return 0;
}
