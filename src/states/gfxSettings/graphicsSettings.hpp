#pragma once

class GraphicsSettings
{
public:
	GraphicsSettings();

	//variables
	std::string title;
	sf::VideoMode resolution;
	bool fullscreen;
	bool verticalSync;
	unsigned framerateLimit;
	std::vector<sf::VideoMode> videoModes;

	//func
	void saveToFile(const std::string path);
	void loadFromFile(const std::string path);
};