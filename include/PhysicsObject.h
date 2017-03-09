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
    PhysicsObject(Mesh* myMesh, BulletBody* physicsBody)
    {
        mesh = myMesh;
        meshOffset = glm::vec3(0.0f);
        meshRotation = glm::vec3(0.0f);
        body = physicsBody;
    }

    PhysicsObject(Mesh* myMesh, glm::vec3 offset, glm::vec3 rotation, BulletBody* physicsBody)
    {
        mesh = myMesh;
        meshOffset = offset;
        meshRotation = rotation;
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

        //Offset
        model = glm::translate(model, meshOffset);
        //Pitch, yaw, roll
        model = glm::rotate(model, meshRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, meshRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, meshRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        mesh->Draw(shader);
    }

private:
    Mesh* mesh;
    BulletBody* body;
    glm::vec3 meshOffset;
    glm::vec3 meshRotation;
};

#endif // PHYSICS_OBJECT_H
