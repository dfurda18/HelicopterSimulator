#pragma once

#include "SceneManager.h"
#include "HelixNode.h"

/**
* This class Represents a node that spinds the wheel.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class HelixSpinningNode : public SceneNode
{
public:
	/**
	* The axis to rotate the torret.
	*/
	glm::vec3 axis;
	/**
	* The rotation angle.
	*/
	float angle;
	/**
	* A pointer to the WheelNode
	*/
	HelixNode* helix;
	/**
	* The default HelixSpinningNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HelixSpinningNode();
	/**
	* The HelixSpinningNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param axis The axis to spin the sheel.
	* @param helixNode The HelixNode that contains this node.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HelixSpinningNode(SceneNode* ParentNode, glm::vec3 axis, HelixNode* helixNode);
	/**
	* Updates the Helix rotating logic.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
};