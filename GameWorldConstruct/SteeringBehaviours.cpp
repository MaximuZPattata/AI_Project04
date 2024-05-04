#include "pch.h"

#include "cControlGameEngine.h"
#include "PlayerAttributes.h"
#include "EnemyAttributes.h"

extern cControlGameEngine gameEngine;

float CLOSEST_DISTANCE = 25.f; // The closest the enemy can get to the player is 25 units(SEEK)

float MAINTAIN_DISTANCE = 75.f; // Length of distance the enemy has to maintain from the player before fleeing(FLEE and APPROACH)

float TIME_HORIZON = 15.0f; // How much further the AI should look into the player's position(PURSUE and EVADE)

// Function to make the enemy go after the player
void Seek(std::string modelName, sPlayerAttributes& playerAttributes, sEnemyAttributes& enemyAttributes)
{
	glm::vec3 direction = glm::normalize(playerAttributes.playerPosition - enemyAttributes.enemyPosition);

	glm::vec3 orientationDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

	glm::quat rotationQuat = glm::rotation(enemyAttributes.enemyForward, orientationDirection);

	// Rotating model using quaternion
	gameEngine.RotateMeshModelUsingQuaternion(enemyAttributes.enemyModelName, rotationQuat);

	// Condition to check if the enemy model is not very close to the player
	if (glm::length(enemyAttributes.enemyPosition - playerAttributes.playerPosition) >= CLOSEST_DISTANCE)
	{
		enemyAttributes.enemyPosition += enemyAttributes.enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving model towards player direction
		gameEngine.MoveModel(modelName, enemyAttributes.enemyPosition.x, enemyAttributes.enemyPosition.y, enemyAttributes.enemyPosition.z);
	}
}

// Function to make the enemy move away from the player
void Flee(std::string modelName, sPlayerAttributes& playerAttributes, sEnemyAttributes& enemyAttributes)
{
	// Condition to check if the player is close enough for the enemy to flee
	if (glm::length(enemyAttributes.enemyPosition - playerAttributes.playerPosition) < MAINTAIN_DISTANCE)
	{
		glm::vec3 fleeDirection = glm::normalize(enemyAttributes.enemyPosition - playerAttributes.playerPosition);

		glm::vec3 orientationDirection = glm::normalize(glm::vec3(fleeDirection.x, 0.0f, fleeDirection.z));

		glm::quat rotationQuat = glm::rotation(enemyAttributes.enemyForward, orientationDirection);

		// Rotating model using quaternion
		gameEngine.RotateMeshModelUsingQuaternion(enemyAttributes.enemyModelName, rotationQuat);

		enemyAttributes.enemyPosition += enemyAttributes.enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime); 

		// Moving the enemy away from the player
		gameEngine.MoveModel(modelName, enemyAttributes.enemyPosition.x, enemyAttributes.enemyPosition.y, enemyAttributes.enemyPosition.z);
	}
}

// Function to make the enemy move towards the player's future position
void Pursue(std::string modelName, sPlayerAttributes& playerAttributes, sEnemyAttributes& enemyAttributes)
{
	glm::vec3 futurePlayerPosition = playerAttributes.playerPosition + playerAttributes.playerVelocity * TIME_HORIZON;

	glm::vec3 pursueDirection = glm::normalize(futurePlayerPosition - enemyAttributes.enemyPosition); // Direction for movement

	glm::vec3 direction = glm::normalize(playerAttributes.playerPosition - enemyAttributes.enemyPosition); // Direction for orientation

	glm::vec3 orientationDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

	glm::quat rotationQuat = glm::rotation(enemyAttributes.enemyForward, orientationDirection);

	// Rotating model using quaternion
	gameEngine.RotateMeshModelUsingQuaternion(enemyAttributes.enemyModelName, rotationQuat);

	if (glm::length(enemyAttributes.enemyPosition - futurePlayerPosition) > 10.0f)
	{
		enemyAttributes.enemyPosition += enemyAttributes.enemySpeed * pursueDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving model towards player direction
		gameEngine.MoveModel(modelName, enemyAttributes.enemyPosition.x, enemyAttributes.enemyPosition.y, enemyAttributes.enemyPosition.z);
	}
}

// Function to make the enemy move away from the player's future position
void Evade(std::string modelName, sPlayerAttributes& playerAttributes, sEnemyAttributes& enemyAttributes)
{
	glm::vec3 futurePlayerPosition = playerAttributes.playerPosition + playerAttributes.playerVelocity * TIME_HORIZON;

	if (glm::length(enemyAttributes.enemyPosition - futurePlayerPosition) < MAINTAIN_DISTANCE)
	{
		glm::vec3 fleeDirection = glm::normalize(enemyAttributes.enemyPosition - futurePlayerPosition);

		glm::vec3 orientationDirection = glm::normalize(glm::vec3(fleeDirection.x, 0.0f, fleeDirection.z));

		glm::quat rotationQuat = glm::rotation(enemyAttributes.enemyForward, orientationDirection);

		// Rotating model using quaternion
		gameEngine.RotateMeshModelUsingQuaternion(enemyAttributes.enemyModelName, rotationQuat);

		enemyAttributes.enemyPosition += enemyAttributes.enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving the enemy away from the player
		gameEngine.MoveModel(modelName, enemyAttributes.enemyPosition.x, enemyAttributes.enemyPosition.y, enemyAttributes.enemyPosition.z);
	}
}

// Function to make the enemy maintain distance from the player
void Approach(std::string modelName, sPlayerAttributes& playerAttributes, sEnemyAttributes& enemyAttributes)
{
	float offset = 10.f;

	if (glm::length(enemyAttributes.enemyPosition - playerAttributes.playerPosition) > MAINTAIN_DISTANCE + offset)
	{
		glm::vec3 direction = glm::normalize(playerAttributes.playerPosition - enemyAttributes.enemyPosition);

		glm::vec3 orientationDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

		glm::quat rotationQuat = glm::rotation(enemyAttributes.enemyForward, orientationDirection);

		// Rotating model using quaternion
		gameEngine.RotateMeshModelUsingQuaternion(enemyAttributes.enemyModelName, rotationQuat);

		// Condition to check if the enemy model is not very close to the player
		if (glm::length(enemyAttributes.enemyPosition - playerAttributes.playerPosition) > MAINTAIN_DISTANCE + offset)
		{
			enemyAttributes.enemyPosition += enemyAttributes.enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

			// Moving model towards player direction
			gameEngine.MoveModel(modelName, enemyAttributes.enemyPosition.x, enemyAttributes.enemyPosition.y, enemyAttributes.enemyPosition.z);
		}
	}

	else if(glm::length(enemyAttributes.enemyPosition - playerAttributes.playerPosition) < MAINTAIN_DISTANCE)
	{
		Flee(modelName, playerAttributes, enemyAttributes);
	}
}