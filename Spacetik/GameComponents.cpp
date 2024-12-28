#pragma once

#include "graphics.h"
#include <random>
#include "vecmath.h"//an kanw sigkrouseis me ton tropo pou leei sto paradeigma
#include "config.h"
using namespace std;

class GameComponent {

protected:
	float center_x;
	float center_y;
	float speed;

public:
	GameComponent(float x, float y, float s) {
		center_x = x;
		center_y = y;
		speed = s;
	}

	float get_center_x() {
		return center_x;
	}
	float get_center_y() {
		return center_y;
	}
	float get_speed() {
		return speed;
	}

	// setters
	void set_center_x(float center_x) {
		this->center_x = center_x;
	}
	void set_center_y(float center_y) {
		this->center_y = center_y;
	}
	void set_speed(float cs) {
		speed = cs;
	}
};


class Ball: public GameComponent {

private:
	float ball_direction_x;
	float ball_direction_y;
	float ball_radius;

	float init_ball_center_x;
	float init_ball_center_y;
	float init_ball_direction_x;
	float init_ball_direction_y;

public:
	Ball(float x, float y, float dx, float dy, float br, float bs):
		GameComponent(x, y, bs){ //default constructor
		init_ball_center_x = x;
		init_ball_center_y = y;
		init_ball_direction_x = dx;
		init_ball_direction_y = dy;

		ball_radius = br;
		startAgain();
	}

	void startAgain() {						//gets called in the start or every time the ball touches the sides
		center_x = init_ball_center_x;
		center_y = init_ball_center_y;

		// generates random numbers between -0.9999 to 0.9999
		random_device seed;
		default_random_engine generator(seed());
		uniform_real_distribution <float> distribution(-1.0, 1.0);
		float dx = distribution(generator);
		float dy = distribution(generator);

		// prokeimenou na min kanei polles anapidiseis sta plagia 
		while (dy < 0.3 && dy > -0.3) dy = distribution(generator);
		while (dx < 0.3 && dx > -0.3) dx = distribution(generator);
		ball_direction_x = dx;
		ball_direction_y = dy;

	}

	float get_ball_radius() {
		return ball_radius;
	}
	float get_ball_direction_x() {
		return ball_direction_x;
	}
	float get_ball_direction_y() {
		return ball_direction_y;
	}


	void set_ball_direction_x(float ball_direction_x) {
		this->ball_direction_x = ball_direction_x;
	}
	void set_ball_direction_y(float ball_direction_y) {
		this->ball_direction_y = ball_direction_y;
	}
	void set_ball_radius(float br) {
		ball_radius = br;
	}
	void set_init_ball_direction_y(float dy) {
		init_ball_direction_y = dy;
	}
	void set_init_ball_direction_x(float dx) {
		init_ball_direction_x = dx;
	}

};


class Bar : public GameComponent{
public:
	float width;
	float height;
	int points;

	Bar(float x, float y, float w, float h, float sp)
		: GameComponent(x,y,sp){
		width = w;
		height = h;
		points = 0;
	}

	void moveup() {
		center_y = center_y - speed;
	}

	void movedown() {
		center_y = center_y + speed;
	}

	bool collisionWithBall(Ball& ball) {
		if (abs(center_x - ball.get_center_x()) - (width / 2) - (ball.get_ball_radius() - 1) < 0) {
			if (abs(center_y - ball.get_center_y()) - (height / 2) - (ball.get_ball_radius() - 1) < 0) {
				return true; //egine sigrousi me tin mpala
			}
		}
		return false;
	}

	float get_points() {
		return points;
	}
	float get_width() {
		return width;
	}
	float get_height() {
		return height;
	}

};


class Game {
public:
	int window_width;
	int window_height;
	int canvas_width;
	int canvas_height;
	int bounces;

	Ball& ball;
	Bar& player1;
	Bar& player2;

	Game(int ww, int wh, int cw, int ch, Ball& ball, Bar& p1, Bar& p2) :
		ball(ball), player1(p1), player2(p2) {
		window_width = ww;
		window_height = wh;
		canvas_width = cw;
		canvas_height = ch;
		bounces = 0;
	}

	static void drawBall() {
		Game* game = reinterpret_cast <Game*>(graphics::getUserData());			// get all game data
		graphics::Brush br;
		br.fill_opacity = 0.95f;
		br.outline_opacity = 0.0f;
		br.texture = assets_path + "sun3.png";
		graphics::drawDisk(game->ball.get_center_x(), game->ball.get_center_y(), game->ball.get_ball_radius(), br);
	}

	static void drawPlayers() {
		Game* game = reinterpret_cast <Game*>(graphics::getUserData());
		graphics::Brush br;
		br.fill_opacity = 0.95f;
		br.outline_opacity = 0.2f;
		br.texture = assets_path + "bar3.png";
		graphics::drawRect(game->player1.get_center_x(), game->player1.get_center_y(), game->player1.width, game->player1.height, br);
		graphics::drawRect(game->player2.get_center_x(), game->player2.get_center_y(), game->player2.width, game->player2.height, br);
	}

	static void drawText() {
		Game* game = reinterpret_cast <Game*>(graphics::getUserData());
		graphics::Brush br;
		br.fill_secondary_color[0] = 1.0f;
		br.fill_secondary_color[1] = 1.0f;
		br.fill_secondary_color[2] = 1.0f;
		graphics::drawText(2, 5, 5, "P1:" + to_string(game->player1.points), br);
		graphics::drawText(game->canvas_width - 13, 5, 5, "  P2:" + to_string(game->player2.points), br);
	}

};