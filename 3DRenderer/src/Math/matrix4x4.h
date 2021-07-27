#pragma once

#include "vector.h"

class Matrix4x4
{
private:
	float m[4][4] = { 0 };	// array that represents 4x4 matrix

public:
	Matrix4x4() {}
	Matrix4x4(char rot, float alpha);
	void make_translation(Vec3f &trans);				/* used for translating point in space by 3d vector */

private:
	/* private utility */
	void make_identity_matrix(void);					/* makes identity matrix */
	void make_X_rotation_matrix(const float alpha);		/* default rotation around X axis */
	void make_Y_rotation_matrix(const float alpha);		/* default rotation around Y axis */
	void make_Z_rotation_matrix(const float alpha);		/* default rotation around Z axis */
};
