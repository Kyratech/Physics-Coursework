#ifndef PLANE_H
#define PLANE_H

#include "PhysicsDemo.h"

const std::vector<struct Vertex> GetPlaneGeometry(float width, float height)
{
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    const std::vector<struct Vertex> vertices = {
		{{-halfWidth, -halfHeight, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom left
		{{halfWidth, -halfHeight, 0.0f},   {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // Bottom Right
		{{-halfWidth, halfHeight, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // Top Left
		{{halfWidth, halfHeight, 0.0f},    {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},	// Top right
		{{-halfWidth, halfHeight, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // Top Left
		{{halfWidth, -halfHeight, 0.0f},   {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}  // Bottom Right
	};

	return vertices;
}

#endif // PLANE_H
