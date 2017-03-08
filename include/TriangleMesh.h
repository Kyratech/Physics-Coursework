#ifndef TRIMESH_H
#define TRIMESH_H

#include "Mesh.h"

class TriangleMesh: public Mesh
{
public:
    /* Constructor */
    TriangleMesh(const std::vector<struct Vertex> vertices, GLfloat colour[3])
    {
        vertexCount = vertices.size();
        r = colour[0];
        g = colour[1];
        b = colour[2];

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        //Set up the vertex buffers
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(struct Vertex), &vertices[0], GL_STATIC_DRAW);

        //Set the vertex attrib pointers
        //Vertex positions
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof (struct Vertex, position));
        glEnableVertexAttribArray(0);
        //Vertex texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof (struct Vertex, textureCoords));
        glEnableVertexAttribArray(1);
        //Normal positions
        glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof (struct Vertex, normal));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    /* Draw the mesh with the supplied texture */
    void Draw(Shader shader)
    {
        GLint colourLocation = glGetUniformLocation(shader.getShaderProgram(), "baseColour");
        glUniform4f(colourLocation, r, g, b, 1.0f);

        //Lighting colour
        GLint lightColourLocation = glGetUniformLocation(shader.getShaderProgram(), "lightColour");
        glUniform4f(lightColourLocation, LIGHT_COLOUR.x, LIGHT_COLOUR.y, LIGHT_COLOUR.z, 1.0f);
        //Lighting position
        GLint lightPositionLocation = glGetUniformLocation(shader.getShaderProgram(), "lightPos");
        glUniform3f(lightPositionLocation, LIGHT_POS.x, LIGHT_POS.y, LIGHT_POS.z);

        GLint viewPosLocation = glGetUniformLocation(shader.getShaderProgram(), "viewPos");
        glUniform3f(viewPosLocation, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);
    }

private:
    GLuint VAO, VBO;
    int vertexCount;
    GLfloat r,g,b;
    glm::vec3 fragmentColour;
};

#endif // MESH_H

