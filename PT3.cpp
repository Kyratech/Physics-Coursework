/*
 * Program to demonstrate elastic collisions in a 3d cube
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "include/World.h"

FILE *flog;

#include "include/CubeGeometry.h"
#include "include/UVSphereGeometry.h"

#include "include/TriangleMesh.h"
#include "include/PhysicsObject.h"


//Want to get rid of these eventually
#include "include/VertexBufferObject.h"
#include "include/glslprogram.h"
#include "include/stb_image.h"
#include "include/utils.h"

/*
 * Set up bullet - globals.
 */
#include <btBulletDynamicsCommon.h>

btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

std::vector<btRigidBody*> MovingBits; // so that can get at all bits
std::vector<btRigidBody*> StaticBits; // especially during clean up.

/* Screen parameters */
const int width = 800;
const int height = 800;

/* Functions to handle input */
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_movement(GLFWwindow *window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

/* Stuff to read the mouse input to move the camera */
GLfloat lastX = width / 2.0;
GLfloat lastY = height / 2.0;

bool firstMouseInput = true;

//Mouse button flags
bool middleMouse = false;

//Key pressed flags
bool keys[1024];

//For calculating delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool stillRunning = true;


/*
 * Bullet Code
 */
btRigidBody* SetSphere(float size, btTransform T)
{
    btCollisionShape* fallshape = new btSphereShape(size);
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(T);
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallshape ->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallshape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    fallRigidBody->setLinearVelocity(btVector3(-5, 20, 0));
    fallRigidBody->setRestitution(COE);
    dynamicsWorld->addRigidBody(fallRigidBody);
    return fallRigidBody;
}

void bullet_init()
{
    /*
     * set up world
     */
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0., GRAVITY, 0));
    /*
     * Set up ground
     */
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    groundRigidBody->setRestitution(COE);
    dynamicsWorld->addRigidBody(groundRigidBody);
    /*
     * Set up left
     */
    btCollisionShape* leftShape = new btStaticPlaneShape(btVector3(1,0,0),1);
    btDefaultMotionState* leftMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-50, 0,0)));
    btRigidBody::btRigidBodyConstructionInfo leftRigidBodyCI(0,leftMotionState,leftShape,btVector3(0,0,0));
    btRigidBody* leftRigidBody = new btRigidBody(leftRigidBodyCI);
    leftRigidBody->setRestitution(COE);
    dynamicsWorld->addRigidBody(leftRigidBody);
    /*
     * Set up right
     */
    btCollisionShape* rightShape = new btStaticPlaneShape(btVector3(-1,0,0),1);
    btDefaultMotionState* rightMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(50, 0,0)));
    btRigidBody::btRigidBodyConstructionInfo rightRigidBodyCI(0,rightMotionState,rightShape,btVector3(0,0,0));
    btRigidBody* rightRigidBody = new btRigidBody(rightRigidBodyCI);
    rightRigidBody->setRestitution(COE);
    dynamicsWorld->addRigidBody(rightRigidBody);
    /*
     * Set up top
     */
    btCollisionShape* topShape = new btStaticPlaneShape(btVector3(0,-1,0),1);
    btDefaultMotionState* topMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0, 100,0)));
    btRigidBody::btRigidBodyConstructionInfo topRigidBodyCI(0,topMotionState,topShape,btVector3(0,0,0));
    btRigidBody* topRigidBody = new btRigidBody(topRigidBodyCI);
    topRigidBody->setRestitution(COE);
    dynamicsWorld->addRigidBody(topRigidBody);
    /*
     * Set up sphere 0
     */
    MovingBits.push_back(SetSphere(5., btTransform(btQuaternion(0,0,1,1),btVector3(-10,25,0))));
    /*
     * Set up sphere 1
     */
    MovingBits.push_back(SetSphere(5., btTransform(btQuaternion(0,0,0,1),btVector3(-10,25,0))));
    /*
     * Set up sphere 2
     */
    MovingBits.push_back(SetSphere(5., btTransform(btQuaternion(0,0,0,1),btVector3(-10,25,0))));

    Print("Setup Bullet ");
    int n = MovingBits.size();
    print(n);
}

