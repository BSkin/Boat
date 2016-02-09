#include "Character.h"

float Character::moveSpeed = 7.0f;  // Ground move speed
float Character::runAcceleration = 14.0f;   // Ground accel
float Character::runDeacceleration = 10.0f;   // Deacceleration that occurs when running on the ground
float Character::airAcceleration = 2.0f;  // Air accel
float Character::airDeacceleration = 2.0f;  // Deacceleration experienced when opposite strafing
float Character::airControl = 0.3f;  // How precise air control is
float Character::sideStrafeAcceleration = 50.0f;   // How fast acceleration occurs to get up to sideStrafeSpeed when side strafing
float Character::sideStrafeSpeed = 1.0f;    // What the max speed to generate when side strafing
float Character::jumpSpeed = 8.0f;  // The speed at which the character's up axis gains when hitting jump
float Character::moveScale = 1.0f;

Character::Character(string m, string t, string s, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path) : PropObject(m, t, s, position, direction, up, path)
{
	airborne = false;
	body->setFriction(0.5f);
	setMass(5.0f);
	setGravity(0, -9.81*4, 0);
	this->direction = glm::vec3(0,0,1);
	this->left = glm::vec3(1,0,0);
	this->up = glm::vec3(0,1,0);
	floorDist = 1.0f;
	
	groundNormal = btVector3(0, 1, 0);
	frameTurnAmount = glm::vec2(0, 0);
	directionAngle = glm::vec2(0, 0);
	localWalkDirection = glm::vec2(0, 0);

	airborneTest = createAirborneTest();
	vehicle = NULL;
	vehicleSpeed = btVector3(0,0,0);

	//weaponList.push_back(new FireballLauncher(this));
	//activeSpell = weaponList.back();
	activeSpell = NULL;
}
Character::~Character() 
{
	while (weaponList.size() > 0) {
		delete weaponList.front();
		weaponList.pop_front();
	}
	delete airborneTest;
}

btRigidBody * Character::createAirborneTest()
{
	btRigidBody * temp;
	btCollisionShape * colShape = PhysicsObject::getCollisionShape("airborneSphere");
	
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
	temp = new btRigidBody(mass,myMotionState,colShape,localInertia, this);
	delete myMotionState;
	return temp;
}

int Character::update(long elapsedTime)
{	
	body->activate(true);
	double speed = 20.0f;
	double e = elapsedTime*0.001;
	
	if (noBoatTimer <= 0) vehicle == NULL;
	else noBoatTimer -= elapsedTime;
	airborne = true;
	testGround();

	
	//setGravity(0,-9.81f*2.0f,0);
	if (airborne) {
		if (magSqr(walkDirection) != 0.0 && walkDirection != glm::vec3(0,0,0))
			airwalk(glm::normalize(walkDirection), elapsedTime);
	}
	else {
		if (walkDirection != glm::vec3(0,0,0))
			walk(glm::normalize(walkDirection) * (float)speed);
		if (walkDirection == glm::vec3(0,0,0)) {
			if ((vehicle == 0 && magSqr(&body->getLinearVelocity()) < 400.0f) ||
				(vehicle != 0 && magSqr(&(body->getLinearVelocity() - vehicle->getVelocity())) < 400.0f)) {
				stopWalking();
				//setGravity(0,0,0);
			}
		}
	}
	
	alive = true;
	walkDirection = glm::vec3(0,0,0);
	localWalkDirection = glm::vec2(0, 0);
		
	if (vehicle != NULL) {
		turn(-vehicle->getAngularVelocity().y()*elapsedTime*0.180f/M_PI, 0);
		btVector3 localPos = (getPosition() - vehicle->getPosition()).rotate(btVector3(0,1,0), vehicle->getAngularVelocity().y()*elapsedTime*0.00095);
		body->translate(localPos - (getPosition() - vehicle->getPosition()));
	}
	
	//for (list<Spell*>::iterator iter = weaponList.begin(); iter != weaponList.end(); iter++) 
	//	(*iter)->updateSpell(btToGLM3(&getPosition()), direction, up, elapsedTime);
	if (activeSpell != NULL)
		activeSpell->updateSpell(btToGLM3(&getPosition()) + glm::vec3(0,1,0), getDirection(), up, elapsedTime);

	int ret = RigidObject::update(elapsedTime);

	return ret;
}

int Character::render(long totalElapsedTime) 
{
	//for (list<Spell*>::iterator iter = weaponList.begin(); iter != weaponList.end(); iter++) 
	//	(*iter)->render(totalElapsedTime);
	if (activeSpell != NULL) 
		activeSpell->render(totalElapsedTime);
	return RigidObject::render(totalElapsedTime);
}

