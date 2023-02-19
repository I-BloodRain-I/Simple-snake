#pragma once
#ifndef GLOBALS
#define GLOBALS

#include <array>
#include <vector>
#include <string>

#include "SDL.h" // for basic system
#include "SDL_image.h" // for IMG_Load 
#include "SDL_ttf.h" // for TTF_Init, TTF_Font, TTF_CloseFont
#undef main

// structure for game objects, such as food or the body of a snake
struct GameObject_s
{
	SDL_Rect size;
	int degrees;
	SDL_Texture *tex;
};

// structure for easy rendering
// contains pointers to the data,
// so that you don't have to copy objects every time
// you change the original
struct QueueRender_s
{
	const int *degrees;
	const int *x, *y;
	const int *h, *w;
	SDL_Texture **tex;
};

// enum of the snake movement
enum Move { 
	UP, 
	RIGHT, 
	DOWN, 
	LEFT
};

// enum of all types of textures in the game
enum Texture {
	BACKGROUND,
	SCORE_BOARD,
	SCORE,
	SNAKE_HEAD,
	SNAKE_BODY,
	SNAKE_BODY_ROTATION,
	SNAKE_TAIL,
	FOOD,
	COUNT
};

// application window pointer
// is used only when starting an application andclosing it
// to get a variable, the function get_window is implemented
static SDL_Window *window_g{ nullptr };

// pointer to the game renderer
// as window_g is assigned at the beginning of the program anddoes
// not change throughout the work
// to get access, the function get_renderer is implemented
static SDL_Renderer *render_g{ nullptr };

// a dynamic array of all the textures in the game
// unlike the previous global variables, it is used frequently
// and therefore the two functions create_texture, update_texure
// and get_texture are implemented
static std::vector<SDL_Texture *> texture_g;

// window size must be evenly divided by GRID_SCALE
constexpr static int WINDOW_WIDTH{ 1056 }, WINDOW_HEIGHT{ 1056 };

// the size of the conditional table in the game
// on which the snake moves
// and on the basis of which the positioningis implemented
constexpr static int GRID_SCALE{ 48 };

// number of cells in width(index = 0) and height(index = 1)
constexpr static int GRID_SIZE[2]{ WINDOW_WIDTH / GRID_SCALE, WINDOW_HEIGHT / GRID_SCALE };


void init_system();

SDL_Texture *create_texture(SDL_Renderer *rend, const char *tex_path);
SDL_Texture *create_texture(SDL_Renderer *rend, const char *text, SDL_Color text_color,
		const char* font_path, int font_size);
void update_texure(SDL_Texture *tex, int index);

SDL_Window *get_window();
SDL_Renderer *get_renderer();
std::vector<SDL_Texture *> &get_textures();

#endif 
