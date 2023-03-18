#pragma once

#include <iostream>
#include "fmod.hpp"

class SoundSystem{

public:

	FMOD::System* audiomgr;
	FMOD::Sound* sfx_move;
	FMOD::Sound* sfx_fire;
	FMOD::Sound* sfx_ambient;
	FMOD::Sound* sfx_song;
	FMOD::Sound* sfx_explosion;
	FMOD::Sound* sfx_button;
	FMOD::Sound* sfx_pickup;

	FMOD_RESULT result;
	FMOD::Channel* channel;

	SoundSystem() {}
	~SoundSystem() {

		sfx_fire->release();
		sfx_pickup->release();
		sfx_button->release();
		sfx_ambient->release();
		sfx_explosion->release();
		sfx_song->release();
	}

	bool init_fmod() { // inicijalizacija zvuka

		FMOD_RESULT result;
		result = FMOD::System_Create(&audiomgr);
		if (result != FMOD_OK) {
			return false;
		}
		result = audiomgr->init(50, FMOD_INIT_NORMAL, NULL);
		if (result != FMOD_OK) {
			return false;
		}
		return true;
	}

	const bool load_audio() { // ucitavanje zvukova

		result = audiomgr->createSound("resources/sounds/blob_fire.mp3", FMOD_DEFAULT, 0, &sfx_fire);
		result = audiomgr->createSound("resources/sounds/ambient.wav", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sfx_ambient);
		result = audiomgr->createSound("resources/sounds/pickup.wav", FMOD_DEFAULT, 0, &sfx_pickup); 
		result = audiomgr->createSound("resources/sounds/song.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sfx_song);
		result = audiomgr->createSound("resources/sounds/explosion.mp3", FMOD_DEFAULT, 0, &sfx_explosion);
		result = audiomgr->createSound("resources/sounds/button.mp3", FMOD_DEFAULT, 0, &sfx_button);
		
		return true;
	}

	void play_music(FMOD::Sound* sfx) {
	
		channel->stop();
		audiomgr->playSound(sfx, 0, false, &channel);
		channel->setVolume(0.2);
	}

	void play_sound(FMOD::Sound* sfx) {

		FMOD::Channel* channel;
		audiomgr->playSound(sfx, 0, false, &channel);
		channel->setVolume(0.5);
	}
};