glm::vec3 bullet_step(int i)
{
    btTransform trans;
    btRigidBody* moveRigidBody;
    int n = MovingBits.size();
    moveRigidBody = MovingBits[i];
    dynamicsWorld->stepSimulation(1/60.f,10);
    moveRigidBody->getMotionState()->getWorldTransform(trans);
    return glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

void bullet_close()
{
    /*
     * This is very minimal and relies on OS to tidy up.
     */
    btRigidBody* moveRigidBody;
    moveRigidBody = MovingBits[0];
    dynamicsWorld->removeRigidBody(moveRigidBody);
    delete moveRigidBody->getMotionState();
    delete moveRigidBody;
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}
/*
void Render(int i, GLSLProgram O, VertexBufferObject vb)
{
    glm::mat4 Projection = glm::mat4(1.0f);
    Projection = glm::ortho(-55., 55., -5., 105., -100., 100.);
    glm::mat4 View = glm::mat4(1.);
    glm::mat4 Model = glm::mat4(1.);
    O.Use();
    vb.SelectVAO();
    O.SetUniform("uProjection", Projection);
    O.SetUniform("uView", View);
    for(int i = 0 ; i < MovingBits.size(); i++) { // loop over shapes
        glm::vec3 position = bullet_step(i);
        fprintf(flog, "%f %f %f ", position[0], position[1], position[2]);
        Model = glm::translate(position);
        O.SetUniform("uModel", Model);
        vb.Draw();
        }
    fprintf(flog, "\n");
    vb.DeSelectVAO();
}
*/
VertexBufferObject makeWireBoxMesh(void) {
    VertexBufferObject Box;
    Box.vboName = "Box";
    Box.SetVerbose(true);
    Box.CollapseCommonVertices( false );
    Box.SetTol( .001f );// how close need to be to collapse vertices, ignored at the moment.
    Box.UseBufferObjects(true); // Not needed as this is the only option.
    Box.glBegin( GL_LINES );

    Box.glVertex3f(0., 0., 0.);
    Box.glVertex3f(-50., 0., 0.);

    Box.glVertex3f(-50., 0., 0.);
    Box.glVertex3f(-50., 100., 0.);

    Box.glVertex3f(-50., 100., 0.);
    Box.glVertex3f(50., 100., 0.);

    Box.glVertex3f(50., 100., 0.);
    Box.glVertex3f(50., 0., 0.);

    Box.glVertex3f(50., 0., 0.);
    Box.glVertex3f(0., 0., 0.);
    Box.glEnd();
    Box.Print();
    return Box;
    }

VertexBufferObject makeWireCircleMesh(float radius) {
    VertexBufferObject Circle;
    Circle.vboName = "Circle";
    Circle.SetVerbose(true);
    Circle.CollapseCommonVertices( false );
    Circle.SetTol( .001f );// how close need to be to collapse vertices, ignored at the moment.
    Circle.UseBufferObjects(true); // Not needed as this is the only option.
    float theta;
    const float Radius = radius;
    Circle.glBegin( GL_LINES );
    glm::vec3 p, n;
    for(theta = 0.0f; theta < PI2; theta += lod) {
        Circle.glVertex3f(Radius * cos(theta), Radius * sin(theta), 0.);
        Circle.glVertex3f(Radius * cos(theta + lod), Radius * sin(theta + lod), 0.);
        }
    Circle.glEnd();
    Circle.Print();
    Circle.makeObj("Circle.obj");
    return Circle;
    }

int main( void )
{
    /* Attempt to initialise GLFW3, the window manager */
	if(!glfwInit())
	{
		std::cout << "Failed to start GLFW3" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Set up the main window */
	GLFWwindow* window = glfwCreateWindow(width, height, "Coursework 1", NULL, NULL);
	if(!window)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* Set the required callback functions */
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_movement);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	/* Set up GLEW before using any OpenGL functions */
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW." << std::endl;
		return -1;
	}

	/* Tell OpenGL the size of the rendering window */
	glViewport(0, 0, width, height);

	/* Turn on depth testing to make stuff in front actually look like it's in front. */
	glEnable(GL_DEPTH_TEST);

    /*Draw wireframes */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* Load the shader program */
    Shader unshadedShader("shader/UnshadedDefault.vert", "shader/UnshadedDefault.frag");

    /* Some colours to use later */
    GLfloat red[3] = {1.0f, 0.0f, 0.0f};
    GLfloat yellow[3] = {1.0f, 1.0f, 0.0f};
    GLfloat green[3] = {0.0f, 0.8f, 0.0f};
    GLfloat cyan[3] = {0.0f, 0.5f, 1.0f};
    GLfloat white[3] = {1.0f, 1.0f, 1.0f};

    /* Create some physics objects to add to the simulation */

    /* Create a sphere object*/
	int segments = 10;
	int rings = 10;
	double radius = 1.0;
    TriangleMesh sphereMesh(GetSpherePhong(segments, rings, radius), white);
    PhysicsObject sphereObject(&sphereMesh, glm::vec3(0.0f), glm::quat());

    /* Main loop */
	while(!glfwWindowShouldClose(window) && stillRunning)
	{
	    //Calculate the time since the last frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		/* Rendering commands */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Generate the view matrix */
		glm::mat4 view;
		view = camera.GetViewMatrix();
		/* Generate the projection matrix */
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)width / (GLfloat)width, 0.1f, 100.0f);

        unshadedShader.Use();
        sphereObject.Draw(unshadedShader, view, projection);

		glfwSwapBuffers(window);
	}

	/* Terminate properly */
	glfwTerminate();
	return 0;
}

/*
* Record the states of keys when one changes
*/
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Check to see if a new key has been pressed or released
	if (action == GLFW_PRESS)
	{
		keys[key] = true;

        if(keys[GLFW_KEY_Q] || keys[GLFW_KEY_ESCAPE])
            stillRunning = false; //Set the flag to close next frame
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.scroll_input(yoffset);
}

/*
* Record the changes in position of the mouse, use it to update the camera
*/
void mouse_movement(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouseInput)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouseInput = false;
	}

	//Difference between current moose position and previous
	GLfloat deltaX = xPos - lastX;
	GLfloat deltaY = lastY - yPos;

	//Update previous
	lastX = xPos;
	lastY = yPos;


	if (middleMouse)
	{
		if (keys[GLFW_KEY_LEFT_SHIFT])
			camera.pan_camera(deltaX, deltaY);
		else
			camera.move_camera(deltaX, deltaY);
	}
}

/*
* Record the state of the middle mouse when it changes
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		middleMouse = true;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		middleMouse = false;
}



