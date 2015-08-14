#include "GridActor.h"
#include <iostream>

#define empty_pair std::pair<int,int>(-2,-2);

void GridActor::gridMove(int x, int y){
	this->next_move = std::pair<int, int>(x, y);
	if(!this->isMoving)
		this->isMoving = true;
}

void GridActor::gridUpdate(){
	std::cout << this->grid_x << endl;
	if (!this->isMoving&&this->next_move != std::pair<int, int>(-2, -2)){
		this->grid_x += this->next_move.first;
		this->grid_y += this->next_move.second;
		this->isMoving = true;
		this->next_move = std::pair<int, int>(-2, -2);
	}
	if (this->xpos < this->grid_x*g_tile_size||
		this->xpos > this->grid_x*g_tile_size||
		this->ypos < this->grid_y*g_tile_size||
		this->ypos > this->grid_y*g_tile_size
		){
		this->moveToTarget();
	}
	else this->isMoving = false;
}

void GridActor::moveToTarget(){
	if (this->xpos < this->grid_x*g_tile_size)
		this->move(this->speed, 0);
	else if (this->xpos > this->grid_x*g_tile_size)
		this->move(-this->speed, 0);
	if (this->ypos < this->grid_y*g_tile_size)
		this->move(0, this->speed);
	else if (this->ypos > this->grid_y*g_tile_size)
		this->move(0, -this->speed);
}