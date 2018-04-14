#include <array>

#include "CollisionCommons.h"

static inline int getSign(float x)
{
	return x > 0 ? 1 : (x == 0 ? 0 : -1);
}

namespace CollisionMath
{
	bool isPointInTriangle(const glm::vec3& pt, const std::array<glm::vec3, 3>& triangle)
	{
		bool b1, b2, b3;

		b1 = getPointInTriangleSign(pt, triangle[1], triangle[2]) < 0.0f;
		b2 = getPointInTriangleSign(pt, triangle[2], triangle[3]) < 0.0f;
		b3 = getPointInTriangleSign(pt, triangle[3], triangle[1]) < 0.0f;

		return (b1 == b2) && (b2 == b3);
	}

	LineIntersectionResult findLine2DIntersection(const glm::vec2& e1v1, const glm::vec2& e1v2, const glm::vec2& e2v1, const glm::vec2& e2v2)
	{
		//ax + by = c
		float a1 = e1v2.y - e1v1.y;
		float b1 = e1v1.x - e1v2.x;
		float c1 = a1 * e1v1.x + b1 * e1v1.y;
		
		float a2 = e2v2.y - e2v1.y;
		float b2 = e2v1.x - e2v2.x;
		float c2 = a2 * e2v1.x + b2 * e2v1.y;
		
		//m = -a/b

		/*Check if slopes are equal, so:

		-a1/b1 = -a2/b2
		a1/b1 = a2/b2
		a1*b2 = a2*b1
		*/

		if (a1*b2 == a2*b1) //Whether they have equal slopes
		{
			if (areEqual(c2, c1 * (a2 / a1))) //Whether one equation is just the other one scaled up
				return LineIntersectionResult::coincidingLines();

			return LineIntersectionResult::parallelLines();
		}
		else //They are normal lines
		{
			const bool parallelToX1 = a1 == 0;
			const bool parallelToY1 = b1 == 0;
			const bool line1Parallel = parallelToX1 || parallelToY1;

			const bool parallelToX2 = a2 == 0;
			const bool parallelToY2 = b2 == 0;
			const bool line2Parallel = parallelToX2 || parallelToY2;
			
			assert(!(parallelToX1 && parallelToY1));
			assert(!(parallelToX2 && parallelToY2));
			assert(!(parallelToX1 && parallelToX2));
			assert(!(parallelToY1 && parallelToY2));

			if (parallelToX1 && !line2Parallel)
			{
				float y = c1 / b1;
				float x = (c2 - (b2 * y)) / a2;
				return LineIntersectionResult::intersectedAt(glm::vec2(x, y));
			}
			else if (parallelToX1 && parallelToY2)
			{
				return LineIntersectionResult::intersectedAt(glm::vec2(c2 / a2, c1 / b1));
			}
			else if (parallelToY1 && !line2Parallel)
			{
				float x = c1 / a1;
				float y = (c2 - (a2 * x)) / b2;
				return LineIntersectionResult::intersectedAt(glm::vec2(x, y));
			}
			else if (parallelToY1 && parallelToX2)
			{
				return LineIntersectionResult::intersectedAt(glm::vec2(c2 / a2, c1 / b1));
			}
			else if (parallelToX2 && !line1Parallel)
			{
				float y = c2 / b2;
				float x = (c1 - (b1 * y)) / a1;
				return LineIntersectionResult::intersectedAt(glm::vec2(x, y));
			}
			else if (parallelToY2 && !line1Parallel)
			{
				float x = c2 / a2;
				float y = (c1 - (a1 * x)) / b1;
				return LineIntersectionResult::intersectedAt(glm::vec2(x, y));
			}
			else
			{
				return LineIntersectionResult::intersectedAt(solveSystem(
					{ a1, b1, -c1 },
					{ a2, b2, -c2 }
				));
			}
		}
	}

	//x = coefficient of var 1
	//y = coefficient of var 2
	//z = constant
	//x*a + y*b + z = 0
	glm::vec2 solveSystem(glm::vec3 e1, const glm::vec3& e2)
	{
		//Use the elimination method
		e1 *= e2.x / e1.x; //e1.x should now = e2.x. e1.y and e1.z are scaled up
		glm::vec3 solution = e2 - e1; //now, solution.x = 0, and we have solution.y and solution.z
		float bCoord = -solution.z / solution.y; // -constant/coefficient
												 //in x*a + y*b + z = 0, a = (-z - y*b)/x
		float aCoord = (-e2.z - e2.y * bCoord) / e2.x;
		return glm::vec2(aCoord, bCoord);
	}

