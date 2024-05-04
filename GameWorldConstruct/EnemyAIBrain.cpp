#include "pch.h"
#include "cEnemyAttributes.h"
#include "Map.h"

#include <cControlGameEngine.h>
#include <random>

extern std::vector<cEnemyAttributes*> enemyDetailsList;

float const CONSTANT_Y = 10.f;

std::vector < sPath > pathList;
std::vector < sDangerZone > dangerZoneList;

double getRandom(double min, double max) 
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	
	std::uniform_real_distribution<double> dis(min, max);

	return dis(gen);
}

void InitializingEnemyAIs()
{
	glm::vec3 enemyPosition;
	glm::vec3 randomVelocity;
	glm::vec3 acceleration = glm::vec3(0.f);
	glm::vec3 enemyForward = glm::vec3(-1.f, 0.f, 0.f);

	float speed = 20.f;
	float maxSpeed = 45.f;
	float rotationSpeed = 3.f;
	float minValue = -10.f;
	float maxValue = 10.f;
	float randomInitialSpeed = 5.f;
	float endPointRadius = 30.f;
	float pathRadius = 25.f;
	float randomPosValue = 200.f;
	float futurePosPredictionDistance = 50.f;

	for (int enemyCount = 1; enemyCount <= 2; enemyCount++)
	{
		std::string modelName = "Beholder0" + std::to_string(enemyCount);
		std::string debugSphere01Name = "Sphere_FuturePos_0" + std::to_string(enemyCount);
		std::string debugSphere02Name = "Sphere_ProjectedPathPos_0" + std::to_string(enemyCount);

		enemyPosition = glm::vec3(getRandom(-randomPosValue, randomPosValue), CONSTANT_Y, getRandom(-randomPosValue, randomPosValue));
		
		randomVelocity = glm::vec3(getRandom(minValue, maxValue), 0.f, getRandom(minValue, maxValue));
		randomVelocity *= randomInitialSpeed;

		cEnemyAttributes* enemy = new cEnemyAttributes(modelName, enemyPosition, enemyForward, randomVelocity, acceleration, speed, rotationSpeed);

		enemy->InitializePathFollowingAttributes(endPointRadius, pathRadius, futurePosPredictionDistance, maxSpeed, debugSphere01Name, debugSphere02Name);

		enemyDetailsList.push_back(enemy);
	}

	//------------------------Initialize Flock Behavior-------------------------------------

	/*glm::vec3 enemyPosition;
	glm::vec3 randomVelocity;
	glm::vec3 enemyForward = glm::vec3(-1.f, 0.f, 0.f);

	float maxSpeed = 15.f;
	float rotationSpeed = 3.f;
	float commonRadius = 25.f;
	float commonMaxForce = 5.f;
	float minValue = -10.f;
	float maxValue = 10.f;
	float boundaryValue = 100.f;
	float randomInitialSpeed = 5.f;

	for (int enemyCount = 1; enemyCount <= 30; enemyCount++)
	{
		std::string modelName = "Beholder0" + std::to_string(enemyCount);

		enemyPosition = glm::vec3(getRandom(-70.f, 70.f), 10.f, getRandom(-70.f, 70.f));

		cEnemyAttributes* enemy = new cEnemyAttributes(modelName, enemyPosition, enemyForward, maxSpeed, rotationSpeed);

		randomVelocity = glm::vec3(getRandom(minValue, maxValue), 0.f, getRandom(minValue, maxValue));

		randomVelocity *= randomInitialSpeed;

		enemy->InitializeFlockingAttributes(randomVelocity, glm::vec3(0.f), commonRadius, commonMaxForce, boundaryValue);

		enemyDetailsList.push_back(enemy);
	}*/

	//-----------------------Initialize Wander Behavior-------------------------------------

	//// Giving each wander variation a color
	//glm::vec3 GreenColor = glm::vec3(0.1, 0.85, 0.1);
	//glm::vec3 WhiteColor = glm::vec3(0.9, 0.9, 0.9);
	//glm::vec3 BlueColor = glm::vec3(0.1, 0.1, 0.85);

	//enemy01.InitializeWanderBehaviour(2.5f, 5.f, 2.f, GreenColor);	// Change parameters for distinct wandering style
	//enemy02.InitializeWanderBehaviour(5.0f, 3.f, 5.f, WhiteColor);	// Change parameters for distinct wandering style
	//enemy03.InitializeWanderBehaviour(1.0f, 7.f, 7.f, BlueColor);	// Change parameters for distinct wandering style

	//enemyDetailsList.push_back(enemy01);
	//enemyDetailsList.push_back(enemy02);
	//enemyDetailsList.push_back(enemy03);
}

