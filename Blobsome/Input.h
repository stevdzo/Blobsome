#pragma once

#include "Button.h"
#include "Character.h"

class Weapon;

class Input
{
public:
	enum Command
	{
		CM_INVALID = -1,
		CM_STOP,
		CM_UI
	}; 
protected:

	Command m_command;
	Button** m_uiElements;
	unsigned int m_uiCount;
	const bool check_for_click(Button* p_element, int x, int y) const;

	Character* player;

public:

	Input();
	Input(Character*);
	~Input();
	
	const Command GetCommand() const { return m_command; }
	void SetCommand(Command command) { m_command = command; }

	void keyboard_key(int key, int x, int y);
	void keyboard_spec(int key, int x, int y);
	void mouse(int button, int state, int x, int y);

	void add_ui_element(Button* m_pElement);

	void key_press_down(unsigned char key, int x, int y);
	void key_press_up(unsigned char key, int x, int y);
	void check_input();

	void assign_player(Character*);
};