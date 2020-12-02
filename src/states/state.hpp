#pragma once

#include "programSettings/programSettings.hpp"
#include "localisation/locale.hpp"
#include "soundManager/soundManager.h"

class State;

class StateData
{
public:
	StateData();

	//var
	sf::RenderWindow* window;
	ProgramSettings* programSettings;
	std::map<std::string, int>* supportedKeys;
	std::stack<State*>* states;
	Locale* locale;
	SoundManager* soundManager;
	std::queue<sf::Event> events;
};

class State
{
public:
	State(StateData* state_data);
	virtual ~State();

	//accessors
	const bool& getQuit() const;

	//func
	void endState();
	void pauseState();
	void unpauseState();

	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

protected:
	StateData* stateData;
	Locale* locale;
	SoundManager* soundManager;
	std::stack<State*>* states;

	sf::RenderWindow* window;
	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	bool quit;

	//Resources
	std::map<std::string, sf::Texture> textures;

	//Func
	virtual void initKeybinds() = 0;

};

