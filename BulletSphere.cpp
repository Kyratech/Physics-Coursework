#include "include/BulletSphere.h"
#include "include/BulletWorld.h"
#include "include/World.h"

/*
 * Creates a sphere centred on the initial position
 * Rotation irrelevent
*/
BulletSphere::BulletSphere(float radius, float mass, glm::vec3 initialPosition, glm::vec3 initialVelocity, BulletWorld* world)
{
	btCollisionShape* cShape = new btSphereShape(radius);

	btDefaultMotionState* mState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(initialPosition.x, initialPosition.y, initialPosition.z)));

    btVector3 fallInertia(0, 0, 0);
    cShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, mState, cShape, fallInertia);
    btRigidBody* rBody = new btRigidBody(fallRigidBodyCI);

    //Set the starting velocity
    rBody->setLinearVelocity(btVector3(initialVelocity.x, initialVelocity.y, initialVelocity.z));
    //Set the restitution
    rBody->setRestitution(COE);

	//IMPORTANT: Store the physics data structures in the superclass fields!
	Init(cShape, mState, rBody, world);
	//Adding the object to a world will allow it to be simulated when stepping the world
	world->addPhysicsObject(this);
}
