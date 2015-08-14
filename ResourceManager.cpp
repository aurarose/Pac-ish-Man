#include "ResourceManager.h"

sf::Texture& ResourceManager::lookUp(string path){
	if (this->textures.find(path) == this->textures.end()){
		sf::Texture texture;
		this->textures[path] = texture;
		this->textures[path].loadFromFile(path);
	}
	return this->textures[path];
}

void ResourceManager::loadSound(string path){
	sf::SoundBuffer buffer;
	if (this->sound_buffers.find(path) == this->sound_buffers.end())
		this->sound_buffers[path] = buffer;
	this->sound_buffers[path].loadFromFile(path);
	sf::Sound sound;
	sound.setBuffer(this->sound_buffers[path]);
	this->sounds[path] = sound;
}

void ResourceManager::playSound(string path){
	this->sounds[path].play();
}

void ResourceManager::stopSound(string path){
	this->sounds[path].stop();
}

void ResourceManager::loopSound(string path){
	this->sounds[path].setLoop(true);
}

bool ResourceManager::soundIsPlaying(string path){
	return this->sounds[path].getStatus()==sf::Sound::Playing;
}