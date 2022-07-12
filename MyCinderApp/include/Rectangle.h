#pragma once
#include "Shape.h"

class Rectangle : public Shape
{
private:
	float sizeX, sizeY;
public:
	Rectangle(vec2 position);
	Rectangle(vec2 position, vec3 color, vec2 force, bool move);
	void drow() override;
	void movment() override;
	bool hit(vec2 clickPosition) override;
	std::string myShape() override;
	int ID() override;
};

