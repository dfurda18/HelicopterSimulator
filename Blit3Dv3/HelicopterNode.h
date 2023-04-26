#pragma once

#include "SceneManager.h"
#include "SceneNodes.h"
#include "Mesh.h"

/**
* This class Represents an Helicopter node. It handles all of its interactions.
* @author Dario Urdapilleta
* @version 1.0
* @since 04/04/2023
*/
class HelicopterNode : public SceneNode
{
private:
	// Configurable constants
	/**
	* The amount of power tha Helicopter increases per second.
	*/
	const float POWER_DELTA = 4.f;
	/**
	* The max power.
	*/
	const float MAX_POWER = 12.f;
	/**
	* The min power.
	*/
	const float MIN_POWER = 0.f;
	/**
	* Top speed
	*/
	const float TOP_SPEED = 200.f;
	/**
	* The min height.
	*/
	const float MIN_HEIGHT = 0.f;
	/**
	* The max height.
	*/
	const float MAX_HEIGHT = 500.f;
	/**
	* Terminal Velocity
	*/
	const float TERMINAL_VELOCITY = 3500.f;
	/**
	* The amount of angle per second the wheels can turn.
	*/
	const float HEADING_DELTA = 0.5f;
	/**
	* The amount forward is scaled when rolling to a side
	*/
	const float FORWARD_MODIFIER = 50.f;
	
	
	/**
	* The speed lost per second when not accelerating.
	*/
	const float FRICTION = 200;
	/**
	* The angle amount the torret is turning per second.
	*/
	const float TORRET_DELTA = 40.f;
	/**
	* Amodifier to return the wheels back to their place when not turning them.
	*/
	const float RETURN_WHEEL_VALUE = 9000.f;
	/**
	* The angle amount the gun is turning per second.
	*/
	const float GUN_DELTA = 5.f;
	/**
	* The max gun angle. This values are reversed. The positive move the gun down.
	*/
	const float MAX_GUN_ANGLE = 6.f;
	/**
	* The min amount for the gun angle.
	*/
	const float MIN_GUN_ANGLE = -4.f;
	/**
	* A constant that define how well the tires can rotate the tank.
	*/
	const float TIRE_TRACTION = 3.f;
	/**
	*  The Gravity vector
	*/
	const glm::vec3 gravity = glm::vec3(0.f, -9.8f, 0.f);

public:
	/**
	* Whether or not the helicopter has landed
	*/
	boolean landed;
	/**
	* The mesh manager.
	*/
	MeshManager* mManager = NULL;
	/**
	* The Node mesh
	*/
	std::string meshFile;
	/**
	* The vector with the distance needed to place the Node to its proper location.
	*/
	glm::vec3 traslation;
	// Controls
	/**
	* The gas level
	*/
	float power;
	/**
	* The gas level
	*/
	float powerUpdate;
	/**
	* The amount the helicopter is moving up and down
	*/
	float pitch;
	/**
	* The amount the helicopter is inclined to the left or right
	*/
	float roll;
	/**
	* The amount the helicopter is turning on the y axis
	*/
	float heading;
	// Values
	/**
	* The Helicopter's position.
	*/
	glm::vec3 position;
	/**
	* The up vector
	*/
	glm::vec3 up;
	/**
	* The vector the up vector is heading torwards
	*/
	glm::vec3 torwards;
	/**
	* The forward vector
	*/
	glm::vec3 forward;
	/**
	* Acceleration
	*/
	glm::vec3 acceleration;
	/**
	* The helicopter's angle.
	*/
	float angle;
	
	/**
	* The default HelicopterNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HelicopterNode();
	/**
	* The HelicopterNode constructor.
	* @param ParentNode The Parent node the APC is instanced from.
	* @param meshManager The Mesh Manager.
	* @param meshFile The main helicopter mesh file
	* @param traslation The mesh needed traslations
	* @param the main body mesh file
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HelicopterNode(SceneNode* ParentNode, MeshManager* meshManager, std::string meshFile, glm::vec3 traslation);
	/**
	* Updates the Helicopter.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
	/**
	* Creates all the Helicopter's children nodes.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void CreateChildren();
};