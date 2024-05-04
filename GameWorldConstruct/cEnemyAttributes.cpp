#include "pch.h"
#include "cEnemyAttributes.h"
#include <cmath>

cEnemyAttributes::cEnemyAttributes(std::string modelName, glm::vec3 position, glm::vec3 forward, glm::vec3 velocity, glm::vec3 acceleration, float speed, float rotationSpeed)
{
	this->initialEnemyPosition = position;
	this->enemyModelName = modelName;
	this->currentEnemyPosition = position;
	this->oldEnemyPosition = this->currentEnemyPosition;
	this->enemyForward = forward;
	this->enemySpeed = speed;
	this->enemyRotationSpeed = rotationSpeed;
	this->enemyVelocity = velocity;
	this->enemyAcceleration = acceleration;
}

// Function to make the enemy move towards the player
void cEnemyAttributes::Seek(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes)
{
	glm::vec3 direction = glm::normalize(playerAttributes.playerPosition - currentEnemyPosition);

	glm::vec3 orientationDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

	glm::quat rotationQuat = glm::rotation(enemyForward, orientationDirection);

	// Rotating model using quaternion
	gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);

	// Condition to check if the enemy model is not very close to the player
	if (glm::length(currentEnemyPosition - playerAttributes.playerPosition) >= CLOSEST_DISTANCE)
	{
		currentEnemyPosition += enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving model towards player direction
		gameEngine.MoveModel(enemyModelName, currentEnemyPosition.x, currentEnemyPosition.y, currentEnemyPosition.z);

		//gameEngine.UseManualColors(enemyModelName, true);
		//gameEngine.ChangeColor(enemyModelName, 0.85f, 0.f, 0.f); // Red
	}
	/*else
		gameEngine.UseManualColors(enemyModelName, false);*/
}

// Function to make the enemy move away from the player
void cEnemyAttributes::Flee(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes)
{
	// Condition to check if the player is close enough for the enemy to flee
	if (glm::length(currentEnemyPosition - playerAttributes.playerPosition) < MAINTAIN_DISTANCE)
	{
		glm::vec3 fleeDirection = glm::normalize(currentEnemyPosition - playerAttributes.playerPosition);

		glm::vec3 orientationDirection = glm::normalize(glm::vec3(fleeDirection.x, 0.0f, fleeDirection.z));

		glm::quat rotationQuat = glm::rotation(enemyForward, orientationDirection);

		// Rotating model using quaternion
		gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);

		currentEnemyPosition += enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving the enemy away from the player
		gameEngine.MoveModel(enemyModelName, currentEnemyPosition.x, currentEnemyPosition.y, currentEnemyPosition.z);

		gameEngine.UseManualColors(enemyModelName, true);
		gameEngine.ChangeColor(enemyModelName, 0.f, 0.f, 0.85f); // Blue
	}
	else
		gameEngine.UseManualColors(enemyModelName, false);
}

// Function to make the enemy move towards the player's future position
void cEnemyAttributes::Pursue(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes)
{
	glm::vec3 futurePlayerPosition = playerAttributes.playerPosition + playerAttributes.playerVelocity * TIME_HORIZON;

	glm::vec3 pursueDirection = glm::normalize(futurePlayerPosition - currentEnemyPosition); // Direction for movement

	glm::vec3 direction = glm::normalize(playerAttributes.playerPosition - currentEnemyPosition); // Direction for orientation

	glm::vec3 orientationDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

	glm::quat rotationQuat = glm::rotation(enemyForward, orientationDirection);

	// Rotating model using quaternion
	gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);

	if (glm::length(currentEnemyPosition - futurePlayerPosition) > 10.0f)
	{
		currentEnemyPosition += enemySpeed * pursueDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving model towards player direction
		gameEngine.MoveModel(enemyModelName, currentEnemyPosition.x, currentEnemyPosition.y, currentEnemyPosition.z);

		gameEngine.UseManualColors(enemyModelName, true);
		gameEngine.ChangeColor(enemyModelName, 0.f, 0.85f, 0.f); // Green
	}
	else
		gameEngine.UseManualColors(enemyModelName, false);
}

