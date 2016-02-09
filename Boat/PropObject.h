#ifndef PROP_OBJECT_H
#define PROP_OBJECT_H

#include "RigidObject.h"

class PropObject : public RigidObject
{
public:
	PropObject(string m, string t, string s, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path, int colGroup = COL_DEFAULT, int colMask = COL_DEFAULT); //ok for now
	virtual ~PropObject();
protected:
	int initBullet(string path,int colGroup, int colMask);
};

#endif