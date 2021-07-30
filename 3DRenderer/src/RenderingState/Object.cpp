#include "../pch.h"
#include "Object.h"
#include "Camera.h"
#include "RenderingState.h"
#include "../App.h"

using RenderingState = States::RenderingState;

Object::Object(std::shared_ptr<Camera> cam)
	:	mMesh(std::make_unique<Mesh>()),
        mCamera(cam),
        mPosition(Vec3f(0, 0, 30))
{
        // initializing translation matrix with invoked object's position
        mMatTrans.make_translation(mPosition); 
}

void Object::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    draw_mesh(target);
}

void Object::draw_mesh(sf::RenderTarget & target) const
{
    std::vector<Triangle> trisToClip;           // vector to store triangles created after first clipping
    unsigned clippedTrisCounter = 0;            // counter of triangles created by clipping

    Matrix4x4 matView = mCamera->get_camera_view_matrix();  // computing camera view matrix 

    /* rotating mesh */
    //mMesh->rotate(RotationType::NONE, matWorld, matTrans);

    // this loop need optimizations mostly
    for (const auto& triangle : mMesh->get_triangles())
    {
        Triangle triangleProjected, triangleTranslated, triangleViewed; // declaring triangles that represents single state

        triangleTranslated = translate_triangle_by_matrix(triangle, mMatTrans);
        if (calculate_normal(triangleTranslated, mCamera->get_position()))
        {
            triangleViewed = translate_triangle_by_matrix(triangleTranslated, matView);                                     // conversion from world space to viewed triangle 
            clip_against_near_plane(trisToClip, triangleProjected, triangleViewed, triangleTranslated, clippedTrisCounter); // clipping against the fNear plane, i store newly created polygons in a vector
        }
    }

    // sorting triangles from the farthest to the closest by averaged Z component 
    //mMesh->sort_triangles(trisToClip);

    // clipping already two dimensional triangles againts window edges
    unsigned drawnTriangles = 0;
    unsigned triId = 0;

    RenderingState::Vertices.clear();
    RenderingState::Vertices.resize(mMesh->mTrisCount * 3);

    for (const auto& triangle : trisToClip)
    {
        std::list<Triangle> trisToProject;

        // adding initial triangle
        trisToProject.push_back(triangle);
        clip_against_window_edges(trisToProject, triangle, clippedTrisCounter);

        // drawing every triangle created from clipping from single triangle from tris_to_clip
        for (auto& i : trisToProject)
        {
            drawnTriangles++;
            mMesh->draw_triangle(i, target, RenderingState::MeshVisible, *mCamera, triId);
            triId++;
        }
    }
    target.draw(RenderingState::Vertices);

    // update label info
    RenderingState::ClippedPolygons = clippedTrisCounter;
    RenderingState::DrawnPolygons = drawnTriangles;
}

void Object::clip_against_near_plane(std::vector<Triangle>& trisToClip, Triangle& triangleProjected, Triangle& triangleViewed, const Triangle& triangleTranslated, unsigned& clippedTris) const
{
    const static struct Near
    {
        Vec3f point = { 0.0f,0.0f,0.1f };
        Vec3f normal = { 0.0f,0.0f,1.0f };
    }nearPlane;

    Triangle newClipped[2];
    int numProducedTrisFromClipping;

    std::tie(numProducedTrisFromClipping, newClipped[0], newClipped[1]) = 
        clip_triangle(nearPlane.point, nearPlane.normal, triangleViewed, clippedTris);

    for (int i = 0; i < numProducedTrisFromClipping; i++)
    {
        triangleProjected = translate_triangle_by_matrix(newClipped[i], RenderingState::ProjectionMatrix);
        triangleProjected.normal = triangleTranslated.normal;
        triangleProjected.color = triangleViewed.color;

        // after doing matrix transformations i got float x,y,z values of vectors in range between 0 and 1, before drawing i have to scale
        // it properly to my window 
        scale_triangle_into_screen(triangleProjected);
        trisToClip.push_back(std::move(triangleProjected));
    }
}

