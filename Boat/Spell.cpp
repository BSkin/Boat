#include "Spell.h"

Spell::Spell(string mPath, string tPath, string sPath, void * owner) : RenderObject(mPath, tPath, sPath)//ok for now
{
	init();
	this->owner = owner;
}

Spell::~Spell()
{

}

void Spell::init()
{
	weaponID = -1;
	ammunition = 1;
}

int Spell::updateSpell(glm::vec3 position, glm::vec3 direction, glm::vec3 up, long elapsedTime)
{
	this->position = position;
	this->direction = direction;
	this->up = up;
	RenderObject::updateMatrices();
	return RenderObject::update(elapsedTime);
}

int Spell::render(long totalElapsedTime)
{
	return RenderObject::render(totalElapsedTime);
}