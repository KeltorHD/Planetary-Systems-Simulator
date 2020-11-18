#pragma once

#include "states/state.hpp"
#include "physSim/physSimulation.hpp"
#include "states/simulationState/simulationState.hpp"

class MainMenuState final:
	public State
{
public:
	MainMenuState(StateData* state_data);
	virtual ~MainMenuState();

	void render(sf::RenderTarget* target = nullptr) override;

private:
	PhysSimulation simulation;
	sf::View camera;

	/*переменные для настроек*/
	bool isSettings;
	char** langs;
	size_t langs_length;
	char** vmodes;
	size_t vmodes_length;

	void initVariables();
	void initKeybinds();

	//func
	void updateInput(const float& dt) override;
	void updateGUI();
	void updateSettingsGUI();
	void update(const float& dt) override;
};