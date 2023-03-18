#pragma once

#include "Sprite.h"
#include "Character.h"

#include <time.h>

class Weapon : public Sprite {

	int m_damage;
	int m_angle;
	float m_speed;

	float m_sin, m_cos;

	bool m_is_projectile;
	bool m_charger_flag;

public:

	Weapon(const unsigned int);
	~Weapon();

	void update(const float p_deltaTime);

	const float get_damage() const;
	void set_damage(const float);

	const float get_weapon_x_vel() const;
	void set_weapon_x_vel(const float);
	const float get_weapon_y_vel() const;
	void set_weapon_y_vel(const float);

	bool is_charger_ready();
	void set_charger_flag(bool);

	int get_angle();
	void set_angle(int);

	void change_angle(int);

	void space_release(Character*);

	float get_speed();
	void set_speed(float);

	void load_speed(float);

	float get_sin();
	void set_sin(float);

	float get_cos();
	void set_cos(float);

	void update_angles(float, float);

	void fire(Character*, float[], float[], int, float);
	void check_collision(Block*);
};