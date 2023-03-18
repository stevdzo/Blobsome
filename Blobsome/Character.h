#pragma once

#include "Sprite.h"
#include "Block.h"
#include <random>

class Weapon;

class Character : public Sprite {

	int m_health;
	int m_damage;
	int m_score;

	float m_jump_vel_x;
	float m_jump_vel_y;
	float m_acceleration;

	int m_enemy_angle;
	int m_ang[2];

	bool m_is_alive;
	bool m_is_character;

	Weapon* m_weapon;

public:

	Character(const unsigned int);
	~Character();

	void update(const float p_deltaTime);

	const float get_health() const;
	void set_health(const float);

	const int get_score() const;
	void set_score(const int);

	//const int get_enemy_angle() const;
	void set_enemy_angle(const int, const int);

	const bool is_alive() const;
	void is_alive(const bool);

	int random_angle();

	void jump(float);
	void move(float, float);
	void block_collision(Block*[]);
	void block_collision(Block*);

	Weapon* get_weapon();
	void set_weapon(Weapon*);

	void movement(bool, char = ' ');

	void jump_direction();
	void x_axis_jump();
	void has_jumped(float);
	void check_boundaries();
};