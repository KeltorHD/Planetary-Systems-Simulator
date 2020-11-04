#pragma once

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
	//void loadSpaceObj()

	void update(const float& dt);
	void render(sf::RenderTarget* target);

private:
	std::vector<SpaceObj*> planetsSim;    /*тела, участвующие в симуляции*/
	std::vector<SpaceObj*> planetsSave;   /*тела, которые хранятся для восстановления исходного состояния*/

	void clear();

};