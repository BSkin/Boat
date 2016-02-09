#ifndef CHARACTER_H
#define CHARACTER_H

#include "glm/gtx/rotate_vector.hpp"

#include "PropObject.h"
#include "Spell.h"
#include "FireballLauncher.h"
#include "AirborneContactCallback.h"

#define maxSpeed 100.0f

class Spell;

class Character : public PropObject
{
public:
	Character(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path); //ok for now
	virtual ~Character();

	virtual int update(long elapsedTime);
	virtual int render(long totalElapsedTime);

	bool getAir() { return getAirborne(); }
	glm::vec2 getAngle() { return directionAngle;  }
protected:
	void runForward();
	void runBackward();
	void runLeft();
	void runRight();

	void walkForward();
	void walkBackward();
	void walkLeft();
	void walkRight();

	void turn(float x, float y);
	void turn(POINT x) { turn((float)x.x, (float)x.y); }
	void turnLeft(float angle);
	void turnRight(float angle);
	void turnUp(float angle);
	void turnDown(float angle);
	
	void setDirection(glm::vec3 direction);
	void setDirection(glm::vec3 direction, glm::vec3 left, glm::vec3 up);

	void primaryClick()		{ if (activeSpell == NULL) return; activeSpell->primaryClick(); }
	void primaryDown()		{ if (activeSpell == NULL) return; activeSpell->primaryDown(); }
	void primaryUp()		{ if (activeSpell == NULL) return; activeSpell->primaryUp(); }
	void primaryRelease()	{ if (activeSpell == NULL) return; activeSpell->primaryRelease(); }

	void secondaryClick()	{ if (activeSpell == NULL) return; activeSpell->secondaryClick(); }
	void secondaryDown()	{ if (activeSpell == NULL) return; activeSpell->secondaryClick(); }
	void secondaryUp()		{ if (activeSpell == NULL) return; activeSpell->secondaryClick(); }
	void secondaryRelease() { if (activeSpell == NULL) return; activeSpell->secondaryClick(); }

	void jump();
private:
	void walk(float x, float y, float z);
	void walk(glm::vec3 val) { walk(val.x, val.y, val.z); }
	void airwalk(float x, float y, float z, long elapsedTime);
	void airwalk(glm::vec3 val, long elapsedTime) { airwalk(val.x, val.y, val.z, elapsedTime); }
	void AirControl(glm::vec3 wishdir, float wishspeed, long elapsedTime);
	void Accelerate(glm::vec3 wishdir, float wishspeed, float accel, long elapsedTime);

	void stopWalking();
	bool getAirborne() { return airborne; }
	void testGround();
	
	Spell * activeSpell;
	list<Spell*> weaponList;

	btRigidBody * airborneTest;
	btRigidBody * createAirborneTest();

	RigidObject * vehicle;
	btVector3 vehicleSpeed;
	long noBoatTimer;

	bool airborne;
	bool sliding;
	float floorDist;
	btVector3 groundPos;
	btVector3 groundNormal;
	glm::vec2 frameTurnAmount;
	glm::vec2 directionAngle;
	glm::vec3 left;
	glm::vec3 walkDirection;
	glm::vec2 localWalkDirection;

	static float moveSpeed;  // Ground move speed
	static float runAcceleration;   // Ground accel
	static float runDeacceleration;   // Deacceleration that occurs when running on the ground
	static float airAcceleration;  // Air accel
	static float airDeacceleration;  // Deacceleration experienced when opposite strafing
	static float airControl;  // How precise air control is
	static float sideStrafeAcceleration;   // How fast acceleration occurs to get up to sideStrafeSpeed when side strafing
	static float sideStrafeSpeed;    // What the max speed to generate when side strafing
	static float jumpSpeed;  // The speed at which the character's up axis gains when hitting jump
	static float moveScale;
};

#endif