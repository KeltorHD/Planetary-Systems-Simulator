#pragma once

#include "states/state.hpp"
#include "physSim/physSimulation.hpp"

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
	PhysSimulation simulation;
	sf::View camera;

	void initVariables();
	void initKeybinds();

	//func
	void updateInput(const float& dt);
	void updateGUI();
	void update(const float& dt);
};