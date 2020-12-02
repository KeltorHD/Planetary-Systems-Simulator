#pragma once

#include "states/mainMenuState/mainMenuState.hpp"

class Program
{
public:
	Program();
	virtual ~Program();

	//core
	void run();

private:
	//Variables
	ProgramSettings programSettings;
	StateData stateData;
	sf::RenderWindow* window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	std::map<std::string, int> supportedKeys;

	//Initialization
	void initVariables();
	void initGraphicsSettings();
	void initWindow();
	void initKeys();
	void initStateData();
	void initStates();

	//func
	void endApplication();

	//update
	void updateDt_ImGui();
	void updateSFMLEvents();
	void update();

	//render
	void render();

};