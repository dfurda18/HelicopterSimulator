#pragma once

#ifndef GL_FRAMEWORK__INCLUDED
#define GL_FRAMEWORK__INCLUDED

//#define _USING_GLEW_

#ifdef _WIN32 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef _USING_GLEW_
#include <gl/glew.h>
#else
#include <gl/gl.h>
#endif
#else
#include <gl/gl.h>
#endif

#endif

#include <glm/glm.hpp> //GLM OpenGL math library
#include <glm/gtc/type_ptr.hpp> //used to covert glm data to OpenGL data
#include <glm/gtc/quaternion.hpp> //quaternions for handling rotation
#include < glm/gtc/matrix_transform.hpp > //transformation functiona ala OpenGL
#include < glm/gtx/norm.hpp> //for length2

#include <cmath>

glm::mat4 GenerateMat4FromDirectionVector(glm::vec3 vDirection)
{
	// Step 1. Setup basis vectors describing the rotation given the input vector and assuming an initial up direction of (0, 1, 0)
	glm::vec3 vUp(0.f, 1.0f, 0.0f);           // Y Up vector            
	glm::vec3 vRight = glm::cross(vUp, vDirection);    // The perpendicular vector to Up and Direction            
	vUp = glm::cross(vDirection, vRight);            // The actual up vector given the direction and the right vector
	// Step 2. Put the three vectors into the matrix to bulid a basis rotation matrix            

	glm::mat4 mBasis(vRight.x, vRight.y, vRight.z, 0.0f,
		vUp.x, vUp.y, vUp.z, 0.0f,
		vDirection.x, vDirection.y, vDirection.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mBasis;
}


glm::quat GenerateQuatFrom2Vectors(glm::vec3 u, glm::vec3 v)
{
	float norm_u_norm_v = 1.f;// set to  sqrt(glm::dot(u, u) * glm::dot(v, v)) instead if u&v not normalized
	float real_part = norm_u_norm_v + glm::dot(u, v);
	glm::vec3 w;

	if (real_part < 1.e-6f * norm_u_norm_v)
	{
		/* If u and v are exactly opposite, rotate 180 degrees
		 * around an arbitrary orthogonal axis. Axis normalisation
		 * can happen later, when we normalise the quaternion. */
		real_part = 0.0f;
		w = abs(u.x) > abs(u.z) ? glm::vec3(-u.y, u.x, 0.f)
			: glm::vec3(0.f, -u.z, u.y);
	}
	else
	{
		/* Otherwise, build quaternion the standard way. */
		w = glm::cross(u, v);
	}

	return glm::normalize(glm::quat(real_part, w.x, w.y, w.z));
}

glm::quat GenerateQuatFromDirectionVector(glm::vec3 vDirection)
{
	return GenerateQuatFrom2Vectors(glm::vec3(0, 0, 1), vDirection);
}



//alternative ways to do this, mostly tested.
// 
//glm::vec3 orthogonal(glm::vec3 v)
//{
//	float x = abs(v.x);
//	float y = abs(v.y);
//	float z = abs(v.z);
//
//	glm::vec3 other = x < y ? (x < z ? glm::vec3(1, 0, 0) : glm::vec3(0, 0, 1)) : (y < z ? glm::vec3(0, 1, 0) : glm::vec3(0, 0, 1));
//	return glm::cross(v, other);
//}

//glm::quat GenerateQuatFrom2Vectors(glm::vec3 u, glm::vec3 v)
//{
//	// It is important that the inputs are of equal length when
//	// calculating the half-way vector.
//	u = glm::normalize(u);
//	v = glm::normalize(v);
//
//	// Unfortunately, we have to check for when u == -v, as u + v
//	// in this case will be (0, 0, 0), which cannot be normalized.
//	if (u == -v)
//	{
//		// 180 degree rotation around any orthogonal vector
//		return glm::quat(0.f, glm::normalize(orthogonal(u)));
//	}
//
//	glm::vec3 half = glm::normalize(u + v);
//	return glm::quat(glm::dot(u, half), glm::cross(u, half));
//}

//glm::quat GenerateQuatFrom2Vectors(glm::vec3 u, glm::vec3 v)
//{
//	float k_cos_theta = glm::dot(u, v);
//	float k = sqrt(glm::length2(u) * glm::length2(v));
//
//	if (k_cos_theta / k == -1)
//	{
//		// 180 degree rotation around any orthogonal vector
//		return glm::quat(0, glm::normalize(orthogonal(u)));
//	}
//
//	return glm::normalize(glm::quat(k_cos_theta + k, glm::cross(u, v)));
//}

//glm::quat align(glm::quat Q, glm::vec3 u, glm::vec3 v)
//{
//	glm::quat U = glm::quat(0, u.x, u.y, u.z);
//	glm::quat V = glm::quat(0, v.x, v.y, v.z);
//	return glm::normalize(length(U * V) * Q - V * Q * U);
//}
//
//glm::quat fromVectors(glm::vec3 u, glm::vec3 v)
//{
//	float d = glm::dot(u, v);
//	glm::vec3 w = glm::cross(u, v);
//
//	glm::quat r = glm::quat(d + sqrt(d * d + glm::dot(w, w)), w);
//	if (r.w != 0) return glm::normalize(r);
//	else return r;
//}