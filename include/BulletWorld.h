#ifndef BULLET_WORLD_H
#define BULLET_WORLD_H

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

int getTwo();

class BulletBody;

class BulletWorld
{
public:
	//Constructor taking value of gravity, acting in +Y direction
	BulletWorld(float gravityStrength);
	~BulletWorld();

	//Functions to change the world
	void stepWorld(float deltaTime);
	void addPhysicsObject(BulletBody* object);
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif
