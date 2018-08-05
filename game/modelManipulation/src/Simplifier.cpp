#include <algorithm>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/mat4x4.hpp>

#include "Simplifier.h"

struct ContractionPair
{
	float cost;
	Index i0, i1;
	glm::vec3 recommendedPos;
	bool deleted;

	ContractionPair(float cost, const Index& i0, const Index& i1, const glm::vec3& recommendedPos)
		: cost(cost), i0(i0), i1(i1), recommendedPos(recommendedPos), deleted(false)
	{}

	bool operator<(const ContractionPair& other) const { return cost < other.cost; }
};

float det(float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33)
{
	return a11*a22*a33 + a13*a21*a32 + a12*a23*a31
		- a13*a22*a31 - a11*a23*a32- a12*a21*a33;
}

float id(const glm::mat4& q, size_t i)
{
	if (i <= 3)
		return q[i][0];
	if (i <= 6)
		return q[i - 3][1];
	else if (i <= 8)
		return q[i - 6 + 1][2];
	else
		return q[3][3];
}

float detid(const glm::mat4& q,
	size_t a11, size_t a12, size_t a13,
	size_t a21, size_t a22, size_t a23,
	size_t a31, size_t a32, size_t a33)
{
	return det(
		id(q, a11), id(q, a12), id(q, a13),
		id(q, a21), id(q, a22), id(q, a23),
		id(q, a31), id(q, a32), id(q, a33)
	);
}

float VertexError(const glm::mat4& q, const glm::vec3& p)
{
	return id(q, 0)*p.x*p.x + 2*id(q, 1)*p.x*p.y + 2*id(q, 2)*p.x*p.z + 2*id(q, 3)*p.x + id(q, 4)*p.y*p.y
		+ 2*id(q, 5)*p.y*p.z + 2*id(q, 6)*p.y + id(q, 7)*p.z*p.z + 2*id(q, 8)*p.z + id(q, 9);
}

float CalcError(const Index& i0, const Index& i1, const IndexedModel& model, glm::vec3& result)
{
	const Vertex& v0 = model.vertices[i0.pos];
	const Vertex& v1 = model.vertices[i1.pos];
	glm::mat4 q = v0.quadric + v1.quadric;
	float deter = detid(q, 0, 1, 2, 1, 4, 5, 2, 5, 7);
	if (deter != 0)
	{
		result.x = -1/deter * detid(q, 1, 2, 3, 4, 5, 6, 5, 7, 8);
		result.y = -1/deter * detid(q, 0, 2, 3, 1, 5, 6, 2, 7, 8);
		result.z = -1/deter * detid(q, 0, 1, 3, 1, 4, 6, 2, 5,  8);

		return VertexError(q, result);
	}
	else
	{
		const glm::vec3& p0 = v0.position;
		const glm::vec3& p1 = v0.position;
		const glm::vec3 pAvg = (p0 + p1) * 0.5f;
		float p0err = VertexError(q, p0);
		float p1err = VertexError(q, p1);
		float pAvgerr = VertexError(q, pAvg);
		if (p0err <= p1err && p0err <= pAvgerr)
		{
			result = p0;
			return p0err;
		}
		else if (p1err <= p0err && p1err <= pAvgerr)
		{
			result = p1;
			return p1err;
		}
		else
		{
			result = pAvg;
			return pAvgerr;
		}
	}
}

void CreateContractionPair(const Index& i0, const Index& i1, const IndexedModel& model, std::vector<ContractionPair>& contractionPairs)
{
	glm::vec3 recommendedPos;
	float error = CalcError(i0, i1, model, recommendedPos);
	contractionPairs.emplace_back(error, i0, i1, recommendedPos);
	const Vertex& v0 = model.vertices[i0.pos];
	const Vertex& v1 = model.vertices[i1.pos];
#if 0
	glm::mat4 quadric = v0.quadric + v1.quadric;

	glm::vec4 vt;
	float error;

	if (glm::determinant(quadric) != 0) //is the matrix invertible?
	{
		glm::mat4 quadricCopy = quadric;
		quadricCopy[0].w = 0;
		quadricCopy[1].w = 0;
		quadricCopy[2].w = 0;
		quadricCopy[3].w = 1;

		vt = glm::inverse(quadricCopy) * glm::vec4(0, 0, 0, 1);
		error = glm::dot(vt * quadric, vt);
	}
	else
	{
		Vertex avg((v0.position + v1.position) * 0.5f);
		glm::vec4 v0_pos(v0.position, 1.0);
		glm::vec4 v1_pos(v1.position, 1.0);
		glm::vec4 vavg_pos(avg.position, 1.0);

		float v0_error = glm::dot(v0_pos * quadric, v0_pos);
		float v1_error = glm::dot(v1_pos * quadric, v1_pos);
		float vavg_error = glm::dot(vavg_pos * quadric, vavg_pos);

		if (v0_error < v1_error && v0_error < vavg_error)
		{
			error = v0_error;
			vt = v0_pos;
		}
		else if (v1_error < v0_error && v1_error < vavg_error)
		{
			error = v1_error;
			vt = v1_pos;
		}
		else
		{
			error = vavg_error;
			vt = vavg_pos;
		}
	}

	contractionPairs.emplace_back(error, i0, i1, vt);
#elif 0
	contractionPairs.emplace_back(
		glm::dot(v1.position - v0.position, v1.position - v0.position),
		i0, i1,
		(v0.position + v1.position) * 0.5f
	);
#endif
}

void SimplifyMesh(IndexedModel& model)
{
	std::vector<ContractionPair> pairs;
	pairs.reserve(model.triangles.size());

	for (const Triangle& tri : model.triangles)
	{
		CreateContractionPair(tri.v[0], tri.v[1], model, pairs);
		CreateContractionPair(tri.v[0], tri.v[2], model, pairs);
		CreateContractionPair(tri.v[1], tri.v[2], model, pairs);
	}

	std::sort(pairs.begin(), pairs.end());

	unsigned int edgesContracted = 0;

	for (ContractionPair& contraction : pairs)
	{
		for (Triangle& tri : model.triangles)
		{
			
			const bool equality[6] {
				tri.v[0].pos == contraction.i0.pos,
				tri.v[1].pos == contraction.i0.pos,
				tri.v[2].pos == contraction.i0.pos,
				tri.v[0].pos == contraction.i1.pos,
				tri.v[1].pos == contraction.i1.pos,
				tri.v[2].pos == contraction.i1.pos,
			};
			const size_t equalityCount = equality[0] + equality[1] + equality[2] + equality[3] + equality[4] + equality[5];

//			if (equalityCount == 1)
//			{
//				if (equality[3])
//					tri.v[0] = contraction.i0;
//				else if (equality[4])
//					tri.v[1] = contraction.i0;
//				else if (equality[5])
//					tri.v[2] = contraction.i0;
//			}
//			else if (equalityCount >= 2)
//			{
//				for (Index& idx : tri.v)
//					idx.deleted = true;
//			}
		}
		model.vertices[contraction.i0.pos].position = (model.vertices[contraction.i0.pos].position + model.vertices[contraction.i1.pos].position) * 0.5f;

		edgesContracted++;
		if (edgesContracted >= pairs.size() / 10.0f)
			break;
	}
}
