#include "../pch.h"
#include "Mesh.h"
#include "Camera.h"
#include "RenderingState.h"

bool Mesh::load_obj(const std::string& fileName)
{
    std::ifstream file("obj/" + fileName);
    std::stringstream ss;
    std::vector<Vec3f> vertices;
    Vec3f vec;
    Triangle triangle;

    char c, line[200];
    int attribute[3];

    if (!file.good())
    {
        std::cerr << "Unable to open *.obj file with 3d model.";
        exit(EXIT_FAILURE);
    }
    while (file.getline(line, 200))
    {
        ss << line;
        if (line[0] == 'v')
        {
            ss >> c >> vec.x >> vec.y >> vec.z;
            vertices.push_back(vec);
        }
        else if (line[0] == 'f')
        {
            ss >> c >> attribute[0] >> attribute[1] >> attribute[2];
            triangle.v[0] = vertices[attribute[0] - 1];
            triangle.v[1] = vertices[attribute[1] - 1];
            triangle.v[2] = vertices[attribute[2] - 1];

            mTris.push_back(triangle);
            mTrisCount++;
        }
        ss = std::stringstream();
    }
    States::RenderingState::AllPolygons = mTrisCount;
    return true;
}

void Mesh::sort_triangles(std::vector<Triangle>& mTris)
{
    sort(mTris.begin(), mTris.end(), [](const Triangle& t1, const Triangle& t2)
    {
        // for sorting im averaging its Z components and im using the averaged values for comparison
        float avgZ1 = (t1.v[0].z + t1.v[1].z + t1.v[2].z) / 3.0f;
        float avgZ2 = (t2.v[0].z + t2.v[1].z + t2.v[2].z) / 3.0f;
        return avgZ1 > avgZ2;
    });
}

void Mesh::draw_triangle(const Triangle &triangle, sf::RenderTarget& target , bool isMeshActive, const Camera& cam, unsigned& triId) const
{
    // drawing colored triangles or just a mesh depending on the choosen mode
    if (!isMeshActive)
    {
        // compute lighting
        Vec3f light_dir = Vec3f(-cam.get_VLookAt().x,-cam.get_VLookAt().y,-cam.get_VLookAt().z);
        light_dir.normalise();

        float dP = dot_product(triangle.normal, light_dir);

        // if dot produt < 0 (angle is greater than 90), then i set the triangle to be almost black 
        if (dP < 0) 
            dP = 0.05f;
        else if (dP >= 0 && dP < 0.1f) 
            dP = 0.1f;

        sf::Color color = sf::Color((int)(dP * 255), 
                                    (int)(dP * 255), 
                                    (int)(dP * 255));

        if(triangle.color == sf::Color::White)
            fill_triangle(target, color, triangle,triId);
        else
            fill_triangle(target, triangle.color, triangle,triId);
    }
    else
        draw_border(target, triangle);
}


void Mesh::draw_border(sf::RenderTarget& target,  const Triangle& triangle ) const
{
    // drawing triangle borders
    sf::VertexArray lines(sf::LinesStrip, 4);
    lines[0].mPosition = sf::Vector2f(triangle.v[0].x, triangle.v[0].y);
    lines[1].mPosition = sf::Vector2f(triangle.v[1].x, triangle.v[1].y);
    lines[2].mPosition = sf::Vector2f(triangle.v[2].x, triangle.v[2].y);
    lines[3].mPosition = sf::Vector2f(triangle.v[0].x, triangle.v[0].y);
    target.draw(lines);
}

void Mesh::fill_triangle(sf::RenderTarget& target, sf::Color color, const Triangle& triangle, unsigned& triId) const
{
    // drawing filled triangle
    /*
    sf::ConvexShape tri;
    tri.setPointCount(3);
    tri.setPoint(0, sf::Vector2f(triangle.v[0].x, triangle.v[0].y));
    tri.setPoint(1, sf::Vector2f(triangle.v[1].x, triangle.v[1].y));
    tri.setPoint(2, sf::Vector2f(triangle.v[2].x, triangle.v[2].y));
    tri.setFillColor(color);
    target.draw(tri);
    */

    sf::Vertex* vertexArray = &States::RenderingState::Vertices[triId * 3];
    vertexArray[0].mPosition = std::move(sf::Vector2f(triangle.v[0].x, triangle.v[0].y));
    vertexArray[1].mPosition = std::move(sf::Vector2f(triangle.v[1].x, triangle.v[1].y));
    vertexArray[2].mPosition = std::move(sf::Vector2f(triangle.v[2].x, triangle.v[2].y));
    
    vertexArray[0].color = vertexArray[1].color = vertexArray[2].color = color;
}



void Mesh::update_rotation(RotationType rotationType, float dt)
{
    if (rotationType == RotationType::CUSTOM) set_ZX_rotation_matrix(dt);
    else if (rotationType == RotationType::X_AXIS) set_X_rotation_matrix(dt);
}

void Mesh::rotate(RotationType rotationType, Matrix4x4& matWorld, const Matrix4x4 &matTrans)
{
    if (rotationType == RotationType::NONE) /* no rotation, just changing position using camera */
        matWorld = Matrix4x4::multiply_matrix_by_matrix(matWorld, matTrans);
    else if (rotationType == RotationType::CUSTOM) // rotation - firstly Z axis, and then X axis
    {
        matWorld = Matrix4x4::multiply_matrix_by_matrix(mRotZ, mRotX);
        matWorld = Matrix4x4::multiply_matrix_by_matrix(matWorld, matTrans);
    }
    else if (rotationType == RotationType::X_AXIS) // rotation - only X axis
    {
        matWorld = Matrix4x4::multiply_matrix_by_matrix(matWorld, mRotX);
        matWorld = Matrix4x4::multiply_matrix_by_matrix(matWorld, matTrans);
    }

}

void Mesh::set_ZX_rotation_matrix(float dt)
{
    static float angle;

    angle += 1.0f * dt;

    //rotation z matrix
    mRotZ.m[0][0] = cosf(angle);
    mRotZ.m[0][1] = sinf(angle);
    mRotZ.m[1][0] = -sinf(angle);
    mRotZ.m[1][1] = cosf(angle);
    mRotZ.m[2][2] = 1;
    mRotZ.m[3][3] = 1;

    //rotation x matrix
    mRotX.m[0][0] = 1;
    mRotX.m[1][1] = cosf(angle * 0.5f);
    mRotX.m[1][2] = sinf(angle * 0.5f);
    mRotX.m[2][1] = -sinf(angle * 0.5f);
    mRotX.m[2][2] = cosf(angle * 0.5f);
    mRotX.m[3][3] = 1;
}

void Mesh::set_X_rotation_matrix(float dt)
{
    static float angle;
    angle += 1.0f * dt;

    //rotation X matrix
    mRotX.make_X_rotation_matrix(angle);
}

void Mesh::set_Y_rotation_matrix(float dt)
{
    static float angle;
    angle += 1.0f * dt;

    //rotation Y matrix
    mRotY.make_Y_rotation_matrix(angle);
}

void Mesh::set_Z_rotation_matrix(float dt)
{
    static float alpha;
    alpha += 1.0f * dt;

    //rotation Z matrix
    mRotZ.make_Z_rotation_matrix(alpha);
}