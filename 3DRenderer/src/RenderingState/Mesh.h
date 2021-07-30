#pragma once

class Camera;

#include "../Math/Utilities.h"

using math::Triangle;
using math::Matrix4x4;

enum class RotationType
{
    X_AXIS,Y_AXIS,Z_AXIS,CUSTOM,NONE
};

class Mesh
{
private:
    std::vector<Triangle> mTris;        // vector of polygons 
    Matrix4x4 mRotZ, mRotX, mRotY;      // rotation matrices 

public:
    unsigned mTrisCount = 0;            // triangles counter
    Mesh() {}

    bool load_obj(const std::string& fileName);                                                                         // loading object from file
    const std::vector<Triangle>& get_triangles() const { return mTris; }
    
    void sort_triangles(std::vector<Triangle>& mTris);                                                                  // sorting triangles by averaged Z value
    void draw_triangle(const Triangle& triangle,sf::RenderTarget& target, bool isMeshActive, const Camera& cam, unsigned& triId) const; // drawing single triangle 
    void draw_border(sf::RenderTarget& target, const Triangle& triangle) const;                                         // drawing a triangle border
    void fill_triangle(sf::RenderTarget& target, sf::Color color, const Triangle& triangle, unsigned& triId) const;                      // filling triangle with color 
    void update_rotation(RotationType rotationType, float dt);                                                          // update rotation accordingly to the choosen type 
    void rotate(RotationType rotationType, Matrix4x4& matWorld, const Matrix4x4& matTrans);                             // updates matWorld by multiplying it by rotation and translation matrices 

private:
    void set_ZX_rotation_matrix(float dt);                                                                              // rotating by nice rotation matrix proposed by javidx9 
    void set_X_rotation_matrix(float dt);                                                                               // elementary rotation around X axis 
    void set_Y_rotation_matrix(float dt);                                                                               // elementary rotation around Y axis 
    void set_Z_rotation_matrix(float dt);
};