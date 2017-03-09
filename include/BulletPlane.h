#ifndef BULLET_PLANE_BODY_H
#define BULLET_PLANE_BODY_H

#include "BulletBody.h"
#include <string>
#include <vector>

class BulletPlane :public BulletBody
{
public:
	BulletPlane(glm::vec3 normal, float normalDistance, BulletWorld* world);
};

#endif

