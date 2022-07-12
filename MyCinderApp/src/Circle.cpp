#include "Circle.h"


Circle::Circle(vec2 position) : Shape(position)
{
	size = randFloat() * 50.0f + 5.0f;
}

Circle::Circle(vec2 position, vec3 color, vec2 force, bool move) :
	Shape(position, color, force, move)
{
	size = randFloat() * 50.0f + 5.0f;
}

void Circle::drow()
{
	gl::color(color.x, color.y, color.z);
	gl::drawSolidCircle(position, size);
}

void Circle::movment()
{
	if (move)
	{
		position.x += force.x;
		if (position.x + size > app::getWindowWidth()) {
			force.x *= -1;
			position.x = app::getWindowWidth() - size;
		}
		else if (position.x - size < 0) {
			force.x *= -1;
			position.x = 0 + size;
		}

		position.y += force.y;
		if (position.y + size > app::getWindowHeight()) {
			force.y *= -1;
			position.y = app::getWindowHeight() - size;
		}
		else if (position.y - size < 0) {
			force.y *= -1;
			position.y = 0 + size;
		}

	}
}

bool Circle::hit(vec2 clickPosition)
{
	return (size > sqrt(pow(position.x - clickPosition.x, 2) + pow(position.y - clickPosition.y, 2)));
}

std::string Circle::myShape()
{
	return "Circle";
}

int Circle::ID() {
	return 1;
}