// Function to make the enemy move away from the player's future position
void cEnemyAttributes::Evade(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes)
{
	glm::vec3 futurePlayerPosition = playerAttributes.playerPosition + playerAttributes.playerVelocity * TIME_HORIZON;

	if (glm::length(currentEnemyPosition - futurePlayerPosition) < MAINTAIN_DISTANCE)
	{
		glm::vec3 fleeDirection = glm::normalize(currentEnemyPosition - futurePlayerPosition);

		glm::vec3 orientationDirection = glm::normalize(glm::vec3(fleeDirection.x, 0.0f, fleeDirection.z));

		glm::quat rotationQuat = glm::rotation(enemyForward, orientationDirection);

		// Rotating model using quaternion
		gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);

		currentEnemyPosition += enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving the enemy away from the player
		gameEngine.MoveModel(enemyModelName, currentEnemyPosition.x, currentEnemyPosition.y, currentEnemyPosition.z);

		gameEngine.UseManualColors(enemyModelName, true);
		gameEngine.ChangeColor(enemyModelName, 0.85f, 0.85f, 0.f); // Yellow
	}
	else
		gameEngine.UseManualColors(enemyModelName, false);
}

// Function to make the enemy maintain distance from the player
void cEnemyAttributes::Approach(cControlGameEngine& gameEngine, sPlayerAttributes& playerAttributes)
{
	float offset = 10.f;

	if (glm::length(currentEnemyPosition - playerAttributes.playerPosition) > MAINTAIN_DISTANCE + offset)
	{
		glm::vec3 direction = glm::normalize(playerAttributes.playerPosition - currentEnemyPosition);

		glm::vec3 orientationDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

		glm::quat rotationQuat = glm::rotation(enemyForward, orientationDirection);

		// Rotating model using quaternion
		gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);

		// Condition to check if the enemy model is not very close to the player
		if (glm::length(currentEnemyPosition - playerAttributes.playerPosition) > MAINTAIN_DISTANCE + offset)
		{
			currentEnemyPosition += enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

			// Moving model towards player direction
			gameEngine.MoveModel(enemyModelName, currentEnemyPosition.x, currentEnemyPosition.y, currentEnemyPosition.z);

			gameEngine.UseManualColors(enemyModelName, true);
			gameEngine.ChangeColor(enemyModelName, 0.f, 0.85f, 0.85f); // Light Blue
		}
		else
			gameEngine.UseManualColors(enemyModelName, false);
	}

	else if (glm::length(currentEnemyPosition - playerAttributes.playerPosition) < MAINTAIN_DISTANCE)
	{
		glm::vec3 fleeDirection = glm::normalize(currentEnemyPosition - playerAttributes.playerPosition);

		glm::vec3 orientationDirection = glm::normalize(glm::vec3(fleeDirection.x, 0.0f, fleeDirection.z));

		glm::quat rotationQuat = glm::rotation(enemyForward, orientationDirection);

		// Rotating model using quaternion
		gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);

		currentEnemyPosition += enemySpeed * orientationDirection * static_cast<float>(gameEngine.deltaTime);

		// Moving the enemy away from the player
		gameEngine.MoveModel(enemyModelName, currentEnemyPosition.x, currentEnemyPosition.y, currentEnemyPosition.z);

		gameEngine.UseManualColors(enemyModelName, true);
		gameEngine.ChangeColor(enemyModelName, 0.f, 0.85f, 0.85f); // Light Blue
	}
	else
		gameEngine.UseManualColors(enemyModelName, false);
}

// Function to initialize the wander behaviour
void cEnemyAttributes::InitializeWanderBehaviour(unsigned decisionTime, float radius1, float radius2, glm::vec3 wanderColor)
{
	if (this->mWanderBehaviour == NULL)
		this->mWanderBehaviour = new sWanderBehaviorAttributes(decisionTime, radius1, radius2, wanderColor);
	else
	{
		std::cout << std::endl;
		std::cout << "WANDER BEHAVIOUR ALREADY INITIALIZED" << std::endl;
	}
}

