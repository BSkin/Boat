#include "PropObject.h"

PropObject::PropObject(string m, string t, string s, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path, int colGroup, int colMask) : RigidObject(m, t, s, position, direction, up, path, colGroup, colMask)
{
	initBullet(path, colGroup, colMask);
}

PropObject::~PropObject() {}

int PropObject::initBullet(string path, int colGroup, int colMask)
{
	colShape = PhysicsObject::getCollisionShape(path);
	return RigidObject::initBullet(path, colGroup, colMask);
}