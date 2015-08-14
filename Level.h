#pragma once
#include <memory>
#include "Actor.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Dot.h"
#include "Energizer.h"
#include "Fruit.h"
#include "Grid.h"

#include <fstream>
#include <iostream>

class Level
{
public:
	Level(string path){ 
		this->loadFromFile(path); 
	}
	std::map<int, shared_ptr<Actor>> getActors(){
		return this->actors;
	}
	void addActor(Actor){

	}

	void loadFromFile(string);
private:
	std::map<int, shared_ptr<Actor>> actors;
	int cur_id = 0;
};

