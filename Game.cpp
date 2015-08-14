#include "Game.h"
#include "Globals.h"

shared_ptr<Actor> Game::addActor(shared_ptr<Actor> actor){
	this->actors_add_list.push_back(actor);
	return actor;
}

void Game::cullActor(int id){
	this->actors_cull_list.push_back(id);
}

void Game::loadLevel(string path){
	Level level(path);
	this->actors = level.getActors();
	this->state = menu;
	this->clock.restart();
}

void Game::update(){
	// State changes
	if (this->state == reset){
		this->state = waiting;
	}
	if (this->state == game_over){
		if (this->clock.getElapsedTime() >= sf::seconds(4)){
			this->state = menu;
		}
	}
	else if (this->state == menu){

	}
	else if (this->state == paused){
		if (this->clock.getElapsedTime() >= sf::seconds(this->pause_duration)){
			this->state = playing;
		}
	}
	else if (this->state == waiting){
		if (this->clock.getElapsedTime() >= sf::seconds(4)){
			this->state = playing;
			this->clock.restart();
		}	
	}
	else if (this->state == next_level){
		if (this->clock.getElapsedTime() >= sf::seconds(3)){
			g_level += 1;
			g_dot_count = 244;
			this->clock.restart();
			this->state = reset;
			resources.playSound("sounds/pacman_beginning.wav");
			for (auto it = this->actors.begin(); it != actors.end(); it++){
				shared_ptr<Actor> a = it->second;
				if (!a->active) a->active = true;

				if (a->actor_type == "pacman"){
					shared_ptr<Pacman> p = std::static_pointer_cast<Pacman>(a);
					p->reset();
				}
				else if (a->actor_type == "ghost"){
					shared_ptr<Ghost> g = std::static_pointer_cast<Ghost>(a);
					g->reset();
				}
			}
		}
	}
	if (this->state == playing){
		if (this->clock.getElapsedTime() >= sf::seconds(30)){
			this->addActor(shared_ptr<Actor>(new Fruit("images/apple.png", 104, 168, 16, 16)));
			this->clock.restart();
		}
	}

	for (auto it = this->actors.begin(); it != actors.end(); it++){
		if (state!=menu)
			(it->second)->update();
		if (this->state == next_level) continue;
		if ((this->state == death||this->state==game_over)&&it->second->actor_type=="ghost") continue;
			(it->second)->draw();
	}
	auto it = this->actors_add_list.begin();
	while (it != actors_add_list.end()){
		actors[(*it)->id] = *it;
		it = actors_add_list.erase(it);
	}
	auto cull = actors_cull_list.begin();
	while (cull != actors_cull_list.end()){
		auto pos = actors.find(*cull);
		actors.erase(pos);
		cull = actors_cull_list.erase(cull);
	}

	actors_add_list.erase(actors_add_list.begin(), actors_add_list.end());
}

void Game::restart(){
	this->clock.restart();
	this->state = waiting;
	resources.playSound("sounds/pacman_beginning.wav");
	for (auto it = this->actors.begin(); it != actors.end(); it++){
		shared_ptr<Actor> a = it->second;
		if (!a->active) a->active = true;

		if (a->actor_type == "pacman"){
			shared_ptr<Pacman> p = std::static_pointer_cast<Pacman>(a);
			p->reset();
		}
		else if (a->actor_type == "ghost"){
			shared_ptr<Ghost> g = std::static_pointer_cast<Ghost>(a);
			g->reset();
		}
	}
	g_lives = 3;
	g_score = 0;
	g_dot_count = 244;
}

void Game::addScore(int add){
	g_score += add;
	if (g_score > g_high_score) g_high_score = g_score;

	g_score_to_life += add;
	if (g_score_to_life >= 10000){
		g_score_to_life = 0;
		g_lives++;
		resources.playSound("sounds/pacman_extrapac.wav");
	}
}

void Game::nextLevel(){
	this->clock.restart();
	this->state = next_level;

}

void Game::pause(int duration){
	this->clock.restart();
	this->state = paused;
	this->pause_duration = duration;
}