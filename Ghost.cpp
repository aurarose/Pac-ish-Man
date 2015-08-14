#define _USE_MATH_DEFINES 
#include <cmath>

#include "Ghost.h"
#include "Globals.h"

Ghost::Ghost(string imgpath, float x, float y, float w, float h,name n) : Actor(imgpath, x, y, w, h){
	this->image_speed = 15;
	this->actor_type = "ghost";

	this->sprite.setOrigin(7.5, 7.5);
	this->sprite.setPosition(x, y);

	this->n = n;
	this->state = scatter;

	this->reset();
}

void Ghost::update(){
	if (game.state == game.playing){
		this->doPlayerCollision();
		this->processMovement();
		this->updateState();
	}
	else if (game.state == game.reset){
		this->reset();
	}

	if (this->state != frightened&&this->state != dead){
		this->updateImage();
	}
	else if (state == dead){
		if (this->cur_tile == this->target_tile){
			this->getNormal();
		}
	}
	
}

void Ghost::updateImage(){
	string name;
	if (n == inky) name = "inky";
	else if (n == blinky) name = "blinky";
	else if (n == pinky) name = "pinky";
	else if (n == clyde) name = "clyde";

	if (this->direction == 0)	   this->setSprite("images/"+name+"_r.png");
	else if (this->direction == 1) this->setSprite("images/"+name+"_d.png");
	else if (this->direction == 2) this->setSprite("images/"+name+"_l.png");
	else if (this->direction == 3) this->setSprite("images/"+name+"_u.png");
}

void Ghost::getScared(){
	this->snapToGrid();
	//this->chooseDir();
	int gx = this->xpos / g_tile_size;
	int gy = this->ypos / g_tile_size;

	if (this->state == dead) return;
	this->state = frightened;
	this->state_timer.restart();
	/*
	if (grid[gy + sign(sin(M_PI*0.5*this->direction))][gx + sign(cos(M_PI*0.5*this->direction))] <1&&
		grid[gy][gx] <1){
		this->direction += 2;
		if (this->direction > 3){
			this->direction -= 4;
		}
	}
	*/
	this->setSprite("images/ghost_scared.png");
	this->image_index_max = 1;
	this->speed =0.3;
}

void Ghost::getNormal(){
	this->speed = 0.7;
	this->state = scatter;
	this->state_timer.restart();
	this->image_speed = 15;
	this->snapToGrid();
}

void Ghost::die(){
	this->setSprite("images/ghost_dead.png");
	this->state = dead;
	this->state_timer.restart();
	this->speed = 0.7;
	resources.playSound("sounds/pacman_eatghost.wav");
	game.pause(1);
}

void Ghost::processMovement(){
	// Warp to other side through gates
	
	if (this->xpos > 216) this->moveTo(8, this->ypos);
	else if (this->xpos < 8) this->moveTo(216, this->ypos);

	int gx = this->xpos / g_tile_size;
	int gy = this->ypos / g_tile_size;

	if (isAtChoicePoint(gx, gy,this->direction)){
		this->snapToGrid();
		this->chooseDir();
		this->cur_tile = sf::Vector2<int>(gx,gy);
	}

	this->move(this->speed*cos(this->direction*0.5*M_PI), this->speed*sin(this->direction*0.5*M_PI));
}

void Ghost::chooseDir(){
	int gx = this->xpos / g_tile_size;
	int gy = this->ypos / g_tile_size;
	float newdir = 0;

	// Recalculate target tile
	
	this->pickTargetTile();

	// Include all viable directions
	vector<int> emptyDirs;
	int barrier = 1+static_cast<int>(state==dead||gy<17);

	if (grid[gy][gx + 1]<barrier && this->direction != 2) emptyDirs.push_back(0);
	if (grid[gy][gx - 1]<barrier && this->direction != 0) emptyDirs.push_back(2);
	if (grid[gy + 1][gx]<barrier && this->direction != 3) emptyDirs.push_back(1);
	if (grid[gy - 1][gx]<barrier && this->direction != 1) emptyDirs.push_back(3);

	if (this->state != frightened){
		// Calculate which has the least distance to target tile
		auto it = emptyDirs.begin();
		float shortest = sqrt(pow(gx - cos(*it*0.5*M_PI), 2) + pow(gy - sin(*it*0.5*M_PI), 2));
		newdir = *(emptyDirs.begin());
		for (; it != emptyDirs.end(); it++){
			float delta_x = (gx + sign(cos(*it*0.5*M_PI))) - this->target_tile.x;
			float delta_y = (gy + sign(sin(*it*0.5*M_PI))) - this->target_tile.y;
			float distance = sqrt(pow(delta_x, 2) + pow(delta_y, 2));

			if (distance < shortest){
				shortest = distance;
				newdir = *it;
			}
		}
	}
	else{
		if (emptyDirs.size()>0)
			newdir = emptyDirs[rand() % emptyDirs.size()];
		else
			newdir = this->direction;
	}
	// Go that direction
	this->direction = newdir;
}

