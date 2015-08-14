#include "Fruit.h"
#include "Globals.h"

Fruit::Fruit(string imgpath, float x, float y, float w, float h) : Actor(imgpath, x, y, w, h){
	x += 4;
	y -= 4;
	this->grid_x = x / 8;
	this->grid_y = y / 8;

	this->image_speed = 15;
	this->sprite.setOrigin(7.5, 7.5);
	this->sprite.setPosition(x, y);
	this->actor_type = "fruit";
	this->timer.restart();

	string fruit_type;
	if (g_level >= 13) fruit_type = "key";
	else if (g_level >= 11) fruit_type = "bell";
	else if (g_level >= 9) fruit_type = "galaxian";
	else if (g_level >= 7) fruit_type = "melon";
	else if (g_level >= 5) fruit_type = "apple";
	else if (g_level >= 3) fruit_type = "orange";
	else if (g_level >= 2) fruit_type = "strawberry";
	else fruit_type = "apple";

	this->setSprite("images/"+fruit_type+".png");
}

void Fruit::update(){
	if (this->active&&this->grid_x == static_cast<int>(g_player->xpos / 8) &&
		this->grid_y == static_cast<int>(g_player->ypos / 8)){
		resources.playSound("sounds/pacman_eatfruit.wav");
		game.addScore(g_level_fruit_score[g_level]);
		game.cullActor(this->id);
		game.pause(1);
	}
	if (this->timer.getElapsedTime() > sf::seconds(10)){
		game.cullActor(this->id);
	}
}
