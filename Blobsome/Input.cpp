#include "stdafx.h"
#include "Input.h"
#include "Weapon.h"

Input::Input() {

	m_command = Command::CM_STOP;
	m_uiElements = new Button* [10];
	m_uiCount = 0;
}

Input::Input(Character* _player) : player(_player) {

	m_command = Command::CM_STOP;
	m_uiElements = new Button * [10];
	m_uiCount = 0;
}

Input::~Input() {

	delete[] m_uiElements;
	m_uiElements = NULL;
}

void Input::mouse(int button, int state, int x, int y) {

	m_command = Command::CM_STOP;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (unsigned int i = 0; i < m_uiCount; i++) {
			Button* element = m_uiElements[i];
			if (element->is_active() == true) {
				if (check_for_click(element, x, y)) {
					element->is_clicked(true);
					m_command = Input::Command::CM_UI;
					return;
				}
			}
		}
	}
	else {
		m_command = Command::CM_INVALID;
	}
}

void Input::add_ui_element(Button* p_element) {
	m_uiElements[m_uiCount] = p_element;
	m_uiCount++;
}

const bool Input::check_for_click(Button* p_element, int x, int y) const {
	float left = p_element->get_position().x;
	float right = p_element->get_position().x + p_element->get_size().width;

	float top = p_element->get_position().y;
	float bottom = p_element->get_position().y + p_element->get_size().height;

	if (x >= left && x <= right && y >= top && y <= bottom) return true;
	else                                           	         return false;
}

void Input::key_press_down(unsigned char key, int x, int y) {

	key_down[key] = true;
}

void Input::key_press_up(unsigned char key, int x, int y) {

	if (key_down[' ']) {
			
		player->get_weapon()->space_release(player);
	}

	key_down[key] = false;
	player->movement(false);
}

void Input::check_input() {

		if (key_down['a'] || key_down['A']) {

			player->movement(true, 'a');
		}
		if (key_down['d'] || key_down['D']) {

			player->movement(true, 'd');
		}
		if (key_down['w'] || key_down['W']) {
		
			player->movement(true, 'w');
		}
		if (key_down[' ']) {

			player->movement(true, ' ');
		}
		if (key_down['p'] || key_down['P']) {
				
			player->movement(true, 'p');
		}
		if (key_down['l'] || key_down['L']) {

			player->movement(true, 'l');
		}
}

void Input::assign_player(Character* _player) { player = _player; }