// Function to make the enemy wander in the game world region
void cEnemyAttributes::Wander(cControlGameEngine& gameEngine)
{
	this->mWanderBehaviour->wanderCountdown -= gameEngine.deltaTime;

	if (this->mWanderBehaviour->wanderCountdown <= 0.f)
	{
		this->mWanderBehaviour->wanderCountdown = this->mWanderBehaviour->wanderDecisionTime;
		
		glm::vec3 direction = this->enemyForward;
		glm::vec3 wanderPoint(this->currentEnemyPosition + direction * this->mWanderBehaviour->wanderRadius1);

		// Finding a random angle to move to/turn to
		float randomAngle = glm::linearRand(0.f, glm::two_pi<float>());

		float xPos = this->mWanderBehaviour->wanderRadius1 * glm::cos(randomAngle);
		float zPos = this->mWanderBehaviour->wanderRadius2 * glm::sin(randomAngle);

		wanderPoint.x += xPos;
		wanderPoint.z += zPos;

		this->mWanderBehaviour->wanderDirection = glm::normalize(wanderPoint - this->currentEnemyPosition);

		//--------------------Creating a sphere to indicate the wander point in game world-------------------------------
		
		this->IndicateWanderPoint(gameEngine, wanderPoint);
	}

	// Fetching the target rotation direction
	glm::quat targetRotationQuat = glm::rotation(enemyForward, this->mWanderBehaviour->wanderDirection);

	// Fetching the target original rotated direction
	glm::quat rotationQuat = gameEngine.GetModelRotationQuat(this->enemyModelName);
	
	// Fetching the interpolated quat(a rotation direction inbetween the current and target direction)
	glm::quat interpolatedQuat = glm::slerp(glm::normalize(rotationQuat), glm::normalize(targetRotationQuat), this->enemyRotationSpeed * static_cast<float>(gameEngine.deltaTime));

	rotationQuat = interpolatedQuat;

	// Rotating model using quaternion
	gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);

	this->currentEnemyPosition += enemySpeed * this->mWanderBehaviour->wanderDirection * static_cast<float>(gameEngine.deltaTime);

	float BOUNDARY_CHECK = 950.f;

	// Checking if the enemy is leaving the boundary of the game region
	if (this->currentEnemyPosition.x > BOUNDARY_CHECK || this->currentEnemyPosition.x < -BOUNDARY_CHECK || this->currentEnemyPosition.z > BOUNDARY_CHECK || this->currentEnemyPosition.z < -BOUNDARY_CHECK)
		this->currentEnemyPosition = this->initialEnemyPosition;

	// Moving model towards player direction
	gameEngine.MoveModel(enemyModelName, currentEnemyPosition.x, currentEnemyPosition.y, currentEnemyPosition.z);

	// Changing colors to distinguish the models
	if (!this->bColorChanged)
	{
		gameEngine.UseManualColors(enemyModelName, true);
		gameEngine.ChangeColor(enemyModelName, this->mWanderBehaviour->wanderVariationColor.x, this->mWanderBehaviour->wanderVariationColor.y, this->mWanderBehaviour->wanderVariationColor.z);
		
		this->bColorChanged = true;
	}
}

// Function to create spheres to indicate the wander point in the game world
void cEnemyAttributes::IndicateWanderPoint(cControlGameEngine& gameEngine, glm::vec3 wanderPoint)
{
	std::string sphereModelName = "Sphere_" + this->enemyModelName;

	this->DeleteOldPoint(gameEngine, sphereModelName);

	gameEngine.LoadModelsInto3DSpace("Sphere_1_unit_Radius_uv.ply", sphereModelName, wanderPoint.x, 10.f, wanderPoint.z);
	gameEngine.ScaleModel(sphereModelName, 4.5f);
	gameEngine.TurnMeshLightsOn(sphereModelName);
	gameEngine.TurnWireframeModeOn(sphereModelName);
	gameEngine.UseManualColors(sphereModelName, true);
	gameEngine.ChangeColor(sphereModelName, this->mWanderBehaviour->wanderVariationColor.x - 0.1, this->mWanderBehaviour->wanderVariationColor.y - 0.1, this->mWanderBehaviour->wanderVariationColor.z - 0.1);
}

