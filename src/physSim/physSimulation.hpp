#pragma once

#include "tinyxml2.h"
#include "spaceObj.hpp"

class PhysSimulation
{
public:
	PhysSimulation();
	~PhysSimulation();

	/*удаленные конструкторы и операторы*/
	PhysSimulation(const PhysSimulation&) = delete;
	PhysSimulation(PhysSimulation&&) = delete;

	PhysSimulation& operator=(const PhysSimulation&) = delete;
	PhysSimulation& operator=(PhysSimulation&&) = delete;

	/*функции*/
	void loadSystemXml(const std::string& path);
	void loadDemoSystem();
	void createSystem(const std::string& name, const std::string& desc);
	void saveSystemXml(const std::string& path) const;
	void saveSystemXml() const;

	void deleteObj(size_t i);   /*удалить i-тый объект*/
	void restoreInitialState(); /*восстановление системы в том состоянии, какое было на момент загрузки*/
	void replaceSimtoSave();    /*объекты из симуляции становятся в вектор для сохранения*/
	void addObj(SpaceObj obj);  /*добавить новый объект в симуляцию*/

	/*get*/
	sf::Vector2f getMaxMassCoord() const;
	const std::string& getName() const;
	const std::string& getDesc() const;
	const size_t getCountObj() const;
	const std::vector<SpaceObj*>& getObjects() const;

	/*set*/
	void setTrajectory(bool set);
	void setName(const char* name);
	void setDescription(const char* desc);
	std::vector<SpaceObj*> setObjects() const;


	void update(const float& dt);
	void render(sf::RenderTarget* target);

	/*v в 0 для максимального тела*/
	void vToNullForMaxObj();

private:
	std::vector<SpaceObj*> planetsSim;					/*тела, участвующие в симуляции*/
	std::vector<SpaceObj*> planetsSave;					/*тела, которые хранятся для восстановления исходного состояния*/
	std::string systemName;								/*имя симулируемой системы*/
	std::string systemDescription;						/*описание симулируемой системы*/
	std::vector<std::list<sf::Vector2f>> trajectory;	/*траектория движения объектов*/
	bool isTrajectory;									/*включено ли отображение траектории*/
	size_t max_count_point;								/*максимальное колчиство точек в траектории*/

	void clear();  /*очистка текущей системы*/

	double fx(const SpaceObj* obj, double locale_x) const; /*расчет ускорения по x*/
	double fy(const SpaceObj* obj, double locale_y) const; /*расчет ускорения по y*/

	void calcVxAndX(SpaceObj* obj, const float& dt); /*расчет скорости и координаты по x*/
	void calcVyAndY(SpaceObj* obj, const float& dt); /*расчет скорости и координаты по y*/
};