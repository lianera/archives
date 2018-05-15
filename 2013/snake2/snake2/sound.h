#pragma once

#include <string>
#include <SDL2/SDL_mixer.h>
#include <map>

class Sound
{
public:
	void Load();
	void Release();
	void PlayChunk(std::string name);
	void PlayMusic(std::string name);
	void StopMusic();

	static Sound* Instance()
	{
		return &instance;
	}
private:
	Sound() {}
	static Sound instance;
	std::string current_music;
	std::map<std::string, Mix_Chunk*> chunks;
	std::map<std::string, Mix_Music*> musics;
};