#pragma once

enum class ePlayerDirection
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

struct sPlayerAttributes
{
	glm::vec3 playerPosition = glm::vec3(0.f);
	glm::vec3 playerVelocity = glm::vec3(0.f);
	glm::vec3 playerForward = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 playerMovingDirection = glm::vec3(0.f, 0.f, -1.f);

	float playerYaw = 0.f;
	float playerPitch = 0.f;
	float playerMovementSpeed = 0.f;
	float decelerationFactor = 0.f;
	float playerRotationSpeed = 0.f;
	
	std::string playerModelName = "";

	ePlayerDirection playerDirection = ePlayerDirection::FRONT;
};