// Function to delete the old sphere model in the previous wander point
void cEnemyAttributes::DeleteOldPoint(cControlGameEngine& gameEngine, std::string sphereModelName)
{
	gameEngine.DeleteMesh(sphereModelName);
}

// Function to calculate the position of the agent during flocking
void cEnemyAttributes::Flock(cControlGameEngine& gameEngine, std::vector <cEnemyAttributes*> enemyAIList)
{
	//-------------------Check Flocking Forces--------------------------

	glm::vec3 separationForce = FlockingSeparation(enemyAIList);
	glm::vec3 alignmentForce = FlockingAlignment(enemyAIList);
	glm::vec3 cohesionForce = FlockingCohesion(enemyAIList);

	this->mFlockingBehaviour->enemyAcceleration += separationForce;
	this->mFlockingBehaviour->enemyAcceleration += alignmentForce;
	this->mFlockingBehaviour->enemyAcceleration += cohesionForce;

	//--------------------Update Enemy Position-------------------------
	
	this->currentEnemyPosition += this->mFlockingBehaviour->enemyVelocity * static_cast<float>(gameEngine.deltaTime);
	this->mFlockingBehaviour->enemyVelocity += this->mFlockingBehaviour->enemyAcceleration;
	//this->mFlockingBehaviour->enemyVelocity = this->LimitMagnitude(this->mFlockingBehaviour->enemyVelocity, this->mFlockingBehaviour->enemyMaxSpeed);

	//---------------Resetting Acceleration to Zero--------------------

	this->mFlockingBehaviour->enemyAcceleration = glm::vec3(0.f);

	//----------------------Check Boundaries----------------------------

	float offsetValue = 0.5f;

	if (this->currentEnemyPosition.x < this->mFlockingBehaviour->minBoundaryX || this->currentEnemyPosition.x > this->mFlockingBehaviour->maxBoundaryX)
	{
		this->mFlockingBehaviour->enemyVelocity.x = -(this->mFlockingBehaviour->enemyVelocity.x);
		this->currentEnemyPosition += this->mFlockingBehaviour->enemyVelocity * static_cast<float>(gameEngine.deltaTime);
		
		if (this->currentEnemyPosition.x < this->mFlockingBehaviour->minBoundaryX)
			this->currentEnemyPosition.x = this->mFlockingBehaviour->minBoundaryX + offsetValue;
		
		if(this->currentEnemyPosition.x > this->mFlockingBehaviour->maxBoundaryX)
			this->currentEnemyPosition.x = this->mFlockingBehaviour->maxBoundaryX - offsetValue;
	}

	if (this->currentEnemyPosition.z < this->mFlockingBehaviour->minBoundaryZ || this->currentEnemyPosition.z > this->mFlockingBehaviour->maxBoundaryZ)
	{
		this->mFlockingBehaviour->enemyVelocity.z = -(this->mFlockingBehaviour->enemyVelocity.z);
		this->currentEnemyPosition += this->mFlockingBehaviour->enemyVelocity * static_cast<float>(gameEngine.deltaTime);

		if (this->currentEnemyPosition.z < this->mFlockingBehaviour->minBoundaryZ)
			this->currentEnemyPosition.z = this->mFlockingBehaviour->minBoundaryZ + offsetValue;

		if (this->currentEnemyPosition.z > this->mFlockingBehaviour->maxBoundaryZ)
			this->currentEnemyPosition.z = this->mFlockingBehaviour->maxBoundaryZ - offsetValue;
	}

	//---------Rotating Enemy towards the moving direction--------------

	glm::vec3 directionOfMovement = glm::normalize(this->mFlockingBehaviour->enemyVelocity);

	glm::quat targetRotationQuat = glm::rotation(this->enemyForward, directionOfMovement);
	glm::quat rotationQuat = gameEngine.GetModelRotationQuat(this->enemyModelName);
	glm::quat interpolatedQuat = glm::slerp(glm::normalize(rotationQuat), glm::normalize(targetRotationQuat), this->enemyRotationSpeed * static_cast<float>(gameEngine.deltaTime));

	rotationQuat = interpolatedQuat;

	//---------------Make changes to the mesh model---------------------

	gameEngine.MoveModel(this->enemyModelName, this->currentEnemyPosition.x, this->currentEnemyPosition.y, this->currentEnemyPosition.z);
	gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, rotationQuat);
}

