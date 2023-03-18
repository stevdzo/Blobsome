#pragma once

#include "Globals.h"
#include "glut.h"
#include <math.h>
#include "SoundSystem.h"

class Sprite {

public:

	struct Point {

		GLfloat x;
		GLfloat y;
	};

	struct Size {

		GLfloat width;
		GLfloat height;
	};

	struct Rect {

		GLfloat top;
		GLfloat bottom;
		GLfloat left;
		GLfloat right;
	};

public:

	GLuint* m_textures;
	unsigned int m_textureIndex;
	unsigned int m_currentFrame;
	unsigned int m_numberOfFramesX;
	unsigned int m_numberOfFramesY;
	unsigned int m_number_off_frames;
	GLfloat m_animationDelay;
	GLfloat m_animationElapsed;

	Point m_position;
	Point m_center;
	float m_radius;
	Size m_size;
	Point m_velocity;
	GLfloat m_friction;
	GLfloat m_ground_level;
	Rect m_collision;

	float m_weapon_x_vel;
	float m_weapon_y_vel;

	std::vector<GLfloat> m_land_points;

	bool m_is_collideable;
	bool m_flip_horizontal;
	bool m_flip_vertical;
	bool m_is_visible;
	bool m_is_active;
	bool m_use_transparency;
	bool m_is_sprite_sheet;
	bool m_is_background;
	bool m_isCharacter;
	bool m_is_grounded;
	bool m_is_gunpoint;

	SoundSystem* m_sound;

public:

	Sprite(const unsigned int p_numberOfTextures);
	Sprite(const Sprite &sprite);
	~Sprite();

	virtual void update(const float p_deltaTime);
	void render();

	void reference_sound(SoundSystem* sound) { m_sound = sound; }

	const bool add_texture(const char* p_fileName);

	void flip(bool b) { m_flip_horizontal = b; }
	bool is_looking_right() { return m_flip_horizontal; }

	const GLuint get_current_frame() { 
		return m_is_sprite_sheet ? m_textures[0] : m_textures[m_currentFrame]; 
	}

	void set_position(const GLfloat p_x, const GLfloat p_y) {
		m_position.x = p_x; 
		m_position.y = p_y; 
	}
	void set_position(const Point p_position) { 
		m_position = p_position; 
	}
	const Point get_position() const { 
		return m_position; 
	}
	const Size get_size() const { return m_size; }

	void set_frame_size(const GLfloat p_width, const GLfloat p_height) {
		m_size.width = p_width; 
		m_size.height = p_height; 
	}

	const GLfloat getVelocity(void) { return m_velocity.x; }

	void set_velocity_x(const GLfloat x) { 
		
		m_velocity.x = x;
	}

	void SetVelocity(const GLfloat p_velocityX, const GLfloat p_velocityY) {

		m_velocity.x = p_velocityX;
		m_velocity.y = p_velocityY; 
	}

	void SetFriction(const GLfloat friction) { 
		m_friction = friction;  
	}

	const Point GetVelocity() const { return m_velocity; }

	void set_number_off_frames(const unsigned int p_frames) { 
		m_number_off_frames = p_frames;
	}
	const bool is_collideable() const { return m_is_collideable; }
	void is_collideable(const bool p_value) { m_is_collideable = p_value; }

	void flip_horizontal(const bool p_value) { m_flip_horizontal = p_value; }
	void flip_vertical(const bool p_value) { m_flip_vertical = p_value; }

	void is_active(const bool p_value) { m_is_active = p_value; }
	const bool is_active() const { return m_is_active; }

	void is_visible(const bool p_value) { m_is_visible = p_value; }
	const bool is_visible() const { return m_is_visible; }

	void UseTransparency(const bool p_value) {m_use_transparency = p_value;}

	void is_background(const bool p_value) { m_is_background = p_value; }
	const bool is_background() const { return m_is_background; }

	void is_grounded(const bool p_value) { m_is_grounded = p_value; }
	const bool is_grounded() const { return m_is_grounded; }

	const Rect get_collision_rect() const;
	void set_collision_rect_offset(const Rect p_rect) { m_collision = p_rect; }

	void set_speed_x(const GLfloat x) {
		
		m_velocity.x = x;
		m_position.x += m_velocity.x;
	}

	void set_speed_y(const GLfloat y) {
	
		m_velocity.y = y;
		m_position.y += m_velocity.y;
	}

	const float get_ground_level() const { return m_ground_level; }
	void set_ground_level(const float level) { m_ground_level = level; }

	const float get_position_x() const { return m_position.x; }
	void set_position_x(const float x) { m_position.x = x; }

	const float get_position_y() const { return m_position.y; }
	void set_position_y(const float y) { m_position.y = y; }

	const float get_weapon_x_vel() const { return m_weapon_x_vel; }
	void set_weapon_x_vel(const float x) { m_weapon_x_vel = x; }

	void is_gunpoint(const bool p_value) { m_is_gunpoint = p_value; }

	void set_radius(const GLfloat p_radius) { m_radius = p_radius; }
	const float get_radius() const { return m_radius; }

	void set_center(const Point p_center) { m_center = p_center; }
	const Point get_center() const;

	const bool intersects_circle(const Sprite* p_sprite) const;
	const bool intersects_rect(const Sprite* p_sprite) const;

	void adjust_gunpoint(Sprite*, float[], float[], int);
};