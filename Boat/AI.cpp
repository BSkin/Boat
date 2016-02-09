#include "AI.h"

AI::AI(string m, string t, string s, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path) : Character(m, t, s, position, direction, up, path) {}
AI::~AI() {}

int AI::update(long elapsedTime)
{
	walkForward();
	turnLeft(0.75f);
	static long totalElapsedTime;
	totalElapsedTime += elapsedTime;
	//if (totalElapsedTime%2000 < 10) primaryClick();

	return Character::update(elapsedTime);
}

int AI::render(long totalElapsedTime)
{
	return Character::render(totalElapsedTime);
}