#pragma once
#include "Actor.h"



class Ghost : public Actor{
	
public:
	enum name{ inky, blinky, pinky, clyde };
	enum state{chase,scatter,frightened,dead,resetting};
	
	
	Ghost(string imgpath, float x, float y, float w, float h, name n);
	virtual void update();
	name n;
	state state;

	void getScared();
	void getNormal();
	void die();


	// Reset position after death state
	void reset();
protected:
	int direction;
	float speed = 0.7;
	sf::Vector2<int> target_tile;
	sf::Vector2<int> cur_tile;
	sf::Clock state_timer;

	void processMovement();
	void doPlayerCollision();
	void pickTargetTile();
	void chooseDir();

	bool isAtChoicePoint(int, int, float);
	bool isInGhostHouse();
	void snapToGrid();

	void updateState();
	void updateImage();

};

