#include "HelixNode.h"
#include "HelicopterNode.h"
#include "HelixSpinningNode.h"
#include "SceneNodes.h"

HelixNode::HelixNode()
{
	typeId = sceneNodeType::HELICOPTER;
	modelMatrix = glm::mat4(1.f);
	parent = NULL;

}

HelixNode::HelixNode(SceneNode* ParentNode, std::string meshFile, glm::vec3 traslation, glm::vec3 axis, MeshManager* meshManager, HelicopterNode* helicopter)
{
	// Set the initial values.
	typeId = sceneNodeType::HELICOPTER;
	mManager = meshManager;
	this->meshFile = meshFile;
	this->traslation = traslation;
	this->axis = axis;
	this->helicopter = helicopter;
	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
	CreateChildren();
}

void HelixNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

void HelixNode::CreateChildren()
{
	// Create the children
	TranslaterNode* t1 = new TranslaterNode(this, this->traslation);
	HelixSpinningNode* hs1 = new HelixSpinningNode(t1, this->axis, this);
	ScaleNode* s1 = new ScaleNode(hs1, vec3(0.05f, 0.05f, 0.05f));
	MeshNode* m1 = new MeshNode(s1, this->meshFile, mManager);

}

float HelixNode::GetSpeed()
{
	return this->helicopter->power * POWER_FACTOR;
}
