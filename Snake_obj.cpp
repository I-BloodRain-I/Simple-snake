#include "Snake_obj.h"

// movement of the snake on the playing field
void Snake::move() {

	// get all segments of the snake's body to change their coordinates and rotation
	auto body = get_body();

	// has the snake's direction changed
	// true if changed
	Move old_direction = get_old_direction();
	bool is_changedDir = old_direction == get_direction() ? false : true;

	auto const &texs = ::get_textures();
	auto const rotation_tex = texs[SNAKE_BODY_ROTATION];

	GameObject_s old_cell{};
	for (int i = 0; i < body.size(); ++i) {

		GameObject_s &body_part = body[i];

		// tail
		if (i == body.size() - 1) {
			auto temp_tex = body_part.tex;
			body_part = old_cell;

			// we go through each segment from tail to head,
			// and if we find a body segment that is not equal to rotation_tex,
			// then we take its rotation angle and assign it to the tail
			// if the body is not too small, there is no problem
			int body_index = 0;
			while (true) {

				if (body[i - body_index].tex != rotation_tex || i - body_index == 0) {
					body_part.degrees = body[i - body_index].degrees;
					break;
				}
				++body_index;
			}
			body_part.tex = temp_tex;

			break;
		}

		// body
		// we assign to each body segment (except for the head and the tail)
		// the attributes of the previous segment
		else if (i != 0) {
			auto temp = body_part;
			body_part = old_cell;
			old_cell = temp;

			continue;
		}

		// head
		old_cell = body_part;
		old_cell.tex = texs[SNAKE_BODY];

		// moving the snake
		// compare the current direction with the previous one
		switch (get_direction())
		{
		case Move::UP:
			body_part.size.y -= 1;
			body_part.degrees = 0;

			if (is_changedDir) {

				if (old_direction == Move::LEFT)	old_cell.degrees = 0;
				else								old_cell.degrees = 270;

				old_cell.tex = rotation_tex;
			}
			break;

		case Move::LEFT:
			body_part.size.x -= 1;
			body_part.degrees = 270;

			if (is_changedDir) {

				if (old_direction == Move::UP)		old_cell.degrees = 180;
				else								old_cell.degrees = 270;

				old_cell.tex = rotation_tex;
			}
			break;

		case Move::DOWN:
			body_part.size.y += 1;
			body_part.degrees = 180;

			if (is_changedDir) {

				if (old_direction == Move::LEFT)	old_cell.degrees = 90;
				else								old_cell.degrees = 180;

				old_cell.tex = rotation_tex;
			}
			break;

		case Move::RIGHT:
			body_part.size.x += 1;
			body_part.degrees = 90;

			if (is_changedDir) {

				if (old_direction == Move::UP)		old_cell.degrees = 90;
				else								old_cell.degrees = 0;

				old_cell.tex = rotation_tex;
			}
			break;
		}
	}

	// value update
	set_old_direction(get_direction());
	set_body(body);

}

// adding new body segments
void Snake::grow_body(int length) {

	auto const &texs = ::get_textures();
	set_lenght(get_length() + length);
	auto &body = get_body();


	// we iterate through each element of the segment until
	// we find a normal body segment (SNAKE_BODY). We assign new segments in its place
	// if no normal body segment is found, assign new segments in place of the head
	bool is_foundBody{ false };
	for (auto iter = body.begin(); iter < body.end(); ++iter) {

		if ((*iter).tex == texs[SNAKE_BODY]) {
			GameObject_s new_seg = *iter;
			body.insert(iter, length, new_seg);
			is_foundBody = true;

			break;
		}

	}

	if (!is_foundBody) {
		auto iter = body.begin();
		GameObject_s new_seg = *iter;
		body.insert(iter, length, new_seg);
	}
}

