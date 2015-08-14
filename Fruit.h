#pragma once
#include "Actor.h"
class Fruit :
	public Actor
{
public:
	Fruit(string imgpath, float x, float y, float w, float h);
	void update();
private:
	int grid_x;
	int grid_y;
	sf::Clock timer;
};

