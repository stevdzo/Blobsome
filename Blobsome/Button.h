#pragma once

#include "Sprite.h"

class Button : public Sprite {

	bool m_is_clicked;

public:

	Button(const unsigned int);
	~Button();

	void is_clicked(const bool p_value);
	const bool is_clicked() const;
};