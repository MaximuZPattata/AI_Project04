#pragma once
#include "pch.h"

struct sPath
{
	glm::vec3 mFromPointA;
	glm::vec3 mToPointB;
};

struct sDangerZone
{
	glm::vec3 innerCirclePosition;
	glm::vec3 outerCirclePosition;

	float innerCircleRadius;
	float outerCircleRadius;
};