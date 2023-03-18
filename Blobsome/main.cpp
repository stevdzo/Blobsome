#include "stdafx.h"
#include <Windows.h>

#include "Sprite.h"
#include "Character.h"
#include "Weapon.h"
#include "GameScreen.h"
#include "Button.h"
#include "Block.h"
#include "Input.h"
#include "SoundSystem.h"

float vreme = 0;

Sprite* background;
Sprite* background_blur;
Sprite* background_credits;
Sprite* ui_paused;
Sprite* ui_game_over;
Sprite* gunpoint;
Sprite* pineapple;

Block* ground;

Character* blob;

Character* enemies[3];

Weapon* small_blomb;
Weapon* big_blomb;
Weapon* blomb;
Weapon* enemy_bombs[3];
Weapon* bomb;

GameScreen* gs_menu;
GameScreen* gs_controls;
GameScreen* gs_game_over;
GameScreen* gs_highscore;
GameScreen* gs_splash;

Button* button_play;
Button* button_credits;
Button* button_quit;
Button* button_resume;
Button* button_restart;
Button* button_menu;
Button* button_controls;
Button* button_back;
Button* button_highscore;

Block* wall;
Block* blocks[10];

std::vector<Character*> vec_enemy;
std::vector<Button*> vec_buttons;

std::ofstream out("highscore.txt", std::ios_base::app);
std::ifstream in("highscore.txt", std::ios_base::in);

SoundSystem* sound;

void build_angles(void) { // pravi tabelu uglova za lakse ispaljivanje bombi

	for (int ang = 0; ang <= 360; ang++) {

		float theta = (float)ang*PI/(float)180;

		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);
	}
}

