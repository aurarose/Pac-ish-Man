#pragma once
#include "Actor.h"
class Energizer :
	public Actor
{
public:
	Energizer(string imgpath, float x, float y, float w, float h) :Actor(imgpath, x, y, w, h){
		this->image_speed = 15;
		this->actor_type = "energizer";
		x += 4;
		y -= 4;
		this->grid_x = x / 8;
		this->grid_y = y / 8;
		//		cout << "xgrid: " << static_cast<int>(xpos / 8) << endl;
		this->sprite.setOrigin(w / 2, h / 2);
		this->sprite.setPosition(x, y);
	}
	virtual void update();
private:
	int grid_x;
	int grid_y;

	void makeGhostsVulnerable();
};