// Function to calculate the flocking alignment
glm::vec3 cEnemyAttributes::FlockingAlignment(std::vector <cEnemyAttributes*> flockBuddiesList)
{
	glm::vec3 alignmentForce = glm::vec3(0.f);

	float totalEnemiesWithinRange = 0.f;

	//-----------Check distance with other flock buddies----------------

	for (cEnemyAttributes* otherFlockBuddies : flockBuddiesList)
	{
		if (otherFlockBuddies != this)
		{
			float distanceBetweenEnemies = glm::distance(this->currentEnemyPosition, otherFlockBuddies->oldEnemyPosition);

			if (distanceBetweenEnemies < this->mFlockingBehaviour->flockMaxRadius)
			{
				alignmentForce += otherFlockBuddies->mFlockingBehaviour->enemyVelocity;
				totalEnemiesWithinRange++;
			}
		}
	}

	if (totalEnemiesWithinRange > 0)
	{
		//--------------Calculating the steering velocity-------------------

		alignmentForce /= totalEnemiesWithinRange;
		alignmentForce = this->SetMagnitude(alignmentForce);
		alignmentForce -= this->mFlockingBehaviour->enemyVelocity;
		alignmentForce = this->LimitMagnitude(alignmentForce, this->mFlockingBehaviour->enemyMaxForce);
	}

	return alignmentForce;
}

// Function to calculate the flocking cohesion
glm::vec3 cEnemyAttributes::FlockingCohesion(std::vector <cEnemyAttributes*> flockBuddiesList)
{
	glm::vec3 cohesionForce = glm::vec3(0.f);

	float totalEnemiesWithinRange = 0.f;

	//-----------Check distance with other flock buddies----------------

	for (cEnemyAttributes* otherFlockBuddies : flockBuddiesList)
	{
		if (otherFlockBuddies != this)
		{
			float distanceBetweenEnemies = glm::distance(this->currentEnemyPosition, otherFlockBuddies->oldEnemyPosition);

			if (distanceBetweenEnemies < this->mFlockingBehaviour->flockMaxRadius)
			{
				cohesionForce += otherFlockBuddies->oldEnemyPosition;
				totalEnemiesWithinRange++;
			}
		}
	}

	if (totalEnemiesWithinRange > 0)
	{
		//--------------Calculating the steering velocity-------------------

		cohesionForce /= totalEnemiesWithinRange;
		cohesionForce -= this->currentEnemyPosition;
		cohesionForce = this->SetMagnitude(cohesionForce);
		cohesionForce -= this->mFlockingBehaviour->enemyVelocity;
		cohesionForce = this->LimitMagnitude(cohesionForce, this->mFlockingBehaviour->enemyMaxForce);
	}

	return cohesionForce;
}

// Function to calculate the flocking seperation
glm::vec3 cEnemyAttributes::FlockingSeparation(std::vector <cEnemyAttributes*> flockBuddiesList)
{
	glm::vec3 separationForce = glm::vec3(0.f);

	float totalEnemiesWithinRange = 0.f;

	//-----------Check distance with other flock buddies----------------

	for (cEnemyAttributes* otherFlockBuddies : flockBuddiesList)
	{
		if (otherFlockBuddies != this)
		{
			float distanceBetweenEnemies = glm::distance(this->currentEnemyPosition, otherFlockBuddies->oldEnemyPosition);

			if (distanceBetweenEnemies < this->mFlockingBehaviour->flockMaxRadius)
			{
				glm::vec3 positionDifference = this->currentEnemyPosition - otherFlockBuddies->oldEnemyPosition;
				positionDifference /= distanceBetweenEnemies;

				separationForce += positionDifference;
				totalEnemiesWithinRange++;
			}
		}
	}

	if (totalEnemiesWithinRange > 0)
	{
		//--------------Calculating the steering velocity-------------------

		separationForce /= totalEnemiesWithinRange;
		separationForce = this->SetMagnitude(separationForce);
		separationForce -= this->mFlockingBehaviour->enemyVelocity;
		separationForce = this->LimitMagnitude(separationForce, this->mFlockingBehaviour->enemyMaxForce);
	}

	return separationForce;
}