void Character::walkForward() 
{	
	walkDirection += direction;
	localWalkDirection.y += 1.0f;
}
void Character::walkBackward() 
{ 
	walkDirection -= direction;
	localWalkDirection.y -= 1.0f;
}
void Character::walkLeft() 
{ 
	walkDirection += left; 
	localWalkDirection.x += 1.0f;
}
void Character::walkRight() 
{ 
	walkDirection -= left; 
	localWalkDirection.x -= 1.0f;
}

void Character::turn(float x, float y) 
{
	frameTurnAmount = glm::vec2(x, y);
	/*direction = glm::rotate(direction, y, left);
	direction = glm::rotate(direction, x, -up);
	left = glm::normalize(glm::cross(up, direction));
	*/

	if (directionAngle.y + y < -89.0f)		directionAngle.y = -89.0f - y;
	else if (directionAngle.y + y > 89.0f)	directionAngle.y = 89.0f - y;
	directionAngle.y += y;

	directionAngle.x += x;
	if (directionAngle.x > 180.0f) directionAngle.x -= 360.0f;
	if (directionAngle.x < -180.0f) directionAngle.x += 360.0f;

	//direction = glm::rotate(direction, y, left);
	//direction = glm::rotate(direction, x, -up);

	direction = glm::rotate(glm::vec3(0, 0, 1), directionAngle.x, -up);
	left = glm::normalize(glm::cross(up, direction));
	direction = glm::rotate(direction, directionAngle.y, left);
}

void Character::turnLeft(float angle) { turn(-angle, 0.0f); }
void Character::turnRight(float angle) { turn(angle, 0.0f); }
void Character::turnUp(float angle) { turn(0.0f, -angle); }
void Character::turnDown(float angle) { turn(0.0f, angle); }

float changeSpeed(float input, float target, float dif) {
	if (target == input) return target;
	if (abs(target-input) < dif) return target;
	float x = (target-input)/abs(target-input)*dif;
	return input + x;
}

btVector3 changeVelocity(btVector3 * currentVel, btVector3 * targetVel, float dif) {
	btVector3 newVel;

	float currentSpeed = currentVel->length();
	float xFactor = abs((targetVel->x() - currentVel->x()) / currentSpeed);
	float yFactor = abs((targetVel->y() - currentVel->y()) / currentSpeed);
	float zFactor = abs((targetVel->z() - currentVel->z()) / currentSpeed);

	newVel.setX(changeSpeed(currentVel->x(), targetVel->x(), dif * xFactor));
	newVel.setY(changeSpeed(currentVel->y(), targetVel->y(), dif *yFactor));
	newVel.setZ(changeSpeed(currentVel->z(), targetVel->z(), dif *zFactor));

	return newVel;
}

void Character::walk(float x, float y, float z)
{
	if (magnitude(&body->getLinearVelocity()) <= maxSpeed)
	{
		btVector3 sideV = (btVector3(x,y,z).cross(-groundNormal)).normalized();
		btVector3 forwardV = (sideV.cross(-groundNormal)).normalized();

		btVector3 v = body->getLinearVelocity();
		btVector3 forward = -forwardV * magnitude(&btVector3(x,y,z));

		if (vehicle == 0)
			body->setLinearVelocity(changeVelocity(&v, &forward, 2.0f));
		else {
			body->setLinearVelocity(changeVelocity(&v, &(forward + vehicle->getVelocity()), 2.0f));
			//body->setLinearVelocity(forward + vehicle->getVelocity());
		}
	}
}

void Character::stopWalking() { 
	btVector3 v = body->getLinearVelocity();
	if (vehicle == 0)
		body->setLinearVelocity(changeVelocity(&v, &btVector3(0,0,0), 2.0f));
	else
		body->setLinearVelocity(changeVelocity(&v, &vehicle->getVelocity(), 10.0f));
	//	body->setLinearVelocity(vehicle->getVelocity());
}

