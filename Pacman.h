#pragma once
#include "Actor.h"
#include "Grid.h"
class Pacman :
	public Actor
{
	friend int sign(float num);
public:
	enum state{ alive,hit,collapsing,dead };
	Pacman(string imgpath, float x, float y, float w, float h);
	virtual void update();
	void die();
	void reset();

	float direction;
private:
	sf::Clock death_timer;
	state state; 

	
	float next_direction;
	float speed = 1.5;

	void processInput();
	void processMovement();
	void processDeath();


};

