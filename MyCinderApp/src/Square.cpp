#include "Square.h"

Square::Square(vec2 position) : Shape(position)
{
	size = randFloat() * 50.0f + 15.0f;
}

Square::Square(vec2 position, vec3 color, vec2 force, bool move) :
	Shape(position,color,force,move)
{
	size = randFloat() * 50.0f + 15.0f;
}

void Square::drow()
{
	gl::color(color.x, color.y, color.z);
	Rectf nik(position.x, position.y, position.x + size, position.y + size);
	gl::drawSolidRect(nik);
}

void Square::movment()
{
	if (move)
	{
		position.x += force.x;
		if (position.x + size > app::getWindowWidth()) {
			force.x *= -1;
			position.x = app::getWindowWidth() - size;
		}
		else if (position.x < 0) {
			force.x *= -1;
			position.x = 0;
		}

		position.y += force.y;
		if (position.y + size > app::getWindowHeight()) {
			force.y *= -1;
			position.y = app::getWindowHeight() - size;
		}
		else if (position.y < 0) {
			force.y *= -1;
			position.y = 0;
		}

	}
}

bool Square::hit(vec2 clickPosition)
{
	if (clickPosition.x < position.x || clickPosition.x > position.x + size)
		return false;
	else if (clickPosition.y < position.y || clickPosition.y > position.y + size)
		return false;
	else
		return true;
}

std::string Square::myShape()
{
	return "Square";
}

int Square::ID() {
	return 2;
}




