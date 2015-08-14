#pragma once
#include "Actor.h"
#include "Globals.h"
class GridActor :
	public Actor
{
public:
	GridActor(string imgpath, float x, float y, float w, float h) : Actor(imgpath, x, y, w, h){
		this->grid_x = x / g_tile_size;
		this->grid_y = y / g_tile_size;
	};
	void gridMove(int x,int y);
	virtual void update(){};
	bool isMoving = false;

protected:
	int grid_x;
	int grid_y;
	float speed=2;
	std::pair<int, int> next_move;

	virtual void gridUpdate();
private:
	void moveToTarget();
};

