#pragma once
#include <list>
#include <algorithm>
#include <SFML\graphics.hpp>

struct Bullet;
class Enemy;
class Game;

namespace Utility {
	// TODO: change bullet from stack memory to unique pointer. Impact on speed to be taken into account
	template<typename T>
	void BorderCheck(T& positionalObj) {
		// check if position of a positional object exceeds boundaries
		// PositionalObj must contain a host(drawing object in sfml context) and position(Vector2f)
		// host reference point is top left corner
		const static sf::Vector2f hostSize = positionalObj.host.getSize();
		if (positionalObj.position.x < 0 || (positionalObj.position.x + hostSize.y) > WINDOW_WIDTH
			|| (positionalObj.position.y) < 0 || (positionalObj.position.y + hostSize.x) > (WINDOW_LENGTH - SCORE_BOARD_HEIGHT)) {
			positionalObj.isExist = false;
		}
	}

	void CheckWindowClosed(Game* currentGame);
	void CollisionCheck(std::list<Enemy>& enemies, std::list<Bullet>& bullets);

	template<typename T>
	bool CheckNotExists(T& positionalObj) {
		// check if a positional object exists
		return !(positionalObj.isExist && positionalObj.isNotCollided);
	}

	template <typename T>
	T BoundCheck(T currentValue, T newValue, T width) {
		// restrict values within the window width range assuming start is at 0
		return  (newValue >= 0.0f && newValue <= width) ? newValue : currentValue;
	}

	// singleton class
	class ScoreBoard {
	private:
		uint64_t score;
		sf::Vector2f position;
		ScoreBoard() :
			score(0),
			position(sf::Vector2f(0.0f, static_cast<float>(WINDOW_LENGTH - SCORE_BOARD_HEIGHT))){};
	public:
		ScoreBoard(const ScoreBoard& obj) = delete; // delete copy constructor
		ScoreBoard& operator=(const ScoreBoard& obj) = delete; // delete assignment operator
		static ScoreBoard& CreateScoreBoard(){
			static ScoreBoard scoreBoard;
			return scoreBoard;
		}
		void AddScore(uint64_t addend) {
			score += addend;
		}
		const uint64_t& GetScore() const {
			return score;
		}
		const sf::Vector2f& GetScoreBoardPosition() const { return position; }
	};
}