void Character::airwalk(float x, float y, float z, long elapsedTime)
{
	//body->applyCentralImpulse(btVector3(x,y,z));
	//return;
	
	glm::vec3 wishdir;
	float wishvel = airAcceleration;
	float accel;

	//var scale = CmdScale();
	
	wishdir = glm::vec3(localWalkDirection.x, 0, localWalkDirection.y);
	wishdir = direction * localWalkDirection.y + left * localWalkDirection.x; // ???

	//float wishspeed = body->getLinearVelocity().length();// 20.0f; // wishdir.length();
	float wishspeed = 20.0f; // wishdir.length();

	wishdir = glm::normalize(wishdir);
	glm::vec3 wishDirNorm = wishdir;
	//wishspeed *= scale;

	// CPM: Aircontrol
	float wishspeed2 = wishspeed;
	glm::vec3 playerVelocity = btToGLM3(&getVelocity());
	if (glm::dot(playerVelocity, wishdir) < 0.0f)
		accel = airDeacceleration;
	else
		accel = airAcceleration;
	// If the player is ONLY strafing left or right
	if (localWalkDirection.y == 0.0f && localWalkDirection.x != 0.0f)
	{
		if (wishspeed > sideStrafeSpeed)
			wishspeed = sideStrafeSpeed;
		accel = sideStrafeAcceleration;
	}

	Accelerate(wishdir, wishspeed, accel, elapsedTime);
	
	if (airControl) AirControl(wishdir, wishspeed2, elapsedTime);
	// !CPM: Aircontrol

	// LEGACY MOVEMENT SEE BOTTOM
}

/**
* Calculates wish acceleration based on player's cmd wishes
*/
void Character::Accelerate(glm::vec3 wishdir, float wishspeed, float accel, long elapsedTime)
{
	float addspeed;
	float accelspeed;
	float currentspeed;

	glm::vec3 playerVelocity = btToGLM3(&body->getLinearVelocity());
	currentspeed = glm::dot(playerVelocity, wishdir);
	addspeed = wishspeed - currentspeed;
	if (addspeed <= 0)
		return;
	accelspeed = accel * wishspeed * elapsedTime * 0.001f;
	if (accelspeed > addspeed)
		accelspeed = addspeed;

	playerVelocity.x += accelspeed * wishdir.x;
	playerVelocity.z += accelspeed * wishdir.z;

	setVelocity(playerVelocity.x, playerVelocity.y, playerVelocity.z);
}

/**
* Air control occurs when the player is in the air, it allows
* players to move side to side much faster rather than being
* 'sluggish' when it comes to cornering.
*/
void Character::AirControl(glm::vec3 wishdir, float wishspeed, long elapsedTime)
{
	float zspeed;
	float speed;
	float dot;
	float k;
	float i;

	// Can't control movement if not moving forward or backward
	//if (cmd.forwardmove == 0 || wishspeed == 0)
	//if (localWalkDirection.y == 0.0f || wishspeed == 0.0f)
	glm::vec3 playerVelocity = btToGLM3(&body->getLinearVelocity());
	if ((playerVelocity.x == 0.0f && playerVelocity.z == 0.0f) || wishspeed == 0.0f)
		return;

	zspeed = playerVelocity.y;
	playerVelocity.y = 0.0f;
	// Next two lines are equivalent to idTech's VectorNormalize() /
	speed = glm::length(playerVelocity);// sqrt(playerVelocity.x*playerVelocity.x + playerVelocity.z*playerVelocity.z);
	playerVelocity = glm::normalize(playerVelocity);

	dot = glm::dot(playerVelocity, wishdir);
	k = 32.0f;
	k *= airControl * dot * dot * elapsedTime * 0.001f;

	// Change direction while slowing down
	if (dot > 0.0f)
	{
		playerVelocity.x = playerVelocity.x * speed + wishdir.x * k;
		playerVelocity.y = playerVelocity.y * speed + wishdir.y * k;
		playerVelocity.z = playerVelocity.z * speed + wishdir.z * k;

		playerVelocity = glm::normalize(playerVelocity);
		//moveDirectionNorm = playerVelocity;
	}

	playerVelocity.x *= speed;
	playerVelocity.y = zspeed; // Note this line
	playerVelocity.z *= speed;
	
	body->setLinearVelocity(glmToBT3(playerVelocity));
}

void Character::jump()
{
	if (airborne) return;
	body->setLinearVelocity(body->getLinearVelocity() + btVector3(0,15.0f,0));
	body->translate(btVector3(0,0.2f,0));
}

void Character::testGround()
{
	string foo = "asdf";
	airborneTest->setWorldTransform(body->getWorldTransform());
	AirborneContactCallback callback(*airborneTest, foo, &airborne, &groundNormal, &vehicle);
	dynamicsWorld->updateAabbs();
	dynamicsWorld->contactTest(airborneTest,callback);

	if (vehicle != NULL)
		noBoatTimer = 500; //in ms
	//groundNormal = btVector3(0,1,0);
}

void Character::setDirection(glm::vec3 direction) {
	this->direction = glm::normalize(direction);
	up = glm::vec3(0,1,0);
	left = glm::normalize(glm::cross(up, direction));
}

void Character::setDirection(glm::vec3 direction, glm::vec3 left, glm::vec3 up) {
	this->direction = glm::normalize(direction);
	this->left = glm::normalize(left);
	this->up = glm::normalize(up);
}