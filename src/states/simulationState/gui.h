#pragma once

#include "physSim/physSimulation.hpp"
#include "localisation/locale.hpp"

class Gui
{
public:
	enum class control_t
	{
		paused,
		play,
		play_koef
	};

	Gui(PhysSimulation* phys, const Locale* locale, sf::View& camera, bool& quit);
	~Gui();

	void updateInput(sf::RenderWindow* window, std::queue<sf::Event>& events);

	void update();

	void render(sf::RenderTarget* target);

	/*get*/
	const control_t& getControl() const;
	const float& getKoef() const;
	const bool& getAlwaysCenter() const;

	/*set*/
	void setControl(control_t ctrl);
	void setKoef(float koef);
	void setAlwaysCenter(bool cond);

private:
	/*связь с simulation*/
	PhysSimulation* simulation;
	const Locale* locale;
	sf::View& camera;
	control_t ctrl;               /*управление симуляцией: пауза, симуляция, симуляция с ускорением/замедлением*/
	float koef;                   /*коэффициент ускорения/замедления*/
	bool& quit;                   /*для закрытия simulation state*/

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

	/*функции*/
	void updateMainMenuBar();     /*обновление меню вверху экрана*/
	void updateControlSim();      /*отрисовка меню управления симуляцией*/
	void updateEditSim();         /*отрисовка меню редактирования симуляцией*/
	void updateAddObj();          /*отрисовка меню добавления объекта*/
};