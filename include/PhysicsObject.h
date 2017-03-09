#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "PhysicsDemo.h"

/*
 * Physics Object encapsulates a physics body and a mesh
 * It draws the mesh at the position and orientation of the body
 */
class PhysicsObject
{
public:
    Mesh* mesh;
    BulletBody* body;

    PhysicsObject(Mesh* myMesh, BulletBody* physicsBody)
    {
        mesh = myMesh;
        body = physicsBody;
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
    {
        //Get the data out of the bullet rigidbody
        glm::vec3 bodyPosition = this->body->getPosition();
        glm::quat bodyRotation = this->body->getRotationQuaternion();

        glm::mat4 model;
        model = glm::translate(model, bodyPosition);
        model = glm::rotate(model, glm::angle(bodyRotation), glm::axis(bodyRotation));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        mesh->Draw(shader);
    }
};

#endif // PHYSICS_OBJECT_H
