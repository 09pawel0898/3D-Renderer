#pragma once

#include "Vector.h"

namespace math
{
	class Matrix4x4
	{
	public:
		float m[4][4] = { 0 };	// array that represents 4x4 matrix

		Matrix4x4() {}
		Matrix4x4(char axis, float angle);
		
		void make_translation(const Vec3f& trans);				// used for translating point in space by 3d vector 
		void make_identity_matrix(void);					// makes identity matrix 
		void make_X_rotation_matrix(float angle);			// default rotation around X axis 
		void make_Y_rotation_matrix(float angle);			// default rotation around Y axis 
		void make_Z_rotation_matrix(float angle);			// default rotation around Z axis 
	
	private:
		Matrix4x4(const std::string& type);
		static const Matrix4x4 Identity;

	public:
		inline static const Matrix4x4& identity_matrix(void){	return Identity;	}
		static Vec3f multiply_matrix_by_vec(const Matrix4x4& matrix, const Vec3f& v);				// returns a vector multiplied by a matrix
		static Matrix4x4 multiply_matrix_by_matrix(const Matrix4x4& mat1, const Matrix4x4& mat2);	// returns a matrix produced by matrix by matrix multiplication
	};
}
