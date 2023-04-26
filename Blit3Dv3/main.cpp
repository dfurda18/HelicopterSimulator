//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"

#include "JoystickState.h"

#include "SceneNodes.h"
#include "HelicopterNode.h"
#include "Camera.h"

Blit3D *blit3D = NULL;

//GLOBAL DATA
GLSLProgram* prog = NULL;
SceneManager *sceneManager = NULL;
MeshManager* meshManager = NULL;
SceneNode* currentNode = NULL;

// Nodes
TranslaterNode* t2 = NULL;
RotatorNode* r1 = NULL;
FloaterNode* f1 = NULL;

HelicopterNode* helicopter = NULL;
MeshNode* terrain = NULL;
MeshNode* buildings = NULL;

// The camera
Camera* camera;

// Other variables
float elapsedTime = 0;
const float timeSlice = 1.f / 60.f;
bool stripped = false;

// joystick controller
bool foundJoystick = false;
int joystickNumber;
JoystickState joystickState;
std::mutex joystickMutex;

bool PollJoystick(int joystickNumber, JoystickState& joystickState)
{
	if (!glfwJoystickPresent(joystickNumber)) return false;

	//first fetch axis states
	joystickState.axisStates = glfwGetJoystickAxes(joystickNumber, &joystickState.axisCount);
	//now fetch button states
	joystickState.buttonStates = glfwGetJoystickButtons(joystickNumber, &joystickState.buttonCount);

	return true;
}

float Clamp(float value, float minAbs)
{
	return abs(value) < minAbs ? 0 : value;
}

void FindJoyStick()
{
	for (joystickNumber = GLFW_JOYSTICK_1; joystickNumber <= GLFW_JOYSTICK_LAST; ++joystickNumber)
	{
		if (glfwJoystickPresent(joystickNumber))
		{
			foundJoystick = true;
			break;
		}
	}

	if (foundJoystick)
	{
		if (!PollJoystick(joystickNumber, joystickState)) foundJoystick = false;
	}
}

/**
* This method is called when starting the program.
*/
void Init()
{
	// Global objects
	sceneManager = new SceneManager();
	meshManager = new MeshManager(blit3D);
	blit3D->SetMode(Blit3DRenderMode::BLIT3D);
	prog = blit3D->sManager->UseShader("lighting.vert", "lighting.frag"); //load/compile/link

	//3d perspective projection
	blit3D->projectionMatrix = glm::perspective(45.0f, (GLfloat)(blit3D->screenWidth) / (GLfloat)(blit3D->screenHeight), 0.1f, 10000.0f);

	prog->setUniform("projectionMatrix", blit3D->projectionMatrix);
	prog->setUniform("viewMatrix", blit3D->viewMatrix);
	prog->setUniform("modelMatrix", glm::mat4(1.f));

	//lighting variables
	glm::vec3 LightPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 LightIntensity = glm::vec3(1.0f, 1.0f, 1.0f);

	//send lighting info to the shader
	prog->setUniform("LightPosition", LightPosition);
	prog->setUniform("LightIntensity", LightIntensity);

	//send alpha to the shader
	prog->setUniform("in_Alpha", 1.f);

	//attributes
	prog->bindAttribLocation(0, "in_Position");
	prog->bindAttribLocation(1, "in_Normal");
	prog->bindAttribLocation(2, "in_Texcoord");

	prog->printActiveUniforms();
	prog->printActiveAttribs();

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Helicopter and camera
	helicopter = new HelicopterNode(sceneManager->sceneGraph, meshManager, "Data/Helicopter.s3d", vec3(0.f, 0.f, 0.f));
	camera = new Camera(helicopter, glm::vec3(100.f, 30.f, 0.f));

	// Add the terrain
	t2 = new TranslaterNode(sceneManager->sceneGraph, glm::vec3(0.f, -200.0f, 0.f));
	r1 = new RotatorNode(t2, glm::vec3(1.f, 0.f, 0.f), -90);
	terrain = new MeshNode(t2, "Data/Terrain.s3d", meshManager);
	buildings = new MeshNode(t2, "Data/Buildings.s3d", meshManager);

}
/**
* This method is called when exiting the program.
*/
void DeInit(void)
{
	// Delete the objects in the scene
	if (sceneManager != NULL) delete sceneManager;

	// Thelete all the meshes
	if (meshManager != NULL) delete meshManager;

	// Delete the camera
	if (camera != NULL) delete camera;
	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}
/**
* This method is called every x seconds.
*/
void Update(double seconds)
{
	if (seconds > 0.15) elapsedTime += 0.15f; //prevent lag spike
	else elapsedTime += static_cast<float>(seconds);

	while (elapsedTime >= timeSlice)
	{
		elapsedTime -= timeSlice;
		// Update all the elements
		sceneManager->UpdateWorld(timeSlice);
		camera->Update(timeSlice);
	}
	
}

void Draw(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);	//clear colour: r,g,b,a 

	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//move camera
	blit3D->viewMatrix = camera->viewMatrix;
	prog->setUniform("viewMatrix", blit3D->viewMatrix);

	//draw stuff here
	sceneManager->DrawWorld();
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence
}

void DoJoystick()
{
	if (foundJoystick)
	{
		if (PollJoystick(joystickNumber, joystickState))
		{
			joystickMutex.lock();
			if (joystickState.axisCount > 0) helicopter->roll = helicopter->landed ? 0.f : Clamp(joystickState.axisStates[0], 0.1);
			if (joystickState.axisCount > 1) helicopter->pitch = helicopter->landed ? 0.f : -Clamp(joystickState.axisStates[1], 0.1);
			if (joystickState.axisCount > 2) helicopter->heading = helicopter->landed ? 0.f : -Clamp(joystickState.axisStates[2], 0.1);
			//if (joystickState.axisCount > 3) helicopter->roll = -joystickState.axisStates[3];
			if (joystickState.axisCount > 5) helicopter->powerUpdate = Clamp(- joystickState.axisStates[5], 0.1);
			joystickMutex.unlock();
		}
		else foundJoystick = false;
	}
	else
	{
		joystickMutex.lock();
		FindJoyStick();
		joystickMutex.unlock();
	}
}

void DoFileDrop(int count, const char** files)
{
	// Not used here
}

int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	// look for the joystick number
	FindJoyStick();

	blit3D = new Blit3D(Blit3DWindowModel::DECORATEDWINDOW, 1024, 1024);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	blit3D->SetDoJoystick(DoJoystick);
	blit3D->SetDoFileDrop(DoFileDrop);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}