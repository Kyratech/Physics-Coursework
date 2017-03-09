#ifndef BULLET_BODY_H
#define BULLET_BODY_H

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class BulletWorld;

class BulletBody
{
private:
	btCollisionShape* collisionShape;
	btDefaultMotionState* motionState;
	btRigidBody* rigidBody;

	btTransform initialTransform;
protected:
	BulletBody() {};
public:
	void Init(btCollisionShape* cs, btDefaultMotionState* ms, btRigidBody* rb, BulletWorld* world);
	~BulletBody();

	glm::vec3 getPosition() const;
	glm::quat getRotationQuaternion() const;
	btRigidBody* getRigidBody() const;
};

#endif
