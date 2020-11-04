#include "pch.h"
#include "spaceObj.hpp"

SpaceObj::SpaceObj(
	double mass, double x, 
	double y, double vx, 
	double vy, double ax, 
	double ay, double radius, sf::Color obj_color, obj_t type)
	: mass(mass), x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay), radius(radius), obj_color(obj_color), type(type)
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