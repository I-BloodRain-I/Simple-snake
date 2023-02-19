#include "Snake_game.h"

// initializing and launching the game
void Game::run() {

    // creating textures and game objects, such as the background or scoreboard
    init_system();
    init_scene();

    // creating the main game objects
    create_snake();
    create_food();

    // the game lasts until it is closed
    game_loop();
}

// creating scene objects: background, scoreboard, etc.
void Game::init_scene() {

    auto const &texs = ::get_textures();
    auto game_objects = get_GameObjects();
    GameObject_s obj;

    // creating a background
    obj.size.x = 0;
    obj.size.y = 0;

    obj.size.w = WINDOW_WIDTH;
    obj.size.h = WINDOW_HEIGHT;

    obj.degrees = 0;
    obj.tex = texs[BACKGROUND];
    game_objects.push_back(obj);

    // creating a scoreboard
    obj.size.x = 15;
    obj.size.y = 15;

    // simple math, the height and width can be anything,
    // but these figures appealed to me
    obj.size.w = GRID_SCALE * 4;
    obj.size.h = GRID_SCALE * 2;

    obj.degrees = 0;
    obj.tex = texs[SCORE_BOARD];
    game_objects.push_back(obj);

    // creating a score (text)
    // we need to know the size of our text with the given font and font size
    // the return is done by reference (the size variable changes)
    SDL_Point size;
    SDL_QueryTexture(texs[SCORE], nullptr, nullptr, &size.x, &size.y);

    obj.size.x = 110;
    obj.size.y = 25;

    obj.size.w = size.x / 4;
    obj.size.h = size.y / 4;

    obj.degrees = 0;
    obj.tex = texs[SCORE];
    game_objects.push_back(obj);

    // pass the new scene objects to our list of all game objects
    // after that we add each object to the render queue
    set_gameObjects(game_objects);
    for (auto &obj : get_GameObjects()) {
        add_toRenderQueue(obj);
    }

}

void Game::add_score(int score) {
    set_score(get_score() + score);

    // after updating the number of points, a new number should be displayed
    // so we create a new texture
    SDL_Texture *tex = create_texture(::get_renderer(), std::to_string(get_score()).c_str(),
        SDL_Color(255, 255, 255, 255), "Assets/Score_font.ttf", 175);

    SDL_Point size;
    SDL_QueryTexture(tex, nullptr, nullptr, &size.x, &size.y);

    // here I divide the actual text size by 4 to increase anti-aliasing
    // this, of course, reduces the image, and you have to use 175 font size
    // instead of, say, 43, but the text looks better
    auto objects = get_GameObjects();
    objects[SCORE].size.w = size.x / 4;
    objects[SCORE].size.h = size.y / 4;

    // first delete the previous texture to free up memory, 
    // and then update it with the new texture
    SDL_DestroyTexture(::get_textures()[SCORE]);
    ::update_texure(tex, SCORE);
    objects[SCORE].tex = tex;

    set_gameObjects(objects);
}

// game cycle management
void Game::game_loop() {

    // the number of frames has passed since the start of the game
    int frame_startTimeMs{ 0 };

    // contains the number of frames from the last "step" of the snake 
    int last_stepMs{ 0 };

    // if it is false, the game stops
    bool is_play = true;

    Snake *snake = get_snake();
    while (is_play) {

        // check if the status of the game has changed
        is_play = get_gameStatus();

        // getting the current snake speed
        // the default is 15 cells per second 
        int speed = snake->get_speed();

        // calculate how often with the current speed the snake will move
        // for example, if the snake's speed is 15, 
        // then it has to move to a new cell every 66 ms (1000 / 15).
        unsigned int stepMs = speed > 0 ? static_cast <unsigned>(1000 / speed) : 0;

        // checks if 66 ms (taken from the previous comment) has elapsed since the last step
        if (SDL_GetTicks() - last_stepMs > stepMs) {

            // get key presses to know where to move the snake
            keyboard_event();

            if (speed > 0) {
                snake->move();
                last_stepMs = SDL_GetTicks();
            }
        }

        // if the snake goes over the edge of the game, move it to the other side 
        border_event();

        // events of the snake's collision with itself or with food
        collision_event();

        // rendering all game objects
        render();

        // explicitly assigning the number of FPS to the game
        // this is done so that the game does not consume more resources than necessary
        // for example we don't need 500 fps in this game, 
        // 60 fps will be enough (1000 / 60)
        SDL_Delay(1000 / 60);
    }
}

