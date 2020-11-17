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
	/*����� � simulation*/
	PhysSimulation* simulation;
	const Locale* locale;
	sf::View& camera;
	control_t ctrl;               /*���������� ����������: �����, ���������, ��������� � ����������/�����������*/
	float koef;                   /*����������� ���������/����������*/
	bool& quit;                   /*��� �������� simulation state*/

	/*��� GUI*/
	bool enableControlSimulation; /*��������������� ����������*/
	bool enableEditSimulation;    /*�������������� �������� � ���������*/
	bool isAlwaysCenter;          /*������ �� ������ �� ����� ��������� �������*/
	char* input_name;             /*��� �������*/
	char* input_desc;             /*�������� �������*/
	char* new_system_name;        /*��� ����� �������*/
	char* new_system_desc;        /*�������� ����� �������*/
	bool isAdding;                /*����� �� ���������� ����?*/
	bool enableAddMenu;           /*�������� �� ���� ���������� �������*/
	SpaceObj* add_obj;            /*����������� ������*/
	SpaceObj* open_edit_menu;     /*���� �� nullptr, ��������� ��������������� ���� ���������*/
	char* edit_name_obj;          /*��������� ����� �������*/
	char** type_names;            /*����� ���� ����� ��������*/
	char** systems;               /*��������� ������� ��� ��������*/
	size_t systems_length;        /*���������� ��������� ������*/

	/*�������*/
	void updateMainMenuBar();     /*���������� ���� ������ ������*/
	void updateControlSim();      /*��������� ���� ���������� ����������*/
	void updateEditSim();         /*��������� ���� �������������� ����������*/
	void updateAddObj();          /*��������� ���� ���������� �������*/
};