#pragma once

#include "tinyxml2.h"
#include "spaceObj.hpp"

class PhysSimulation
{
public:
	PhysSimulation() = default;

	/*удаленные конструкторы и операторы*/
	PhysSimulation(const PhysSimulation&) = delete;
	PhysSimulation(PhysSimulation&&) = delete;

	PhysSimulation& operator=(const PhysSimulation&) = delete;
	PhysSimulation& operator=(PhysSimulation&&) = delete;

	/*функции*/
	void loadSystemXml(const std::string& path);
	void saveSystemXml(const std::string& path) const;

	sf::Vector2f getMaxMassCoord() const;

	void restoreInitialState(); /*восстановление системы в том состоянии, какое было на момент загрузки*/

	void update(const float& dt);
	void render(sf::RenderTarget* target);

private:
	std::vector<SpaceObj*> planetsSim;    /*тела, участвующие в симуляции*/
	std::vector<SpaceObj*> planetsSave;   /*тела, которые хранятся для восстановления исходного состояния*/
	std::string systemName;               /*имя симулируемой системы*/
	std::string systemDescription;        /*описание симулируемой системы*/

	void clear();  /*очистка текущей системы*/

	double fx(const SpaceObj* obj, double locale_x) const; /*расчет ускорения по x*/
	double fy(const SpaceObj* obj, double locale_y) const; /*расчет ускорения по y*/

	void calcVxAndX(SpaceObj* obj, const float& dt); /*расчет скорости и координаты по x*/
	void calcVyAndY(SpaceObj* obj, const float& dt); /*расчет скорости и координаты по y*/
};