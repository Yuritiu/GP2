#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"

struct Vertex
{
public:
	glm::vec3 pos; // 3D position
	glm::vec2 texCoord; // texture UV
	glm::vec3 normal; // vertex normal
	glm::vec3 tangent; // tangent vector (for bump mapping)
	glm::vec3 bitangent; // bitangent (for bump mapping)

	Vertex(const glm::vec3& p = glm::vec3(0.0f),
		const glm::vec2& tc = glm::vec2(0.0f))
		: pos(p)
		, texCoord(tc)
		, normal(0.0f, 0.0f, 0.0f)
		, tangent(0.0f, 0.0f, 0.0f)
		, bitangent(0.0f, 0.0f, 0.0f)
	{
	}
};

struct Sphere
{
public:

	Sphere() {}

	Sphere(glm::vec3& pos, float radius)
	{
		this->pos = pos;
	}

	glm::vec3 GetPos() { return pos; }
	float GetRadius() { return radius; }

	void SetPos(glm::vec3 pos)
	{
		this->pos = pos;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

private:
	glm::vec3 pos;
	float radius;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void drawVertexes();
	void loadVertexs(Vertex* vertices, unsigned int numVertices);
	void draw();
	void init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void loadModel(const std::string& filename);
	void initModel(const IndexedModel& model);
	void updateSphereData(glm::vec3 pos, float radius);
	glm::vec3 getSpherePos() { return meshSphere.GetPos(); }
	float getSphereRadius() { return meshSphere.GetRadius(); }

private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	Sphere meshSphere;
	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw
};