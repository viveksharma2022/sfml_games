#pragma once
#include "Game.h"

namespace Utility {
	template<typename T>
	void RenderMap(sf::RenderWindow& mWindow, std::vector<T>& vec) {
		mWindow.clear();
		for (auto& v : vec) {
			mWindow.draw(v.GetHost());
		}
		mWindow.display();
	}
}