// Function to set the agent
glm::vec3 cEnemyAttributes::SetMagnitude(glm::vec3 steeringVelocity)
{
	//float velocityX = steeringVelocity.x;
	//float velocityZ = steeringVelocity.z;

	//double magnitude = sqrt(velocityX * velocityX + velocityZ * velocityZ);

	//double scaleFactor = this->mFlockingBehaviour->enemyMaxSpeed / magnitude;

	//steeringVelocity.x *= scaleFactor;
	//steeringVelocity.z *= scaleFactor;

	float currentMagnitude = glm::length(steeringVelocity);

	if (currentMagnitude != 0)
		steeringVelocity = steeringVelocity * (this->mFlockingBehaviour->enemyMaxSpeed / currentMagnitude);

	return steeringVelocity;
}

// Function to limit the magnitude of the agent's speed
glm::vec3 cEnemyAttributes::LimitMagnitude(glm::vec3 steeringVelocity, float maxSpeed)
{
	float magnitudeSquared = glm::dot(steeringVelocity, steeringVelocity);

	if (magnitudeSquared > maxSpeed * maxSpeed)
	{
		float scaleFactor = maxSpeed / sqrt(magnitudeSquared);
		steeringVelocity *= scaleFactor;
	}

	return steeringVelocity;
}

// Function to initialize the parameters required by an agent for flocking process
void cEnemyAttributes::InitializeFlockingAttributes(glm::vec3 velocity, glm::vec3 acceleration, float flockRadius, float maxForce, float boundaryValue)
{
	if(this->mFlockingBehaviour == NULL)
		this->mFlockingBehaviour = new sFlockingBehaviourAttributes(velocity, acceleration, this->enemySpeed, flockRadius, maxForce, boundaryValue);
}

//---------------------------------------------------------PROJECT 04-----------------------------------------------------------------------------------------

// Function to initialize the parametes required by an agent for path following
void cEnemyAttributes::InitializePathFollowingAttributes(float closeToEndPointRadius, float pathRadius, 
	float futurePositionPredictionDistance, float maximumSpeed, std::string futurePosSphere, std::string projectedPosSphere)
{
	if (this->mPathFollowBehaviour == NULL)
		this->mPathFollowBehaviour = new sPathFollowingBehaviourAttributes(closeToEndPointRadius, pathRadius, futurePositionPredictionDistance, maximumSpeed, futurePosSphere, projectedPosSphere);
}

