#include "pch.h"
#include "physSimulation.hpp"

void PhysSimulation::loadSystemXml(const std::string& path)
{
	using namespace tinyxml2;
	XMLDocument sys;
	if (sys.LoadFile(path.c_str()) == XMLError::XML_SUCCESS)
	{
		for (auto object = sys.FirstChildElement("system")->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement())
		{
			SpaceObj* tmp = new SpaceObj
			(
				object->FirstChildElement("mass")->DoubleText(),
				object->FirstChildElement("x")->DoubleText(),
				object->FirstChildElement("y")->DoubleText(),
				object->FirstChildElement("vx")->DoubleText(),
				object->FirstChildElement("vy")->DoubleText(),
				object->FirstChildElement("name")->GetText(),
				object->FirstChildElement("r")->DoubleText(),
				{
					static_cast<sf::Uint8>(object->FirstChildElement("color")->FirstChildElement("r")->UnsignedText()),
					static_cast<sf::Uint8>(object->FirstChildElement("color")->FirstChildElement("g")->UnsignedText()),
					static_cast<sf::Uint8>(object->FirstChildElement("color")->FirstChildElement("b")->UnsignedText()),
				},
				static_cast<SpaceObj::obj_t>(object->FirstChildElement("type")->UnsignedText())
				);
			this->planetsSim.push_back(tmp);
			this->planetsSave.push_back(new SpaceObj(*tmp));
		}
		this->systemName = { path.substr(path.find_last_of('/') + 1, path.size() - path.find_last_of('/') - 5) };
	}
	else
	{
		throw "NOT LOAD FILE " + path;
	}
}

void PhysSimulation::update(const float& dt)
{
	for (size_t i = 0; i < 5/*magic*/; i++)
	{
		for (size_t j = 0; j < this->planetsSim.size(); j++) /*вычисление x, y, vx, vy для каждой планеты*/
		{
			this->calcVxAndX(this->planetsSim[j], dt);
			this->calcVyAndY(this->planetsSim[j], dt);
		}
	}
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		this->planetsSim[i]->update();
	}
}

void PhysSimulation::render(sf::RenderTarget* target)
{
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		this->planetsSim[i]->render(target);
	}
}

void PhysSimulation::clear()
{
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		delete this->planetsSim[i];
	}
	for (size_t i = 0; i < this->planetsSave.size(); i++)
	{
		delete this->planetsSave[i];
	}
	this->systemName = "DEFAULT";
}

double PhysSimulation::fx(const SpaceObj* obj, double locale_x) const
{
	double a{};
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		if (obj == this->planetsSim[i]) continue; /*если встречаем сами себя*/

		double r{ std::hypot(this->planetsSim[i]->x - locale_x, this->planetsSim[i]->y - obj->y) };
		a += this->planetsSim[i]->mass * (this->planetsSim[i]->x - locale_x) / std::pow(r, 3.);
	}
	return a;
}

double PhysSimulation::fy(const SpaceObj* obj, double locale_y) const
{
	double a{};
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		if (obj == this->planetsSim[i]) continue; /*если встречаем сами себя*/

		double r{ std::hypot(this->planetsSim[i]->x - obj->x, this->planetsSim[i]->y - locale_y) };
		a += this->planetsSim[i]->mass * (this->planetsSim[i]->y - locale_y) / std::pow(r, 3.);
	}
	return a;
}

void PhysSimulation::calcVxAndX(SpaceObj* obj, const float& dt)
{
	double k1{ dt * this->fx(obj, obj->x) };
	double q1{ dt * obj->vx };

	double k2{ dt * this->fx(obj, obj->x + q1 / 2) };
	double q2{ dt * (obj->vx + k1 / 2) };

	double k3{ dt * this->fx(obj, obj->x + q2 / 2) };
	double q3{ dt * (obj->vx + k2 / 2) };

	double k4{ dt * this->fx(obj, obj->x + q3) };
	double q4{ dt * (obj->vx + k3) };

	obj->vx += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
	obj->x += (q1 + 2 * q2 + 2 * q3 + q4) / 6;
}

void PhysSimulation::calcVyAndY(SpaceObj * obj, const float& dt)
{
	double k1{ dt * this->fy(obj, obj->y) };
	double q1{ dt * obj->vy };

	double k2{ dt * this->fy(obj, obj->y + q1 / 2) };
	double q2{ dt * (obj->vy + k1 / 2) };

	double k3{ dt * this->fy(obj, obj->y + q2 / 2) };
	double q3{ dt * (obj->vy + k2 / 2) };

	double k4{ dt * this->fy(obj, obj->y + q3) };
	double q4{ dt * (obj->vy + k3) };

	obj->vy += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
	obj->y += (q1 + 2 * q2 + 2 * q3 + q4) / 6;
}
