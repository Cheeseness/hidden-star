#ifndef HS_MAIN
#define HS_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "SDL.h" 
#include "SDL_image.h"

typedef struct font_map{
	int x;
	int y;
	int a;
}font_map;

#include "gui.h"
#include "intro.h"
#include "menu.h"
#include "planets.h"
#include "combat.h"
#include "comms.h"
//#include "travel.h"
#include "starmap.h"
#include "user_events.h"
#include "audio.h"

#define main_resX 64
#define main_resY 64

#define P_MODE0 0
#define P_MODE1 1
#define P_MODE2 2
#define NOFLIP 3
#define FLIPV 4
#define FLIPH 5
#define FLIPHV 6
#define TWINK1 10
#define TWINK2 11
#define TWINK3 12
#define TWINK4 13
#define TWINK5 14
#define TWINK6 15

#define FONT_EARTH 0
#define FONT_SNEEB 1
#define FONT_KRULL 2
#define FONT_PLINK 3
#define FONT_CYCLE 4

#define SCENE_INTRO 0
#define SCENE_MAIN_MENU 1
#define SCENE_COMBAT 2
#define SCENE_COMMS 3
#define SCENE_TRAVEL 4
#define SCENE_TRADE 5
#define SCENE_PLANET_GEN 6
#define SCENE_STARMAP 7


SDL_Surface *main_screen;
SDL_Window *main_window;
SDL_Renderer *main_renderer;

int main_scale;
int main_scene;
int frame_skip;

SDL_Texture *font[4];

font_map fonts[256];
char* pref_path;
int screenshot_counter;
Vector *starmap;

SDL_Surface* Load_srf(char *filename);
SDL_Texture* Load_tex(char *filename);
void main_blit(SDL_Texture *tex, int x, int y, int mode, SDL_Color *color);
int draw_text(int x, int y, char *text, int length, int font_set, int font_set2, int translation_offset, SDL_Color color);
void draw_number(int x, int y, int num);
void draw_scene();
void save_screenshot();
int menu_quit();
void main_input(SDL_Event event);
int main_setup();
void main_menu_setup();
char * wrap_text(char *text, int max_width);
int init_game(int i);

#endif
