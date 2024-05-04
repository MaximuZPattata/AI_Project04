<<--------------------------------------------------------------------------------Artificial_Intelligence_Project_04------------------------------------------------------------------------------------------>>

# Developers :
	- This project was done by Max Sherwin(1128059)

## About Project :
	- The project reads the scene contents( i.e the model properties, light properties, etc) from the json file located in "Artificial_Intelligence_Project_04->GameWorldConstruct->SceneDescription.json".

	- In the scene I have placed 2 Beholders and 1 controllable character(Spiderman). Controls are given below.
 
	- I have specified(hardcoded) the paths the Beholders should follow in the scene. Also specified the dangerZones(inner/outer circle) that the player can use to manipulate the Beholders to pursue player.

	- The Beholders follow the Craig Reynold's theory for path following and follow the specified path. I have also enabled debug spheres to display the future agent position and to display the projected 	position onto the path from the future predicted position.
	
	- The player(Spiderman) can move into the circle placed around the path to make the enemy agents either face the player/pursue the player. 
	
	- I have added 2 camera modes(Free flow camera and 3rd person camera). Controls given below.
 	
	- I've also added a video demo explaining the simulation and code part.

### How to build and run : 
	- The solution "Artificial_Intelligence_Project_04.sln" has to be built(either in debug or release platform mode) and then once succeeded(probably will!), make the "GameWorldConstruct" project as the startup project and now it'll be able to run for some visual treat.
	
	- When building the solution, the "MaxGameEngine" project should be run first since it is a static project without which the project with contents(GameWorldConstruct) cannot be run.
	
#### Controls :
		- "I" key for enabling and disabling debug spheres
		- "P" key for shifting camera mode

	<<Free-flow camera mode>>
		- W,A,S,D,Q,E for camera movement(Front, Left, Back, Right, Up and Down respectively)
		- Mouse movement for camera rotation
		- Spacebar to pause and resume mouse mouvement(by default it is set to pause, so click on Spacebar when the scene starts to rotate camera with mouse)
		- 1, 2, 3, 4, 5 buttons are default camera showcase angles.

	<<Third person camera mode>>
		- W,A,S,D for player movement
		- Mouse movement for camera rotation
		- Mouse scroll button to zoom further in and out(towards/away from the player)
	
	
