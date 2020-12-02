#pragma once

class SoundManager
{
public:
	SoundManager(const std::string& path_to_sounds, const std::string& format);
	~SoundManager();

	/*load sound*/
	bool loadSound(std::string name);

	/*set Volume*/
	void setAllVolume(float volume);
	void setVolume(std::string name, float volume);

	/*set repeat*/
	void setRepeatre(std::string name, bool repeat);
	
	/*play*/
	void play(std::string name);

	/*stop*/
	void stop(std::string name);

	/*isPlyaing*/
	bool isPlaying(std::string name);

private:
	std::map<std::string, sf::SoundBuffer> buffer;
	std::map<std::string, sf::Sound> sound;
	std::string path;
	std::string format;
};