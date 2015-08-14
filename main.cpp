#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "Globals.h"
#include "Level.h"
#include "Actor.h"
#include "Pacman.h"

using namespace std;

int main(){
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);
	view.reset(sf::FloatRect(0, 0, 224, 288));

	window.setView(view);

	game.loadLevel("level1.txt");

	// Load sounds
	resources.loadSound("sounds/pacman_beginning.wav");
	resources.loadSound("sounds/pacman_death.wav");
	resources.loadSound("sounds/pacman_chomp.wav");
	resources.loadSound("sounds/pacman_extrapac.wav");
	resources.loadSound("sounds/pacman_eatghost.wav");
	resources.loadSound("sounds/pacman_eatfruit.wav");

	// Background, messy
	int bg_flash_timer = 0;
	sf::Texture bg_texture;
	bg_texture.loadFromFile("images/walls.png");
	sf::Sprite bg_sprite(bg_texture);
	bg_sprite.setPosition(0, 24);
	// UI

	// Menu
	sf::Sprite pacman_menu_spr(resources.lookUp("images/pacman_menu.png"));
	pacman_menu_spr.setPosition(90, 130);

	// Lives
	sf::Texture life_tex;
	life_tex.loadFromFile("images/life.png");

	//Fruit
	sf::Sprite cherries_spr(resources.lookUp("images/cherries.png"));
	sf::Sprite strawberry_spr(resources.lookUp("images/strawberry.png"));
	sf::Sprite orange_spr(resources.lookUp("images/orange.png"));
	sf::Sprite apple_spr(resources.lookUp("images/apple.png"));
	sf::Sprite melon_spr(resources.lookUp("images/melon.png"));
	sf::Sprite galaxian_spr(resources.lookUp("images/galaxian.png"));
	sf::Sprite bell_spr(resources.lookUp("images/bell.png"));
	sf::Sprite key_spr(resources.lookUp("images/key.png"));

	cherries_spr.setPosition(192, 272);
	strawberry_spr.setPosition(176, 272);
	orange_spr.setPosition(160, 272);
	apple_spr.setPosition(144, 272);
	melon_spr.setPosition(128, 272);
	galaxian_spr.setPosition(112, 272);
	bell_spr.setPosition(96, 272);
	bell_spr.setPosition(80, 272);

	// Text
	sf::Font font;
	font.loadFromFile("Joystix.ttf");
	sf::Font title_font;
	title_font.loadFromFile("Crackman.ttf");

	// Main menu
	sf::Text title_txt("Pac-ish Man", title_font);
	title_txt.setPosition(30, 60);
	title_txt.setScale(0.75, 0.75);
	title_txt.setColor(sf::Color::Yellow);

	sf::Text start_txt("press enter to start", font);
	start_txt.setPosition(30, 180);
	start_txt.setScale(0.35, 0.35);

	// Score
	sf::Text score_txt(to_string(g_score), font);
	score_txt.setPosition(16, 8);
	score_txt.setScale(0.35, 0.35);

	sf::Text score_word_txt("Score", font);
	score_word_txt.setPosition(16, 0);
	score_word_txt.setScale(0.35, 0.35);

	// HighScore
	sf::Text hscore_txt(to_string(g_high_score), font);
	hscore_txt.setPosition(120, 8);
	hscore_txt.setScale(0.35, 0.35);

	sf::Text hscore_word_txt("High Score", font);
	hscore_word_txt.setPosition(88, 0);
	hscore_word_txt.setScale(0.35, 0.35);

	sf::Text get_ready_txt("Ready!", font);
	get_ready_txt.setColor(sf::Color::Yellow);
	get_ready_txt.setPosition(80, 154);
	get_ready_txt.setScale(0.5, 0.5);

	sf::Text game_over_txt("Game Over", font);
	game_over_txt.setColor(sf::Color::Red);
	game_over_txt.setPosition(68, 154);
	game_over_txt.setScale(0.4, 0.4);

	
	///////////////////////////////////////////////

	while (window.isOpen()){
		window.clear();
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		if (game.state == Game::menu&&sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
			game.restart();
		}
			

		if (game.state == Game::next_level&&bg_flash_timer<180){
			if (bg_flash_timer % 45 == 23)
				bg_sprite.setTexture(resources.lookUp("images/walls_flash.png"));
			else if (bg_flash_timer % 45 == 0){
				bg_sprite.setTexture(resources.lookUp("images/walls.png"));
			}
			bg_flash_timer++;
		}
		else{
			if (bg_flash_timer != 0){
				bg_flash_timer = 0;
				bg_sprite.setTexture(resources.lookUp("images/walls.png"));
			}
		}
		if (game.state != Game::menu){
			window.draw(bg_sprite);
			game.update();
			// UI

			//Lives
			for (auto i = 1; i < g_lives; i++){
				sf::Sprite sprite(life_tex);
				sprite.setPosition(16 + i * 16, 272);
				window.draw(sprite);
			}
			// Fruit


			if (g_level >= 13) {
				window.draw(key_spr);
			}
			if (g_level >= 11) {
				window.draw(bell_spr);
			}
			if (g_level >= 9)  {
				window.draw(galaxian_spr);
			}
			if (g_level >= 7)  {
				window.draw(melon_spr);
			}
			if (g_level >= 5)  {
				window.draw(apple_spr);
			}
			if (g_level >= 3)  {
				window.draw(orange_spr);
			}
			if (g_level >= 2)  {
				window.draw(strawberry_spr);
			}
			window.draw(cherries_spr);



			// Texts
			score_txt.setString(to_string(g_score));
			hscore_txt.setString(to_string(g_high_score));
			window.draw(score_word_txt);
			window.draw(hscore_word_txt);
			window.draw(score_txt);
			if (g_high_score>0)	window.draw(hscore_txt);
			if (game.state == game.waiting)
				window.draw(get_ready_txt);
			if (game.state == game.game_over)
				window.draw(game_over_txt);
		}
		else{
			window.draw(title_txt);
			window.draw(start_txt);
			window.draw(pacman_menu_spr);
		}
		
		window.display();
	}
}
