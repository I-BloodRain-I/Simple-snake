#include "Globals.h"

// global variable getters
SDL_Window *get_window()                  { return window_g; }
SDL_Renderer *get_renderer()              { return render_g; }
std::vector<SDL_Texture *> &get_textures(){ return texture_g; }

// conditional system initialization
// window, renderer and game textures are created
void init_system() {

    // re-execution protection
    if (window_g || render_g) { return; }

    // initialize all SDL systems
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    // creates a window
    window_g = SDL_CreateWindow("Catch Eggs",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0);

    // creates a renderer
    // SDL_RENDERER_ACCELERATED - flag that allows you to use hardware acceleration
    render_g = SDL_CreateRenderer(window_g, -1, SDL_RENDERER_ACCELERATED);

    // change the size of the array beforehand
    // this is not crucial
    // because you can increase the size in passing using texture_g.push_back()
    texture_g.resize(Texture::COUNT);

    // creates a background texture
    std::string tex_path = "Assets/Background.jpg";
    texture_g[BACKGROUND] = create_texture(render_g, tex_path.c_str());

    // creates a scoreboard texture
    tex_path = "Assets/Score_board.png";
    texture_g[SCORE_BOARD] = create_texture(render_g, tex_path.c_str());

    // creates a score (font) texture
    // the font size is 175px, but it's quite small in the game
    // this is because I actually divide it by 4
    // so it's equivalent to ~43px, but if you remove the division by 4
    // and leave only 43px, the font won't be as smooth.
    texture_g[SCORE] = create_texture(render_g, "0",
        SDL_Color(255, 255, 255, 255), "Assets/Score_font.ttf", 175);

    // the creation of all snake textures, mind you:
    // SNAKE_HEAD
    // SNAKE_BODY
    // SNAKE_BODY_ROTATION - the texture when the snake turns
    // SNAKE_TAIL
    for (int i = SNAKE_HEAD; i <= SNAKE_TAIL; ++i) {
        tex_path = "Assets/Green/Snake" + std::to_string(i - SNAKE_HEAD) + ".png";
        texture_g[i] = create_texture(render_g, tex_path.c_str());
    }

    // create a food texture
    tex_path = "Assets/Food0.png";
    texture_g[FOOD] = create_texture(render_g, tex_path.c_str());
}

// creates a texture and returns a pointer to the address in memory
SDL_Texture *create_texture(SDL_Renderer *rend, const char *tex_path) {

    SDL_Surface *surface = IMG_Load(tex_path);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    return tex;
}

// redefining a function to create a texture with a font
SDL_Texture *create_texture(SDL_Renderer *rend, const char *text, SDL_Color text_color, const char* font_path, int font_size) {
    
    static bool run_once{ true };
    if (run_once) {
        TTF_Init();
        run_once = false;
    }
   
    TTF_Font *font = TTF_OpenFont(font_path, font_size);
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, text_color);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, textSurface);

    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);

    return tex;
}

// preferably use this option to change the texture in the array
void update_texure(SDL_Texture *tex, int index) {
    texture_g[index] = tex;
}