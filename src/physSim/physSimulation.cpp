#include "pch.h"
#include "physSimulation.hpp"

static const char* demo_system{ "<?xml version=\"1.0\" encoding=\"UTF-8\"?><system><system_name>Base</system_name><description>Sun, Earth and Moon</description><object><name>Sun</name><type>0</type><mass>10000</mass><x>500</x><y>370</y><vx>0</vx><vy>0</vy><r>20</r><color><r>255</r><g>255</g><b>0</b></color></object><object><name>Earth</name><type>1</type><mass>200</mass><x>180</x><y>370</y><vx>0</vx><vy>-5</vy><r>7</r><color><r>0</r><g>0</g><b>255</b></color></object><object><name>Moon</name><type>2</type><mass>15</mass><x>160</x><y>370</y><vx>0</vx><vy>-8</vy><r>4</r><color><r>72</r><g>72</g><b>72</b></color></object></system>" };

PhysSimulation::PhysSimulation()
	: isTrajectory(false)
{
}

PhysSimulation::~PhysSimulation()
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

void PhysSimulation::loadSystemXml(const std::string& path)
{
	using namespace tinyxml2;
	XMLDocument sys;
	this->clear();
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
		this->systemName = { sys.FirstChildElement("system")->FirstChildElement("system_name")->GetText() };
		this->systemDescription = { sys.FirstChildElement("system")->FirstChildElement("description")->GetText() ? sys.FirstChildElement("system")->FirstChildElement("description")->GetText() : "NONE" };
		this->trajectory.resize(this->planetsSave.size());
	}
	else
	{
		throw "NOT LOAD FILE " + path;
	}
}

void PhysSimulation::loadDemoSystem()
{
	using namespace tinyxml2;
	XMLDocument sys;
	if (sys.Parse(demo_system) == XMLError::XML_SUCCESS)
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
		this->systemName = { sys.FirstChildElement("system")->FirstChildElement("system_name")->GetText() };
		this->systemDescription = { sys.FirstChildElement("system")->FirstChildElement("description")->GetText() };
		this->trajectory.resize(this->planetsSave.size());
	}
}

void PhysSimulation::createSystem(const std::string& name, const std::string& desc)
{
	this->clear();
	this->systemName = name;
	this->systemDescription = desc;
	this->trajectory.resize(0);
}

void PhysSimulation::saveSystemXml(const std::string& path) const
{
	using namespace tinyxml2;
	XMLDocument sys;
	
	sys.Clear();
	auto decl = sys.NewDeclaration();
	sys.InsertEndChild(decl);

	auto system = sys.NewElement("system");
	auto system_name = sys.NewElement("system_name");
	system_name->SetText(this->systemName.c_str());
	system->InsertEndChild(system_name);
	auto description = sys.NewElement("description");
	description->SetText(this->systemDescription.c_str());
	system->InsertEndChild(description);

	for (size_t i = 0; i < this->planetsSave.size(); i++)
	{
		auto obj = sys.NewElement("object");

		auto name = sys.NewElement("name");
		name->SetText(this->planetsSave[i]->name.c_str());
		obj->InsertEndChild(name);

		auto type = sys.NewElement("type");
		type->SetText(static_cast<int>(this->planetsSave[i]->type));
		obj->InsertEndChild(type);

		auto mass = sys.NewElement("mass");
		mass->SetText(this->planetsSave[i]->mass);
		obj->InsertEndChild(mass);

		auto x = sys.NewElement("x");
		x->SetText(this->planetsSave[i]->x);
		obj->InsertEndChild(x);

		auto y = sys.NewElement("y");
		y->SetText(this->planetsSave[i]->y);
		obj->InsertEndChild(y);

		auto vx = sys.NewElement("vx");
		vx->SetText(this->planetsSave[i]->vx);
		obj->InsertEndChild(vx);

		auto vy = sys.NewElement("vy");
		vy->SetText(this->planetsSave[i]->vy);
		obj->InsertEndChild(vy);

		auto radius = sys.NewElement("r");
		radius->SetText(this->planetsSave[i]->radius);
		obj->InsertEndChild(radius);

		auto color = sys.NewElement("color");

		auto r = sys.NewElement("r");
		r->SetText(this->planetsSave[i]->obj_color.r);
		color->InsertEndChild(r);

		auto g = sys.NewElement("g");
		g->SetText(this->planetsSave[i]->obj_color.g);
		color->InsertEndChild(g);

		auto b = sys.NewElement("b");
		b->SetText(this->planetsSave[i]->obj_color.b);
		color->InsertEndChild(b);

		obj->InsertEndChild(color);

		system->InsertEndChild(obj);
	}

	sys.InsertEndChild(system);

	if (sys.SaveFile(path.c_str()) != XML_SUCCESS)
		throw "NOT SAVE FILE " + path;
}

void PhysSimulation::saveSystemXml() const
{
	this->saveSystemXml("systems/" + this->systemName + ".xml");
}

sf::Vector2f PhysSimulation::getMaxMassCoord() const
{
	size_t i = std::distance(this->planetsSim.begin(), std::max_element(this->planetsSim.begin(), this->planetsSim.end(), [](const SpaceObj* o1, const SpaceObj* o2) {return o1->mass <= o2->mass; }));
	if (i != this->planetsSim.size())
	{
		return sf::Vector2f
		(
			static_cast<float>(this->planetsSim[i]->x),
			static_cast<float>(this->planetsSim[i]->y)
		);
	}
	return sf::Vector2f();
}

const std::string& PhysSimulation::getName() const
{
	return this->systemName;
}

const std::string& PhysSimulation::getDesc() const
{
	return this->systemDescription;
}

