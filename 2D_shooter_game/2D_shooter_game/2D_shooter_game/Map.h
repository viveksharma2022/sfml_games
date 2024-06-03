#pragma once
#include <iostream>
#include "definitions.h"
#include "sfml\Graphics.hpp"

enum ObjectPenetrationType {
	TRANSPARENT,
	OPAQUE
};

enum ObjectType {
	GRASS,
	TREES,
	WATER,
	WOODEN_BLOCKS
};

class MapTile {
protected:
	sf::Vector2f position;
	sf::RectangleShape host;
	sf::Texture texture;
	ObjectPenetrationType objPenetrationType;
	ObjectType objType;
public:
	MapTile(sf::Vector2f position, const std::string& texFile, 
		ObjectPenetrationType objPenetrationType,
		ObjectType objType):
		position(position),
		host({ TILE_WIDTH, TILE_HEIGHT }),
		objPenetrationType(objPenetrationType),
		objType(objType)
	{
		host.setPosition(position); // set the initial position of the player
		if (texture.loadFromFile(texFile)) {
			host.setTexture(&texture);// set the texture for the player
		}
	}
	void SetPosition(sf::Vector2f& position) {
		this->position = position;
	}
	const sf::Vector2f& GetPosition() {
		return this->position;
	}
};

class Trees: public MapTile {
public:
	Trees(sf::Vector2f position, const std::string& texFile) :
		MapTile(position, texFile, ObjectPenetrationType::OPAQUE, ObjectType::TREES) {}
};

class WoodenBlocks : public MapTile {
public:
	WoodenBlocks(sf::Vector2f position, const std::string& texFile) :
		MapTile(position, texFile, ObjectPenetrationType::OPAQUE, ObjectType::WOODEN_BLOCKS) {}
};

class Water : public MapTile {
public:
	Water(sf::Vector2f position, const std::string& texFile) :
		MapTile(position, texFile, ObjectPenetrationType::TRANSPARENT, ObjectType::WATER) {}
};

class Grass : public MapTile {
public:
	Grass(sf::Vector2f position, const std::string& texFile) :
		MapTile(position, texFile, ObjectPenetrationType::TRANSPARENT, ObjectType::GRASS) {}
};

constexpr bool map[MAP_WIDTH][MAP_HEIGHT] = {
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS}
};