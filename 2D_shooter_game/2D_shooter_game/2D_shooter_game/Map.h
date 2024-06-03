#pragma once
#include <iostream>
#include "definitions.h"
#include "sfml\Graphics.hpp"

const std::string grassTexFile = "assets\\grass.jpg";
const std::string brickTexFile = "assets\\brick.jpg";
static sf::Texture grassTexture;
static sf::Texture brickTexture;

enum ObjectPenetrationType {
	TRANSPARENT,
	OPAQUE
};

enum ObjectType {
	GRASS,
	TREES,
	WATER,
	BRICK
};

class MapTile {
protected:
	sf::Vector2f position;
	sf::RectangleShape host;
	ObjectPenetrationType objPenetrationType;
	ObjectType objType;
public:
	MapTile(sf::Vector2f position, 
		ObjectPenetrationType objPenetrationType,
		ObjectType objType):
		position(position),
		host(sf::Vector2f({ TILE_WIDTH, TILE_HEIGHT })),
		objPenetrationType(objPenetrationType),
		objType(objType)
	{}
	void SetPosition(sf::Vector2f& position) {
		this->position = position;
	}
	const sf::Vector2f& GetPosition() {
		return this->position;
	}
	sf::RectangleShape& GetHost() { return host; };
};

class Trees: public MapTile {
public:
	Trees(sf::Vector2f position) :
		MapTile(position, ObjectPenetrationType::OPAQUE, ObjectType::TREES) {}
};

class Brick : public MapTile {
public:
	Brick(sf::Vector2f position) :
		MapTile(position, ObjectPenetrationType::OPAQUE, ObjectType::BRICK) {
		host.setPosition(position); // set the initial position of the host
		if (brickTexture.loadFromFile(brickTexFile)) {
			host.setTexture(&brickTexture);// set the texture for the host
		}
	}
};

class Water : public MapTile {
public:
	Water(sf::Vector2f position) :
		MapTile(position, ObjectPenetrationType::TRANSPARENT, ObjectType::WATER) {}
};

class Grass : public MapTile {
public:
	Grass(sf::Vector2f position) :
		MapTile(position, ObjectPenetrationType::TRANSPARENT, ObjectType::GRASS) {
		host.setPosition(position); // set the initial position of the host
		if (grassTexture.loadFromFile(grassTexFile)) {
			host.setTexture(&grassTexture);// set the texture for the host
		}
	}
};

constexpr ObjectType map[MAP_GRIDS_X][MAP_GRIDS_Y] = {
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, BRICK, BRICK, BRICK, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, BRICK, BRICK, BRICK, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, BRICK, BRICK, BRICK, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
	{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS}
};