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
    glm::vec3 worldPosition;
    glm::quat rotation;

    PhysicsObject(Mesh* myMesh, glm::vec3 initialPosition, glm::quat initialRotation)
    {
        mesh = myMesh;
        worldPosition = initialPosition;
        rotation = initialRotation;
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
    {
        glm::mat4 model;
        model = glm::translate(model, this->worldPosition);
        model = glm::rotate(model, glm::angle(rotation), glm::axis(rotation));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        mesh->Draw(shader);
    }

    void setPostion(glm::vec3 newPos)
    {
        worldPosition = newPos;
    }

    void setRotation(glm::quat newRot)
    {
        rotation = newRot;
    }
};

#endif // PHYSICS_OBJECT_H
