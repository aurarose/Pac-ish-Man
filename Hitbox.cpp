#include "hitbox.h"
#include <iostream>

using namespace std;

Hitbox::Hitbox(float x, float y, float width, float height){
	this->x_offset = x;
	this->y_offset = y;
	this->width = width;
	this->height = height;
};

void Hitbox::updatePosition(float x, float y){
	this->xpos = x;
	this->ypos = y;
}

bool Hitbox::collides(Hitbox other){
	float x1 = this->x_offset+this->xpos;
	float x2 = other.x_offset + other.xpos;
	float y1 = this->y_offset + this->ypos;
	float y2 = other.y_offset + other.ypos;
	float w1 = this->width;
	float w2 = other.width;
	float h1 = this->height;
	float h2 = other.height;

	bool xi = x1 <= x2 + w2 && x1 + w1 >= x2;
	bool yi = y1 <= y2 + h2 && y1 + h1 >= y2;

	return xi&&yi;
}

