#include "include/BulletBody.h"
#include "include/BulletWorld.h"

void BulletBody::Init(btCollisionShape* cs, btDefaultMotionState* ms, btRigidBody* rb, BulletWorld* world)
{
	collisionShape = cs;
	motionState = ms;
	rigidBody = rb;

	//Store the intitial transform so we can reset the object later if need be
	rigidBody->getMotionState()->getWorldTransform(initialTransform);
}

BulletBody::~BulletBody()
{
	delete rigidBody;
	delete motionState;
	delete collisionShape;
}

glm::vec3 BulletBody::getPosition() const
{
	btTransform transform;
	this->rigidBody->getMotionState()->getWorldTransform(transform);
	btVector3 origin = transform.getOrigin();
	glm::vec3 position(origin.x(), origin.y(), origin.z());
	return position;
}

glm::quat BulletBody::getRotationQuaternion() const
{
	btTransform transform;
	this->rigidBody->getMotionState()->getWorldTransform(transform);
	btQuaternion rotation = transform.getRotation();
	glm::quat rotationQuat = glm::angleAxis(rotation.getAngle(), glm::vec3(rotation.getAxis().getX(), rotation.getAxis().getY(), rotation.getAxis().getZ()));
	return rotationQuat;
}

btRigidBody* BulletBody::getRigidBody() const
{
	return rigidBody;
}