// Function to allow agents to follow a specified path
void cEnemyAttributes::FollowPath(cControlGameEngine& gameEngine, std::vector < sPath >& PathList, std::vector < sDangerZone >& DangerZoneList, sPlayerAttributes& player)
{
	if (!PathList.empty() || !DangerZoneList.empty())
	{
		if (this->IsPlayerInsideInnerCircle(player, DangerZoneList))
		{
			if(!this->mPathFollowBehaviour->bChangePath)
				this->mPathFollowBehaviour->bChangePath = true;
			
			this->Seek(gameEngine, player);
		}
		else if (this->IsPlayerInsideOuterCircle(player, DangerZoneList))
		{
			if (!this->mPathFollowBehaviour->bChangePath)
				this->mPathFollowBehaviour->bChangePath = true;

			this->FacePlayer(player, gameEngine);
		}
		else
		{
			if (this->mPathFollowBehaviour->bChangePath)
			{
				//--------------------------Check for new path to follow----------------------------------------

				this->mPathFollowBehaviour->currentPathFollowed = this->CheckForClosestPath(this->currentEnemyPosition, PathList);

				this->mPathFollowBehaviour->bChangePath = false;
			}
			else
			{
				//-------------------Predicting the future position of the enemy-----------------------------------

				glm::vec3 futureEnemyPosition = this->PredictFuturePosition(static_cast <float> (gameEngine.deltaTime));

				// Moving debug sphere to future enemy position
				gameEngine.MoveModel(this->mPathFollowBehaviour->futurePosSphereName, futureEnemyPosition.x, 5.f, futureEnemyPosition.z);

				float distance = glm::distance(this->mPathFollowBehaviour->currentPathFollowed.mToPointB, futureEnemyPosition);

				if (distance > 0)
				{
					//----Calculating the scalar projection onto the path from the future position-----------------

					glm::vec3 enemyProjectedPositionInPath = this->FindEnemyProjectionOntoPath(this->mPathFollowBehaviour->currentPathFollowed.mFromPointA,
						futureEnemyPosition, this->mPathFollowBehaviour->currentPathFollowed.mToPointB);

					// Moving debug sphere to projected position in the path
					gameEngine.MoveModel(this->mPathFollowBehaviour->projectedPathPosSphereName, enemyProjectedPositionInPath.x, 5.f, enemyProjectedPositionInPath.z);

					//-----Calculate distance between future enemy position and scalar projected position on path--------

					float projectionDistance = glm::distance(futureEnemyPosition, enemyProjectedPositionInPath);

					//----------------------Seeking the projected postition in the path----------------------------

					if (projectionDistance > glm::distance(this->mPathFollowBehaviour->currentPathFollowed.mFromPointA, this->mPathFollowBehaviour->currentPathFollowed.mToPointB))
					{
						glm::vec3 steeringForce = this->SeekTheTarget(this->mPathFollowBehaviour->currentPathFollowed.mToPointB);

						this->enemyAcceleration += steeringForce;
					}
					else if (projectionDistance > this->mPathFollowBehaviour->pathRadius)
					{
						glm::vec3 steeringForce = this->SeekTheTarget(enemyProjectedPositionInPath);

						this->enemyAcceleration += steeringForce;
					}

					//--------------------------------Calculating velocity-----------------------------------------

					this->enemyVelocity += this->enemySpeed * this->enemyAcceleration * static_cast <float> (gameEngine.deltaTime);

					float fullSpeed = glm::length(this->enemyVelocity);

					//-------------Checking if velocity is crossing the min/max speed threshold---------------------

					if (fullSpeed > this->mPathFollowBehaviour->maxSpeed)
						this->enemyVelocity = glm::normalize(this->enemyVelocity) * this->mPathFollowBehaviour->maxSpeed;
					else if (fullSpeed < this->enemySpeed)
						this->enemyVelocity = glm::normalize(this->enemyVelocity) * this->enemySpeed;

					//-------------------------Calculating position-------------------------------------------------

					this->currentEnemyPosition += this->enemyVelocity * static_cast <float> (gameEngine.deltaTime);

					//------------------------Resetting acceleration------------------------------------------------

					this->enemyAcceleration = glm::vec3(0.f);

					//------------------Rotating Enemy towards the moving direction--------------------------------

					glm::vec3 directionOfMovement = glm::normalize(this->enemyVelocity);

					glm::quat targetRotationQuat = glm::rotation(this->enemyForward, directionOfMovement);
					glm::quat rotationQuat = gameEngine.GetModelRotationQuat(this->enemyModelName);
					glm::quat interpolatedQuat = glm::slerp(glm::normalize(rotationQuat), glm::normalize(targetRotationQuat), this->enemyRotationSpeed * static_cast<float>(gameEngine.deltaTime));

					//-------------------------Make changes to the mesh model--------------------------------------

					gameEngine.MoveModel(this->enemyModelName, this->currentEnemyPosition.x, this->currentEnemyPosition.y, this->currentEnemyPosition.z);
					gameEngine.RotateMeshModelUsingQuaternion(enemyModelName, interpolatedQuat);
				}

				//-----------Change path if enemy reaches destination-----------------

				if (distance <= this->mPathFollowBehaviour->destinationRadius)
					this->mPathFollowBehaviour->bChangePath = true;
			}
		}
	}
	else
		std::cout << "ERROR : PATH LIST/DANGER ZONE LIST IS NOT PREDEFINED" << std::endl << std::endl;
}

