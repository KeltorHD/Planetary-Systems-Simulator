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
	control_t ctrl;              /*управление симуляцией: пауза, симуляция, симуляция с ускорением/замедлением*/
	float koef;                  /*коэффициент ускорения/замедления*/

	/*для GUI*/
	bool enableControlSimulation; /*манипулирование симуляцией*/
	bool enableEditSimulation;    /*редактирование объектов в симуляции*/
	char* input_name;             /*имя системы*/
	char* input_desc;             /*описание системы*/
	bool isAdding;                /*режим ли добавления тела?*/
	SpaceObj* add_obj;            /*добавляемый объект*/
	char* edit_name_obj;          /*изменение имени объекта*/
	char** type_names;            /*имена всех типов объектов*/

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