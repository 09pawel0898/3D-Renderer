#include "../pch.h"
#include "Matrix4x4.h"
#include "Utilities.h"

namespace math
{
	Matrix4x4::Matrix4x4(char axis, float angle)
	{
		axis = std::tolower(axis);
		switch (axis)
		{
		case 'x': this->make_X_rotation_matrix(angle); break;
		case 'y': this->make_Y_rotation_matrix(angle); break;
		case 'z': this->make_Z_rotation_matrix(angle); break;
		}
	}

	void Matrix4x4::make_translation(Vec3f& trans)
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

	Vec3f Matrix4x4::multiply_matrix_by_vec(const Matrix4x4& matrix, const Vec3f& v)
	{
		Vec3f output;
		// i got 3d vector and a 4x4 matrix on the input, so what i need is to expand input vector by adding fourth elementwith value 1
		output.x = (float)(v.x * matrix.m[0][0] + v.y * matrix.m[1][0] + v.z * matrix.m[2][0] + 1.0 * matrix.m[3][0]);
		output.y = (float)(v.x * matrix.m[0][1] + v.y * matrix.m[1][1] + v.z * matrix.m[2][1] + 1.0 * matrix.m[3][1]);
		output.z = (float)(v.x * matrix.m[0][2] + v.y * matrix.m[1][2] + v.z * matrix.m[2][2] + 1.0 * matrix.m[3][2]);

		// and i got one extra coordinate in the output that i will need just for dividing the others by it before projection
		output.w = (float)(v.x * matrix.m[0][3] + v.y * matrix.m[1][3] + v.z * matrix.m[2][3] + matrix.m[3][3]);
		return output;
	}

	Matrix4x4 Matrix4x4::multiply_matrix_by_matrix(const Matrix4x4& mat1, const Matrix4x4& mat2)
	{
		Matrix4x4 result;
		for (uint8_t i = 0; i < 4; i++)
		{
			for (uint8_t j = 0; j < 4; j++)
			{
				result.m[i][j] = 0;
				for (uint8_t k = 0; k < 4; k++)
				{
					result.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
				}
			}
		}
		return result;
	}
}