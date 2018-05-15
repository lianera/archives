#include <vector>
#include <string>
#include "sound.h"

Sound Sound::instance;

using std::vector;
using std::string;

void Sound::Load()
{
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
		throw Mix_GetError();

	vector<string> chunkfiles = {
		"click.wav", "eat.wav", "win.wav", "mouseon.wav", "fail.wav"
	};

	vector<string> musicfiles = {
		"menubkm.mid", "map0.mid"
	};

	for (string c : chunkfiles) {
		Mix_Chunk* p = Mix_LoadWAV(("res/" + c).c_str());
		chunks[c] = p;
	}
	for (string m : musicfiles) {
		Mix_Music* p = Mix_LoadMUS(("res/" + m).c_str());
		musics[m] = p;
	}

}

void Sound::Release()
{
	for (auto p : chunks)
		Mix_FreeChunk(p.second);
	for (auto p : musics)
		Mix_FreeMusic(p.second);

	Mix_CloseAudio();
}

void Sound::PlayChunk(std::string name)
{
	Mix_PlayChannel(-1, chunks.at(name), 0);
}

void Sound::PlayMusic(std::string name)
{
	if (name == current_music)
		return;
	Mix_RewindMusic();
	Mix_PlayMusic(musics.at(name), -1);
	current_music = name;
}

void Sound::StopMusic()
{
	current_music.clear();
	Mix_HaltMusic();
}