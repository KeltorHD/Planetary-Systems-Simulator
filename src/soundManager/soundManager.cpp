#include "pch.h"
#include "soundManager.hpp"

SoundManager::SoundManager(const std::string& path_to_sounds, const std::string& format)
	:path(path_to_sounds), format(format)
{
}

SoundManager::~SoundManager()
{
}

bool SoundManager::loadSound(std::string name)
{
	if (this->buffer[name].loadFromFile(path + name + format))
	{
		this->sound[name].setBuffer(this->buffer[name]);
		return true;
	}
	return false;
}

void SoundManager::setAllVolume(float volume)
{
	for (auto& i : this->sound)
	{
		i.second.setVolume(volume);
	}
}

void SoundManager::setVolume(std::string name, float volume)
{
	this->sound[name].setVolume(volume);
}

void SoundManager::setRepeat(std::string name, bool repeat)
{
	this->sound[name].setLoop(repeat);
}

void SoundManager::stop(std::string name)
{
	this->sound[name].stop();
}

bool SoundManager::isPlaying(std::string name)
{
	return (this->sound[name].getStatus() == sf::Sound::Playing);
}

void SoundManager::play(std::string name)
{
	if (this->sound[name].getStatus() != sf::Sound::Playing)
		this->sound[name].play();
}