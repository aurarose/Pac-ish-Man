#include "Actor.h"
#include "Globals.h"
#include <iostream>
using namespace std;

Actor::Actor(string imgpath,float x,float y,float width,float height){
	this->xpos = x;
	this->ypos = y;
	this->width = width;
	this->height = height;
	this->id = g_cur_id++;

	this->colliders.push_back(Hitbox(0,0,width,height));
	this->colliders[0].updatePosition(x, y);

	if (imgpath != ""){
		this->setSprite(imgpath);
	}
	this->active = true;
}

void Actor::move(float x, float y){
	this->xpos += x;
	this->ypos += y;
	this->sprite.setPosition(this->xpos, this->ypos);
	for (auto it = this->colliders.begin(); it != this->colliders.end(); it++){
		it->updatePosition(this->xpos, this->ypos);
	}
}

void Actor::moveTo(float x, float y){
	if (x >= 0 && x + this->colliders[0].width < 640)
		this->xpos = x;
	else if (x < 0) this->xpos = 0;
	else
		this->xpos = 640 - this->colliders[0].width;
	if (y >= 0 && y < 350)
		this->ypos = y;
	else if (y < 0) this->ypos = 0;
	else
		this->ypos = 480 - 130;
	this->sprite.setPosition(this->xpos, this->ypos);
	for (auto it = this->colliders.begin(); it != this->colliders.end(); it++){
		it->updatePosition(this->xpos, this->ypos);
	}
}

float Actor::distanceToActor(Actor other){
	float x_dist = this->xpos - other.xpos;
	float y_dist = this->ypos - other.ypos;
	return sqrt(x_dist*x_dist + y_dist*y_dist);
}

bool Actor::collidesWithActor(Actor other){
	for (auto this_it = this->colliders.begin(); this_it != this->colliders.end(); this_it++){
		for (auto o_it = other.colliders.begin(); o_it != other.colliders.end(); o_it++){
			if (this_it->collides(*o_it)) return true;
		}
	}
	return false;
}

void Actor::setHitbox(float x, float y, float width, float height){
	this->colliders.erase(this->colliders.begin(),this->colliders.end());
	this->colliders.push_back(Hitbox(x,y,width,height));
}

void Actor::addHitbox(float x, float y, float width, float height){
	this->colliders.push_back(Hitbox(x, y, width, height));
}
void Actor::draw(){
	if (!active) return;
	if (this->image_index_max > 0){
		std::pair<int, int> xy = coordsFromIndex(this->image_index, this->width, this->height, this->tex_max_x, this->tex_max_y);
		int y = xy.second;
		int x = xy.first;
		this->sprite.setTextureRect(sf::IntRect(x, y, this->width, this->height));
	}
	if (this->image_speed > 0){
		this->image_timer++;
		if (this->image_timer > (60 / this->image_speed)){
			this->image_index++;
			if (this->image_index > this->image_index_max) this->image_index = 0;
			this->image_timer = 0;
		}
	}
	window.draw(this->sprite);
}

void Actor::setSprite(string imgpath){
	if (imgpath != ""){
		this->sprite.setTexture(resources.lookUp(imgpath));
		sf::Texture t = resources.lookUp(imgpath);

		this->tex_max_x = t.getSize().x;
		this->tex_max_y = t.getSize().y;

		this->image_index_max = (this->tex_max_x / width)*(this->tex_max_y / height) - 1;
	}
	else{
		this->sprite.setTexture(resources.lookUp(imgpath));
	}
}

// Helper function for spritesheet animation
// Converts the image_index to the position on the sheet to draw the current sprite from
std::pair<int,int> coordsFromIndex(int& index,int width,int height,int max_x,int max_y){
	int x = (index*width) % (max_x);
	int y = ((index*width) / (max_x))*height;
	return std::pair<int,int> (x,y);
}