#include "pch.h"

#include "cControlGameEngine.h"
#include "PlayerAttributes.h"
#include "cJsonWriter.h"
#include "sMouseAttributes.h"

extern cControlGameEngine gameEngine;

extern cJsonWriter jsonWriter;

const float POSITION_OFFSET = 0.0f;

extern sPlayerAttributes playerAttributes;

extern void MakeDebugSpheresVisible(cControlGameEngine& gameEngine);

cMesh* controlMeshModel;

sMouseAttributes mouseAttributes;

static std::map<int, bool> keyPressedStateMap;
static std::map<int, bool> keyReleasedStateMap;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    float ROTATION_DEGREES = 5.0f;

    const float CAMERA_SPEED = 300.0f;

    const float MODEL_MOVE_SPEED = 1.0f;

    float CAMERA_MOVEMENT = CAMERA_SPEED * gameEngine.deltaTime;

    glm::vec3 CAMERA_TARGET = gameEngine.GetCurrentCameraTarget();
    glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 CAMERA_POSITION = gameEngine.GetCurrentCameraPosition();

    //---------------Controls for Mesh Models----------------------------------------------------------

    //if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    //{
    //    controlMeshModel = gameEngine.GetCurrentModelSelected();

    //    std::string currentModelName = controlMeshModel->friendlyName;

    //    glm::vec3 currentModelPosition = gameEngine.GetModelPosition(currentModelName);

    //    float currentModelScaleValue = gameEngine.GetModelScaleValue(currentModelName);

    //    if (key == GLFW_KEY_W && action) // Decrement model's Z position
    //    {
    //        gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y, currentModelPosition.z - MODEL_MOVE_SPEED);
    //    }
    //    if (key == GLFW_KEY_A && action) // Decrement model's X position
    //    {
    //        gameEngine.MoveModel(currentModelName, currentModelPosition.x - MODEL_MOVE_SPEED, currentModelPosition.y, currentModelPosition.z);
    //    }
    //    if (key == GLFW_KEY_S && action) // Increment model's Z position
    //    {
    //        gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y, currentModelPosition.z + MODEL_MOVE_SPEED);
    //    }
    //    if (key == GLFW_KEY_D && action) // Increment model's X position
    //    {
    //        gameEngine.MoveModel(currentModelName, currentModelPosition.x + MODEL_MOVE_SPEED, currentModelPosition.y, currentModelPosition.z);
    //    }
    //    if (key == GLFW_KEY_Q && action) // Increment model's Y position 
    //    {
    //        gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y + MODEL_MOVE_SPEED, currentModelPosition.z);
    //    }
    //    if (key == GLFW_KEY_E && action) // Decrement model's Y position
    //    {
    //        gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y - MODEL_MOVE_SPEED, currentModelPosition.z);
    //    }
    //    if (key == GLFW_KEY_I && action) // Rotate model's X axis - 5 degrees
    //    {
    //        gameEngine.RotateMeshModel(currentModelName, ROTATION_DEGREES, 1.f, 0.f, 0.f);
    //    }
    //    if (key == GLFW_KEY_O && action) // Rotate model's Y axis - 5 degrees
    //    {
    //        gameEngine.RotateMeshModel(currentModelName, ROTATION_DEGREES, 0.f, 1.f, 0.f);
    //    }
    //    if (key == GLFW_KEY_P && action) // Rotate model's Z axis - 5 degrees
    //    {
    //        gameEngine.RotateMeshModel(currentModelName, ROTATION_DEGREES, 0.f, 0.f, 1.f);
    //    }
    //    if (key == GLFW_KEY_C && action) // Decrement model's scale value
    //    {
    //        gameEngine.ScaleModel(currentModelName, currentModelScaleValue - 0.01f);
    //    }
    //    if (key == GLFW_KEY_V && action) // Increment model's scale value
    //    {
    //        gameEngine.ScaleModel(currentModelName, currentModelScaleValue + 0.01f);
    //    }
    //    if (key == GLFW_KEY_Z && action) // Shift to previous model
    //    {
    //        gameEngine.ShiftToPreviousMeshInList();
    //    }
    //    if (key == GLFW_KEY_X && action) // Shift to next model
    //    {
    //        gameEngine.ShiftToNextMeshInList();
    //    }
    //    if (key == GLFW_KEY_B && action) // Change to wireframe mode
    //    {
    //        gameEngine.TurnWireframeModeOn(currentModelName);
    //    }
    //}

    if (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
        {
            keyPressedStateMap[key] = true;
        }

        if (action == GLFW_RELEASE)
        {
            keyPressedStateMap[key] = false;
        }
    }

    if (mods == 0)
    {
        if (key == GLFW_KEY_L && action) // Key to write model enries to file
        {
            jsonWriter.WriteDataToTextFile(gameEngine);
        }

        if (key == GLFW_KEY_I && action) // Key to make debug spheres invisible/visible
        {
            MakeDebugSpheresVisible(gameEngine);
        }
    
        if (key == GLFW_KEY_P && action) // Key to shift camera mode
        {
            gameEngine.ShiftCameraView();

            if (gameEngine.IsFreeFlowCamOn()) // Make cursor visible and display the game world from the 1st camera showcase angle
            {
                CAMERA_POSITION = glm::vec3(0.0f, 50.0f, 300.0f);
                gameEngine.MoveCameraTarget(0.0f, 0.0f, -1.0f);
                gameEngine.yaw = -90.0f;
                gameEngine.pitch = 0.f;
                mouseAttributes.resetMouseMoved();

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else // Make cursor invisible and recenter the cursor to the center of the screen
            {
                int screenWidth, screenHeight;

                glfwGetWindowSize(window, &screenWidth, &screenHeight);

                glfwSetCursorPos(window, screenWidth/2, screenHeight/2);

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }

        if (!gameEngine.IsFreeFlowCamOn()) // Keys to be pressed when in 3rd person camera mode
        {
            //--------------------------------------------------------Functions that happen when keys are pressed----------------------------------------------------------------------------------------------

            //if (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D)
            //{
            //    float forwardX = -sin(glm::radians(playerAttributes.playerYaw));
            //    float forwardZ = -cos(glm::radians(playerAttributes.playerYaw));

            //    glm::vec3 forwardDirection = glm::vec3(forwardX, 0.0f, forwardZ);

            //    printf("Something Pressed !!\n");

            //    if (action == GLFW_PRESS || action == GLFW_REPEAT)
            //    {
            //        if (key == GLFW_KEY_W)
            //        {
            //            printf("W Pressed\n");
            //            playerAttributes.playerMovingDirection = forwardDirection * playerAttributes.playerMovementSpeed;

            //            playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move forward
            //            playerAttributes.playerDirection = ePlayerDirection::FRONT;
            //        }

            //        if (key == GLFW_KEY_S)
            //        {
            //            printf("S Pressed\n");
            //            playerAttributes.playerMovingDirection = -forwardDirection * playerAttributes.playerMovementSpeed;

            //            playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move backward
            //            playerAttributes.playerDirection = ePlayerDirection::BACK;
            //        }

            //        if (key == GLFW_KEY_A)
            //        {
            //            printf("A Pressed\n");
            //            playerAttributes.playerMovingDirection = -glm::cross(forwardDirection, glm::vec3(0.0f, 1.0f, 0.0f)) * playerAttributes.playerMovementSpeed;

            //            playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move left
            //            playerAttributes.playerDirection = ePlayerDirection::LEFT;
            //        }

            //        if (key == GLFW_KEY_D)
            //        {
            //            printf("D Pressed\n");

            //            playerAttributes.playerMovingDirection = glm::cross(forwardDirection, glm::vec3(0.0f, 1.0f, 0.0f)) * playerAttributes.playerMovementSpeed;

            //            playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move right
            //            playerAttributes.playerDirection = ePlayerDirection::RIGHT;
            //        }
            //    }

                //printf("PlayerForward : (%f, %f, %f)\n", forwardDirection.x, forwardDirection.y, forwardDirection.z);

                //printf("PlayerVelocity : (%f, %f, %f)\n", playerAttributes.playerVelocity.x, playerAttributes.playerVelocity.y, playerAttributes.playerVelocity.z);



                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                {
                    float forwardX = -sin(glm::radians(playerAttributes.playerYaw));
                    float forwardZ = -cos(glm::radians(playerAttributes.playerYaw));

                    glm::vec3 forwardDirection = glm::vec3(forwardX, 0.0f, forwardZ);

                    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    {
                        playerAttributes.playerMovingDirection = forwardDirection * playerAttributes.playerMovementSpeed;

                        playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move forward
                        playerAttributes.playerDirection = ePlayerDirection::FRONT;
                    }

                    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    {

                        playerAttributes.playerMovingDirection = -forwardDirection * playerAttributes.playerMovementSpeed;

                        playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move backward
                        playerAttributes.playerDirection = ePlayerDirection::BACK;
                    }

                    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    {

                        playerAttributes.playerMovingDirection = -glm::cross(forwardDirection, glm::vec3(0.0f, 1.0f, 0.0f)) * playerAttributes.playerMovementSpeed;

                        playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move left
                        playerAttributes.playerDirection = ePlayerDirection::LEFT;
                    }

                    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    {
                        playerAttributes.playerMovingDirection = glm::cross(forwardDirection, glm::vec3(0.0f, 1.0f, 0.0f)) * playerAttributes.playerMovementSpeed;

                        playerAttributes.playerVelocity = playerAttributes.playerMovingDirection; // Move right
                        playerAttributes.playerDirection = ePlayerDirection::RIGHT;
                    }

                    //printf("PlayerForward : (%f, %f, %f)\n", forwardDirection.x, forwardDirection.y, forwardDirection.z);

                    //printf("PlayerVelocity : (%f, %f, %f)\n", playerAttributes.playerVelocity.x, playerAttributes.playerVelocity.y, playerAttributes.playerVelocity.z);

                }

             /*   if (action == GLFW_RELEASE)
                {
                    if (key == GLFW_KEY_W)
                    {
                        printf("W Released\n");

                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }

                    if (key == GLFW_KEY_S)
                    {
                        printf("S Released\n");;

                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }

                    if (key == GLFW_KEY_A)
                    {
                        printf("A Released\n");

                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }

                    if (key == GLFW_KEY_D)
                    {
                        printf("D Released\n");

                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }
                }*/
//}

            //--------------------------------------------------------Functions that happen when keys are released---------------------------------------------------------------------------------------------

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
            {
                if (playerAttributes.playerDirection == ePlayerDirection::FRONT)
                {
                    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
                    {
                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }
                }
                else if (playerAttributes.playerDirection == ePlayerDirection::BACK)
                {
                    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
                    {
                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }
                }
                else if (playerAttributes.playerDirection == ePlayerDirection::LEFT)
                {
                    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
                    {
                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }
                }
                else if (playerAttributes.playerDirection == ePlayerDirection::RIGHT)
                {
                    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
                    {
                        playerAttributes.playerVelocity = glm::vec3(0.f);
                    }
                }
            }
        }

        if (gameEngine.IsFreeFlowCamOn()) // Keys to be pressed when in free-flow cam mode
        {
            //------------------------------Camera Showcases------------------------------------------------

            if (key == GLFW_KEY_1 && action)
            {
                CAMERA_POSITION = glm::vec3(0.0f + POSITION_OFFSET, 50.0f + POSITION_OFFSET, 300.0f + POSITION_OFFSET);
                gameEngine.MoveCameraTarget(0.0f, 0.0f, -1.0f);
                gameEngine.yaw = -90.0f;
                gameEngine.pitch = 0.f;
                mouseAttributes.resetMouseMoved();
            }

            if (key == GLFW_KEY_2 && action)
            {
                CAMERA_POSITION = glm::vec3(-290.0f + POSITION_OFFSET, 70.0f + POSITION_OFFSET, 0.f + POSITION_OFFSET);
                gameEngine.MoveCameraTarget(1.f, -0.06f, 0.03f);
                gameEngine.yaw = -358.241f;
                gameEngine.pitch = -3.65f;
                mouseAttributes.resetMouseMoved();
            }

            if (key == GLFW_KEY_3 && action)
            {
                CAMERA_POSITION = glm::vec3(-4.25f + POSITION_OFFSET, 55.f + POSITION_OFFSET, -280.f + POSITION_OFFSET);
                gameEngine.MoveCameraTarget(-0.0115f, 0.009f, 1.0f);
                gameEngine.yaw = -269.345f;
                gameEngine.pitch = 0.5f;
                mouseAttributes.resetMouseMoved();
            }

            if (key == GLFW_KEY_4 && action)
            {
                CAMERA_POSITION = glm::vec3(270.f + POSITION_OFFSET, 53.f + POSITION_OFFSET, 6.f + POSITION_OFFSET);
                gameEngine.MoveCameraTarget(-1.f, -0.016f, -0.035f);
                gameEngine.yaw = -177.998f;
                gameEngine.pitch = -0.9f;
                mouseAttributes.resetMouseMoved();
            }

            if (key == GLFW_KEY_5 && action)
            {
                CAMERA_POSITION = glm::vec3(3.05f, 895.f, 200.f);
                gameEngine.MoveCameraTarget(-0.0012f, -0.98f, -0.22f);
                gameEngine.yaw = -90.3f;
                gameEngine.pitch = -77.55f;
                mouseAttributes.resetMouseMoved();
            }

            //------------------------------Move Camera--------------------------------------------------------

            if (key == GLFW_KEY_W && action)
            {
                CAMERA_POSITION += CAMERA_MOVEMENT * CAMERA_TARGET;
            }
            if (key == GLFW_KEY_A && action)
            {
                CAMERA_POSITION -= glm::normalize(glm::cross(CAMERA_TARGET, CAMERA_UP)) * CAMERA_MOVEMENT;
            }
            if (key == GLFW_KEY_S && action)
            {
                CAMERA_POSITION -= CAMERA_MOVEMENT * CAMERA_TARGET;
            }
            if (key == GLFW_KEY_D && action)
            {
                CAMERA_POSITION += glm::normalize(glm::cross(CAMERA_TARGET, CAMERA_UP)) * CAMERA_MOVEMENT;
            }
            if (key == GLFW_KEY_Q && action)
            {
                CAMERA_POSITION += CAMERA_MOVEMENT * CAMERA_UP;
            }
            if (key == GLFW_KEY_E && action)
            {
                CAMERA_POSITION -= CAMERA_MOVEMENT * CAMERA_UP;
            }
            if (key == GLFW_KEY_SPACE && action) // Key to pause mouse movement
            {
                if (mouseAttributes.bEnableMouseMovement)
                    mouseAttributes.bEnableMouseMovement = false;
                else
                    mouseAttributes.bEnableMouseMovement = true;

                mouseAttributes.resetMouseMoved();
            }

            gameEngine.MoveCameraPosition(CAMERA_POSITION.x, CAMERA_POSITION.y, CAMERA_POSITION.z);
        }
    }

    return;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    int screenWidth, screenHeight;

    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    // Free-Flow Camera View
    if (gameEngine.IsFreeFlowCamOn())
    {
        if (mouseAttributes.bEnableMouseMovement)
        {
            //------------------------Initializing Mouse Parameters--------------------------------------

            glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 CAMERA_POSITION = gameEngine.GetCurrentCameraPosition();

            float xOffset = 0.0f;
            float yOffset = 0.0f;

            //----------------Calculating Cursor Restrictions in X and Y Plane---------------------------

            if (xPos >= screenWidth - 1 || xPos <= 0) // Rotate Cam when mouse moves to the edge of the screen(left or right)
            {
                xOffset = 0.0f;

                if (xPos <= 0)
                    xOffset = -10.0f;
                else if (xPos >= screenWidth - 1)
                    xOffset = 10.0f;

                gameEngine.yaw += mouseAttributes.mouseSensitivity * xOffset;
            }

            if (yPos >= screenHeight - 1 || yPos <= 20) // Rotate Cam when mouse moves to the edge of the screen(up or down)
            {
                yOffset = 0.0f;

                if (yPos <= 20)
                    yOffset = 10.0f;
                else if (yPos >= screenHeight - 1)
                    yOffset = -10.0f;

                gameEngine.pitch += mouseAttributes.mouseSensitivity * yOffset;
            }

            //-----------------------Calculating the last X and Y Pos-------------------------------------

            if (mouseAttributes.bMouseMoved)
            {
                mouseAttributes.freeCamLastX = xPos;
                mouseAttributes.freeCamLastY = yPos;
                mouseAttributes.bMouseMoved = false;
            }

            //-------------------Calculating offsets with XY Coordinates----------------------------------

            xOffset = xPos - mouseAttributes.freeCamLastX;
            yOffset = mouseAttributes.freeCamLastY - yPos; //Reversed Y

            xOffset *= mouseAttributes.mouseSensitivity;
            yOffset *= mouseAttributes.mouseSensitivity;

            //----------------------Calculating Camera's Yaw and Pitch-----------------------------------

            gameEngine.yaw += xOffset;
            gameEngine.pitch += yOffset;

            //-------------------------Restricting Vertical Movements------------------------------------

            if (gameEngine.pitch > 90.0f)
                gameEngine.pitch = 90.0f;

            if (gameEngine.pitch < -90.0f)
                gameEngine.pitch = -90.0f;

            //----------------------------Calculating Camera Front---------------------------------------

            glm::vec3 front;

            front.x = cos(glm::radians(gameEngine.yaw)) * cos(glm::radians(gameEngine.pitch));
            front.y = sin(glm::radians(gameEngine.pitch));
            front.z = sin(glm::radians(gameEngine.yaw)) * cos(glm::radians(gameEngine.pitch));

            glm::vec3 cameraFront = glm::normalize(front);

            gameEngine.MoveCameraTarget(cameraFront.x, cameraFront.y, cameraFront.z);

            //-------------------------Updating Last Mouse Position--------------------------------------

            mouseAttributes.freeCamLastX = xPos;
            mouseAttributes.freeCamLastY = yPos;
        }
    }

    // Player's 3rd person view
    else 
    {
        //-----------------------Calculating the last X and Y Pos------------------------------------

        mouseAttributes.TPV_LastX = screenWidth / 2;
        mouseAttributes.TPV_LastY = screenHeight / 2;

        //-------------------Calculating offsets with XY Coordinates---------------------------------

        double xOffset = screenWidth / 2 - xPos;
        double yOffset = screenHeight / 2 - yPos;

        //----------------------Calculating Player's Yaw and Pitch-----------------------------------

        playerAttributes.playerYaw += xOffset * mouseAttributes.mouseSensitivity * 2.5f;
        playerAttributes.playerPitch += yOffset * mouseAttributes.mouseSensitivity;

        //------------------------Allowing 360 Horizontal Movements----------------------------------

        if (playerAttributes.playerYaw > mouseAttributes.TPV_maxYaw)
            playerAttributes.playerYaw -= mouseAttributes.TPV_maxYaw;

        else if (playerAttributes.playerYaw < mouseAttributes.TPV_minYaw)
            playerAttributes.playerYaw += mouseAttributes.TPV_maxYaw;

        //-------------------------Restricting Vertical Movements------------------------------------

        if (playerAttributes.playerPitch > mouseAttributes.TPV_maxPitch)
            playerAttributes.playerPitch = mouseAttributes.TPV_maxPitch;

        else if (playerAttributes.playerPitch < -mouseAttributes.TPV_maxPitch)
            playerAttributes.playerPitch = -mouseAttributes.TPV_maxPitch;

        //------------------Recenter the Cursor to the Center of the Screen--------------------------

        glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);

        //-------------------------Updating Last Mouse Position--------------------------------------

        mouseAttributes.TPV_LastX = screenWidth / 2;
        mouseAttributes.TPV_LastY = screenHeight / 2;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Scroll to zoom in 3rd person cam view
    if (!gameEngine.IsFreeFlowCamOn())
    {
        gameEngine.cameraDistance -= yoffset * mouseAttributes.scrollSpeed;
        gameEngine.cameraDistance = glm::clamp(gameEngine.cameraDistance, mouseAttributes.minZoomDistance, mouseAttributes.maxZoomDistance);
    }
}