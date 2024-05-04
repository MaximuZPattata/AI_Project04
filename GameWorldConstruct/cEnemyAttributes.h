#pragma once
#include "pch.h"

#include "PlayerAttributes.h"
#include "Map.h"

#include <cControlGameEngine.h>

struct sPathFollowingBehaviourAttributes
{
	sPathFollowingBehaviourAttributes(float closeToEndPointRadius, 
		float radiusAroundPath, float futurePositionPredictionDistance, float maximumSpeed , std::string futurePosSphere, std::string projectedPosSphere)
		: destinationRadius(closeToEndPointRadius)
		, pathRadius(radiusAroundPath)
		, futurePredictionDistance(futurePositionPredictionDistance)
		, maxSpeed(maximumSpeed) 
		, futurePosSphereName(futurePosSphere)
		, projectedPathPosSphereName(projectedPosSphere)	{}

	bool bChangePath = true;

	float destinationRadius;
	float futurePredictionDistance;
	float pathRadius;
	float maxSpeed;

	std::string futurePosSphereName;
	std::string projectedPathPosSphereName;

	sPath currentPathFollowed;
};

struct sFlockingBehaviourAttributes
{
	sFlockingBehaviourAttributes(glm::vec3 velocity, glm::vec3 acceleration, float speed, float radius, float maxForce, float boundaryValue)
		: enemyVelocity(velocity)
		, enemyAcceleration(acceleration)
		, enemyMaxSpeed(speed)
		, flockMaxRadius(radius) 
		, enemyMaxForce(maxForce) 
		, minBoundaryX(-boundaryValue) 
		, maxBoundaryX(boundaryValue) 
		, minBoundaryZ(-boundaryValue) 
		, maxBoundaryZ(boundaryValue) {}

	glm::vec3 enemyVelocity;
	glm::vec3 enemyAcceleration;
	
	float enemyMaxSpeed;
	float enemyMaxForce;
	float flockMaxRadius;
	float minBoundaryX;
	float maxBoundaryX;
	float minBoundaryZ;
	float maxBoundaryZ;
};

struct sWanderBehaviorAttributes
{
	sWanderBehaviorAttributes(float decisionTime, float radius1, float radius2, glm::vec3 wanderColor)
		: wanderCountdown(0.f)
		, wanderDecisionTime(decisionTime)
		, wanderRadius1(radius1)
		, wanderRadius2(radius2) 
		, wanderVariationColor(wanderColor) {}

	glm::vec3 wanderDirection = glm::vec3(0.f);
	glm::vec3 wanderVariationColor;

	float wanderCountdown;
	float wanderDecisionTime;
	float wanderRadius1;
	float wanderRadius2;
};

class cEnemyAttributes
{
public:
	glm::vec3 initialEnemyPosition;
	glm::vec3 currentEnemyPosition;
	glm::vec3 oldEnemyPosition;
	glm::vec3 enemyForward;
	glm::vec3 enemyVelocity;
	glm::vec3 enemyAcceleration;

	std::string enemyModelName;
	
	float enemySpeed;
	float enemyRotationSpeed;

	cEnemyAttributes(std::string modelName, glm::vec3 position, glm::vec3 forward, glm::vec3 velocity, glm::vec3 acceleration, float speed, float rotationSpeed);

	void Seek(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes);
	void Flee(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes);
	void Pursue(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes);
	void Evade(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes);
	void Approach(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes);
	void InitializeWanderBehaviour(unsigned decisionTime, float radius1, float radius2, glm::vec3 wanderColor);
	void Wander(cControlGameEngine& gameEngine);
	void InitializeFlockingAttributes(glm::vec3 velocity, glm::vec3 acceleration, float flockRadius, float maxForce, float boundaryValue);
	void Flock(cControlGameEngine& gameEngine, std::vector<cEnemyAttributes*> enemyAIList);
	void InitializePathFollowingAttributes(float closeToEndPointRadius, float pathRadius, 
		float futurePositionPredictionDistance, float maximumSpeed, std::string futurePosSphere, std::string projectedPosSphere);
	void FollowPath(cControlGameEngine& gameEngine, std::vector < sPath >& PathList, std::vector < sDangerZone >& DangerZoneList, sPlayerAttributes& player);

	std::string GetFuturePosSphereName();
	std::string GetProjectedPathPosSphereName();

private:

	float CLOSEST_DISTANCE = 25.f; // The closest the enemy can get to the player is 25 units(SEEK)
	float MAINTAIN_DISTANCE = 75.f; // Length of distance the enemy has to maintain from the player before fleeing(FLEE and APPROACH)
	float TIME_HORIZON = 15.0f; // How much further the AI should look into the player's position(PURSUE and EVADE)

	bool bColorChanged = false;

	sWanderBehaviorAttributes* mWanderBehaviour = NULL;
	sFlockingBehaviourAttributes* mFlockingBehaviour = NULL;
	sPathFollowingBehaviourAttributes* mPathFollowBehaviour = NULL;

	void IndicateWanderPoint(cControlGameEngine& gameEngine, glm::vec3 wanderPoint);
	void DeleteOldPoint(cControlGameEngine& gameEngine, std::string sphereModelName);
	void FacePlayer(sPlayerAttributes& player, cControlGameEngine& gameEngine);

	glm::vec3 SetMagnitude(glm::vec3 steeringVelocity);
	glm::vec3 LimitMagnitude(glm::vec3 steeringVelocity, float maxSpeed);
	glm::vec3 FlockingAlignment(std::vector <cEnemyAttributes*> enemyAIList);
	glm::vec3 FlockingCohesion(std::vector <cEnemyAttributes*> enemyAIList);
	glm::vec3 FlockingSeparation(std::vector <cEnemyAttributes*> enemyAIList);
	glm::vec3 PredictFuturePosition(float deltaTime);
	glm::vec3 FindEnemyProjectionOntoPath(glm::vec3 initialPathPos, glm::vec3 enemyCurrentPosition, glm::vec3 pathEndPos);
	glm::vec3 SeekTheTarget(glm::vec3 target);

	bool IsPlayerInsideOuterCircle(sPlayerAttributes& player, std::vector < sDangerZone > dangerZoneList);
	bool IsPlayerInsideInnerCircle(sPlayerAttributes& player, std::vector < sDangerZone > dangerZoneList);

	sPath& CheckForClosestPath(glm::vec3 enemyCurrentPosition, std::vector < sPath > PathList);
};