void Snake::update_skinID(int skin_id) {

	// if the skin's ID hasn't changed, don't do anything.
	if (skin_id == get_skinID()) { return; }

	auto &texs = ::get_textures();
	std::string tex_path;
	auto &body = get_body();

	// go through each texture of the snake's body, and if you find
	// an existing body with the current texture, replace the texture with
	// a new one of the same type
	// for example, the green head of the snake is replaced only by
	// the red (conditionally) head of the snake
	switch (skin_id)
	{
	case 0:
		for (int i = SNAKE_HEAD; i <= SNAKE_TAIL; ++i) {
			auto old_texture = texs[i];
			tex_path = "Assets/Green/Snake" + std::to_string(i - SNAKE_HEAD) + ".png";
			texs[i] = create_texture(::get_renderer(), tex_path.c_str());

			for (int j = 0; j < body.size(); ++j)
				if (body[j].tex == old_texture)
					body[j].tex = texs[i];
			SDL_DestroyTexture(old_texture);
		}
		break;

	case 1:
		for (int i = SNAKE_HEAD; i <= SNAKE_TAIL; ++i) {
			auto old_texture = texs[i];
			tex_path = "Assets/Brown/Snake" + std::to_string(i - SNAKE_HEAD) + ".png";
			texs[i] = create_texture(::get_renderer(), tex_path.c_str());

			for (int j = 0; j < body.size(); ++j)
				if (body[j].tex == old_texture)
					body[j].tex = texs[i];
			SDL_DestroyTexture(old_texture);
		}
		break;

	case 2:
		for (int i = SNAKE_HEAD; i <= SNAKE_TAIL; ++i) {
			auto old_texture = texs[i];
			tex_path = "Assets/Red/Snake" + std::to_string(i - SNAKE_HEAD) + ".png";
			texs[i] = create_texture(::get_renderer(), tex_path.c_str());

			for (int j = 0; j < body.size(); ++j)
				if (body[j].tex == old_texture)
					body[j].tex = texs[i];
			SDL_DestroyTexture(old_texture);
		}
		break;
	}

	this->_skin_id = skin_id;

}

// the current position of the snake's head [x, y]
int *Snake::get_pos()			{
	static int pos_snake[2];
	const auto &head = get_body()[0];

	pos_snake[0] = head.size.x;
	pos_snake[1] = head.size.y;

	return pos_snake;
}

// getters
int Snake::get_length()					{ return this->_length; }
int Snake::get_skinID()					{ return this->_skin_id; }
int Snake::get_speed()					{ return this->_speed; }
Move Snake::get_direction()			{ return this->_direction; }
Move Snake::get_old_direction()		{ return this->_old_direction; }
std::vector<GameObject_s> &Snake::get_body()  { return this->_body; }

// setters
void Snake::set_lenght(int length)				{ this->_length = length; }
void Snake::set_speed(int speed)				{ this->_speed = speed; }
void Snake::set_direction(Move direction)		{ this->_direction = direction; }
void Snake::set_old_direction(Move direction) { this->_old_direction = direction; }
void Snake::set_body(std::vector<GameObject_s> &body) { this->_body = body; }


// updating the food position
void Food::generate(Snake *snake, int skind_id) {

	auto food = get_foodInfo();

	// get random pos
	static std::random_device dev;
	static std::mt19937 rng(dev());

	static std::uniform_int_distribution<std::mt19937::result_type> rand_width(0, GRID_SIZE[0] - 1);
	static std::uniform_int_distribution<std::mt19937::result_type> rand_height(0, GRID_SIZE[1] - 1);

	// creates a new food position on x and y
	// if the new coordinates intersect the snake's body or scoreboard,
	// then look for new coordinates
	int rand_x, rand_y;
	while (true) {

		rand_x = rand_width(rng);
		rand_y = rand_height(rng);

		// 4 - the width of the scoreboard in cells (with a reserve)
		// 6 - the height of the scoreboard in cells (with a reserve)
		bool is_collision{ false };
		if (rand_x < 4 && rand_y < 6)
			continue;

		for (auto const &body : snake->get_body()) {
			if (rand_x == body.size.x && rand_y == body.size.y) {
				is_collision = true;
				break;
			}
		}

		if (!is_collision) { break; }
	}

	update_skinID(skind_id);

	food->size.x = rand_x;
	food->size.y = rand_y;

	// the size of the food will always be according to the size of the cell
	food->size.w = GRID_SCALE;
	food->size.h = GRID_SCALE;

}

// the food skin has not yet been updated in the game
// function not tested and may be changed / removed in the future
void Food::update_skinID(int skin_id) {
	if (skin_id == get_skinID()) { return; }

	this->_food_info.tex = ::get_textures()[FOOD];

	this->_skin_id = skin_id;
}

int *Food::get_pos() { 
	static int pos_food[2];

	pos_food[0] = this->_food_info.size.x;
	pos_food[1] = this->_food_info.size.y;

	return pos_food;
}

// getters
GameObject_s *Food::get_foodInfo()	{ return &this->_food_info; }
int Food::get_skinID()				{ return this->_skin_id; }