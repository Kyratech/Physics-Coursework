#ifndef BULLET_CUBE_BODY_H
#define BULLET_CUBE_BODY_H

#include "BulletBody.h"
#include <string>
#include <vector>

class BulletCube :public BulletBody
{
public:
	BulletCube(float edgeLength, float mass, glm::vec3 initialPosition, glm::vec3 initialVelocity, BulletWorld* world);
};

#endif


