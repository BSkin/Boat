#ifndef RIGIDOBJECT_H
#define RIGIDOBJECT_H

#include "PhysicsObject.h"

class RigidObject : public PhysicsObject
{
public:
	RigidObject();
	RigidObject(string, string, string); //ok for now
	RigidObject(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path, int colGroup, int colMask); //ok for now
	virtual ~RigidObject() = 0;

	virtual int render(long totalElapsedTime);
	virtual int update(long elapsedTime);

	btVector3 getWorldPosFromLocalPos(float x, float y, float z) { if (!physInit) return btVector3(0,0,0); return body->getWorldTransform() * btVector3(x, y, z); }
	btVector3 getLocalPosFromWorldPos(float x, float y, float z) { if (!physInit) return btVector3(0,0,0); return body->getWorldTransform().inverse() * btVector3(x, y, z); }
	btVector3 getLocalPosFromWorldPos(glm::vec3 vec)			{ return getLocalPosFromWorldPos(vec.x, vec.y, vec.z); }

	virtual btVector3 getAngularVelocity()						{ if (!physInit) return btVector3(0,0,0); return body->getAngularVelocity(); }
	virtual btVector3 getVelocity()								{ if (!physInit) return btVector3(0,0,0); return body->getLinearVelocity(); }
	virtual void setAngularVelocity(float x, float y, float z)	{ if (!physInit) return; body->setAngularVelocity(btVector3(x, y, z)); }
	virtual void setAngularVelocity(glm::vec3 vel)				{ setAngularVelocity(vel.x, vel.y, vel.z); }
	virtual void setVelocity(float x, float y, float z)			{ if (!physInit) return; body->setLinearVelocity(btVector3(x, y, z)); }
	virtual void setVelocity(glm::vec3 vel)						{ setVelocity(vel.x, vel.y, vel.z); }

	virtual void setGravity(glm::vec3 grav)						{ if (!physInit) return; body->setGravity(btVector3(grav.x, grav.y, grav.z)); }
	virtual void setGravity(float x, float y, float z)			{ if (!physInit) return; body->setGravity(btVector3(x, y, z)); }
	virtual void setFriction(float x)							{ if (!physInit) return; body->setFriction(x); }
	virtual void setMass(float x);
	virtual void setRotation(glm::vec3 lookAt, glm::vec3 up);
	virtual void applyCentralImpulse(float x, float y, float z)	{ if (!physInit) return; body->applyCentralImpulse(btVector3(x, y, z)); }
	virtual btVector3 getPosition()								{ if (!physInit) return btVector3(0,0,0); return body->getCenterOfMassPosition(); }
	virtual void wakeUp()										{ if (!physInit) return; body->activate(true); }

	virtual void collisionCallback(RigidObject * other);
protected:
	int initBullet(string path, int colGroup, int colMask);
	int collisionGroup, collisionMask;
	string collisionPath;

	//btTriangleMesh *mTriMesh;// = new btTriangleMesh();
	btRigidBody * body;
};

#endif