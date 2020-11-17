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

	float keyTime;
	float keyTimeMax;

	PhysSimulation simulation;
	sf::View camera;
	control_t ctrl;               /*управление симуляцией: пауза, симуляция, симуляция с ускорением/замедлением*/
	float koef;                   /*коэффициент ускорения/замедления*/

	/*для GUI*/
	bool enableControlSimulation; /*манипулирование симуляцией*/
	bool enableEditSimulation;    /*редактирование объектов в симуляции*/
	bool isAlwaysCenter;          /*всегда ли камера на самом массивном объекте*/
	char* input_name;             /*имя системы*/
	char* input_desc;             /*описание системы*/
	char* new_system_name;        /*имя новой системы*/
	char* new_system_desc;        /*описание новой системы*/
	bool isAdding;                /*режим ли добавления тела?*/
	bool enableAddMenu;           /*включено ли меню добавления объекта*/
	SpaceObj* add_obj;            /*добавляемый объект*/
	SpaceObj* open_edit_menu;     /*если не nullptr, открываем соответствующее меню изменения*/
	char* edit_name_obj;          /*изменение имени объекта*/
	char** type_names;            /*имена всех типов объектов*/
	char** systems;               /*доступные системы для загрузки*/
	size_t systems_length;        /*количество доступных систем*/

	void initVariables();
	void initKeybinds();

	const bool getKeyTime();

	//func
	void updateInput(const float& dt);
	void updateGUI();
	void updateMainMenuBar();
	void updateControlSim();
	void updateEditSim();
	void updateAddObj();

	void updateKeyTime(const float& dt);
	void update(const float& dt);
};