	glm::vec4 obtainPlaneEquation(const std::array<glm::vec3, 3>& triangle)
	{
		glm::vec3 edge1 = triangle[2] - triangle[1];
		glm::vec3 edge2 = triangle[0] - triangle[1];
		//a(x - x0) + b(y - y0) + c(z - z0) = 0
		//ax + by + cz + d = 0
		glm::vec3 normal = glm::cross(edge1, edge2);

		return glm::vec4(
			normal.x, normal.y, normal.z,
			-normal.x * triangle[0].x - normal.y * triangle[0].y - normal.z * triangle[0].z
		);
	}

	glm::vec3 findPointOnIntersectionLine(const glm::vec4& p1, const glm::vec4& p2)
	{
		if (p1.y == p2.y && p1.z == p2.z) //Cannot solve with x = 0
		{
			//Solve with y = 0
			glm::vec2 result = solveSystem(
				glm::vec3(p1.x, p1.z, p1.w),
				glm::vec3(p2.x, p2.z, p2.w)
			);
			return glm::vec3(result.x, 0, result.y);
		}
		else //Can solve with x = 0
		{
			//Solve with x = 0
			glm::vec2 result = solveSystem(
				glm::vec3(p1.y, p1.z, p1.w),
				glm::vec3(p2.y, p2.z, p2.w)
			);
			return glm::vec3(0, result.x, result.y);
		}
	}

	/*
	Plane equation is ax + by + cx + d = 0, we're solving for y
	by = -ax - cz - d
	y = (-ax - cz - d) / b
	*/
	glm::vec3 findPointOnPlane(const glm::vec4& plane, float x, float z)
	{
		return glm::vec3(x, (-plane.x * x - plane.z * z - plane.w) / plane.y, z);
	}

	std::array<float, 2> calculateXAxisIntersection(const std::array<glm::vec3, 3>& triangle)
	{
		constexpr unsigned int edgeIndices[6]{
			0, 2,
			1, 0,
			2, 1
		};

		std::array<float, 2> array;
		int nextIndex = 0;

		for (int i = 0; i < 3; i++)
		{
			if (nextIndex == 2)
				break;
			const glm::vec3& v1 = triangle[edgeIndices[i * 2]];
			const glm::vec3& v2 = triangle[edgeIndices[i * 2 + 1]];
			//calculating y = mx + b of the edge
			/*
			x1, y1    x2, y2
			m = (y2 - y1) / (x2 - x1)
			y = mx + b
			b = -mx + y

			Solving for y=0:
			0 = mx + b
			x = -b / m
			*/
			const float m = (v2.y - v1.y) / (v2.x - v1.x);
			const float b = -m * v1.x + v1.y;
			const float zero = -b / m;
			//Coordinates of intersection are (zero, 0), so check if
			//they are in the SEGMENT

			const glm::vec2 min(
				std::min(v1.x, v2.x),
				std::min(v1.y, v2.y)
			);
			const glm::vec2 max(
				std::max(v1.x, v2.x),
				std::max(v1.y, v2.y)
			);
			if ((min.y < 0 && max.y > 0) &&
				(min.x < zero && max.x > zero))
			{
				array[nextIndex] = zero;
				nextIndex++;
			}
		}
		return array;
	}

	std::vector<std::array<unsigned int, 2>> getEdgesWhichCrossXAxis(const std::array<glm::vec3, 3>& triangle)
	{
		std::vector<std::array<unsigned int, 2>> edges;
		int signs[3]{
			getSign(triangle[0].y),
			getSign(triangle[1].y),
			getSign(triangle[2].y)
		};
		if (signs[0] != signs[1])
			edges.push_back({ 0, 1 });

		if (signs[1] != signs[2])
			edges.push_back({ 1, 2 });

		if (signs[2] != signs[0])
			edges.push_back({ 2, 0 });
		return edges;
	}

	void projectTo2DPlane(glm::vec3& outVertex, const glm::vec3& newOrigin, const glm::mat3& basisVectors)
	{
		outVertex = glm::vec3(
			glm::dot(basisVectors[0], outVertex - newOrigin),
			glm::dot(basisVectors[1], outVertex - newOrigin),
			glm::dot(basisVectors[2], outVertex - newOrigin)
		);
	}

	void unprojectFrom2DPlane(glm::vec3& outVertex2D, const glm::vec3& originUsed, const glm::mat3& basisVectors)
	{
		outVertex2D = outVertex2D.x * basisVectors[0] +
			outVertex2D.y * basisVectors[1] +
			+ originUsed;
	}
}