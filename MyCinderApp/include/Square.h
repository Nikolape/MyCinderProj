#pragma once
#include "Shape.h"



class Square : public Shape
{
private:
	float size;
	
public:
	Square(vec2 position);
	Square(vec2 position, vec3 color, vec2 force, bool move);
	void drow() override;
	void movment() override;
	bool hit(vec2 clickPosition) override;
	std::string myShape() override;
	int ID() override;
};

