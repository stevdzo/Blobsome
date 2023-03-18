#include "stdafx.h"
#include "Character.h"
#include "Weapon.h"

Character::Character(const unsigned int p_numberOfTextures) : Sprite(p_numberOfTextures)
{
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
	m_friction = 0.0f;
	m_acceleration = 0.0;
	m_ground_level = ::ground_level;

	m_health = ::player_health;
	m_score = 0;

	m_collision.left = 0.0f;
	m_collision.right = 0.0f;
	m_collision.top = 0.0f;
	m_collision.bottom = 0.0f;

	m_jump_vel_x = 0.0f;
	m_jump_vel_y = 0.0f;
	m_is_collideable = true;
	m_flip_horizontal = false;
	m_flip_vertical = false;
	m_is_visible = false;
	m_is_active = false;
	m_is_sprite_sheet = false;
	m_is_background = false;
	m_is_character = true;
	m_is_alive = true;
	m_is_grounded = true;
}

Character::~Character() {
	delete[] m_textures;
}

const float Character::get_health() const { return m_health; }
void Character::set_health(const float hp) { m_health = hp; }

const int Character::get_score() const { return m_score; }
void Character::set_score(const int score) { m_score = score; }

const bool Character::is_alive() const { return m_is_alive; }
void Character::is_alive(const bool p_value) { m_is_alive = p_value; }

//const int Character::get_enemy_angle() const { return m_enemy_angle; }
void Character::set_enemy_angle(const int x, const int y) { m_ang[0] = x; m_ang[1] = y; }

int Character::random_angle() { return ((rand() % m_ang[0]) + m_ang[1]); }

void Character::update(const float p_deltaTime) {

	Sprite::update(p_deltaTime); // nekakva implementacija gravitacije na karaktera
	if (!m_is_grounded) {

		if (m_position.y < m_ground_level) {
			m_position.y += ::gravity_force;
		}
	}
}

void Character::jump(float dt) { // funkcija za skok

	if (m_position.y <= m_ground_level) {

		m_position.y += 10 * m_acceleration * dt * -1;
		m_position.x += ::x_coef * ::direction;
	}
	if (m_position.y > m_ground_level) {
		m_position.y = m_ground_level;
	}
}

void Character::move(float timer, float dt) {

	int n;
	timer += dt;
	if (timer > 2.0f) {
			
		n = rand() % 2;
		if (n == 1) {
		
			m_position.x += m_velocity.x * dt;
		} else 
			m_position.x -= m_velocity.x * dt;
	}
}

void Character::block_collision(Block* blocks[]) { // interakcija sa platformama

	for (int i = 0; i < 10; i++) {

		if (m_position.x + m_size.width > blocks[i]->get_position_x() &&
			m_position.x < blocks[i]->get_position_x() + blocks[i]->get_size().width) {

			if (m_position.y <= blocks[i]->get_position_y() - m_size.height) {

				m_ground_level = blocks[i]->get_position_y() - m_size.height;
				return;
			}
		}
	}
	m_ground_level = ::ground_level;
}

void Character::block_collision(Block* block) { // interakcija sa blokovima

	const float& character_current_position_x = m_position.x;
	const float& character_current_position_y = m_position.y;

	const float& block_current_position_x = block->get_position_x();
	const float& block_current_position_y = block->get_position_y();

	if (character_current_position_x + m_size.width > block_current_position_x) {

		m_position.x = block_current_position_x - m_size.width;
	}
}

Weapon* Character::get_weapon() {

	return m_weapon;
}

void Character::set_weapon(Weapon* weapon) {

	m_weapon = weapon;
}

void Character::movement(bool is_active, char direction) {

	if (is_active) {

		switch (direction) {

		case 'a':

			if (m_position.y == m_ground_level) {

				m_velocity.x = -::velocity;
				flip_horizontal(true);
			}
			break;

		case 'd':

			if (m_position.y == m_ground_level) {

				m_velocity.x = ::velocity;
				flip_horizontal(false);
			}
			break;
		case 'w':
			
			if (m_position.y == m_ground_level) {
				m_is_grounded = true;
				m_acceleration = 7.0f;
			}
			break;
		case ' ':

			m_weapon->set_charger_flag(true);
			m_weapon->load_speed(0.5);
			if (m_weapon->is_grounded()) {

				if (m_weapon->get_speed() > ::blomb_max_speed) {

					m_sound->play_sound(m_sound->sfx_fire);

					m_weapon->is_visible(true);
					m_weapon->is_grounded(false);
					m_weapon->set_weapon_x_vel((m_weapon->get_speed() * 0.8) * m_weapon->get_cos());
					m_weapon->set_weapon_y_vel((-m_weapon->get_speed() * 0.8) * m_weapon->get_sin());
				}

			}
			break;
		case 'p':

			if (get_weapon()->is_grounded() && get_weapon()->get_angle() < 90) {

				get_weapon()->change_angle(2);
			}
			break;
		case 'l':

			if (get_weapon()->is_grounded() && get_weapon()->get_angle() > 0) {

				get_weapon()->change_angle(-2);
			}
			break;
		}
	}
	else m_velocity.x = 0.0f;
}

void Character::jump_direction() {

	if (!m_flip_horizontal) ::direction = 1;
	else				  ::direction = -1;
}

void Character::x_axis_jump() { // pomeraj po x osi usled skoka

	if (m_position.y == m_ground_level) ::x_coef = 0;
	else								::x_coef = 2;
}

void Character::has_jumped(float dt) { // provera da li je igrac skocio

	if (m_is_grounded) {

		jump_direction();
		x_axis_jump();
		jump(dt);
		m_acceleration -= 0.2f;
	}
}

void Character::check_boundaries() { // ne dozvoljava igracu da ide van opsega terena

	if (m_position.x < 0.0f) {

		m_position.x = 0.0f;
	}
	if (m_position.x > SCREEN_WIDTH - m_size.width) {

		m_position.x = SCREEN_WIDTH - m_size.width;
	}
}