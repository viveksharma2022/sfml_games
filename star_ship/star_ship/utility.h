#pragma once

struct Bullet;
namespace Utility {
	// TODO: change bullet from stack memory to unique pointer. Impact on speed to be taken into account
	template<typename T>
	void BorderCheck(T& positionalObj) {
		// check if position of a positional object exceeds boundaries
		// PositionalObj must contain a host(drawing object in sfml context) and position(Vector2f)
		// host reference point is top left corner
		const static sf::Vector2f hostSize = positionalObj.host.getSize();
		if (positionalObj.position.x < 0 || (positionalObj.position.x + hostSize.y) > WINDOW_WIDTH
			|| (positionalObj.position.y) < 0 || (positionalObj.position.y + hostSize.x) > WINDOW_LENGTH) {
			positionalObj.isExist = false;
		}
	}

	void CollisionCheck(Bullet& b);

	template<typename T>
	bool CheckNotExists(T& positionalObj) {
		// check if a positional object exists
		return !positionalObj.isExist;
	}

	template <typename T>
	T BoundCheck(T currentValue, T newValue, T width) {
		// restrict values within the window width range not
		return  (newValue >= 0.0f && newValue <= width) ? newValue : currentValue;
	}

}