void draw_text(char *string, float x, float y, const float r, const float g, const float b) {

	glColor3f(r, g, b);
	char *c;
	glRasterPos3f(x, y, 0.0);
	for (c = string; *c != '\0'; c++) {

		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}

void draw_health() { // iscrtava health iznad karaktera

	char output[50];
	int health;

	health = blob->get_health();
	sprintf(output, "%d", health);
	if (blob->is_alive())
	draw_text(output, blob->get_position_x() + blob->get_size().width / 3.0f, blob->get_position_y() - 15.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 3; i++) {
		health = enemies[i]->get_health();
		sprintf(output, "%d", health);
		if (enemies[i]->is_alive())
			draw_text(output, enemies[i]->get_position_x() + enemies[i]->get_size().width / 3.0f, 
							  enemies[i]->get_position_y() - 15.0f, 1.0f, 1.0f, 1.0f);
	}
}

void draw_level() { // ispisuje trenutni nivo

	char output[50];

	sprintf(output, "Level: %d", level);
	draw_text(output, 10.0f, 60.0f, 1.0f, 1.0f, 1.0f);
}

void draw_score() { // ispisuje trenutni skor

	char output[50];
	int score = blob->get_score();

	sprintf(output, "Score: %d", score);
	draw_text(output, 10.0f, 30.0f, 1.0f, 1.0f, 1.0f);
}

std::vector<std::string> vec_scores;

bool comparator(std::string s1, std::string s2) {

	if (s1.size() == s2.size()) {
		return s1 < s2;
	}
	else {
		return s1.size() < s2.size();
	}
}

void draw_high_score() { // prikazuje highscore

	float space_y = 40.0f;	
	std::string str;

	while (std::getline(in, str)) {

		if (str.size() > 0)
			vec_scores.push_back(str);
	}
	in.close();	

	std::sort(vec_scores.rbegin(), vec_scores.rend(), comparator);

	for (int i = 0; i < vec_scores.size(); i++) {

		printf("%s\n", vec_scores[i].c_str());
		draw_text((char*)vec_scores[i].c_str(), SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + space_y, 1.0f, 1.0f, 1.0f);
		space_y += 40.0f;
	}
}

void credits_text() { // prikazuje credits

	char* text_arr[4]{ "BLOBSOME PARTY 2020",
					   "Author: Stevan Jovanov",
					   "Music and sound effects: Stevan Jovanov",
					   "Programming: Stevan Jovanov" };

	for (int i = 0; i < 4; i++) {
		draw_text(text_arr[i], SCREEN_WIDTH / 2.0f - 100.0f, text_pos[i], 1.0f, 1.0f, 1.0f);
		text_pos[i] += 6.0f;
	}	
}

// ucitavanje svih tekstura
const bool load_blocks() {

	wall = new Block(1);
	wall->set_frame_size(20.0f, 400.0f);
	wall->set_position(SCREEN_WIDTH / 2, 660.0f);
	wall->set_number_off_frames(1);
	wall->add_texture("resources/blocks/wall.png");

	wall->is_active(true);
	wall->is_visible(true);
	wall->is_collideable(true);

	for (int i = 0; i < 10; i++) {
	
		blocks[i] = new Block(1);
		blocks[i]->set_frame_size(100.0f, 20.0f);
		blocks[i]->set_number_off_frames(1);
		blocks[i]->add_texture("resources/blocks/block.png");

		blocks[i]->is_active(true);
		blocks[i]->is_visible(true);
		blocks[i]->is_collideable(true);
	}
	blocks[0]->set_position(80.0f, 400.0f);
	blocks[1]->set_position(230.0f, 470.0f);
	blocks[2]->set_position(100.0f, 530.0f);
	blocks[3]->set_position(250.0f, 600.0f);
	blocks[4]->set_position(400.0f, 660.0f);
	blocks[5]->set_position(550.0f, 730.0f);	
	blocks[6]->set_position(450.0f, 800.0f);
	blocks[7]->set_position(250.0f, 860.0f);
	blocks[8]->set_position(400.0f, 930.0f);
	blocks[9]->set_position(200.0f, 990.0f);

	return true;
}

const bool load_buttons() {

	button_play = new Button(1);
	button_play->set_frame_size(220.0f, 75.0f);
	button_play->set_position(1475.0f, BUTTON_Y);
	button_play->set_number_off_frames(1);
	button_play->add_texture("resources/buttons/button_play.png");

	button_play->is_active(true);
	button_play->is_visible(true);
	input->add_ui_element(button_play);

	button_credits = new Button(1);
	button_credits->set_frame_size(240.0f, 50.0f);
	button_credits->set_position(1475.0f, BUTTON_Y + 120.0f);
	button_credits->set_number_off_frames(1);
	button_credits->add_texture("resources/buttons/button_credits.png");

	button_credits->is_active(true);
	button_credits->is_visible(true);
	input->add_ui_element(button_credits);

	button_controls = new Button(1);
	button_controls->set_frame_size(299.0f, 50.0f);
	button_controls->set_position(1475.0f, BUTTON_Y + 220.0f);
	button_controls->set_number_off_frames(1);
	button_controls->add_texture("resources/buttons/button_controls.png");

	button_controls->is_active(true);
	button_controls->is_visible(true);

	button_highscore = new Button(1);
	button_highscore->set_frame_size(329.0f, 50.0f);
	button_highscore->set_position(1475.0f, BUTTON_Y + 320.0f);
	button_highscore->set_number_off_frames(1);
	button_highscore->add_texture("resources/buttons/button_highscore.png");

	button_highscore->is_active(true);
	button_highscore->is_visible(true);
	input->add_ui_element(button_highscore);

	input->add_ui_element(button_controls);
	button_resume = new Button(1);
	button_resume->set_frame_size(220.0f, 50.0f);
	button_resume->set_position(ui_paused->get_position().x + 180.0f, ui_paused->get_position().y + 30.0f);
	button_resume->set_number_off_frames(1);
	button_resume->add_texture("resources/buttons/button_resume.png");

	button_resume->is_active(false);
	button_resume->is_visible(true);
	input->add_ui_element(button_resume);

	button_restart = new Button(1);
	button_restart->set_frame_size(249.0f, 50.0f);
	button_restart->set_position(ui_paused->get_position().x + 170.0f, ui_paused->get_position().y + 130.0f);
	button_restart->set_number_off_frames(1);
	button_restart->add_texture("resources/buttons/button_restart.png");

	button_restart->is_active(false);
	button_restart->is_visible(true);
	input->add_ui_element(button_restart);

	button_menu = new Button(1);
	button_menu->set_frame_size(155.0f, 50.0f);
	button_menu->set_position(ui_paused->get_position().x + 210.0f, ui_paused->get_position().y + 230.0f);
	button_menu->set_number_off_frames(1);
	button_menu->add_texture("resources/buttons/button_menu.png");

	button_menu->is_active(true);
	button_menu->is_visible(true);
	input->add_ui_element(button_menu);

	button_quit = new Button(1);
	button_quit->set_frame_size(145.0f, 58.0f);
	button_quit->set_position(ui_paused->get_position().x + 220.0f, ui_paused->get_position().y + 320.0f);
	button_quit->set_number_off_frames(1);
	button_quit->add_texture("resources/buttons/button_quit.png");

	button_quit->is_active(true);
	button_quit->is_visible(true);
	input->add_ui_element(button_quit);	

	button_back = new Button(1);
	button_back->set_frame_size(150.0f, 50.0f);
	button_back->set_position(SCREEN_WIDTH - button_back->get_size().width - 30.0f,
							 SCREEN_HEIGHT - button_back->get_size().height - 30.0f);
	button_back->set_number_off_frames(1);
	button_back->add_texture("resources/buttons/button_back.png");

	button_back->is_active(true);
	button_back->is_visible(true);
	input->add_ui_element(button_back);

	vec_buttons.push_back(button_play);
	vec_buttons.push_back(button_controls);
	vec_buttons.push_back(button_credits);
	vec_buttons.push_back(button_highscore);
	vec_buttons.push_back(button_menu);
	vec_buttons.push_back(button_back);
	vec_buttons.push_back(button_restart);
	vec_buttons.push_back(button_resume);
	vec_buttons.push_back(button_quit);

	return true;
}

const bool load_screens() {

	gs_menu = new GameScreen(1);
	gs_menu->set_frame_size(1920.0f, 1080.0f);
	gs_menu->set_number_off_frames(1);
	gs_menu->add_texture("resources/screens/main_menu.png");
	
	gs_menu->is_active(true);
	gs_menu->is_visible(true);	

	gs_controls = new GameScreen(1);
	gs_controls->set_frame_size(1920.0f, 1080.0f);
	gs_controls->set_number_off_frames(1);
	gs_controls->add_texture("resources/screens/controls_screen.png");

	gs_controls->is_active(true);
	gs_controls->is_visible(true);

	gs_game_over = new GameScreen(1);
	gs_game_over->set_frame_size(1920.0f, 1080.0f);
	gs_game_over->set_number_off_frames(1);
	gs_game_over->add_texture("resources/screens/game_over_screen.png");

	gs_game_over->is_active(true);
	gs_game_over->is_visible(true);

	gs_highscore = new GameScreen(1);
	gs_highscore->set_frame_size(1920.0f, 1080.0f);
	gs_highscore->set_number_off_frames(1);
	gs_highscore->add_texture("resources/screens/highscore_screen.png");

	gs_highscore->is_visible(true);
	gs_highscore->is_active(false);

	return true;
}

const bool load_splash() {

	game_state = GS_SPLASH;

	gs_splash = new GameScreen(1);
	gs_splash->set_frame_size(1920.0f, 1080.0f);
	gs_splash->set_number_off_frames(1);
	gs_splash->add_texture("resources/screens/splash_screen.png");

	gs_splash->is_visible(true);
	gs_splash->is_active(true);

	return true;
}

const bool load_weapons() {

	big_blomb = new Weapon(1);
	big_blomb->set_frame_size(32.0f, 32.0f);
	big_blomb->set_number_off_frames(1);
	big_blomb->add_texture("resources/weapons/big_blomb.png");
	big_blomb->set_position(blob->get_position().x + blob->get_size().width / 2, blob->get_position().y + blob->get_size().height / 2);

	big_blomb->is_active(true);
	big_blomb->is_visible(true);
	big_blomb->is_grounded(true);
	big_blomb->is_collideable(true);
	
	small_blomb = new Weapon(1);
	small_blomb->set_frame_size(16.0f, 16.0f);
	small_blomb->set_number_off_frames(1);
	small_blomb->add_texture("resources/weapons/small_blomb.png");
	small_blomb->set_position(blob->get_position().x + blob->get_size().width / 2, blob->get_position().y + blob->get_size().height / 2);

	small_blomb->is_active(true);
	small_blomb->is_visible(true);
	small_blomb->is_grounded(true);
	small_blomb->is_collideable(true);

	blomb = small_blomb;

	blomb->reference_sound(sound);

	blob->set_weapon(blomb);

	bomb = new Weapon(1);
	bomb->set_frame_size(64.0f, 64.0f);
	bomb->set_number_off_frames(1);
	bomb->add_texture("resources/weapons/bomb.png");
	bomb->set_position(rand() % (int)SCREEN_WIDTH / 2, 100.0f);

	bomb->is_active(true);
	bomb->is_visible(true);
	bomb->is_grounded(true);
	bomb->is_collideable(true);
	bomb->set_damage(40);

	for (int i = 0; i < 3; i++) {
		enemy_bombs[i] = new Weapon(1);
		enemy_bombs[i]->set_position(enemies[i]->get_position().x + enemies[i]->get_size().width / 2, 
								    enemies[i]->get_position().y + enemies[i]->get_size().height / 2);
		enemy_bombs[i]->set_frame_size(16.0f, 16.0f);
		enemy_bombs[i]->set_number_off_frames(1);
		enemy_bombs[i]->add_texture("resources/weapons/bazooka.png");

		enemy_bombs[i]->is_visible(true);
		enemy_bombs[i]->is_active(true);
		enemy_bombs[i]->is_grounded(true);
	}

	return true;
}

const bool load_images() {

	background = new Sprite(1);
	background->set_frame_size(1920.0f, 1080.0f);
	background->set_number_off_frames(1);
	background->add_texture("resources/backgrounds/bgr.png");

	background->is_visible(true);
	background->is_active(false);
	background->is_background(true);

	background_blur = new Sprite(1);
	background_blur->set_frame_size(1920.0f, 1080.0f);
	background_blur->set_number_off_frames(1);
	background_blur->add_texture("resources/backgrounds/bgr_blur.png");

	background_blur->is_visible(true);
	background_blur->is_active(false);
	background_blur->is_background(true);

	background_credits = new Sprite(1);
	background_credits->set_frame_size(1920.0f, 1080.0f);
	background_credits->set_number_off_frames(1);
	background_credits->add_texture("resources/backgrounds/bgr_credits.png");

	background_credits->is_visible(true);
	background_credits->is_active(false);
	background_credits->is_background(true);	

	ui_paused = new Sprite(1);	
	ui_paused->set_frame_size(583.0f, 390.0f);
	ui_paused->set_number_off_frames(1);
	ui_paused->add_texture("resources/ui/ui_paused.png");
	ui_paused->set_position(SCREEN_WIDTH / 2.0f - ui_paused->get_size().width / 2.0f, 
		SCREEN_HEIGHT / 2.0f - ui_paused->get_size().height / 2.0f);

	ui_paused->is_visible(true);

	ui_game_over = new Sprite(1);
	ui_game_over->set_frame_size(583.0f, 294.0f);
	ui_game_over->set_number_off_frames(1);
	ui_game_over->add_texture("resources/ui/ui_game_over.png");
	ui_game_over->set_position(SCREEN_WIDTH / 2.0f - ui_game_over->get_size().width / 2.0f,
		SCREEN_HEIGHT / 2.0f - ui_game_over->get_size().height / 2.0f + 60.0f);

	ui_game_over->is_visible(true);

	return true;
}

const bool load_textures() {

	ground = new Block(1);
	ground->set_position(0.0f, 1060.0f);
	ground->set_frame_size(1920.0f, 20.0f);
	ground->set_number_off_frames(1);
	ground->add_texture("resources/blocks/game_ground.png");

	ground->is_visible(true);
	ground->is_active(false);

	gunpoint = new Sprite(1);
	gunpoint->set_frame_size(16.0f, 16.0f);
	gunpoint->set_number_off_frames(1);
	gunpoint->add_texture("resources/gunpoint.png");

	gunpoint->is_gunpoint(true);
	gunpoint->is_active(true);
	gunpoint->is_visible(true);

	pineapple = new Sprite(1);
	pineapple->set_frame_size(51.0f, 100.0f);
	pineapple->set_number_off_frames(1);
	pineapple->add_texture("resources/pineapple.png");

	pineapple->is_active(false);
	pineapple->is_visible(false);

	blob = new Character(1);

	blob->set_position(100.0f, blob->get_ground_level());
	blob->set_frame_size(64.0f, 64.0f);
	blob->set_number_off_frames(12);
	blob->add_texture("resources/spritesheets/blob_moving_right.png");

	blob->reference_sound(sound);

	input->assign_player(blob);

	for (int i = 0; i < 3; i++) {
		enemies[i] = new Character(1);
		
		enemies[i]->set_frame_size(64.0f, 64.0f);
		enemies[i]->set_number_off_frames(1);
		enemies[i]->add_texture("resources/enemy.png");

		enemies[i]->set_health(40);
		enemies[i]->is_visible(true);
		enemies[i]->is_active(true);

		vec_enemy.push_back(enemies[i]);
	}
	enemies[0]->set_position(1800.0f, ground_level);
	enemies[1]->set_position(1300.0f, ground_level);
	enemies[2]->set_position(1500.0f, ground_level);

	enemies[0]->set_enemy_angle(30, 42);
	enemies[1]->set_enemy_angle(15, 65);
	enemies[2]->set_enemy_angle(50, 30);

	enemies[2]->set_velocity_x(40.0f);

	return true;
}

// proveravam koliziju objekata
void check_collisions() {

	blomb->check_collision(wall);

	for (int i = 0; i < 3; i++) {

		if (enemy_bombs[i]->intersects_rect(wall)) {
		
			sound->play_sound(sound->sfx_explosion);
			enemy_bombs[i]->is_grounded(true);
		}

		if (enemy_bombs[i]->intersects_rect(blob)) {

			sound->play_sound(sound->sfx_explosion);

			blob->set_health(blob->get_health() - enemy_bombs[i]->get_damage());
			blob->set_score(blob->get_score() - 300);
			enemy_bombs[i]->is_grounded(true);
		}
		if (enemy_bombs[i]->intersects_rect(ground)) {

			sound->play_sound(sound->sfx_explosion);
		}
		for (int j = 0; j < 10; j++) {

			if (enemy_bombs[i]->intersects_rect(blocks[j])) {

				sound->play_sound(sound->sfx_explosion);
				enemy_bombs[i]->is_grounded(true);
			}
		}
	}
	if (bomb->intersects_rect(blob)) {

		sound->play_sound(sound->sfx_explosion);
		blob->set_health(blob->get_health() - bomb->get_damage());
		bomb->is_grounded(true);
	}
	for (int i = 0; i < 10; i++) {
	
		if (blomb->intersects_rect(blocks[i])) {
		
			sound->play_sound(sound->sfx_explosion);
			blomb->is_grounded(true);
		}

		if (bomb->intersects_rect(blocks[i])) {
		
			sound->play_sound(sound->sfx_explosion);
			bomb->is_grounded(true);
		}
	}
	if (blomb->intersects_rect(wall)) {

		sound->play_sound(sound->sfx_explosion);
		blomb->is_grounded(true);
	}
	for (int i = 0; i < 3; i++) {
		if (blomb->intersects_rect(enemies[i])) {
			sound->play_sound(sound->sfx_explosion);
			blob->set_score(blob->get_score() + 500);
			enemies[i]->set_health(enemies[i]->get_health() - blomb->get_damage());
			blomb->is_grounded(true);
			if (enemies[i]->get_health() <= 0) {
				enemy_counter++;
			}
		}
	}
	if (blob->intersects_rect(pineapple)) {
	
		sound->play_sound(sound->sfx_pickup);
		pineapple_flag = false;
		pineapple_effect_flag = true;
		pineapple_remove_timer = 0.0f;
		pineapple->is_active(false);
		pineapple->is_visible(false);
		blomb->set_frame_size(32.0f, 32.0f);		
	}
}

void check_health() { // provera health-a

	if (blob->get_health() <= 0) {

		blob->is_alive(false);
		blob->is_visible(false);
		blob->is_active(false);
		game_state = GameState::GS_GAMEOVER;
	}
	for (int i = 0; i < 3; i++) {
		if (enemies[i]->get_health() <= 0) {

			enemies[i]->is_alive(false);
			enemies[i]->is_visible(false);
			enemies[i]->is_active(false);		
		}
	}
}

void rect(double x, double y, double width, double height, float fRed, float fGreen, float fBlue) {
	
	glLineWidth(8.0);

	glBegin(GL_LINE_LOOP);
	glColor3f(fRed, fGreen, fBlue);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
}

void load_charger(void) {
	
	if (blomb->is_charger_ready()) {
		glPushMatrix();
		glTranslatef((blob->get_position().x + blob->get_size().width / 2),
					 (blob->get_position().y + blob->get_size().height / 2) + 8, 0.0);
		glRotatef(blomb->get_angle(), 0.0f, 0.0f, -1);
		glScalef(space, 0, 0);

		// tranzicija boja
		green_alpha -= 0.6 * p_delta_time * space;
		red_alpha += p_delta_time * space;
		if (green_alpha < 0.0) { green_alpha = 0.0f; }
		//printf("GR: %f\n", g);	
		if (red_alpha > 1.0) { red_alpha = 1.0f; }
		//printf("RE: %f\n", r);
		rect(0.0, 0.0, 30.0f, 10.0f, red_alpha, green_alpha, 0);
		//rect(0.0, 0.0, 30.0f, 10.0f, 1.0f, 1.0f, 1.0f);
		glPopMatrix();
		space += 0.1f;
	}	
}

void spawn_pineapple() { // spawnujue se pickup

	int n = rand() % 10;
	if (!pineapple_flag) {
		pineapple_spawn_timer += p_delta_time;
	}
	if (pineapple_spawn_timer > 18.0f) {
	
		pineapple_flag = true; // flag od koga zavisi tajmer za spawn
		pineapple_spawn_timer = 0.0f;		
		pineapple->is_active(true);
		pineapple->is_visible(true);		
		pineapple->set_position(blocks[n]->get_position().x + blocks[n]->get_size().width / 2 - pineapple->get_size().width / 2,
			blocks[n]->get_position().y - pineapple->get_size().height - 10.0f);
		
	}
	if (pineapple->is_active()) {

		pineapple_remove_timer += p_delta_time;
	}
	if (pineapple_remove_timer > 12.0f) {
	
		pineapple_flag = false;
		pineapple_remove_timer = 0.0f;	
		pineapple->is_active(false);
		pineapple->is_visible(false);
	}
}

void pineapple_effect() {

	if (pineapple_effect_flag) {

		effect_timer += p_delta_time;
	}
	if (effect_timer > 15.0f) {

		if (blomb->is_grounded()) {
			pineapple_effect_flag = false;
			blomb->set_frame_size(16.0f, 16.0f);
			effect_timer = 0.0f;
			printf("ANANAS\n");
		}
	}
}

void move_enemy(Character* obj) {

	int n;
	enemy_move_timer += p_delta_time;
	if (enemy_move_timer > 2.0f) {

		n = rand() % 3;
		if (n == 1) {

			obj->set_velocity_x(40.0f);
		}
		else if (n == 2) {
			obj->set_velocity_x(-40.0f);
		}
		else
			obj->set_velocity_x(0.0f);
		enemy_move_timer = 0.0f;
	}
	if (obj->GetVelocity().x > 0 && obj->get_position_x() + obj->get_size().width >= enemies[0]->get_position_x()) {
	
		obj->set_velocity_x(-40.0f);
	}
	if (obj->GetVelocity().x < 0 && obj->get_position_x() <= enemies[1]->get_position_x() + enemies[1]->get_size().width) {

		obj->set_velocity_x(40.0f);
	}
} 

void spawn_drop_bomb() { // na drugom nivou ispaljuje bombu sa neba u random intervalima 

	if (level == 2) {
		drop_bomb_timer += p_delta_time;
		if (drop_bomb_timer > 4.0f) { 
		
			bomb->is_grounded(false);
			drop_bomb_timer = 0.0f;		
		}
	}
}

void blomb_active() { // zvuk eksplozije kada blomb padne na beton

	if (blob->is_alive()) {
		if (blomb->get_position().y >= blomb_ground_level) {

			sound->play_sound(sound->sfx_explosion);
			blomb->is_grounded(true);
		}
	}
}

void weapon_active(Character* enemy[], Weapon* bomb[]) { // ispaljuje bombu svakih x sekundi

	enemy_bomb_timer += p_delta_time;
	for (int i = 0; i < 3; i++) {
		if (enemy[i]->is_alive()) {
			if (enemy_bomb_timer > 5.0f) {

				bomb[i]->is_grounded(false);
			}
			if (bomb[i]->get_position().y >= blomb_ground_level) {

				sound->play_sound(sound->sfx_explosion);
				angle = enemy[i]->random_angle();				
				bomb[i]->is_grounded(true);
				bomb[i]->set_weapon_x_vel(-15 * cos_look[angle]);
				bomb[i]->set_weapon_y_vel(-15 * sin_look[angle]);
				enemy_bomb_timer = 0.0f;
			}			
		}	
	}	
}

void start_init() { // poziva se klikom na play i restart

	level = 1;
	blob->is_alive(true);
	blob->is_visible(true);
	blob->is_active(true);
	blob->set_position(100.0f, blob->get_ground_level());
	blob->set_health(100.0f);
	blob->set_score(0);
	blob->set_position(100.0f, blob->get_ground_level());
	blomb->set_position(blob->get_position().x + blob->get_size().width / 2, 
					   blob->get_position().y + blob->get_size().height / 2);
	blob->is_grounded(true);
	blomb->is_grounded(true);
	for (int i = 0; i < 3; i++) {
	
		enemies[i]->set_health(40.0f);
		enemies[i]->is_visible(true);	
		enemies[i]->is_alive(true);
		enemies[i]->is_active(true);
		enemy_bombs[i]->set_position(enemies[i]->get_position().x + enemies[i]->get_size().width / 2,
									enemies[i]->get_position().y + enemies[i]->get_size().height / 2);
	}
	enemy_bomb_timer = 0.0f;
}

void next_level() { // prelazak na sledeci nivo

	if (enemy_counter == 3) {

		enemy_counter = 0;
		//counter = 5;
		level++;

		blob->set_health(100.0f);
		blomb->set_position(blob->get_position().x + blob->get_size().width / 2,
						   blob->get_position().y + blob->get_size().height / 2);
		blob->is_grounded(true);
		blomb->is_grounded(true);
		for (int i = 0; i < 3; i++) {

			enemies[i]->set_health(60.0f);
			enemies[i]->is_alive(true);
			enemies[i]->is_visible(true);
			enemies[i]->is_active(true);
			enemy_bombs[i]->set_position(enemies[i]->get_position().x + enemies[i]->get_size().width / 2,
										enemies[i]->get_position().y + enemies[i]->get_size().height / 2);
		}
		enemy_bomb_timer = 0.0f;
	}
}

void game_won() { // kraj igre

	if (level == 2) {

		if (enemy_counter == 3) {
			
			out << blob->get_score() << '\n';
			game_state = GS_GAMEOVER;
			enemy_counter = 0;
		}
	}
}

void process_input() { // procesira ulaz sa tastature i misa

	Input::Command command = input->GetCommand();

	ui_timer += p_delta_time;
	if (ui_timer > UI_THRESHOLD) {

		ui_timer = 0.0f;
		switch (command) {

		case Input::Command::CM_UI: {

			if (button_play->is_clicked()) {
						
				sound->play_music(sound->sfx_song);
				sound->play_sound(sound->sfx_button);
				start_init();

				button_play->is_clicked(false);
				button_play->is_active(false);	
				button_highscore->is_active(false);
				button_controls->is_active(false);
				button_credits->is_active(false);
				button_quit->is_active(false);
				button_back->is_active(false);

				game_state = GS_RUNNING;
			}
			if (button_credits->is_clicked()) {
			
				sound->play_sound(sound->sfx_button);

				button_credits->is_clicked(false);

				button_controls->is_active(false);
				button_play->is_active(false);
				button_credits->is_active(false);

				button_back->is_active(true);

				game_state = GS_CREDITS;

			}
			if (button_highscore->is_clicked()) {
			
				sound->play_sound(sound->sfx_button);

				button_highscore->is_clicked(false);

				button_highscore->is_active(false);
				button_controls->is_active(false);				
				button_play->is_active(false);
				button_credits->is_active(false);	

				button_menu->is_active(true);

				game_state = GS_HIGHSCORE;
			}
			if (button_controls->is_clicked()) {

				sound->play_sound(sound->sfx_button);

				button_controls->is_clicked(false);

				button_controls->is_active(false);
				button_play->is_active(false);
				button_credits->is_active(false);

				button_menu->is_active(true);

				game_state = GS_CONTROLS;
			}
			if (button_resume->is_clicked()) {

				sound->play_sound(sound->sfx_button);

				button_resume->is_clicked(false);
				button_resume->is_active(false);

				game_state = GS_RUNNING;
			}
			if (button_restart->is_clicked()) {
			
				sound->play_sound(sound->sfx_button);

				button_restart->is_clicked(false);
				button_restart->is_active(false);				

				start_init();
				game_state = GS_RUNNING;
			}
			if (button_menu->is_clicked()) {			
				
				sound->play_music(sound->sfx_ambient);
				sound->play_sound(sound->sfx_button);

				button_menu->is_clicked(false);
				button_menu->is_active(false);
				button_resume->is_active(false);
				button_restart->is_active(false);

				game_state = GS_MENU;
			}
			if (button_quit->is_clicked()) {
							
				sound->play_sound(sound->sfx_button);

				exit(0);
			}
			if (button_back->is_clicked()) {

				sound->play_sound(sound->sfx_button);

				button_back->is_clicked(false);
				button_back->is_active(false);			

				game_state = GS_MENU;
			}
		}								  
		break;
		}
		//input->SetCommand(Input::CM_STOP);	
	}
}
	
void update_game_state() { // azuriranje pozicije objekata u zavisnosti od stanja igre

	switch (game_state) {

	case GS_MENU:
		gs_menu->update(p_delta_time);	
		break;
	case GS_LOADING: {
	
		gs_splash->update(p_delta_time);
		splash_timer += p_delta_time;
		if (splash_timer > SPLASH_THRESHOLD) {
		
			game_state = GS_MENU;
		}
	}
		break;
	case GS_RUNNING:		

		background->update(p_delta_time);
		ground->update(p_delta_time);
		blob->update(p_delta_time);
		bomb->update(p_delta_time);
		gunpoint->update(p_delta_time);
		wall->update(p_delta_time);

		blob->block_collision(blocks);
		blob->block_collision(wall);	
		blob->check_boundaries();
		blob->has_jumped(p_delta_time);

		blomb->fire(blob, sin_look, cos_look, blomb->get_angle(), 1);
		blomb->update_angles(sin_look[blomb->get_angle()], cos_look[blomb->get_angle()]);
		for (int i = 0; i < 3; i++) {
			enemy_bombs[i]->fire(enemies[i], sin_look, cos_look, angle, -1);
		}
		move_enemy(enemies[2]);	
		weapon_active(enemies, enemy_bombs);
		blomb_active();	
		
		check_collisions();
		check_health();
		
		spawn_drop_bomb();
		spawn_pineapple();
		pineapple_effect();
		gunpoint->adjust_gunpoint(blob, sin_look, cos_look, blomb->get_angle());	
			
		next_level();
		game_won();

		input->check_input();

		break;
	case GS_PAUSED:
		break;
	case GS_CONTROLS:
		gs_controls->update(p_delta_time);
		break;
	case GS_GAMEOVER:
		if (out_flag) {
			out << blob->get_score() << '\n';
			out_flag = false;
		}
		break;
	}
}

void render_game_state() { // iscrtavanje tekstura svakog frejma igre, opet u zavisnosti od stanja igre

	switch (game_state) {

	case GS_MENU:

		button_play->is_active(true);
		button_credits->is_active(true);
		button_controls->is_active(true);
		button_highscore->is_active(true);
		button_quit->is_active(true);

		button_quit->set_position(1475.0f, BUTTON_Y + 400.0f);

		gs_menu->render();
		button_play->render();
		button_credits->render();
		button_controls->render();
		button_highscore->render();
		button_quit->render();

		break;
	case GS_LOADING:
		gs_splash->render();
		break;
	case GS_RUNNING:
		background->render();
		ground->render();
		blob->render();
		blomb->render();
		bomb->render();
		gunpoint->render();
		pineapple->render();

		for (int i = 0; i < 3; i++) {
			enemies[i]->render();
			enemy_bombs[i]->render();
		}	
		wall->render();
		for (int i = 0; i < 10; i++) { blocks[i]->render(); }
		draw_health();
		draw_score();
		draw_level();		
	
		if (blomb->is_grounded()) load_charger();
		else {
			blomb->set_charger_flag(false);
			space = 0.0f;
			red_alpha = 0.0f;
			green_alpha = 1.0f;
		}
		//glScalef(blomb_speed, 0.0f, 0.0f);

		break;
	case GS_PAUSED:
	
		button_resume->is_active(true);
		button_restart->is_active(true);
		button_menu->is_active(true);
		button_quit->is_active(true);
		
		background_blur->render();
		ui_paused->render();

		button_menu->set_position(ui_paused->get_position().x + 210.0f, ui_paused->get_position().y + 230.0f);		
		button_quit->set_position(ui_paused->get_position().x + 220.0f, ui_paused->get_position().y + 320.0f);

		button_resume->render();
		button_restart->render();
		button_menu->render();
		button_quit->render();

		break;
	case GS_CREDITS:

		background_credits->render();
		credits_text();

		button_back->is_active(true);
		button_back->render();

		break;
	case GS_CONTROLS:

		gs_controls->render();

		button_back->is_active(true);
		button_back->render();

		break;
	case GS_GAMEOVER:	

		gs_game_over->render();
		ui_game_over->render();

		button_quit->set_position(ui_game_over->get_position().x + 220.0f, ui_paused->get_position().y + 320.0f);

		button_menu->is_active(true);
		button_restart->is_active(true);
		button_quit->is_active(true);

		button_menu->render();
		button_restart->render();
		button_quit->render();

		break;
	case GS_HIGHSCORE:

		button_back->is_active(true);

		gs_highscore->render();
		draw_high_score();
		button_back->render();

		break;
	}
}
void update() { 

	update_game_state();
	process_input();
}

void render() {

	glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	render_game_state();
	
	glutSwapBuffers();
}

void start_game() { // pocetak igre, splash animacija

	load_splash();	

	splash_timer = 0.0f;
	ui_timer = 0.0f;
	enemy_bomb_timer = 0.0f;
	input = new Input();
	sound = new SoundSystem();
	sound->init_fmod();
	sound->load_audio();

	

	sound->play_music(sound->sfx_ambient);
}

void game_loop() { // petlja igre, GS_SPLASH ucitava sve teksture

	if (game_state == GS_SPLASH) {

		load_screens();
		load_images();
		load_buttons();
		load_textures();
		load_blocks();
		load_weapons();

		build_angles();
		game_state = GS_LOADING;
	}
	update();
	render();
}

void end_game() { // kraj igre i brisanje objekata
	
	for (auto& i : vec_buttons) { delete i; }
	for (auto& i : enemies) { delete i; }
	for (auto& i : enemy_bombs) { delete i; }

	delete text_pos;
	delete background;
	delete background_blur;
	delete background_credits;
	delete bomb;
	delete blomb;
	delete big_blomb;
	delete small_blomb;
	delete blob;
	delete gunpoint;
	delete pineapple;
	delete wall;
	delete ground;
	delete input;
	delete ui_paused;
	delete ui_game_over;
	delete gs_menu;
	delete gs_controls;
	delete gs_game_over;
	delete gs_highscore;
	delete gs_splash;
	delete sound;
}

void reshape(GLsizei p_width, GLsizei p_height) {

	/*screen_height = (GLfloat)p_height;
	screen_width = (GLfloat)p_width;*/

	GLsizei h = p_height;
	GLsizei w = p_width;
	if (h == 0) h = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init_gl() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_DEPTH_TEST);

	previousTime = glutGet(GLUT_ELAPSED_TIME);
}

void timer(int value) {

   glutPostRedisplay();
   glutTimerFunc(refreshMills, timer, 0);
}

void display() {

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	p_delta_time = (float)(currentTime - previousTime) / 500;
	previousTime = currentTime;	

	vreme += p_delta_time;

	game_loop();
}

void key_press_down(unsigned char key, int x, int y) {

	switch (key) {

	case 27:

		if (game_state == GS_RUNNING) game_state = GS_PAUSED;
		break;

	default:
		input->key_press_down(key, x, y);
	}
}

void key_press_up(unsigned char key, int x, int y) { input->key_press_up(key, x, y); }

void mouse(int button, int state, int x, int y) {

	input->mouse(button, state, x, y);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Blobsome Party");

	// Petlje
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);

	// Kontrola misa i tastature
	glutKeyboardFunc(key_press_down);
	glutKeyboardUpFunc(key_press_up);
	glutMouseFunc(mouse);	

	// Inicijalizacija pocetnih vrednosti
	init_gl();
	start_game();

	// Petlja igre
	glutMainLoop();

	// Kraj igre
	end_game();

	return 0;
}