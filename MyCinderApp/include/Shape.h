#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include "cinder/Rand.h"
#include "cinder/CinderMath.h"
#include "cinder/CinderImGui.h"

#include <fstream>
#include <memory>
#include <ctime>

using namespace ci;
using namespace ci::app;

class Shape
{
public:
	Shape(vec2 position);
	Shape(vec2 position, vec3 color, vec2 force, bool move);
	~Shape() = default;

	vec2 getPosition();
	vec3 getColor();
	vec2 getForce();
	bool getMove();

	void setPosition(vec2 setPosition);
	void setColor(vec3 setColor);
	void setForce(vec2 setForce);
	void setMove(bool setMove);

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	virtual void drow() = 0;
	virtual void movment() = 0;
	virtual bool hit(vec2 clickPosition) = 0;
	virtual std::string myShape() = 0;
	virtual int ID() = 0;

protected:
	vec2 position;
	vec3 color;
	vec2 force;
	bool move = false;
};

