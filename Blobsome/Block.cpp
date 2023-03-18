#include "stdafx.h"
#include "Block.h"

Block::Block(const unsigned int p_numberOfTextures) : Sprite(p_numberOfTextures) {

	m_textures = new GLuint[p_numberOfTextures];
	m_textureIndex = 0;
	m_currentFrame = 0;
	m_numberOfFramesX = 0;
	m_number_off_frames = 0;
	m_animationDelay = 0.25f;
	m_animationElapsed = 0.0f;
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_size.height = 0.0f;
	m_size.width = 0.0f;
	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_friction = 0.0f;
	m_ground_level = ::ground_level;

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
	m_is_gunpoint = false;
	m_is_grounded = false;
}

Block::~Block() {
	delete[] m_textures;
}
