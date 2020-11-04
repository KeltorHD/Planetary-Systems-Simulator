#include "pch.h"
#include "physSimulation.hpp"

void PhysSimulation::loadSystemXml(const std::string& path)
{
	using namespace tinyxml2;
	XMLDocument sys;
	if (sys.LoadFile(path.c_str()))
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
}
