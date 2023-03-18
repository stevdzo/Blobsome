#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(const unsigned int p_numberOfTextures) : Sprite(p_numberOfTextures) {

	m_textures = new GLuint[p_numberOfTextures];
	m_textureIndex = 0;
	m_currentFrame = 0;
	m_numberOfFramesX = 0;
	m_numberOfFramesY = 0;
	m_animationDelay = 0.25f;
	m_animationElapsed = 0.0f;
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_size.height = 0.0f;
	m_size.width = 0.0f;
	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_ground_level = ::blomb_ground_level;
	m_damage = ::weapon_damage;
	m_speed = 1.0f;
	m_angle = 0;

	m_collision.left = 0.0f;
	m_collision.right = 0.0f;
	m_collision.top = 0.0f;
	m_collision.bottom = 0.0f;

	m_weapon_x_vel = 0.0f;
	m_weapon_y_vel = 0.0f;

	m_is_collideable = true;
	m_flip_horizontal = false;
	m_flip_vertical = false;
	m_is_visible = false;
	m_is_active = false;
	m_is_sprite_sheet = false;
	m_is_background = false;
	m_isCharacter = false;
	m_is_projectile = false;
	m_is_gunpoint = false;
	m_charger_flag = false;
}

Weapon::~Weapon() {
	delete[] m_textures;
}

void Weapon::update(const float p_deltaTime) {

	Sprite::update(p_deltaTime);
		
	if (m_is_grounded) {
			
		m_weapon_y_vel = 5.0f;
		m_position.x = rand() % (int)(960 - m_size.width);
		m_position.y = -100.0f;
	}
	if (!m_is_grounded) {

		if (m_position.y < m_ground_level) {

			m_weapon_y_vel += 0.2;
			m_position.y += m_weapon_y_vel * p_deltaTime;
		}
		if (m_position.y >= m_ground_level) {
					
			m_is_grounded = true;
		}
	}	
}

const float Weapon::get_damage() const { return m_damage; }
void Weapon::set_damage(const float dmg) { m_damage = dmg; }

const float Weapon::get_weapon_x_vel() const { return m_weapon_x_vel; }

void Weapon::set_weapon_x_vel(const float x) {
	m_weapon_x_vel = x;
}

const float Weapon::get_weapon_y_vel() const {
	return m_weapon_y_vel;
}

void Weapon::set_weapon_y_vel(const float y) {
	m_weapon_y_vel = y;
}

bool Weapon::is_charger_ready() {

	return m_charger_flag;
}

void Weapon::set_charger_flag(bool flag) {

	m_charger_flag = flag;
}

int Weapon::get_angle() {
	
	return m_angle;
}

void Weapon::set_angle(int angle) {

	m_angle = angle;
}

void Weapon::change_angle(int dir) {

	m_angle += dir;
}

void Weapon::space_release(Character* player) {

	if (is_grounded()) { // kada igrac pusti taster SPACE u bilo kom trenutku, bombu ispaljuje jacinom koja je do tada izracunata

		m_sound->play_sound(m_sound->sfx_fire);

		m_is_visible = true;
		m_is_grounded = false;
		m_weapon_x_vel = (m_speed * 0.8) * m_cos;
		m_weapon_y_vel = (-m_speed * 0.8) * m_sin;
	}
	m_speed = 0.0f;
}

float Weapon::get_speed() {

	return m_speed;
}

void Weapon::set_speed(float speed) {

	m_speed = speed;
}

void Weapon::load_speed(float num) {

	m_speed += num;
}

float Weapon::get_sin() {

	return m_sin;
}

void Weapon::set_sin(float _sin) {

	m_sin = _sin;
}

float Weapon::get_cos() {

	return m_cos;
}

void Weapon::set_cos(float _cos) {

	m_cos = _cos;
}

void Weapon::update_angles(float sin, float cos) {

	m_sin = sin;
	m_cos = cos;
}

void Weapon::fire(Character* character, float sin[], float cos[], int angle, float direction) { // ispaljivanje bombi

	if (m_is_grounded) {

		m_is_visible = false;
		m_position.x = character->m_position.x + character->m_size.width / 2;
		m_position.y = character->m_position.y + character->m_size.height / 2;
	}
	if (!m_is_grounded) {

		m_is_visible = true;
		m_position.x += 8 * cos[angle] * direction;
		m_position.y -= 8 * sin[angle];

		m_position.x += m_weapon_x_vel;
		m_position.y += m_weapon_y_vel;

		//m_weapon_x_vel -= 0.01;
		m_weapon_y_vel += 0.3;
	}
}

void Weapon::check_collision(Block* block) {

	if (m_position.x + m_size.width >= block->get_position_x()) {

		m_position.x -= m_velocity.x;
		m_position.y += m_velocity.y;
	}
}