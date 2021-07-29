#pragma once

#include "../Math/Utilities.h"

using namespace math;

class Camera
{
private: 
	Vec3f mVUp = { 0,1,0 };
	Vec3f mVLookAt;			// camera look at vector
	Vec3f mVTarget = { 0,0,1 };

	Vec3f mPosition;			// position in world space
	float mY_yaw , mX_yaw ;		// x and y deviation
	Matrix4x4 mMatCameraRotYX;

public:
	explicit Camera(Vec3f position);
	void handle_camera_event(const sf::Keyboard::Key& key, bool keyPressed);
	const Matrix4x4& get_camera_view_matrix(void);

private:
	Matrix4x4& get_camera_rotation_matrix(float dt);	// returns translation matrix that represents camera rotation 
	Matrix4x4& rotate_camera_to_mouse_pos(float dt);

public:
	Vec3f& get_position(void)				{ return mPosition;  }
	const Vec3f& get_position(void) const	{ return mPosition;  }
	const Vec3f& get_VLookAt(void) const	{ return mVLookAt; }
	const Vec3f& get_VTarget(void) const	{ return mVTarget; }
	const Vec3f& get_Vup(void) const		{ return mVUp; }
};
