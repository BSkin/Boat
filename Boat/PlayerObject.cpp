#include "PlayerObject.h"

InputManager * PlayerObject::inputManager = NULL;
Camera * PlayerObject::camera = NULL;

PlayerObject::PlayerObject(string m, string t, string s, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path) : Character(m, t, s, position, direction, up, path)
{
	body->setAngularFactor(0);
}
PlayerObject::~PlayerObject() {}

int PlayerObject::update(long elapsedTime)
{
	if (isKeyDown(IM_W)) walkForward();
	if (isKeyDown(IM_S)) walkBackward();
	if (isKeyDown(IM_A)) walkLeft();
	if (isKeyDown(IM_D)) walkRight();
	//if (isKeyPressed(IM_SPACE)) jump();
	if (isKeyDown(IM_SPACE)) jump();

	if (isKeyPressed(IM_M1))	primaryClick();
	if (isKeyReleased(IM_M1))	primaryRelease();
	if (isKeyDown(IM_M1))		primaryDown();
	if (isKeyUp(IM_M1))			primaryUp();

	if (isKeyPressed(IM_M2))	secondaryClick();
	if (isKeyReleased(IM_M2))	secondaryRelease();
	if (isKeyDown(IM_M2))		secondaryDown();
	if (isKeyUp(IM_M2))			secondaryUp();

	if (isMouseScrollUp())
	{
		PropObject * temp = new PropObject("Models\\sword.obj", "Textures\\error.tga", "Shaders\\default.glsl",
			camera->getPos()+camera->getLookAtVector(), camera->getLookAtVector(), camera->getUpVector(), "sword");
		//RagdollObject * temp = new RagdollObject(box, t, s, camera->getPos(), camera->getLookAtVector(), camera->getUpVector());
		temp->setVelocity(camera->getLookAtVector()*10.0f);
		temp->setRotation(camera->getLookAtVector(), camera->getUpVector());
	}
	if (isMouseScrollDown())
	{
		PropObject * temp = new PropObject("Models\\box.obj", "Textures\\error.tga", "Shaders\\default.glsl",
			camera->getPos()+camera->getLookAtVector(), camera->getLookAtVector(), camera->getUpVector(), "box1x1");
		//RagdollObject * temp = new RagdollObject(box, t, s, camera->getPos(), camera->getLookAtVector(), camera->getUpVector());
		temp->setVelocity(camera->getLookAtVector()*10.0f);
		temp->setRotation(camera->getLookAtVector(), camera->getUpVector());
	}

	POINT p = inputManager->getMouseMovement();
	turn(p.x*0.05f, p.y*0.05f);

	int ret = Character::update(elapsedTime);

	//setDirection(camera->getLookAtVector());
	camera->setPosition(btToGLM3(&getPosition()) + glm::vec3(0,1,0));
	camera->setLookAtVector(getDirection());
	//camera->update(elapsedTime);

	return ret;
}

int PlayerObject::render(long totalElapsedTime)
{
	return Character::render(totalElapsedTime);
}