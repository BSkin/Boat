#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include "RigidObject.h"

class MeshObject : public RigidObject
{
public:
	MeshObject(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up, int colGroup = COL_DEFAULT, int colMask = COL_DEFAULT); //ok for now
	virtual ~MeshObject();
protected:
	int initBullet(int colGroup, int colMask);
	btTriangleMesh * mTriMesh;// = new btTriangleMesh();
};

#endif