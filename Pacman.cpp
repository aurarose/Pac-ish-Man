#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>

#include "Pacman.h"
#include "Globals.h"

Pacman::Pacman(string imgpath, float x, float y, float w, float h) :Actor(imgpath, x, y, w, h){
	this->image_speed = 15;
	this->sprite.setOrigin(7.5, 7.5);
	this->sprite.setPosition(x , y );
	this->direction = M_PI;
	this->state = alive;
	this->actor_type = "pacman";
}

void Pacman::update(){
	if (game.state == game.waiting){
		this->image_index = 0;
		this->image_speed = 0;
	}
	else if (game.state == game.playing){
		this->processInput();
		this->processMovement();

		this->sprite.setRotation(this->direction * 180 / M_PI);
	}
	else if (game.state == game.paused){
		this->image_speed = 0;
	}
	else if (game.state == game.death){
		this->processDeath();
	}
	
}

void Pacman::die(){
	game.state = game.death;
	this->state = hit;
	this->death_timer.restart();
	
}

void Pacman::processInput(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		this->next_direction = M_PI;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		this->next_direction = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
		this->next_direction = 1.5*M_PI;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
		this->next_direction = 0.5*M_PI;
	}
}

void Pacman::processMovement(){

	// Calculate position on grid
	int gx = this->xpos / g_tile_size;
	int gy = this->ypos / g_tile_size;

	int gmovex = sign(cos(this->direction));
	int gmovey = sign(sin(this->direction));

	int gnmovex = sign(cos(this->next_direction));
	int gnmovey = sign(sin(this->next_direction));
	
	// Perform queued turn
	if (this->next_direction != this->direction&&
		grid[gy+gnmovey][gx+gnmovex]<1&&
		this->xpos>8&&this->xpos<216){
		if (gmovex != 0) this->moveTo(gx * g_tile_size + 4,ypos);
		if (gmovey != 0) this->moveTo(xpos,gy * g_tile_size + 4);
		this->direction = this->next_direction;
	}
	// Continue movement to next tiles
	// If target tile is free, move
	if (gx<1||gx>26||grid[gy+gmovey][gx+gmovex] <1){
		this->move(this->speed*cos(this->direction), this->speed*sin(this->direction));
		if (this->image_speed == 0) this->image_speed = 15;
	}
	// Otherwise don't
	else{
		this->image_speed = 0;
		this->image_index = 1;
	}

	// Warp to other side through gates
	if (this->xpos > 224) this->moveTo(0,this->ypos);
	else if (this->xpos < 0) this->moveTo(224, this->ypos);
}

void Pacman::processDeath(){
	if (this->state == hit){
		this->image_speed = 0;
		if (death_timer.getElapsedTime() >= sf::seconds(1)){
			death_timer.restart();
			state = collapsing;
			resources.playSound("sounds/pacman_death.wav");
			this->image_index = 0;
			this->image_speed = 15;
			this->sprite.setRotation(0);
			this->setSprite("images/pacman_death.png");
		}
		
	}
	else if (state == collapsing){
		if (this->image_index>=10){
			image_speed = 0;
			image_index = 11;
			death_timer.restart();
			state = dead;
			if (g_lives<=1)
				game.state = game.game_over;
			game.clock.restart();
		}
	}
	else if (state == dead){
		if (game.state!=game.game_over&&death_timer.getElapsedTime() >= sf::seconds(2)){
			if (g_lives>1)
				g_lives -= 1;
			this->reset();

			if (game.state != Game::game_over){
				game.clock.restart();
				game.state = Game::reset;
				resources.playSound("sounds/pacman_beginning.wav");
			}
			
		}
	}
}

void Pacman::reset(){
	this->moveTo(104, 212);
	this->direction = M_PI;
	this->sprite.setRotation(this->direction);
	this->image_index = 1;
	this->setSprite("images/pacman.png");


	death_timer.restart();
	state = alive;
	this->image_index = 1;
}

int sign(float num){
	if (abs(num)<0.01) return 0;
	if (num > 0) return 1;
	if (num < 0) return -1;
	return 0;
}