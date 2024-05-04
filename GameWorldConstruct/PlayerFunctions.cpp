#include "pch.h"

#include "PlayerAttributes.h"
#include <cControlGameEngine.h>
#include <cmath>

extern sPlayerAttributes playerAttributes;

void InitializePlayer()
{
    playerAttributes.playerPosition = glm::vec3(0.f, 10.f, 0.f);
    playerAttributes.playerVelocity = glm::vec3(0.f);
    playerAttributes.playerMovementSpeed = 25.f;
    playerAttributes.playerRotationSpeed = 10.f;
    playerAttributes.playerYaw = 0.f;
    playerAttributes.playerModelName = "Spiderman";
    playerAttributes.decelerationFactor = 6.f;
}

glm::vec3 GetDirectionToLook()
{
    float lookDistance = 200.f;

    glm::vec3 targetToLookAt = glm::vec3(0.f);

    //---------------Checking which direction user wants the player to turn------------------------------

    if (playerAttributes.playerDirection == ePlayerDirection::FRONT)
        targetToLookAt = playerAttributes.playerPosition + playerAttributes.playerMovingDirection * lookDistance;

    else if (playerAttributes.playerDirection == ePlayerDirection::BACK)
        targetToLookAt = playerAttributes.playerPosition + playerAttributes.playerMovingDirection * lookDistance;

    else if (playerAttributes.playerDirection == ePlayerDirection::LEFT)
        targetToLookAt = playerAttributes.playerPosition + playerAttributes.playerMovingDirection * lookDistance;
    
    else if (playerAttributes.playerDirection == ePlayerDirection::RIGHT)
        targetToLookAt = playerAttributes.playerPosition + playerAttributes.playerMovingDirection * lookDistance;

    //---------------Normalizing the direction to look at and returning it-------------------------------

    return glm::normalize(targetToLookAt - playerAttributes.playerPosition);
}

void UpdatePlayerPosition(cControlGameEngine& gameEngine, GLFWwindow* window)
{
    //-----------------------------------Update player position---------------------------------------------------------

    playerAttributes.playerPosition += playerAttributes.playerVelocity * static_cast <float> (gameEngine.deltaTime);

    //-----------------------------------Update player rotation---------------------------------------------------------

    glm::vec3 DirectionToLookAt = GetDirectionToLook();

    glm::quat targetRotationQuat = glm::rotation(playerAttributes.playerForward, DirectionToLookAt);

    glm::quat rotationQuat = gameEngine.GetModelRotationQuat(playerAttributes.playerModelName);

    glm::quat interpolatedQuat = glm::slerp(glm::normalize(rotationQuat), glm::normalize(targetRotationQuat), playerAttributes.playerRotationSpeed * static_cast <float> (gameEngine.deltaTime));

    //----------------------------------Update player mesh model--------------------------------------------------------

    gameEngine.MoveModel(playerAttributes.playerModelName, playerAttributes.playerPosition.x, playerAttributes.playerPosition.y, playerAttributes.playerPosition.z);
    gameEngine.RotateMeshModelUsingQuaternion(playerAttributes.playerModelName, interpolatedQuat);

}
