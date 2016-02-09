#include "GameObject.h"

glm::mat4 * GameObject::viewMatrix = NULL;
glm::mat4 * GameObject::projMatrix = NULL;
list<GameObject *> * GameObject::gameObjectList = NULL;
AssetManager * GameObject::assetManager= NULL;

void GameObject::init()
{
	alive = true;
	gameObjectList->push_back(this);
}

GameObject::GameObject()
{
	init();
}

GameObject::GameObject(string mPath, string tPath, string sPath) : RenderObject(mPath, tPath, sPath)
{
	init();
}

GameObject::GameObject(string mPath, string tPath, string sPath, glm::vec3 position, glm::vec3 direction, glm::vec3 up) : RenderObject(mPath, tPath, sPath, position, direction, up)
{
	init();
}

GameObject::~GameObject()
{
	
}

char * GameObject::serialize()
{
	return NULL;
}

void GameObject::deserialize(string buffer)
{

}

int GameObject::update(long elapsedTime)
{
	return RenderObject::update(elapsedTime);
}

int GameObject::render(long totalElapsedTime)
{
	return RenderObject::render(totalElapsedTime);
}