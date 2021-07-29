#pragma once

class Mesh;
class Camera;

#include "../Math/Utilities.h"
#include "Mesh.h"

using namespace math;

class Object : public sf::Drawable
{
public:
	std::unique_ptr<Mesh> mMesh;
	std::shared_ptr<Camera> mCamera;

public:
	explicit Object(std::shared_ptr<Camera> cam);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;             
	Vec3f& get_position(void)				{ return mPosition; }
	const Vec3f& get_position(void) const	{ return mPosition; }

private:
	Matrix4x4 mMatTrans;
	Vec3f mPosition;								// object position
	RotationType mRotation = RotationType::NONE;    // object rotation type

	void draw_mesh(sf::RenderTarget& target) const;
	void clip_against_near_plane(std::vector<Triangle>& trisToClip, Triangle& triangleProjected, Triangle& triangleViewed, const Triangle& triangleTranslated, unsigned& clippedTris) const ;
	void clip_against_window_edges(std::list<Triangle>& trisToProject, const Triangle& triangle, unsigned& clippedTris) const;
	void scale_triangle_into_screen(Triangle& triangle) const ; 

	// clips triangle against given plane point and its normal, returns number od triangles produced from clipping 
	std::tuple<uint8_t, Triangle, Triangle> clip_triangle(Vec3f planePoint, Vec3f planeNormal, Triangle& triangleToClip, unsigned& clippedTris) const;

};
