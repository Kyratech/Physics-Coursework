#include "include/BulletPlane.h"
#include "include/BulletWorld.h"

BulletPlane::BulletPlane(glm::vec3 normal, float normalDistance, BulletWorld* world)
{
	btCollisionShape* cShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), normalDistance);

	btDefaultMotionState* mState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	//This is the construction info for a non-dynamic object: moves under NO circumstance
	btRigidBody::btRigidBodyConstructionInfo cInfo(0, mState, cShape, btVector3(0, 0, 0));

	btRigidBody* rBody = new btRigidBody(cInfo);

	//IMPORTANT: Store the physics data structures in the superclass fields!
	Init(cShape, mState, rBody, world);
	//Adding the object to a world will allow it to be simulated when stepping the world
	world->addPhysicsObject(this);
}

