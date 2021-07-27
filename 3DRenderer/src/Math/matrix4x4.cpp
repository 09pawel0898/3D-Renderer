#include "../pch.h"
#include "matrix4x4.h"

Matrix4x4::Matrix4x4(char axis,float angle)
{ 
	axis = std::tolower(axis);
	switch (axis)
	{
		case 'x': this->make_X_rotation_matrix(angle); break;
		case 'y': this->make_Y_rotation_matrix(angle); break;
		case 'z': this->make_Z_rotation_matrix(angle); break;
	}
}

void Matrix4x4::make_translation(Vec3f &trans)
{
	make_identity_matrix();
	m[3][0] = trans.x;
	m[3][1] = trans.y;
	m[3][2] = trans.z;
}

void Matrix4x4::make_identity_matrix()
{
	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;
}

void Matrix4x4::make_X_rotation_matrix(const float alpha)
{
	m[0][0] = 1;
	m[1][1] = cosf(alpha);
	m[1][2] = -sinf(alpha);
	m[2][1] = sinf(alpha);
	m[2][2] = cosf(alpha);
}

void Matrix4x4::make_Y_rotation_matrix(const float alpha)
{
	m[0][0] = cosf(alpha);
	m[0][2] = sinf(alpha);
	m[1][1] = 1;
	m[2][0] = -sinf(alpha);
	m[2][2] = cosf(alpha);
}

void Matrix4x4::make_Z_rotation_matrix(const float alpha)
{
	m[0][0] = cosf(alpha);
	m[0][1] = -sinf(alpha);
	m[1][0] = sinf(alpha);
	m[1][1] = cosf(alpha);
	m[2][2] = 1;
}