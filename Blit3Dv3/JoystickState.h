#pragma once

class JoystickState
{
public:
	int axisCount; //how many axes there are for this joystick...yes, "axes" is the plural of "axis", I know it looks funny
	const float* axisStates; //array of axisCount floats, between -1 and 1 in value
	int buttonCount; //how many buttons there are for this joystick
	const unsigned char* buttonStates; //array of buttonCount unsigned chars, will either be GLFW_PRESS or GLFW_RELEASE in value
};
