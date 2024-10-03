#pragma once
#include "raylib.h"

//Class for all sounds related
class sound_manager {
public:
	static void LoadSounds();
	static void LoadMusic();
	static void PlaySoundFromList(int sound);
	static void playMusicFromList(int music);
	static void stopMusic();
};

//Sound list
#define MAX_SOUNDS 9
enum soundAssets {
	SOUND_MOUSE_TOUCH = 0,
	SOUND_RESTART_LEVEL,
	SOUND_LEVEL_ERROR,
	SOUND_UT_NOISE,
	SOUND_DRUMROLL,
	SOUND_HIGHSCORE,
	SOUND_TADA,
	SOUND_WOMPWOMP,
	SOUND_AVERAGE
};

Sound sounds[MAX_SOUNDS];

//Load all sounds
void sound_manager::LoadSounds() {
	sounds[SOUND_MOUSE_TOUCH] = LoadSound("assets/snd_mouse.wav");
	sounds[SOUND_RESTART_LEVEL] = LoadSound("assets/snd_restart.wav");
	sounds[SOUND_LEVEL_ERROR] = LoadSound("assets/snd_error.wav");
	sounds[SOUND_UT_NOISE] = LoadSound("assets/snd_noise.wav");
	sounds[SOUND_DRUMROLL] = LoadSound("assets/snd_drumroll.wav");
	sounds[SOUND_HIGHSCORE] = LoadSound("assets/snd_ballchime.ogg");
	sounds[SOUND_TADA] = LoadSound("assets/snd_won.wav");
	sounds[SOUND_WOMPWOMP] = LoadSound("assets/mus_wawa.ogg");
	sounds[SOUND_AVERAGE] = LoadSound("assets/snd_buyitem.wav");
}


//Music list
#define MAX_MUSIC 3

enum musicAssets {
	MUSIC_TITLE_SCREEN = 0,
	MUSIC_GAME,
	MUSIC_ENDING
};


Music musics[MAX_MUSIC];

//Pretty necessary for music swapping
Music current_song;

//Load all music
void sound_manager::LoadMusic() {
	musics[MUSIC_TITLE_SCREEN] = LoadMusicStream("assets/thrashmachine.wav");
	musics[MUSIC_GAME] = LoadMusicStream("assets/Levitated_Trickles.wav");
	musics[MUSIC_ENDING] = LoadMusicStream("assets/CDTidalTempest.wav");
}

//Play a sound from the library
void sound_manager::PlaySoundFromList(int sound) {
	PlaySound(sounds[sound]);
}

//Play music from the library
void sound_manager::playMusicFromList(int music) {
	PlayMusicStream(musics[music]);
	current_song = musics[music];
}


//Stop the current song.
void sound_manager::stopMusic() {
	StopMusicStream(current_song);
}
