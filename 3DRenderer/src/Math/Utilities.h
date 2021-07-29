#pragma once

#include "Vector.h"
#include "Matrix4x4.h"
#include "Triangle.h"

namespace math
{
	// calculate normal and returns true if the triangle ought to be visible, false otherwise
	bool calculate_normal(Triangle& triangleTranslated, const Vec3f& cameraPos);

	// returns dot product of two vectors
	inline float dot_product(const Vec3f& vec1, const Vec3f& vec2) { return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z; }
	
	// returns distance from plane to point in 3d space
	float distance_from_plane_to_point(const Vec3f& plane_point, Vec3f& plane_normal, const Vec3f& point);	

	// returns a cross product of two vectors 
	Vec3f cross_product(const Vec3f& vec1, const Vec3f& vec2);	

	// returns a point of plane and line intersection
	Vec3f vector_intersect_plane(const Vec3f& line_start, const Vec3f& line_end, const Vec3f& plane_point, Vec3f& plane_normal);
	
	// returns a pointAtMatrix depending on camera position
	Matrix4x4 point_at_matrix(const Vec3f& pos, const Vec3f& forward, const  Vec3f& up);	
	
	// returns inversed instance of pointAtMatrix
	Matrix4x4 inverse_point_at_matrix(const Matrix4x4& matrix);	

	// returns a triangle translated by a matrix
	Triangle translate_triangle_by_matrix(const Triangle& tri, const Matrix4x4& matrix);

	// returns absolute value of a float
	inline float absf(float val) {	return (val >= 0) ? val : -val;	}

	// initializes projection matrix with appropriate values
	void init_projection_matrix(Matrix4x4& matrix, int windowHeight, int windowWidth);
}