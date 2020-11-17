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
	/*св€зь с simulation*/
	PhysSimulation* simulation;
	const Locale* locale;
	sf::View& camera;
	control_t ctrl;               /*управление симул€цией: пауза, симул€ци€, симул€ци€ с ускорением/замедлением*/
	float koef;                   /*коэффициент ускорени€/замедлени€*/
	bool& quit;                   /*дл€ закрыти€ simulation state*/

	/*дл€ GUI*/
	bool enableControlSimulation; /*манипулирование симул€цией*/
	bool enableEditSimulation;    /*редактирование объектов в симул€ции*/
	bool isAlwaysCenter;          /*всегда ли камера на самом массивном объекте*/
	char* input_name;             /*им€ системы*/
	char* input_desc;             /*описание системы*/
	char* new_system_name;        /*им€ новой системы*/
	char* new_system_desc;        /*описание новой системы*/
	bool isAdding;                /*режим ли добавлени€ тела?*/
	bool enableAddMenu;           /*включено ли меню добавлени€ объекта*/
	SpaceObj* add_obj;            /*добавл€емый объект*/
	SpaceObj* open_edit_menu;     /*если не nullptr, открываем соответствующее меню изменени€*/
	char* edit_name_obj;          /*изменение имени объекта*/
	char** type_names;            /*имена всех типов объектов*/
	char** systems;               /*доступные системы дл€ загрузки*/
	size_t systems_length;        /*количество доступных систем*/

	/*функции*/
	void updateMainMenuBar();     /*обновление меню вверху экрана*/
	void updateControlSim();      /*отрисовка меню управлени€ симул€цией*/
	void updateEditSim();         /*отрисовка меню редактировани€ симул€цией*/
	void updateAddObj();          /*отрисовка меню добавлени€ объекта*/
};