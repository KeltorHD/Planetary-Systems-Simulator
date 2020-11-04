#pragma once

class PhysSimulation;

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
		double vx, double vy, const std::string& name,
		double radius, sf::Color obj_color, obj_t type);

	/*дистанция до другого объекта*/
	double distance(const SpaceObj& obj) const;

	/*дружественный класс для физической симуляции*/
	friend class PhysSimulation;

	void render(sf::RenderTarget* target);
	
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