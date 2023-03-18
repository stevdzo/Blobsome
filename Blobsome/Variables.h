
class Input;

namespace {

	extern int previousTime;
	extern int refreshMills;
	extern int angle;
	extern int player_health;
	extern int blomb_angle;;
	extern int level;
	extern int enemy_counter;
	extern int counter;
	extern int direction;
	extern int weapon_damage;
	extern int number_of_enemies;

	extern float p_delta_time;
	extern float velocity;
	extern float blomb_max_speed;
	extern float blomb_speed;
	extern float enemy_bomb_timer;
	extern float drop_bomb_timer;
	extern float ui_timer;
	extern float splash_timer;
	extern float pineapple_spawn_timer;
	extern float pineapple_remove_timer;
	extern float effect_timer;
	extern float enemy_move_timer;
	extern float text_pos[4];
	extern float blob_acceleration;	
	extern float x_coef;
	extern float space;
	extern float ground_level;
	extern float blomb_ground_level;
	extern float green_alpha;
	extern float red_alpha;
	extern float gravity_force;
	extern float wind_force;
	extern float cos_look[361];
	extern float sin_look[361];
	
	extern bool out_flag;
	extern bool pineapple_flag;
	extern bool pineapple_effect_flag;

	extern bool key_down[256];

	extern enum GameState;

	extern GameState game_state;

	extern Input* input;
}