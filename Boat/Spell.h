#ifndef SPELL_H
#define SPELL_H

#include "RenderObject.h"

#define SHOTGUN			0
#define FIREBALL			1
#define WARP_GRENADE	2


class Spell : public RenderObject
{
public:
	Spell(void * owner);
	Spell(string, string, string, void * owner); //ok for now
	//string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up, GameObject * owner); //ok for now
	virtual ~Spell();

	virtual void primaryClick() {}
	virtual void primaryDown() {}
	virtual void primaryUp() {}
	virtual void primaryRelease() {}

	virtual void secondaryClick() {}
	virtual void secondaryDown() {}
	virtual void secondaryUp() {}
	virtual void secondaryRelease() {}

	virtual int updateSpell(glm::vec3 position, glm::vec3 direction, glm::vec3 up, long elapsedTime);
	virtual int render(long totalElapsedTime);
protected:
	void * owner;
	int ammunition;
	short weaponID;
private:
	void init();
};

#endif SPELL_H