const size_t PhysSimulation::getCountObj() const
{
	return this->planetsSim.size();
}

const std::vector<SpaceObj*>& PhysSimulation::getObjects() const
{
	return this->planetsSim;
}

void PhysSimulation::setTrajectory(bool set)
{
	if (set)
	{
		this->isTrajectory = true;
	}
	else
	{
		this->isTrajectory = false;
		for (auto& i : this->trajectory) /*очищаем траектории*/
		{
			i.clear();
		}
	}
}

void PhysSimulation::setName(const char* name)
{
	this->systemName = name;
}

void PhysSimulation::setDescription(const char* desc)
{
	this->systemDescription = desc;
}

std::vector<SpaceObj*> PhysSimulation::setObjects() const
{
	return this->planetsSim;
}

void PhysSimulation::deleteObj(size_t i)
{
	if (i < this->planetsSim.size())
	{
		this->planetsSim.erase(this->planetsSim.begin() + i);
	}
}

void PhysSimulation::restoreInitialState()
{
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		delete this->planetsSim[i];
	}
	this->planetsSim.clear();
	for (size_t i = 0; i < this->planetsSave.size(); i++)
	{
		this->planetsSim.push_back(new SpaceObj(*this->planetsSave[i]));
	}
	for (auto& i : this->trajectory) /*очищаем траектории*/
	{
		i.clear();
	}
}

void PhysSimulation::replaceSimtoSave()
{
	for (size_t i = 0; i < this->planetsSave.size(); i++)
	{
		delete this->planetsSave[i];
	}
	this->planetsSave.clear();
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		this->planetsSave.push_back(new SpaceObj(*this->planetsSim[i]));
	}
}

void PhysSimulation::addObj(SpaceObj obj)
{
	this->planetsSim.push_back(new SpaceObj(obj));
	this->planetsSave.push_back(new SpaceObj(obj));
	this->trajectory.resize(this->trajectory.size() + 1u);
}

void PhysSimulation::update(const float& dt)
{
	for (size_t i = 0; i < 5/*magic*/; i++)
	{
		for (size_t j = 0; j < this->planetsSim.size(); j++) /*вычисление x, y, vx, vy для каждой планеты*/
		{
			this->calcVxAndX(this->planetsSim[j], dt);
			this->calcVyAndY(this->planetsSim[j], dt);
			for (size_t k = 0; k < this->planetsSim.size(); k++) /*проверка на столкновение*/
			{
				if (k != j)
				{
					if (std::hypot(this->planetsSim[k]->x - this->planetsSim[j]->x, this->planetsSim[k]->y - this->planetsSim[j]->y) < this->planetsSim[j]->radius + this->planetsSim[k]->radius)
					{
						if (this->planetsSim[k]->mass != this->planetsSim[j]->mass)
						{
							auto ptr_max = this->planetsSim[k]->mass < this->planetsSim[j]->mass ? this->planetsSim[j] : this->planetsSim[k];
							auto ptr_min = this->planetsSim[k]->mass < this->planetsSim[j]->mass ? this->planetsSim[k] : this->planetsSim[j];
							size_t max_mass = this->planetsSim[k]->mass < this->planetsSim[j]->mass ? j : k;
							size_t min_mass = this->planetsSim[k]->mass < this->planetsSim[j]->mass ? k : j;

							ptr_max->mass = ptr_max->mass + ptr_min->mass;
							ptr_max->radius = std::sqrt(ptr_max->radius * ptr_max->radius + ptr_min->radius * ptr_min->radius);
							delete this->planetsSim[min_mass];
							this->planetsSim.erase(this->planetsSim.begin() + min_mass);
						}
						else
						{
							this->planetsSim[k]->mass = this->planetsSim[k]->mass + this->planetsSim[j]->mass;
							this->planetsSim[k]->radius = std::sqrt(this->planetsSim[k]->radius * this->planetsSim[k]->radius + this->planetsSim[j]->radius * this->planetsSim[j]->radius);
							delete this->planetsSim[j];
							this->planetsSim.erase(this->planetsSim.begin() + j);
						}
					}
				}
			}
		}
	}
	for (size_t i = 0; i < this->planetsSim.size(); i++)
	{
		this->planetsSim[i]->update();

		if (this->isTrajectory)
		{
			try
			{
				this->trajectory[i].push_front(sf::Vector2f
				(
					static_cast<float>(this->planetsSim[i]->x),
					static_cast<float>(this->planetsSim[i]->y)
				));
			}
			catch (const std::exception&)
			{
				this->trajectory[i].clear();
			}
		}
	}
}

void PhysSimulation::render(sf::RenderTarget* target)
{
	for (size_t i = 0; i < this->trajectory.size(); i++)
	{
		if (this->isTrajectory)
		{
			sf::VertexArray tr;
			for (const auto& i : this->trajectory[i])
			{
				tr.append(i);
			}
			target->draw(tr);
		}
		if (i < this->planetsSim.size())
		{
			this->planetsSim[i]->render(target);
		}
	}
}

void PhysSimulation::vToNullForMaxObj()
{
	size_t i = std::distance(this->planetsSim.begin(), std::max_element(this->planetsSim.begin(), this->planetsSim.end(), [](const SpaceObj* o1, const SpaceObj* o2) {return o1->mass <= o2->mass; }));
	if (i != this->planetsSim.size())
	{
		this->planetsSim[i]->vx = 0;
		this->planetsSim[i]->vy = 0;
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
	this->planetsSave.clear();
	this->planetsSim.clear();
	this->systemName = "DEFAULT";
	this->systemDescription = "";
	for (auto& i : this->trajectory)
	{
		i.clear();
	}
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
