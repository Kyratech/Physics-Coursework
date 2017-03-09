#include "include/BulletWorld.h"
#include "include/BulletBody.h"
#include <iostream>

int getTwo()
{
    return 2;
}

/*
 * Uses the assumption that gravity is going to work in the +Y direction, as other constructs treat +Y as 'up'
*/
BulletWorld::BulletWorld(float gravityStrength)
{
	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	//Round G to 10 m.s^(-2)
	dynamicsWorld->setGravity(btVector3(0, gravityStrength, 0));
}

BulletWorld::~BulletWorld()
{
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}

void BulletWorld::stepWorld(float deltaTime)
{
	//Max sub-steps of 10 allows simulation to run as slow as 6fps before losing time
	//Could possibly make this dynamic, but fixed value is sufficient for casual use
	dynamicsWorld->stepSimulation(deltaTime, 10);
}

void BulletWorld::addPhysicsObject(BulletBody* object)
{
	dynamicsWorld->addRigidBody(object->getRigidBody());
}
