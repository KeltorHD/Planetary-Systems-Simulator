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
	PhysSimulation simulation;

	/*���������� ��� ��������*/
	bool isSettings;
	char** lang;
	size_t lang_length;

	void initVariables();
	void initKeybinds();

	//func
	void updateInput(const float& dt);
	void updateGUI(const float& dt);
	void update(const float& dt);
};

