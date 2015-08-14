#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "hitbox.h"

using namespace std;

class Hitbox;

class Actor
{
	friend std::pair<int, int> coordsFromIndex(int& index, int width, int height, int max_x, int max_y);
public:
	Actor(string,float,float,float,float);

	float xpos;
	float ypos;
	float width;
	float height;
	int id;
	string actor_type;
	sf::Sprite sprite;
	vector<Hitbox> colliders;
	bool active;

	void draw();
	void setSprite(string);

	// moves x units right and y units down
	void move(float, float);
	// moves to point (x,y)
	void moveTo(float, float);

	void setHitbox(float, float, float, float);
	void addHitbox(float,float,float,float);
	virtual void update(){}
	float distanceToActor(Actor);
	bool collidesWithActor(Actor);

	// Animation/image manipulation

protected:
	int tex_max_x;
	int tex_max_y;
	int image_speed = 0;
	int image_timer = 0;
	int image_index = 0;
	int image_index_max = 0;
	
};