void Object::clip_against_window_edges(std::list<Triangle>& trisToProject, const Triangle& triangle, unsigned& clippedTris) const
{
    Triangle newClipped[2];
    unsigned newTris = 1;

    struct Plane
    {
        Vec3f point, normal;
    };

    static const Plane planes[4] =
    {
        {{ 0.0f, 0.0f, 0.0f },              { 0.0f, 1.0f, 0.0f }},
        {{ 0.0f, App::HEIGHT + 1, 0.0f },   { 0.0f, -1.0f, 0.0f }},
        {{ 0.0f, 0.0f, 0.0f },              { 1.0f, 0.0f, 0.0f }},
        {{ App::WIDTH + 1, 0.0f, 0.0f },    { -1.0f, 0.0f, 0.0f }}
    };

    std::for_each(planes, planes + 4, [&](const Plane& plane) 
    {
            uint8_t trisToAdd = 0;
            while (newTris > 0)
            {
                Triangle triToClip = trisToProject.front();
                trisToProject.pop_front();
                newTris--;

                std::tie(trisToAdd, newClipped[0], newClipped[1]) =
                    clip_triangle(plane.point, plane.normal, triToClip, clippedTris);
                    
                for (uint8_t i = 0; i < trisToAdd; i++)
                {
                    newClipped[i].normal = triangle.normal;
                    trisToProject.push_back(std::move(newClipped[i]));
                }
            }
            newTris = trisToProject.size();
    });
}

void Object::scale_triangle_into_screen(Triangle& triangle) const
{
    static const Vec3f shift_view = { 1,1,0 };

    triangle.v[0].x *= -1.0f;
    triangle.v[1].x *= -1.0f;
    triangle.v[2].x *= -1.0f;
    triangle.v[0].y *= -1.0f;
    triangle.v[1].y *= -1.0f;
    triangle.v[2].y *= -1.0f;
    triangle.v[0] = triangle.v[0] + shift_view;
    triangle.v[1] = triangle.v[1] + shift_view;
    triangle.v[2] = triangle.v[2] + shift_view;
    triangle.v[0].x *= 0.5f * App::WIDTH;
    triangle.v[0].y *= 0.5f * App::HEIGHT;
    triangle.v[1].x *= 0.5f * App::WIDTH;
    triangle.v[1].y *= 0.5f * App::HEIGHT;
    triangle.v[2].x *= 0.5f * App::WIDTH;
    triangle.v[2].y *= 0.5f * App::HEIGHT;
}

std::tuple<uint8_t, Triangle, Triangle> Object::clip_triangle(Vec3f planePoint, Vec3f planeNormal, Triangle& triangleToClip, unsigned& clippedTris) const
{
    std::pair<unsigned, Vec3f*[3]> ins;
    std::pair<unsigned, Vec3f*[3]> out;
    float pointDistance[3] {0} ;

    for (uint8_t i = 0; i < 3; i++)
    {
        if ((pointDistance[i] = distance_from_plane_to_point(planePoint, planeNormal, triangleToClip.v[i])) >= 0)
            ins.second[ins.first++] = &triangleToClip.v[i];
        else out.second[out.first++] = &triangleToClip.v[i];
    }

    // if all points are behind the field of wiev, i reject the whole triangle
    // (no valid triangles returned)
    if (ins.first == 0) 
        return std::make_tuple(0, Triangle(), Triangle());
    else if (ins.first == 1 && out.first == 2)
    {
        Triangle out1;
        out1.v[0] = *ins.second[0];
        out1.v[1] = vector_intersect_plane(*ins.second[0], *out.second[0], planePoint, planeNormal);
        out1.v[2] = vector_intersect_plane(*ins.second[0], *out.second[1], planePoint, planeNormal);
        if (RenderingState::ClippingVisible) 
            out1.color = sf::Color::Red;
        clippedTris++;
        return std::make_tuple(1, out1, Triangle());
    }
    else if (ins.first == 2 && out.first == 1)
    {
        Triangle out1, out2;
        out1.v[0] = *ins.second[0];
        out1.v[1] = *ins.second[1];
        out1.v[2] = vector_intersect_plane(*ins.second[0], *out.second[0], planePoint, planeNormal);
        if (RenderingState::ClippingVisible)
            out1.color = sf::Color::Yellow;

        out2.v[0] = *ins.second[1];
        out2.v[1] = out1.v[2];
        out2.v[2] = vector_intersect_plane(*ins.second[1], *out.second[0], planePoint, planeNormal);
        if (RenderingState::ClippingVisible)
            out2.color = sf::Color::Green;

        clippedTris += 2;
        return std::make_tuple(2, out1, out2);
    }
    else if (ins.first == 3)
        return std::make_tuple(1, Triangle(std::move(triangleToClip)), Triangle());
}