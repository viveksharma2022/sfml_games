#pragma once
#include "Game.h"

class Player;
enum Orientation;
class Game;
struct CollisionDirections;

namespace Utility {
	template<typename T>
	void RenderMap(sf::RenderWindow& mWindow, std::vector<T>& vec) {
		for (auto& v : vec) {
			mWindow.draw(v.GetHost());
		}
	}
	
	void RenderPlayer(sf::RenderWindow& mWindow, const std::unique_ptr<Player>& player);
	inline Orientation SwitchOrientation(Orientation currentOrientation);
	bool IsExceedingBoundary(const sf::Vector2f& currentValue, const sf::Vector2f& newValue);
	bool IsTouchingOpaques(const sf::FloatRect& hostBox, const std::vector<MapTile>& opaqueTiles);
	CollisionDirections IsTouchingOpaquesDetailed(const sf::FloatRect& hostBox, const std::vector<MapTile>& opaqueTiles);
	void RenderBullets(Game* game);
	void UpdateBullets(Game* game);
	void ApplyBoundaryConditionsToBullets(Game* game);
	void ApplyGravityEffect(Game* game);
	void GetIntersectingRectangle(const sf::FloatRect& rect1, const sf::FloatRect& rect2, sf::FloatRect& intersectingRectangle);
	void RenderCommon(Game* game);
}