// destructor calls after the game ends
void Game::destroy_game() {

    // destroy textures
    for (auto &tex : get_textures()) {
        SDL_DestroyTexture(tex);
    }

    // destroy renderer
    SDL_DestroyRenderer(get_renderer());

    // destroy window
    SDL_DestroyWindow(get_window());

    // close SDL
    SDL_Quit();
}

// creating a snake and placing it on the playing field
void Game::create_snake() {

    // we don't need more than 1 snake
    static Snake snake;

    auto const &texs = ::get_textures();
    int const length = snake.get_length();

    // when rendering, we refer to previous memory addresses,
    // and we don't need them to change
    // that's why we allocate memory for the snake's body equal to the playing field
    std::vector<GameObject_s> &body = snake.get_body();
    body.reserve(GRID_SIZE[0] * GRID_SIZE[1]);

    // creating a snake body
    // i0 = snake head
    // i1 = snake body (default length = 3)
    // i2 = snake tail
    for (int i = 0; i < 2 + length; ++i) {

        // in the beginning the snake always moves to the left,
        // so the body is built horizontally
        GameObject_s snake_part;
        snake_part.size.x = (GRID_SIZE[0] / 2) + i;
        snake_part.size.y = (GRID_SIZE[1] / 2);

        snake_part.size.w = GRID_SCALE;
        snake_part.size.h = GRID_SCALE;

        // the snake sprites originally look up,
        // and 270 degrees is enough to simulate movement to the left
        snake_part.degrees = 270;

        if (i == 0)					snake_part.tex = texs[SNAKE_HEAD];
        else if (i == 1 + length)	snake_part.tex = texs[SNAKE_TAIL];
        else						snake_part.tex = texs[SNAKE_BODY];

        // we add it to the rendering queue, to the first index (the second position),
        // because the first one has the background
        // we don't need the snake to be behind the background
        body.push_back(snake_part);
        add_toRenderQueue(body.at(i), 1);
    }

    set_snake(&snake);
}

// Creating food and placing it on the playing field
void Game::create_food() {


    static Food food;
    food.generate(get_snake());
    add_toRenderQueue(*food.get_foodInfo(), 1);
    set_food(&food);

}

// move or close key event
void Game::keyboard_event() {

    // key press event
    SDL_Event event;
    Snake *snake = get_snake();

    // does the new direction of the snake from the current direction
    bool is_posChanged{ false };


    while (SDL_PollEvent(&event)) {

        switch (event.type) {

        // handling of close button
        case SDL_QUIT:
            set_gameStatus(false);
            break;

        case SDL_KEYDOWN:
            // keyboard API for key pressed
            // a snake cannot turn 180 degrees if it moves up,
            // it cannot turn sharply down, only left or right
            switch (event.key.keysym.scancode) {

            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                if (snake->get_direction() != Move::DOWN) {
                    snake->set_direction(Move::UP);
                    is_posChanged = true;
                }
                break;

            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                if (snake->get_direction() != Move::RIGHT) {
                    snake->set_direction(Move::LEFT);
                    is_posChanged = true;
                }
                break;

            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                if (snake->get_direction() != Move::UP) {
                    snake->set_direction(Move::DOWN);
                    is_posChanged = true;
                }
                break;

            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                if (snake->get_direction() != Move::LEFT) {
                    snake->set_direction(Move::RIGHT);
                    is_posChanged = true;
                }
                break;

            default:
                break;
            }
        }

        if (is_posChanged)
            break;
    }
}

// the event of the snake leaving the game map
void Game::border_event() {

    // From the point of view of optimization, 
    // it would be better to pass an object by reference, but I was working on OOP
    auto body = get_snake()->get_body();

    // if the snake is out of bounds, teleport it to the other side
    for (auto &part : body) {

        // right boundary
        if (part.size.x > GRID_SIZE[0] - 1) {
            part.size.x = 0;
            break;
        }

        // left boundary
        if (part.size.x < 0) {
            part.size.x = GRID_SIZE[0] - 1;
            break;
        }

        // bottom boundary
        if (part.size.y > GRID_SIZE[1] - 1) {
            part.size.y = 0;
            break;
        }

        // upper boundary
        if (part.size.y < 0) {
            part.size.y = GRID_SIZE[1] - 1;
            break;
        }
    }
    get_snake()->set_body(body);
}

