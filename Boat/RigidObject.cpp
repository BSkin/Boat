#include "RigidObject.h"

RigidObject::RigidObject() : PhysicsObject() {}
RigidObject::RigidObject(string m, string t, string s) : PhysicsObject(m, t, s) {}
RigidObject::RigidObject(string m, string t, string s, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path, int colGroup, int colMask) : PhysicsObject(m, t, s, position, direction, up) 
{
	collisionPath = path;
	collisionGroup = colGroup;
	collisionMask = colMask;
}
RigidObject::~RigidObject() 
{ 
	if (!physInit) return;

	dynamicsWorld->removeCollisionObject( body );
	delete body->getMotionState();
	delete body; 
}

int RigidObject::initBullet(string path, int collisionGroup, int collisionMask)
{
	//Collision Shape already created by child class
	
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass = ((path == "model" || path == "heightField") ? 0.0f : 1.0f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass,localInertia);
		
	/*startTransform.setOrigin(btVector3(
					btScalar(position.x),
					btScalar(position.y),
					btScalar(position.z)));*/

	worldMatrix = glm::inverse(glm::lookAt(position, position+direction, up));
	startTransform.setFromOpenGLMatrix(glm::value_ptr(worldMatrix));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	body = new btRigidBody(mass,myMotionState,colShape,localInertia, this);

	dynamicsWorld->addRigidBody(body, collisionGroup, collisionMask);
	body->setFriction(0.5f);

	//body->setActivationState(ISLAND_SLEEPING);	
	//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	return PhysicsObject::initBullet();
}

int RigidObject::render(long totalElapsedTime)
{
	if (!physInit) return -1;
	body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(worldMatrix)); // * scale
	normalMatrix = glm::transpose(glm::inverse(worldMatrix));

	return GameObject::render(totalElapsedTime);
}

int RigidObject::update(long elapsedTime)
{
	PhysicsObject::update(elapsedTime);
	if (!physInit) initBullet(collisionPath, collisionGroup, collisionMask);
	if (!physInit) return -1;
	
	return 0;
}

void RigidObject::collisionCallback(RigidObject * otherObject)
{
	
}

void RigidObject::setRotation(glm::vec3 lookAt, glm::vec3 up) 
{
	direction = lookAt;
	this->up = up;
	btVector3 posBT = body->getCenterOfMassPosition(); 
	glm::vec3 posGLM = glm::vec3(posBT.x(), posBT.y(), posBT.z());
	worldMatrix = glm::inverse(glm::lookAt(posGLM, posGLM+lookAt, up));

	btTransform trans;	
	trans.setFromOpenGLMatrix(glm::value_ptr(worldMatrix));
	body->setWorldTransform(trans);
}

void RigidObject::setMass(float x)
{
	if (!physInit) return;

	//dynamicsWorld->removeRigidBody( body );
	btVector3 inertia;
	body->getCollisionShape()->calculateLocalInertia( x, inertia );
	body->setMassProps(x, inertia);
	//dynamicsWorld->addRigidBody( body );
}