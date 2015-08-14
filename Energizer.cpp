#include "Energizer.h"
#include "Globals.h"

void Energizer::update(){
	if (this->active&&this->grid_x == static_cast<int>(g_player->xpos / 8) &&
		this->grid_y == static_cast<int>(g_player->ypos / 8)){
		this->active = false;
		game.addScore(50);
		makeGhostsVulnerable();
		g_dot_count--;
		if (g_dot_count < 1){
			game.nextLevel();
		}
	}
}

void Energizer::makeGhostsVulnerable(){
	g_ghost_combo = 0;
	for (auto it = game.actors.begin(); it != game.actors.end(); it++){
		if (it->second->actor_type == "ghost"){
			shared_ptr<Ghost> g = std::static_pointer_cast<Ghost>(it->second);
			g->getScared();
		}
	}
}

