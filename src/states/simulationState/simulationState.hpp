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
	enum class control_t
	{
		paused,
		play,
		play_koef
	};
	PhysSimulation simulation;
	sf::View camera;
	control_t ctrl;              /*управление симул€цией: пауза, симул€ци€, симул€ци€ с ускорением/замедлением*/
	float koef;                  /*коэффициент ускорени€/замедлени€*/

	/*дл€ GUI*/
	bool enableControlSimulation; /*манипулирование симул€цией*/
	bool enableEditSimulation;    /*редактирование объектов в симул€ции*/
	char* input_name;             /*им€ системы*/

	void initVariables();
	void initKeybinds();

	//func
	void updateInput(const float& dt);
	void updateGUI();
	void updateMainMenuBar();
	void updateControlSim();
	void updateEditSim();
	void update(const float& dt);
};