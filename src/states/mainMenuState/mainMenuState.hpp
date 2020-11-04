#pragma once

#include "states/state.hpp"
#include "physSim/physSimulation.hpp"

class MainMenuState :
	public State
{
public:
	MainMenuState(StateData* state_data);
	virtual ~MainMenuState();

	void render(sf::RenderTarget* target = nullptr);

private:
	void initVariables();
	void initKeybinds();
	void initGUI();

	//func
	void updateInput(const float& dt);
	void updateGUI(const float& dt);
	void update(const float& dt);
	void renderGUI(sf::RenderTarget& target);
};

