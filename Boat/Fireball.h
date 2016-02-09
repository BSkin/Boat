#ifndef FIREBALL_H
#define FIREBALL_H

#include "PropObject.h"

class Fireball : public PropObject
{
public:
	Fireball(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path, void * owner = 0); //ok for now
	virtual ~Fireball();

	virtual void collisionCallback(RigidObject * other);
protected:
	virtual void explode();
	btRigidBody * initExplosionBody();
	void * owner;
	float force, damage;
};

#endif