#include "FireballLauncher.h"
#include "Fireball.h"

FireballLauncher::FireballLauncher(void * owner) : Spell("Models\\fireballlauncher.obj", "Textures\\FireballLauncher.tga", "Shaders\\default.glsl", owner)
{
	weaponID = FIREBALL;
}

FireballLauncher::~FireballLauncher()
{

}

void FireballLauncher::primaryClick()
{
	Fireball * temp = new Fireball("Models\\fireballsmall.obj", "Textures\\error.tga", "Shaders\\default.glsl", 
		position+direction*0.5f, direction, up, "sphere1", owner);
	temp->setVelocity(direction*30.0f);
	temp->setGravity(0,0,0);
	temp->setRotation(direction, up);
} 