#include "Dot.h"
#include "Globals.h"

void Dot::update(){
	if (this->active&&this->grid_x == static_cast<int>(g_player->xpos / 8)&&
		this->grid_y == static_cast<int>(g_player->ypos / 8)){
		this->active = false;
		game.addScore(10);

		if (!resources.soundIsPlaying("sounds/pacman_chomp.wav")){
			resources.playSound("sounds/pacman_chomp.wav");
		}
		g_dot_count--;
		cout << g_dot_count << endl;
		if (g_dot_count < 1){
			game.nextLevel();
		}
	}
}
