#ifndef GLOBALS_H
#define GLOBALS_H

#include "Variables.h"
#include <iostream>
#include <math.h>
//#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#define PI ((float)3.141592654f)
#define BUTTON_Y 345.0f
#define SCREEN_WIDTH 1920.0f
#define SCREEN_HEIGHT 1080.0f
#define UI_THRESHOLD ((float)0.001f)
#define SPLASH_THRESHOLD ((float)20.0f)

namespace {

	int previousTime;
	int refreshMills = 30.0f;
	int angle;
	int player_health = 100;
	int blomb_angle = 0;
	int level = 1;
	int enemy_counter = 0;
	int counter = 3;
	int direction = -1;
	int weapon_damage = 20;
	int number_of_enemies = 3;

	float p_delta_time = 0.0f;
	float velocity = 40.0f;
	float blomb_max_speed = 19.0f;
	float blomb_speed = 1.0f;
	float enemy_bomb_timer = 0.0f;
	float drop_bomb_timer = 0.0f;
	float ui_timer;
	float splash_timer = 0.0f;
	float pineapple_spawn_timer = 0.0f;
	float pineapple_remove_timer = 0.0f;
	float effect_timer = 0.0f;
	float enemy_move_timer = 0.0f;
	float ground_level = 995.0f;
	float blomb_ground_level = 1043.0f;
	float text_pos[4]{ -50.0, -150.0, -250.0, -350.0f }; // credits
	float blob_acceleration;	
	float x_coef;
	float space = 0.0f;
	float green_alpha = 1.0f;
	float red_alpha = 0.0f;
	float gravity_force = 0.2f;  // gravity
	float wind_force = 0.0f; // = -0.01; // wind resistance
	float cos_look[361];
	float sin_look[361];

	bool out_flag = true;
	bool pineapple_flag = false;
	bool pineapple_effect_flag = false;

	bool key_down[256];

	enum GameState {

		GS_MENU,
		GS_SPLASH,
		GS_LOADING,
		GS_RUNNING,
		GS_PAUSED,
		GS_HIGHSCORE,
		GS_GAMEOVER,
		GS_CONTROLS,
		GS_CREDITS,
		GS_NEXTLEVEL,
	};

	GameState game_state;

	Input* input;
}
#endif // !GLOBALS_h