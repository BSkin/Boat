#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "InputManager.h"
#include "Camera.h"

class PlayerObject : public Character
{
public:
	PlayerObject();
	PlayerObject(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path); //ok for now
	virtual ~PlayerObject();

	static void setStatics(InputManager * i, Camera * c) { inputManager = i; camera = c; }

	virtual int update(long elapsedTime);
	virtual int render(long totalElapsedTime);
protected:
	inline bool isKeyPressed(long e) { return inputManager->isKeyPressed(e); }
	inline bool isKeyReleased(long e) { return inputManager->isKeyReleased(e); }
	inline bool isKeyDown(long e) { return inputManager->isKeyDown(e); }
	inline bool isKeyUp(long e) { return inputManager->isKeyUp(e); }
	inline bool isMouseScrollUp() { return inputManager->isMouseScrollUp(); }
	inline bool isMouseScrollDown() { return inputManager->isMouseScrollDown(); }

private:
	static InputManager * inputManager;
	static Camera * camera;
};

#endif