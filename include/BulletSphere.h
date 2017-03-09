#ifndef BULLET_SPHERE_BODY_H
#define BULLET_SPHERE_BODY_H

#include "BulletBody.h"
#include <string>
#include <vector>

class BulletSphere :public BulletBody
{
public:
	BulletSphere(float radius, float mass, glm::vec3 initialPosition, glm::vec3 initialVelocity, BulletWorld* world);
};

#endif
