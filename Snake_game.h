#pragma once
#ifndef SNAKE_GAME
#define SNAKE_GAME

#include "Globals.h"
#include "Snake_obj.h"

// the main class that controls the game
class Game {
private:
	
	// the list of objects that need to be rendered,
	// the very first element will be at the back of the scene
	// and vice versa, the very last element of the list
	// will be in the foreground
	std::vector<QueueRender_s> _queue_render;

	// array stores game objects to be referenced during rendering
	std::vector<GameObject_s> _scene_objects;

	Snake *_snake;
	Food *_food;

	// Whether the game continues, 
	// if false, the game stops and destructors are called
	bool _is_play;
	int _score;

public:

	Game() : _queue_render{}, _scene_objects{}, _snake{ nullptr },
		_food{ nullptr }, _is_play{ true }, _score{ 0 } {}

	~Game() {
		destroy_game();
	}

	void run();
	void init_scene();
	void add_score(int score);
	void game_loop();
	void destroy_game();

	void create_snake();
	void create_food();

	void keyboard_event();
	void border_event();
	void collision_event();

	void add_toRenderQueue(GameObject_s &obj, int index = -1);
	void add_toRenderQueue(QueueRender_s &obj, int index = -1);
	void remove_fromRenderQueue(int index);
	void render();

	std::vector<QueueRender_s> &get_queueRender();
	std::vector<GameObject_s> &get_GameObjects();
	Snake	*get_snake();
	Food	*get_food();
	bool	 get_gameStatus();
	int		 get_score();

	void set_queueRender(std::vector<QueueRender_s> &queue_render);
	void set_gameObjects(std::vector<GameObject_s> &game_objects);
	void set_snake(Snake *snake);
	void set_food(Food *food);
	void set_gameStatus(bool status);
	void set_score(int score);

};

#endif