#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "RenderObject.h"

class GameObject : public RenderObject
{
public:
	GameObject();
	GameObject(string, string, string); //ok for now
	GameObject(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up); //ok for now
	virtual ~GameObject();

	bool isAlive() { return alive; }
	glm::vec3 getPosition() { return position; }
	glm::vec3 getDirection() { return direction; }
	glm::vec3 getUp() { return up; }

	virtual char * serialize();
	virtual void deserialize(string buffer);
	virtual int getSerializedSize() { return 0; }

	virtual int update(long elapsedTime);
	virtual int render(long totalElapsedTime);

	static void setStatics(list<GameObject *> * goList) { gameObjectList = goList; }

protected:
	bool alive;
private:
	void init();

	static list<GameObject *> * gameObjectList;
};

#endif