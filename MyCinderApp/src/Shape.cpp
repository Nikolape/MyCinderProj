#include "Shape.h"

Shape::Shape(vec2 position) : position(position)
{
	this->color = randVec3();
	if (color.x < 0) color.x *= -1;
	if (color.y < 0) color.y *= -1;
	if (color.z < 0) color.z *= -1;
	this->force = randVec2() * 2.0f;
	this->move = randBool();
}

Shape::Shape(vec2 position, vec3 color, vec2 force, bool move) :
	position(position), color(color), force(force), move(move)
{
}

vec2 Shape::getPosition() { return position; }
vec3 Shape::getColor() { return color; }
vec2 Shape::getForce() { return force; }
bool Shape::getMove() { return move; }

void Shape::setPosition(vec2 setPosition) { position = setPosition; }
void Shape::setColor(vec3 setColor) { color = setColor; }
void Shape::setForce(vec2 setForce) { force = setForce; }
void Shape::setMove(bool setMove) { move = setMove; }

void Shape::moveUp() { position.y -= 10; }
void Shape::moveDown() { position.y += 10; }
void Shape::moveLeft() { position.x -= 10; }
void Shape::moveRight() { position.x += 10; }


