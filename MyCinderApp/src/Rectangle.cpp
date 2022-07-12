#include "Rectangle.h"


Rectangle::Rectangle(vec2 position) : Shape(position)
{
	sizeX = randFloat() * 50.0f + 15.0f;
	sizeY = randFloat() * 50.0f + 15.0f;
}

Rectangle::Rectangle(vec2 position, vec3 color, vec2 force, bool move) :
	Shape(position, color, force, move)
{
	sizeX = randFloat() * 50.0f + 15.0f;
	sizeY = randFloat() * 50.0f + 15.0f;
}

void Rectangle::drow()
{
	gl::color(color.x, color.y, color.z);
	Rectf nik(position.x, position.y, position.x+sizeX, position.y + sizeY);
	gl::drawSolidRect(nik);
}

void Rectangle::movment()
{
	if (move)
	{
		position.x += force.x;
		if (position.x + sizeX > app::getWindowWidth()) {
			force.x *= -1;
			position.x = app::getWindowWidth() - sizeX;
		}
		else if (position.x < 0) {
			force.x *= -1;
			position.x = 0;
		}

		position.y += force.y;
		if (position.y + sizeY > app::getWindowHeight()) {
			force.y *= -1;
			position.y = app::getWindowHeight() - sizeY;
		}
		else if (position.y < 0) {
			force.y *= -1;
			position.y = 0;
		}

	}
}

bool Rectangle::hit(vec2 clickPosition)
{
	if (clickPosition.x < position.x || clickPosition.x > position.x + sizeX) 
		return false;
	else if (clickPosition.y < position.y || clickPosition.y > position.y + sizeY) 
		return false;
	else
		return true;
}

std::string Rectangle::myShape()
{
	return "Rectangle";
}

int Rectangle::ID() {
	return 3;
}