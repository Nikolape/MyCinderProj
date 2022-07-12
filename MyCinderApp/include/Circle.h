#pragma once
#include "Shape.h"

class Circle : public Shape
{
private:
	float size;

public:
	Circle(vec2 position);
	Circle(vec2 position, vec3 color, vec2 force, bool move);
	void drow() override;
	void movment() override;
	bool hit(vec2 clickPosition) override;
	std::string myShape() override;
	int ID() override;

};
