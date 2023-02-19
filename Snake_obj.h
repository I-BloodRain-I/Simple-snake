#pragma once
#ifndef SNAKE_OBJ
#define SNAKE_OBJ

#include <random>
#include "Globals.h"

class Snake {
private:

	// snake length excluding tail and head
	int _length;

	// perhaps it would have been better to do enum, 
	// but I found it necessary to do without it, 
	// since the skin is changed very rarely
	// there are three snake skins in the game:
	// 0 - green skin
	// 1 - brown skin
	// 2 - red skin
	int _skin_id;

	// the number of cells per second that the snake traverses
	int _speed;

	// current direction of the snake
	Move _direction;

	// previous direction of the snake
	Move _old_direction;

	// array of snake body segments
	// first index - head
	// last index -  tail
	// other index - body
	std::vector<GameObject_s> _body;

public:

	Snake() : _length { 3 }, _skin_id{ 0 }, _speed{ 15 }, _direction{ Move::LEFT },
		_old_direction{ Move::LEFT } {}

	~Snake() {}

	void move();
	void grow_body(int length);
	void update_skinID(int skin_id);
	int *get_pos();

	int		get_length();
	int		get_skinID();
	int		get_speed();
	Move	get_direction();
	Move	get_old_direction();
	std::vector<GameObject_s> &get_body();

	void set_lenght(int length);
	void set_speed(int speed);
	void set_direction(Move direction);
	void set_old_direction(Move direction);
	void set_body(std::vector<GameObject_s> &body);

};

class Food {
private:

	// contains important information such as position, textures, and other
	GameObject_s _food_info;

	// actually there is only 1 skin for food in the game right now,
	// but maybe in the future I will add more
	int _skin_id;

public:

	Food() : _food_info{}, _skin_id{ -1 } {}

	~Food() {}

	void generate(Snake *snake, int skin_id = 0);
	void update_skinID(int skin_id);
	int *get_pos();

	GameObject_s *get_foodInfo();
	int	get_skinID();

};

#endif