// Function to calculate the distance and return the closest path for the agent
sPath& cEnemyAttributes::CheckForClosestPath(glm::vec3 enemyCurrentPosition, std::vector < sPath > PathList)
{
	float distance = 0.f;

	sPath pathToBeTaken = PathList[0];

	for (int index = 0; index < PathList.size(); index++)
	{
		float tempDistance = glm::distance(PathList[index].mFromPointA, enemyCurrentPosition);

		if (distance == 0.f)
			distance = tempDistance;
		else
		{
			if (tempDistance < distance)
			{
				distance = tempDistance;

				pathToBeTaken = PathList[index];
			}
		}
	}

	return pathToBeTaken;
}

// Function to calculate the distance and return the closest path for the agent
glm::vec3 cEnemyAttributes::PredictFuturePosition(float deltaTime)
{
	glm::vec3 tempFutureVelocity = glm::normalize(this->enemyVelocity) * this->mPathFollowBehaviour->futurePredictionDistance;

	glm::vec3 tempFuturePosition = this->currentEnemyPosition + tempFutureVelocity;

	return tempFuturePosition;
}

// Function to calculate the scalar projection of agent's future position onto path
glm::vec3 cEnemyAttributes::FindEnemyProjectionOntoPath(glm::vec3 initialPathPos, glm::vec3 enemyCurrentPosition, glm::vec3 pathEndPos)
{
	glm::vec3 vector1 = enemyCurrentPosition - initialPathPos;
	glm::vec3 vector2 = glm::normalize(pathEndPos - initialPathPos);

	float pathDotProduct = glm::dot(vector1, vector2);

	glm::vec3 enemyProjectedPositionInPath;

	if (pathDotProduct < 0)
		enemyProjectedPositionInPath = pathEndPos;
	else
	{
		float scalarProjection = glm::dot(vector2, vector1);

		vector2 *= scalarProjection;

		enemyProjectedPositionInPath = vector2 + initialPathPos;
	}

	return enemyProjectedPositionInPath;
}

// Function to seek a specified target
glm::vec3 cEnemyAttributes::SeekTheTarget(glm::vec3 target)
{
	glm::vec3 desiredVelocity = glm::normalize(target - this->currentEnemyPosition) * this->enemySpeed;

	glm::vec3 steeringForce = desiredVelocity - this->enemyVelocity;

	return steeringForce;
}

// Function to get the debug sphere name
std::string cEnemyAttributes::GetFuturePosSphereName()
{
	return this->mPathFollowBehaviour->futurePosSphereName;
}

// Function to get the debug sphere name
std::string cEnemyAttributes::GetProjectedPathPosSphereName()
{
	return this->mPathFollowBehaviour->projectedPathPosSphereName;
}

// Function to make agents face the player
void cEnemyAttributes::FacePlayer(sPlayerAttributes& player, cControlGameEngine& gameEngine)
{
	glm::vec3 directionToPlayer = glm::normalize(player.playerPosition - this->currentEnemyPosition);

	glm::quat targetRotationQuat = glm::rotation(this->enemyForward, directionToPlayer);
	glm::quat rotationQuat = gameEngine.GetModelRotationQuat(this->enemyModelName);
	glm::quat interpolatedQuat = glm::slerp(glm::normalize(rotationQuat), glm::normalize(targetRotationQuat), this->enemyRotationSpeed * static_cast<float>(gameEngine.deltaTime));

	gameEngine.RotateMeshModelUsingQuaternion(this->enemyModelName, interpolatedQuat);
}

// Function to check if player is inside outer circle
bool cEnemyAttributes::IsPlayerInsideOuterCircle(sPlayerAttributes& player, std::vector < sDangerZone > dangerZoneList)
{
	for (int index = 0; index < dangerZoneList.size(); index++)
	{
		float distance = glm::distance(player.playerPosition, dangerZoneList[index].outerCirclePosition);

		if (distance <= dangerZoneList[index].outerCircleRadius)
			return true;
	}

	return false;
}

// Function to check if player is inside inner circle
bool cEnemyAttributes::IsPlayerInsideInnerCircle(sPlayerAttributes& player, std::vector < sDangerZone > dangerZoneList)
{
	for (int index = 0; index < dangerZoneList.size(); index++)
	{
		float distance = glm::distance(player.playerPosition, dangerZoneList[index].innerCirclePosition);

		if (distance <= dangerZoneList[index].innerCircleRadius)
			return true;
	}

	return false;
}