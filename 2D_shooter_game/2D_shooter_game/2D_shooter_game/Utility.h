#pragma once
#include "Game.h"

class Player;
enum Orientation;

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

}
