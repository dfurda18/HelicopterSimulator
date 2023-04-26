#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "HelicopterNode.h"
#include "HelixNode.h"
#include "Heading2Quat.h"

HelicopterNode::HelicopterNode()
{
	typeId = sceneNodeType::HELICOPTER;
	modelMatrix = glm::mat4(1.f);
	power = 0.f;
	powerUpdate = 0.f;
	pitch = 0.f;
	roll = 0.f;
	heading = 0.f;
	position = vec3(0.f, 0.f, 0.f);
	up = vec3(0.f, 1.0f, 0.f);
	torwards = vec3(0.f, 1.0f, 0.f);
	acceleration = vec3(0.f, 0.f, 0.f);
	angle = 0.f;
	landed = true;
}

HelicopterNode::HelicopterNode(SceneNode* ParentNode, MeshManager* meshManager, std::string meshFile, glm::vec3 traslation)
{
	typeId = sceneNodeType::HELICOPTER;
	this->meshFile = meshFile;
	this->traslation = traslation;
	mManager = meshManager;
	modelMatrix = glm::mat4(1.f);
	power = 0.f;
	powerUpdate = 0.f;
	pitch = 0.f;
	roll = 0.f;
	heading = 0.f;
	position = vec3(0.f, 0.f, 0.f);
	up = vec3(0.f, 1.0f, 0.f);
	torwards = vec3(0.f, 1.0f, 0.f);
	acceleration = vec3(0.f, 0.f, 0.f);
	angle = 0.f;
	landed = true;

	forward = vec3(0.f, 0.f, 0.f);

	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
	CreateChildren();
}

void HelicopterNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	// Handle the input
	power += powerUpdate * POWER_DELTA * seconds;
	if (power > MAX_POWER)
	{
		power = MAX_POWER;
	}
	if (power < MIN_POWER)
	{
		power = MIN_POWER;
	}

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	glm::quat headingMatrix = glm::quat(1.f, 0.f, 0.f, 0.f);

	// Spin the helicopter only when the height is more then 0
	if (!landed)
	{
		this->angle += this->heading * HEADING_DELTA;
		if (this->angle > 360.f)
		{
			this->angle -= 360.f;
		}
		if (this->angle < -360.f)
		{
			this->angle += 360.f;
		}

		
	}

	// Calculate the up vector
	torwards = glm::vec3(this->pitch, 3.0f, this->roll);
	torwards = glm::normalize(torwards);
	torwards = glm::rotate(torwards, glm::radians(this->angle), glm::vec3(0.f, 1.0f, 0.f));

	headingMatrix = GenerateQuatFrom2Vectors(up, torwards);

	// Move torwards the up vector and gravity
	acceleration += gravity + (torwards * power);

	if (landed)
	{
		acceleration.x = 0.f;
		acceleration.z = 0.f;
	}

	if (acceleration.y < -TERMINAL_VELOCITY)
	{
		acceleration.y = -TERMINAL_VELOCITY;
	}
	float speed = glm::length(acceleration);
	if (speed > TOP_SPEED)
	{
		acceleration = glm::normalize(acceleration) * TOP_SPEED;
		speed = TOP_SPEED;
	}
	this->position += acceleration * (float)seconds;
	if (position.y <= MIN_HEIGHT)
	{
		acceleration.y = MIN_HEIGHT;
		position.y = MIN_HEIGHT;
		this->landed = true;
	}
	else {
		this->landed = false;
	}

	if (position.y > MAX_HEIGHT)
	{
		acceleration.y = 0.f;
		position.y = MAX_HEIGHT;
	}

	float angleInRadians = glm::radians(this->angle);
	// Calculate the forward vector
	forward.x = std::cos(-angleInRadians);
	forward.z = std::sin(-angleInRadians);
	forward.y = 0.f;

	// scale forward for the camera
	if (this->roll != 0) {
		forward *= abs(this->roll) * FORWARD_MODIFIER;
	}

	
	// Transform the node's matrix
	modelMatrix = glm::translate(modelMatrix, this->position);
	if (!landed)
	{
		modelMatrix = modelMatrix * glm::mat4_cast(headingMatrix);
		
	}
	modelMatrix = glm::rotate(modelMatrix, angleInRadians, vec3(0.f, 1.0f, 0.f));

	speed = speed = glm::length(acceleration);
	if (speed > 0)
	{
		acceleration = glm::normalize(acceleration) * speed;
	}
	
	snodeMutex.unlock();

	
	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

void HelicopterNode::CreateChildren()
{
	// The hull mesh
	TranslaterNode* t1 = new TranslaterNode(this, traslation);
	RotatorNode* r1 = new RotatorNode(t1, vec3(0.f, 1.0f, 0.f), 180);
	ScaleNode* s1 = new ScaleNode(r1, vec3(0.05f, 0.05f, 0.05f));
	MeshNode* m1 = new MeshNode(s1, meshFile, mManager);

	// The main helix mesh
	HelixNode* mainHelix = new HelixNode(t1, "Data/MainHelix.s3d", vec3(0.f, 12.5f, 0.f), vec3(0.f, 1.0f, 0.f), mManager, this);

	// The back Helix
	HelixNode* backHelix = new HelixNode(r1, "Data/BackHelix.s3d", vec3(38.75f, 2.95f, 0.f), vec3(0.f, 0.f, 1.f), mManager, this);


}