// the event of a snake colliding with itself or with food
void Game::collision_event() {

    Snake *snake = get_snake();
    Food *food = get_food();

    auto &body = snake->get_body();
    auto const &head_pos = snake->get_pos(); // [x, y]
    auto const food_pos = food->get_pos();   // [x, y]

    // To count that the snake swallowed the food,
    // it is necessary that the head of the snake was at the same coordinates as the food
    if (head_pos[0] == food_pos[0] && head_pos[1] == food_pos[1]) {

        // increase the body of the snake by 1 segment
        snake->grow_body(1);

        // changing the position of food
        food->generate(snake);

        // since the new segment replaces the value at the memory address
        // that pointed to the snake's tail,
        // we need to specify this in the rendering as well,
        // so as not to "lose" our tail
        add_toRenderQueue(body[body.size() - 1], 1);
        add_score(1);

        // the snake's skin changes after the snake eats 10 and 25 eggs
        int score = get_score();
        if (score == 10)    snake->update_skinID(1);
        if (score == 25)    snake->update_skinID(2);
    }

    for (int i = 1; i < body.size(); ++i) {
        const auto &curr_part = body[i];

        if (head_pos[0] == curr_part.size.x && head_pos[1] == curr_part.size.y) {
            snake->set_speed(0);
            break;
        }
    }
}

// adding game objects to the render queue
void Game::add_toRenderQueue(GameObject_s &obj, int index) {

    auto queue_render = get_queueRender();

    QueueRender_s queue;
    queue.degrees = &obj.degrees;

    queue.x = &obj.size.x;
    queue.y = &obj.size.y;

    queue.h = &obj.size.h;
    queue.w = &obj.size.w;

    queue.tex = &obj.tex;

    // if no index is specified, add to the end
    if (index == -1) {
        queue_render.push_back(queue);
    }
    else {
        auto iter = std::next(queue_render.begin(), index);
        queue_render.insert(iter, queue);
    }
    set_queueRender(queue_render);
}
void Game::add_toRenderQueue(QueueRender_s &obj, int index) {

    auto queue_render = get_queueRender();

    // if no index is specified, add to the end
    if (index == -1) {
        queue_render.push_back(obj);
    }
    else {
        auto iter = std::next(queue_render.begin(), index);
        queue_render.insert(iter, obj);
    }
    set_queueRender(queue_render);
}

// remove game objects from the rendering queue.
// the function is not used in the program, as there is no need,
// but it works andyou can use it
void Game::remove_fromRenderQueue(int index) {

    auto queue_render = get_queueRender();
    auto iter = std::next(queue_render.begin(), index);
    queue_render.erase(iter);
    set_queueRender(queue_render);

}

// render game objects from the render queue
void Game::render() {
    auto const rend = ::get_renderer();

    // cleaning up the renderer
    SDL_RenderClear(rend);

    auto const &queue_render = get_queueRender();
    size_t queue_size = queue_render.size();
    int queue_index = 0;

    for (auto const &part : queue_render) {
        
        // to specify specific texture sizes, an object of type SDL_Rect is expected
        SDL_Rect rect{ *part.x * GRID_SCALE, *part.y * GRID_SCALE, *part.w, *part.h };

        // some crutch, but I don't see the point in redoing it.
        // if it is a scoreboard or score, then the position is calculated
        // as pixel by pixel, not cell by cell, as in the rest of the case
        if (queue_index == queue_size - 1 || queue_index == queue_size - 2) {
            rect.y = *part.y; rect.x = *part.x;
        }

        // object rendering
        SDL_RenderCopyEx(rend, *part.tex, nullptr, &rect, *part.degrees, nullptr, SDL_FLIP_NONE);
        queue_index++;
    }

    // triggers the double buffers
    // for multiple rendering
    SDL_RenderPresent(rend);
}

// getters
std::vector<QueueRender_s> &Game::get_queueRender() 
                                    { return this->_queue_render; }
std::vector<GameObject_s> &Game::get_GameObjects() 
                                    { return this->_scene_objects; }
Snake *Game::get_snake()            { return this->_snake; }
Food *Game::get_food()              { return this->_food; }
bool Game::get_gameStatus()         { return this->_is_play; }
int Game::get_score()               { return this->_score; }

// setters
void Game::set_queueRender(std::vector<QueueRender_s> &queue_render)
                                            { this->_queue_render = queue_render; }
void Game::set_gameObjects(std::vector<GameObject_s> &game_objects)
                                            { this->_scene_objects = game_objects; }
void Game::set_snake(Snake *snake)          { this->_snake = snake; }
void Game::set_food(Food *food)             { this->_food = food; }
void Game::set_gameStatus(bool status)      { this->_is_play = status; }
void Game::set_score(int score)             { this->_score = score; }