#include "Fireball.h"
#include "ExplosionContactCallback.h"

Fireball::Fireball(string m, string t, string s, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path, void * owner) : PropObject(m, t, s, position, direction, up, path)
{
	this->owner = owner;
	force = 2.5f;
	damage = 15.0f;
	collisionType = "projectile"; 
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}
Fireball::~Fireball() {}

btRigidBody * Fireball::initExplosionBody()
{
	btRigidBody * temp;
	btCollisionShape * colShape = PhysicsObject::getCollisionShape("rocketExplosion");
	
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass = 1.0f;
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass,localInertia);
		
	startTransform.setOrigin(btVector3(body->getWorldTransform().getOrigin()));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	temp = new btRigidBody(mass,myMotionState,colShape,localInertia, owner);
	delete myMotionState;
	return temp;
}

void Fireball::explode()
{
	btRigidBody * tgtBody = initExplosionBody();
	string foo = "asdf";
	ExplosionContactCallback callback(*tgtBody, foo, force, damage);
	dynamicsWorld->updateAabbs();
	dynamicsWorld->contactTest(tgtBody,callback);
	delete tgtBody;
}

void Fireball::collisionCallback(RigidObject * otherObject)
{
	if (otherObject == owner) return;
 	alive = false;
	explode();
}