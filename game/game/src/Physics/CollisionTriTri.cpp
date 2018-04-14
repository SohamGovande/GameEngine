#include <iostream>
#include "CollisionCommons.h"
#include "CollisionMath.h"

CollisionResult CollisionDetection::checkTriangleTriangle(bool calculateIntersectionPoint, std::array<glm::vec3, 3> tri1, std::array<glm::vec3, 3> tri2)
{
	using namespace CollisionMath;

	glm::vec4 p1 = obtainPlaneEquation(tri1);
	glm::vec4 p2 = obtainPlaneEquation(tri2);

	//Divisions are expensive, so replace it with multiplication:
	//x2 / x1 == y2 / y1 becomes x2 * y1 == y2 * x1

	//Parallel check
	if (areEqual(p2.x * p1.y, p2.y * p1.x) &&
		areEqual(p2.z * p1.y, p2.y * p1.z))
	{
		if (!areEqual(p1.w, p2.w)) //If they are parallel planes
			return CollisionResult::noCollision();

		//They are the same plane

		//Line (0, 0) to (1, 0) is perpendicular to the line on (0, 0) to (0, 1) that lies on that plane
		glm::vec3 point1 = findPointOnPlane(p1, 0, 0);
		glm::vec3 point2 = findPointOnPlane(p1, 1, 0);
		glm::vec3 point3 = findPointOnPlane(p1, 0, 1);

		const glm::mat3 basisVectors(
			glm::normalize(point2 - point1),
			glm::normalize(point3 - point1),
			glm::normalize(glm::vec3(p1.x, p1.y, p1.z))
		);

		projectTo2DPlane(tri1[0], point1, basisVectors);
		projectTo2DPlane(tri1[1], point1, basisVectors);
		projectTo2DPlane(tri1[2], point1, basisVectors);

		projectTo2DPlane(tri2[0], point1, basisVectors);
		projectTo2DPlane(tri2[1], point1, basisVectors);
		projectTo2DPlane(tri2[2], point1, basisVectors);

		std::vector<glm::vec2> intersectionLocations;

		//Loop through every combination of vertices in the triangle
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				glm::vec2 t1v1(tri1[i].x, tri1[i].y);
				glm::vec2 t1v2(tri1[(i + 1) % 3].x, tri1[(i + 1) % 3].y);
				glm::vec2 t2v1(tri2[j].x, tri2[j].y);
				glm::vec2 t2v2(tri2[(j + 1) % 3].x, tri2[(j + 1) % 3].y);

				LineIntersectionResult inter = findLine2DIntersection(t1v1, t1v2, t2v1, t2v2);
				
				if (inter.parallel)
					continue;
				
				//now, we know the lines are intersecting
				glm::vec2 minCoordsTri1(
					std::min(t1v1.x, t1v2.x),
					std::min(t1v1.y, t1v2.y)
				);
				glm::vec2 maxCoordsTri1(
					std::max(t1v1.x, t1v2.x),
					std::max(t1v1.y, t1v2.y)
				);
				glm::vec2 minCoordsTri2(
					std::min(t2v1.x, t2v2.x),
					std::min(t2v1.y, t2v2.y)
				);
				glm::vec2 maxCoordsTri2(
					std::max(t2v1.x, t2v2.x),
					std::max(t2v1.y, t2v2.y)
				);

				if ((inter.location.x >= minCoordsTri1.x && inter.location.x <= maxCoordsTri1.x) &&
					(inter.location.y >= minCoordsTri1.y && inter.location.y <= maxCoordsTri1.y) &&
					(inter.location.x >= minCoordsTri2.x && inter.location.x <= maxCoordsTri2.x) &&
					(inter.location.y >= minCoordsTri2.y && inter.location.y <= maxCoordsTri2.y))
				{
					//There was an intersection!
					if (!calculateIntersectionPoint)
						return CollisionResult::collisionExists();

					if (inter.coinciding)
					{
						glm::vec2 max(
							std::min(maxCoordsTri1.x, maxCoordsTri2.x),
							std::min(maxCoordsTri1.y, maxCoordsTri2.y)
						);
						glm::vec2 min(
							std::max(minCoordsTri1.x, minCoordsTri2.x),
							std::max(minCoordsTri1.y, minCoordsTri2.y)
						);
						intersectionLocations.push_back((min + max) * 0.5f);
					}
					else
					{
						intersectionLocations.push_back(inter.location);
					}
				}
			}	
		}

		if (intersectionLocations.size() == 0) //No edges intersect
		{
			glm::vec3 centerOfGravity;
			//Check if all points in one triangle are contained in the other triangle
			if (isPointInTriangle(tri1[0], tri2) &&
				isPointInTriangle(tri1[1], tri2) &&
				isPointInTriangle(tri1[2], tri2))
			{
				centerOfGravity = (tri1[0] + tri1[1] + tri1[2]) / 3.0f;
			}
			else if (isPointInTriangle(tri2[0], tri1) &&
				isPointInTriangle(tri2[1], tri1) &&
				isPointInTriangle(tri2[2], tri1))
			{
				centerOfGravity = (tri2[0] + tri2[1] + tri2[2]) / 3.0f;
			}
			else //They aren't contained in one another, so they don't intersect.
			{
				return CollisionResult::noCollision();
			}
			unprojectFrom2DPlane(centerOfGravity, point1, basisVectors);
			return CollisionResult::collidedAt(centerOfGravity);
		}
		else //Edges do intersect
		{
			glm::vec3 avg(0, 0, 0);

			for (const glm::vec2& location : intersectionLocations)
			{
				avg.x += location.x;
				avg.y += location.y;
			}
			avg /= (float)intersectionLocations.size();

			unprojectFrom2DPlane(avg, point1, basisVectors);

			return CollisionResult::collidedAt(avg);
		}
	}
	else //The triangle is made of non-coplanar points, so perform regular checking
	{
		const glm::vec3 intersectionLineDir = glm::normalize(glm::cross(glm::vec3(p1.x, p1.y, p1.z), glm::vec3(p2.x, p2.y, p2.z)));
		const glm::vec3 pointOnPlane = findPointOnIntersectionLine(p1, p2);

		const glm::mat3 basisVectors[2]{
			//we don't need to normalize jhat because it's going to be 0 anyway
			glm::mat3(
				intersectionLineDir,
				glm::normalize(glm::cross(intersectionLineDir, glm::vec3(p1.x, p1.y, p1.z))),
				glm::normalize(glm::vec3(p1.x, p1.y, p1.z))
			),
			glm::mat3(
				intersectionLineDir,
				glm::normalize(glm::cross(intersectionLineDir, glm::vec3(p2.x, p2.y, p2.z))),
				glm::normalize(glm::vec3(p2.x, p2.y, p2.z))
			)
		};

		projectTo2DPlane(tri1[0], pointOnPlane, basisVectors[0]);
		projectTo2DPlane(tri1[1], pointOnPlane, basisVectors[0]);
		projectTo2DPlane(tri1[2], pointOnPlane, basisVectors[0]);

		projectTo2DPlane(tri2[0], pointOnPlane, basisVectors[1]);
		projectTo2DPlane(tri2[1], pointOnPlane, basisVectors[1]);
		projectTo2DPlane(tri2[2], pointOnPlane, basisVectors[1]);

		//std::array<unsigned int, 2> represents an edge (two indices which point to 2 vertices)
		using edge = std::array<unsigned int, 2>;
		const std::vector<edge> edgesTri1 = getEdgesWhichCrossXAxis(tri1);
		const std::vector<edge> edgesTri2 = getEdgesWhichCrossXAxis(tri2);

		if (edgesTri1.size() > 0 && edgesTri2.size() > 0)
		{
			std::array<float, 2> zeroesTri1 = calculateXAxisIntersection(tri1);
			std::array<float, 2> zeroesTri2 = calculateXAxisIntersection(tri2);

			float zeroTri1Min = std::min(zeroesTri1[0], zeroesTri1[1]);
			float zeroTri1Max = std::max(zeroesTri1[0], zeroesTri1[1]);
			float zeroTri2Min = std::min(zeroesTri2[0], zeroesTri2[1]);
			float zeroTri2Max = std::max(zeroesTri2[0], zeroesTri2[1]);

			if ((zeroTri2Max >= zeroTri1Max && zeroTri2Min <= zeroTri1Max) || //segment 2 is ahead of and overlapping segment 1
				(zeroTri2Min <= zeroTri1Min && zeroTri2Max >= zeroTri1Min) || //segment 2 is behind of and overlapping segment 1
				(zeroTri2Min <= zeroTri1Min && zeroTri2Max >= zeroTri1Max) || //segment 2 completely contains segment 1
				(zeroTri2Min >= zeroTri1Min && zeroTri2Max <= zeroTri1Max))   //segment 2 is completely contained in segment 1
			{
				//There is an intersection!
				if (!calculateIntersectionPoint)
					return CollisionResult::collisionExists();

				glm::vec3 minIntersection(std::min(zeroTri1Max, zeroTri2Max), 0, 0);
				glm::vec3 maxIntersection(std::max(zeroTri1Min, zeroTri2Min), 0, 0);

				unprojectFrom2DPlane(minIntersection, pointOnPlane, basisVectors[0]);
				unprojectFrom2DPlane(maxIntersection, pointOnPlane, basisVectors[0]);

				return CollisionResult::collidedAt((minIntersection + maxIntersection) / 2.f);
			}
		}
		else
		{
			std::cout << "Triangles don't intersect the x-axis, so no intersection.";
		}
		return CollisionResult::noCollision();
	}
}
