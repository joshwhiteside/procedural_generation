#pragma once

#include "../../nclgl/Vector3.h"
#include "../../nclgl/Matrix4.h"

class RigidBody {

	//<---------LINEAR-------->>
	Vector3 m_position;
	Vector3 m_linearVelocity;
	Vector3 m_force;
	float	m_invMass;

	//<---------ANGULAR------->>
	Matrix4 m_orientation;
	Vector3	m_angularVelocity;
	Vector3 m_torque;
	Matrix4 m_invInertia;

};