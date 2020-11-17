#pragma once

#include "states/state.hpp"
#include "physSim/physSimulation.hpp"
#include "gui.h"

static const int camera_move = 80;
static const float camera_scroll = 0.004f;

class SimulationState :
	public State
{
public:
	SimulationState(StateData* state_data);
	virtual ~SimulationState();

	void render(sf::RenderTarget* target = nullptr);

private:
	float keyTime;
	float keyTimeMax;

	PhysSimulation simulation;
	Gui gui;
	sf::View camera;

	void initVariables();
	void initKeybinds();

	const bool getKeyTime();

	//func
	void updateInput(const float& dt);
	void updateGUI();

	void updateKeyTime(const float& dt);
	void update(const float& dt);
};