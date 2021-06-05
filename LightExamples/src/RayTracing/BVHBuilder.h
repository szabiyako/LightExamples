#pragma once
#include <vector>
#include "glm/glm.hpp"
#include <functional>

namespace BVH {
struct AABB
{
private:
	glm::vec3 min;
	glm::vec3 max;

public:
	AABB() :
		min({ 0, 0, 0 }),
		max({ 0, 0, 0 }) {}

	AABB(glm::vec3 min, glm::vec3 max) :
		min(min),
		max(max) {}


	void surrounding(AABB const& aabb)
	{
		min = glm::min(min, aabb.min);
		max = glm::max(max, aabb.max);
	}

	bool rayIntersect(glm::vec3& origin, glm::vec3& direction, float& mint)
	{
		if (glm::all(glm::greaterThan(origin, this->min)) && glm::all(glm::lessThan(origin, this->max)))
			return true;

		glm::vec3 t0 = (this->min - origin) / direction;
		glm::vec3 t1 = (this->max - origin) / direction;
		glm::vec3 tmin = glm::min(t0, t1), tmax = glm::max(t0, t1);
		float tminf = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
		float tmaxf = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

		if (tminf > tmaxf)
			return false;


		return tminf > 0.0f;
	}

	glm::vec3& getMin() { return min; }
	glm::vec3& getMax() { return max; }
};

struct Node
{
	/*bool leftChildIsTriangle;
	bool rightChildIsTriangle;*/
	float childIsTriangle;
	float leftChild;
	float rightChild;
	AABB aabb;

	Node() : /*leftChildIsTriangle(false), rightChildIsTriangle(false)*/ childIsTriangle(0), leftChild(-1), rightChild(-1) {}
};

struct Triangle
{
private:
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 vertex3;
	int index;
	glm::vec3 center;
	AABB aabb;

public:
	Triangle(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3, int index) :
		vertex1(vertex1),
		vertex2(vertex2),
		vertex3(vertex3),
		index(index)
	{
		aabb = genAABB();
		center = genCenter();
	}

	Triangle() :Triangle(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), -1) {};

	bool rayIntersect(glm::vec3& origin, glm::vec3& direction, glm::vec3& normal, float& mint)
	{
		glm::vec3 e1 = vertex2 - vertex1;
		glm::vec3 e2 = vertex3 - vertex1;
		glm::vec3 P = glm::cross(direction, e2);
		float det = glm::dot(e1, P);

		if (abs(det) < 1e-4)
			return false;

		float inv_det = 1.0 / det;
		glm::vec3 T = origin - vertex1;
		float u = glm::dot(T, P) * inv_det;

		if (u < 0.0 || u > 1.0)
			return false;

		glm::vec3 Q = glm::cross(T, e1);
		float v = glm::dot(direction, Q) * inv_det;

		if (v < 0.0 || (v + u) > 1.0)
			return false;

		float tt = glm::dot(e2, Q) * inv_det;

		if (tt <= 0.0 || tt > mint)
			return false;

		normal = glm::normalize(glm::cross(e1, e2));
		mint = tt;
		return true;
	}

	glm::vec3& getCenter() { return center; }
	AABB& getAABB() { return aabb; }
	int getIndex() { return index; }

	glm::vec3 getCenter() const { return center; }
	AABB getAABB() const { return aabb; }
	int getIndex() const { return index; }

private:
	glm::vec3 genCenter()
	{
		glm::vec3 sum = vertex1 + vertex2 + vertex3;
		glm::vec3 centerDim = sum / 3.0f;
		return centerDim;
	}

	AABB genAABB()
	{
		return AABB(
			glm::min(glm::min(vertex1, vertex2), vertex3),
			glm::max(glm::max(vertex1, vertex2), vertex3));
	}
};

class BVHBuilder
{
public:
	BVHBuilder();
	void build(std::vector<float> const& vertexRaw);
	void travel(glm::vec3& origin, glm::vec3& direction, glm::vec3& color, float& minT);
	void travelCycle(glm::vec3& origin, glm::vec3& direction, glm::vec3& color, float& minT);
	Node * const bvhToTexture();
	int getNodesSize();
	std::vector<Node> getNodes();
private:
	void buildRecurcive(int nodeIndex, std::vector<Triangle>const& vecTriangle);
	bool travelRecurcive(Node& node, glm::vec3& origin, glm::vec3& direction, glm::vec3& color, float& minT);
	bool travelStack(Node& node, glm::vec3& origin, glm::vec3& direction, glm::vec3& color, float& minT);
	int  texSize;
	std::vector<Node> nodeList;
	std::vector<Triangle> vecTriangle;
};
}
