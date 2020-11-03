#pragma once

class SpaceObj
{
public:
	enum class obj_t /*типы объектов*/
	{
		star,
		planet,
		satellite,
		dwarf_planet,
		asteroid,
		count = 5
	};

	SpaceObj(
		double mass, double x, double y,
		double vx, double vy,
		double ax, double ay,
		double radius, sf::Color obj_color, obj_t type);
	
private:
	double mass;			 /*масса*/
	double x, y;			 /*положение тела*/
	double vx, vy;			 /*скорость по иксу, игреку*/
	double ax, ay;			 /*ускорение по иксу, игреку*/
	double radius;		 	 /*радиус в пикселях*/
	sf::Color obj_color;	 /*цвет тела*/
	std::string name;		 /*имя тела*/
	obj_t type;				 /*тип тела*/
	sf::CircleShape shape;	 /*окружность для отрисовки*/
};