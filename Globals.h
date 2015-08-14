#include "ResourceManager.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <memory>

#pragma once

using actor_ptr = shared_ptr<Actor>;

extern ResourceManager resources;
extern Game game;

extern sf::RenderWindow window;
extern sf::View view;

extern int g_cur_id;

extern int g_tile_size;
extern int g_level;
extern int g_lives;
extern int g_score;
extern int g_high_score;
extern int g_score_to_life;
extern int g_dot_count;
extern int g_ghost_combo;
extern actor_ptr g_player;
extern shared_ptr<Ghost> g_blinky;
extern vector<int> g_level_fruit_score;