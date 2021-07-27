#pragma once

#include "Vector.h"
#include "Matrix4x4.h"
#include "Triangle.h"

namespace math
{
	/* calculate normal and returns true if the triangle ought to be visible, false otherwise */
	//bool calculateNormal(Triangle& triangleTranslated, const Camera& cam);

	// returns dot product of two vectors
	float dot_product(const Vec3f& vec1, const Vec3f& vec2);	

	// returns distance from plane to point in 3d space
	float distance_from_plane_to_point(const Vec3f& plane_point, Vec3f& plane_normal, const Vec3f& point);	

	// returns a cross product of two vectors 
	Vec3f cross_product(const Vec3f& vec1, const Vec3f& vec2);	

	// returns a point of plane and line intersection
	Vec3f vector_intersect_plane(Vec3f& line_start, Vec3f& line_end, Vec3f& plane_point, Vec3f& plane_normal);
	
	
	Matrix4x4 point_at_matrix(const Vec3f& pos, const Vec3f& forward, const  Vec3f& up);	// returns a pointAtMatrix depending on camera position
	Matrix4x4 inverse_point_at_matrix(const Matrix4x4& matrix);	// returns inversed instance of pointAtMatrix

	// returns a triangle translated by a matrix
	Triangle translate_triangle_by_matrix(const Triangle& tri, const Matrix4x4& matrix);

	// clips triangle against given plane point and its normal, returns number od triangles produced from clipping 
	int clip_triangle(Vec3f plane_point, Vec3f plane_normal, Triangle& input, Triangle& out1, Triangle& out2, unsigned& clipped_tris_counter, bool clip_hint);
}