#pragma once

class Hitbox
{
public:
	Hitbox(float x, float y, float width, float height);
	float height;
	float width;
	float x_offset;
	float y_offset;
	float xpos;
	float ypos;

	bool collides(Hitbox);
	void updatePosition(float,float);
private:
	
};