void Ghost::pickTargetTile(){
	if (this->state == dead){
		this->target_tile = sf::Vector2<int>(13, 16);
	}
	shared_ptr<Pacman> p = std::static_pointer_cast<Pacman>(g_player);
	// Blinky
	if (n == blinky){
		if (this->state == scatter){
			this->target_tile = sf::Vector2<int>(25, 0);
		}
		else if (this->state == chase){
			this->target_tile = sf::Vector2<int>(static_cast<int>(g_player->xpos / g_tile_size),
				static_cast<int>(g_player->ypos / g_tile_size));
		}
	}
	// Pinky
	else if (n == pinky){
		if (this->state == scatter){
			this->target_tile = sf::Vector2<int>(3, 0);
		}
		else if (this->state == chase){
			this->target_tile = sf::Vector2<int>(static_cast<int>(p->xpos / g_tile_size)+4*sign(cos(p->direction)),
				static_cast<int>(p->ypos / g_tile_size) + 4 * sign(sin(p->direction)));
		}
	}
	// Inky
	else if (n == inky){
		if (this->state == scatter){
			this->target_tile = sf::Vector2<int>(3, 35);
		}
		else if (this->state == chase){
			sf::Vector2<int> player_point = sf::Vector2<int>(static_cast<int>(p->xpos / g_tile_size) + 4 * sign(cos(p->direction)),
				static_cast<int>(p->ypos / g_tile_size) + 4 * sign(sin(p->direction)));
			sf::Vector2<int> blinky_point = sf::Vector2<int>(static_cast<int>(g_blinky->xpos / g_tile_size),
				static_cast<int>(g_blinky->ypos / g_tile_size));
			sf::Vector2<int> pb_vector = player_point - blinky_point;
			pb_vector *= 2;
			this->target_tile = blinky_point+pb_vector;
		}
	}
	// Clyde
	else if (n == clyde){
		if (this->state == scatter){
			this->target_tile = sf::Vector2<int>(25, 35);
		}
		else if (this->state == chase){
			int gx = this->xpos / g_tile_size;
			int gy = this->ypos / g_tile_size;

			sf::Vector2<int> player_point = sf::Vector2<int>(static_cast<int>(p->xpos / g_tile_size),
				static_cast<int>(p->ypos / g_tile_size));

			float distance = sqrt(pow((player_point.x - gx), 2) + pow((player_point.y - gy), 2));
			if (distance > 8){
				this->target_tile = sf::Vector2<int>(static_cast<int>(g_player->xpos / g_tile_size),
					static_cast<int>(g_player->ypos / g_tile_size));
			}
			else{
				this->target_tile = sf::Vector2<int>(25, 35);
			}
			
		}
	}

}

// Return whether the ghost is at a turning point

// 1. If running into a wall, return true, snap to grid
// 2. Must be at center of a grid square
// 3. Must have more than one viable direction to move

bool Ghost::isAtChoicePoint(int gx, int gy,float dir){
	if (gx == cur_tile.x&&gy == cur_tile.y) return false;

	// 1.
	if (grid[gy + sign(sin(dir*0.5*M_PI))][gx + sign(cos(dir*0.5*M_PI))] > 0){
		if (abs(static_cast<int>(floor(this->xpos)) - (gx * 8 + 4))<1 && abs(static_cast<int>(floor(this->ypos)) - (gy * 8 + 4))<1){
			this->snapToGrid();
			return true;
		}
	}
	// 2.
	if (static_cast<int>(floor(this->xpos)) == gx * 8 + 4 && static_cast<int>(floor(this->ypos)) == gy * 8 + 4){
		
		
	// 3.
		bool r = (grid[gy][gx + 1] < 1 && this->direction != 2);
		bool l = (grid[gy][gx - 1] < 1 && this->direction != 0);
		bool d = (grid[gy + 1][gx] < 1 && this->direction != 3);
		bool u = (grid[gy - 1][gx] < 1 && this->direction != 1);

		if (r + l + d + u>0) return true;
	}
	return false;
}

bool Ghost::isInGhostHouse(){
	int gx = this->xpos / g_tile_size;
	int gy = this->ypos / g_tile_size;
	return false;
}

// Snaps the ghost to the center of its current grid square
void Ghost::snapToGrid(){
	this->xpos = 8*floor(this->xpos / 8) + 4;
	this->ypos = 8*floor(this->ypos / 8) + 4; 
}

void Ghost::updateState(){
	if (state == scatter){
		if (state_timer.getElapsedTime() >= sf::seconds(7)){
			state_timer.restart();
			state = chase;
		}
	}
	if (state == chase){
		if (state_timer.getElapsedTime() >= sf::seconds(30)){
			state_timer.restart();
			state = scatter;
		}
	}
	if (state == frightened){
		if (state_timer.getElapsedTime() >= sf::seconds(7)){
			this->getNormal();
		}
		else if (state_timer.getElapsedTime() >= sf::seconds(5)){
			this->image_index_max = 3;
		}
		
	}
}

void Ghost::doPlayerCollision(){
	int gx = this->xpos / g_tile_size;
	int gy = this->ypos / g_tile_size;

	int pgx = g_player->xpos / g_tile_size;
	int pgy = g_player->ypos / g_tile_size;

	if (gx == pgx&&gy == pgy){
		if (state == frightened){
			g_ghost_combo++;
			game.addScore(100 * pow(2, g_ghost_combo));
			this->die();
		}
		else if(state==chase||state==scatter){
			shared_ptr<Pacman> p = std::static_pointer_cast<Pacman>(g_player);
			p->die();
			this->state_timer.restart();
		}
		
	}
}

void Ghost::reset(){
	if (this->n == blinky){
		this->moveTo(112,116);
		this->direction = 2;
	}
	else if (this->n == inky){
		this->moveTo(88+8, 152-12);
		this->direction = 3;
	}
	else if (this->n == pinky){
		this->moveTo(104 + 8, 152 - 12);
		this->direction = 1;
	}
	else if (this->n == clyde){
		this->moveTo(120 + 8, 152 - 12);
		this->direction = 3;
	}
	this->snapToGrid();
	this->cur_tile = sf::Vector2<int>(-1, -1);
	this->state = scatter;
	this->speed = 0.7;
	this->state_timer.restart();
}

