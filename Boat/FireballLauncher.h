#ifndef FIREBALL_LAUNCHER_H
#define FIREBALL_LAUNCHER_H

#include "Spell.h"

class FireballLauncher : public Spell
{
public:
	FireballLauncher(void * owner);
	~FireballLauncher();

	virtual void primaryClick();
private:

};

#endif