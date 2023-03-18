#include "stdafx.h"
#include "Sprite.h"
#include "SOIL.h"

Sprite::Sprite(const unsigned int p_numberOfTextures) {

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
	m_friction   = 0.0f;
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

	m_sound = new SoundSystem();
}

Sprite::~Sprite() {
	delete[] m_textures;
}

const bool Sprite::add_texture(const char* p_imageName) {

	GLuint texture = SOIL_load_OGL_texture(p_imageName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	if (texture == 0) {
		return false;
	}
	m_textures[m_textureIndex] = texture;
	m_textureIndex++;

	if (m_textureIndex == 1 && m_number_off_frames > 1) {
		m_is_sprite_sheet = true;
	}
	else {
		m_is_sprite_sheet = false;
	}
	return true;
}

void Sprite::render() {

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_DEPTH_TEST);

	if (m_is_visible) {

		if (m_use_transparency) {

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glBindTexture(GL_TEXTURE_2D, get_current_frame());

		glBegin(GL_QUADS);

		GLfloat x = m_position.x;
		GLfloat y = m_position.y;

		GLfloat w = m_size.width;
		GLfloat h = m_size.height;

		GLfloat texWidth = (GLfloat)m_textureIndex / (GLfloat)m_number_off_frames;
		GLfloat texHeight = 1.0f;
		GLfloat u = 0.0f;
		GLfloat v = 0.0f;
		if (m_textureIndex < m_number_off_frames) u = (GLfloat)m_currentFrame * texWidth;

		if (m_flip_horizontal) {

			glTexCoord2f(u + texWidth, v); glVertex2f(x, y);
			glTexCoord2f(u, v); glVertex2f(x + w, y);
			glTexCoord2f(u, v + texHeight); glVertex2f(x + w, y + h);
			glTexCoord2f(u + texWidth, v + texHeight); glVertex2f(x, y + h);
		}
		else {

			glTexCoord2f(u, v); glVertex2f(x, y);
			glTexCoord2f(u + texWidth, v); glVertex2f(x + w, y);
			glTexCoord2f(u + texWidth, v + texHeight); glVertex2f(x + w, y + h);
			glTexCoord2f(u, v + texHeight); glVertex2f(x, y + h);
		}
		glEnd();

		if (m_use_transparency) {
			glDisable(GL_BLEND);
		}
		glDisable(GL_TEXTURE_2D);
	}
}

void Sprite::update(const float p_deltaTime) {

	float dt = p_deltaTime;

	if (m_is_active) {

		m_animationElapsed += dt;
		if (m_animationElapsed >= m_animationDelay) {

			m_currentFrame++;

			if (m_currentFrame >= m_number_off_frames) m_currentFrame = 0;

			m_animationElapsed = 0.0f;
		}
		m_position.x += m_velocity.x * dt;
	}
}

const Sprite::Rect Sprite::get_collision_rect() const {

	Rect rect;

	rect.left = m_position.x + m_collision.left;
	rect.right = m_position.x + m_size.width + m_collision.right;
	rect.top = m_position.y + m_collision.top;
	rect.bottom = m_position.y + m_size.height + m_collision.bottom;

	return rect;
}

const Sprite::Point Sprite::get_center() const
{
	Point center;

	center.x = this->get_position().x + m_center.x;
	center.y = this->get_position().y + m_center.y;

	return center;
}

const bool Sprite::intersects_circle(const Sprite* p_sprite) const {

	if (this->is_collideable() && p_sprite->is_collideable() && this->is_active() && p_sprite->is_active()) {
		const Point p1 = this->get_center();
		const Point p2 = p_sprite->get_center();
		float y = p2.y - p1.y;
		float x = p2.x - p1.x;
		float d = x * x + y * y;
		/*float r1 = this->get_radius() * this->get_radius();
		float r2 = p_sprite->get_radius() * p_sprite->get_radius();*/
		float r1 = (m_size.width / 2 + m_size.height / 2) / 2;
		float r2 = (p_sprite->get_size().width / 2 + p_sprite->get_size().height / 2) / 2;
		if (d <= r1 + r2) {
			return true;
		}
	}
	return false;
}

const bool Sprite::intersects_rect(const Sprite* p_sprite) const {

	if (this->is_collideable() && p_sprite->is_collideable() && this->is_active() && p_sprite->is_active()) {
		const Rect recta = this->get_collision_rect();
		const Rect rectb = p_sprite->get_collision_rect();
		if (recta.left >= rectb.left && recta.left <= rectb.right && recta.top >= rectb.top && recta.top <= rectb.bottom)
		{
			return true;
		}
		else if (recta.right >= rectb.left && recta.right <= rectb.right && recta.top >= rectb.top && recta.top <= rectb.bottom)
		{
			return true;
		}
		else if (recta.left >= rectb.left && recta.right <= rectb.right && recta.top < rectb.top && recta.bottom > rectb.bottom)
		{
			return true;
		}
		else if (recta.top >= rectb.top && recta.bottom <= rectb.bottom && recta.left < rectb.left && recta.right > rectb.right)
		{
			return true;
		}
		else if (rectb.left >= recta.left && rectb.left <= recta.right &&
			rectb.top >= recta.top && rectb.top <= recta.bottom)
		{
			return true;
		}
		else if (rectb.right >= recta.left && rectb.right <= recta.right && rectb.top >= recta.top && rectb.top <= recta.bottom)
		{
			return true;
		}
		else if (rectb.left >= recta.left && rectb.right <= recta.right && rectb.top < recta.top && rectb.bottom > recta.bottom)
		{
			return true;
		}
		else if (recta.top >= rectb.top && recta.bottom <= rectb.bottom && recta.left < rectb.left && recta.right > rectb.right)
		{
			return true;
		}
		else if (rectb.top >= recta.top && rectb.bottom <= recta.bottom && rectb.left < recta.left && rectb.right > recta.right)
		{
			return true;
		}
	}
	return false;
}

void Sprite::adjust_gunpoint(Sprite* character, float sin[], float cos[], int angle) {

	if (m_is_gunpoint && m_is_active) {

		m_position.x = (character->get_position().x + character->get_size().width / 2) + 120 * cos[angle];
		m_position.y = (character->get_position().y + character->get_size().height / 2) - 120 * sin[angle];
	}
}