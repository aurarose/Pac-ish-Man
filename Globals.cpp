#include <memory>
#include "Game.h"
#include "Actor.h"
#include "globals.h"

// Engine variables
ResourceManager resources;
sf::RenderWindow window(sf::VideoMode(224*2, 288*2), "Pac Man");
sf::View view;

Game game;
int g_cur_id = 0;

// Game variables
actor_ptr g_player;
shared_ptr<Ghost> g_blinky;
int g_tile_size = 8;
int g_score = 0;
int g_high_score = 0;
int g_score_to_life = 0;
int g_lives = 3;
int g_level = 1;
int g_dot_count = 244;
int g_ghost_combo = 0;
vector<int> g_level_fruit_score{
	0,
	100,300,
	500,500,
	700,700,
	1000,1000,
	2000, 2000,
	3000, 3000,
	5000
};