void UpdateAIPositions()
{
	for (cEnemyAttributes* currentEnemy : enemyDetailsList)
		currentEnemy->oldEnemyPosition = currentEnemy->currentEnemyPosition;
}

void CreatePathPatternForEnemies()
{
	sPath path01;
	sPath path02;
	sPath path03;
	sPath path04;
	sPath path05;
	sPath path06;

	path01.mFromPointA = glm::vec3(-174, CONSTANT_Y, -182);
	path01.mToPointB = glm::vec3(0, CONSTANT_Y, -182);

	path02.mFromPointA = glm::vec3(0, CONSTANT_Y, -182);
	path02.mToPointB = glm::vec3(0, CONSTANT_Y, 51);

	path03.mFromPointA = glm::vec3(0, CONSTANT_Y, 51);
	path03.mToPointB = glm::vec3(232, CONSTANT_Y, 51);

	path04.mFromPointA = glm::vec3(232, CONSTANT_Y, 51);
	path04.mToPointB = glm::vec3(232, CONSTANT_Y, 225);

	path05.mFromPointA = glm::vec3(232, CONSTANT_Y, 225);
	path05.mToPointB = glm::vec3(-174, CONSTANT_Y, 225);

	path06.mFromPointA = glm::vec3(-174, CONSTANT_Y, 225);
	path06.mToPointB = glm::vec3(-174, CONSTANT_Y, -182);

	pathList.push_back(path01);
	pathList.push_back(path02);
	pathList.push_back(path03);
	pathList.push_back(path04);
	pathList.push_back(path05);
	pathList.push_back(path06);
}

void CreateDangerZones(cControlGameEngine& gameEngine)
{
	sDangerZone dangerZone01;
	sDangerZone dangerZone02;

	glm::vec3 innerCirclePos_01 = gameEngine.GetModelPosition("Inner_Circle_01");
	glm::vec3 innerCirclePos_02 = gameEngine.GetModelPosition("Inner_Circle_02");
	glm::vec3 outerCirclePos_01 = gameEngine.GetModelPosition("Outer_Circle_01");
	glm::vec3 outerCirclePos_02 = gameEngine.GetModelPosition("Outer_Circle_02");

	dangerZone01.innerCirclePosition = glm::vec3(innerCirclePos_01.x, CONSTANT_Y, innerCirclePos_01.z);
	dangerZone01.outerCirclePosition = glm::vec3(outerCirclePos_01.x, CONSTANT_Y, outerCirclePos_01.z);
	dangerZone01.innerCircleRadius = 20.f;
	dangerZone01.outerCircleRadius = 40.f;
				
	dangerZone02.innerCirclePosition = glm::vec3(innerCirclePos_02.x, CONSTANT_Y, innerCirclePos_02.z);
	dangerZone02.outerCirclePosition = glm::vec3(outerCirclePos_02.x, CONSTANT_Y, outerCirclePos_02.z);
	dangerZone02.innerCircleRadius = 20.f;
	dangerZone02.outerCircleRadius = 40.f;

	dangerZoneList.push_back(dangerZone01);
	dangerZoneList.push_back(dangerZone02);
}

void MakeDebugSpheresVisible(cControlGameEngine& gameEngine)
{
	for (cEnemyAttributes* currentEnemy : enemyDetailsList)
	{
		gameEngine.TurnVisibilityOn(currentEnemy->GetFuturePosSphereName());
		gameEngine.TurnVisibilityOn(currentEnemy->GetProjectedPathPosSphereName());	
	}
}