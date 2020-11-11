#include "pch.h"
#include "spaceObj.hpp"

SpaceObj::SpaceObj(
	double mass, double x, double y,
	double vx, double vy, const std::string& name,
	double radius, sf::Color obj_color, obj_t type)
	: mass(mass), x(x), y(y), vx(vx), vy(vy), radius(radius), obj_color(obj_color), type(type), name(name)
{
	this->shape.setRadius(static_cast<float>(this->radius));
	this->shape.setOrigin(static_cast<float>(this->radius), static_cast<float>(this->radius));
	this->shape.setFillColor(this->obj_color);
	this->shape.setPosition(static_cast<float>(this->x), static_cast<float>(this->y));
}

double SpaceObj::distance(const SpaceObj& obj) const
{
	return std::hypot(this->x - obj.x, this->y - obj.y);
}

const double& SpaceObj::getMass() const
{
	return this->mass;
}

const double& SpaceObj::getX() const
{
	return this->x;
}

const double& SpaceObj::getY() const
{
	return this->y;
}

const double& SpaceObj::getVx() const
{
	return this->vx;
}

const double& SpaceObj::getVy() const
{
	return this->vy;
}

const double& SpaceObj::getRadius() const
{
	return this->radius;
}

const sf::Color& SpaceObj::getColor() const
{
	return this->obj_color;
}

const std::string& SpaceObj::getName() const
{
	return this->name;
}

const SpaceObj::obj_t& SpaceObj::getType() const
{
	return this->type;
}

void SpaceObj::setName(const char* name)
{
	this->name = name;
}

void SpaceObj::setColor(const sf::Color& var)
{
	this->obj_color = var;
}

void SpaceObj::setType(obj_t type)
{
	this->type = type;
}

double& SpaceObj::setMass()
{
	return this->mass;
}

double& SpaceObj::setX()
{
	return this->x;
}

double& SpaceObj::setY()
{
	return this->y;
}

double& SpaceObj::setVx()
{
	return this->vx;
}

double& SpaceObj::setVy()
{
	return this->vy;
}

double& SpaceObj::setRadius()
{
	return this->radius;
}

void SpaceObj::update()
{
	this->shape.setPosition(static_cast<float>(this->x), static_cast<float>(this->y));
}

void SpaceObj::render(sf::RenderTarget* target)
{
	this->shape.setRadius(static_cast<float>(this->radius));
	this->shape.setOrigin(static_cast<float>(this->radius), static_cast<float>(this->radius));
	this->shape.setPosition(static_cast<float>(this->x), static_cast<float>(this->y));
	this->shape.setFillColor(this->obj_color);
	target->draw(this->shape);
}

std::string SpaceObj::objToString(obj_t type)
{
	switch (type)
	{
	case SpaceObj::obj_t::star:
		return "star";
		break;
	case SpaceObj::obj_t::planet:
		return "planet";
		break;
	case SpaceObj::obj_t::satellite:
		return "satellite";
		break;
	case SpaceObj::obj_t::dwarf_planet:
		return "dwarf_planet";
		break;
	case SpaceObj::obj_t::asteroid:
		return "asteroid";
		break;
	case SpaceObj::obj_t::count:
		break;
	}
	return "error";
}
