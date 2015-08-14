#include "Level.h"
#include "Globals.h"

// Load a level from a file specified by path
void Level::loadFromFile(string path){
	
	ifstream ifile(path);
	string line;
	g_cur_id = 0;

	while (getline(ifile, line)){
		
		// Trim leading and trailing whitespace
		if (line.find_first_not_of('\t')!=string::npos&&
			line.find_last_not_of('\t') != string::npos)
		line = line.substr(line.find_first_not_of('\t'),line.find_last_not_of('\t'));

		// If not object data skip
		if (line.find("object id") == string::npos) continue;

		int length = line.find("gid=")-line.find("type=");
		string type = "";
		if (line.find("type=")!=string::npos) type = line.substr(line.find("type=")+6,length-8);
		//std::cout << type << endl;
		// Pacman
		if (type == "Pacman"){	
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			g_player = shared_ptr<Actor>(new Pacman("images/pacman.png", x, y-12, 16, 16));
			this->actors[this->cur_id++] = g_player;
		}
		// Blinky
		if (type == "Blinky"){
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			this->actors[this->cur_id++] = shared_ptr<Actor>(new Ghost("images/ghost_u.png", x+8, y-12, 16, 16, Ghost::blinky));
			g_blinky = std::static_pointer_cast<Ghost>(this->actors[this->cur_id-1]);
		}
		// Inky
		if (type == "Inky"){
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			this->actors[this->cur_id++] = shared_ptr<Actor>(new Ghost("images/ghost_u.png", x + 8, y - 12, 16, 16, Ghost::inky));
		}
		// Pinky
		if (type == "Pinky"){
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			this->actors[this->cur_id++] = shared_ptr<Actor>(new Ghost("images/ghost_u.png", x + 8, y - 12, 16, 16, Ghost::pinky));
		}
		// Clyde
		if (type == "Clyde"){
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			this->actors[this->cur_id++] = shared_ptr<Actor>(new Ghost("images/ghost_u.png", x + 8, y - 12, 16, 16, Ghost::clyde));
		}
		// Walls
		else if (type == "wall"){	
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			grid[y/8-1][x/8] = 2;
		}
		// Ghostbox Wall
		else if (type == "ghost_gate"){
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			grid[y / 8 - 1][x / 8] = 1;
		}
		// Pellets
		else if (type == "pellet"){
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			this->actors[this->cur_id++] = shared_ptr<Actor>(new Dot("images/dot.png", x, y, 8, 8));
		}
		// Energizers
		else if (type == "power"){
			int x = 0;
			int y = 0;
			x = stoi(line.substr(line.find("x=") + 3, line.find("y=") - line.find("x=") - 2));
			y = stoi(line.substr(line.find("y=") + 3, line.find("width=") - line.find("y=") - 2));
			this->actors[this->cur_id++] = shared_ptr<Actor>(new Energizer("images/power.png", x, y, 8, 8));
		}
		
	}
	g_cur_id = this->cur_id;
}