#include "../pch.h"
#include "Utilities.h"

namespace math
{
    
    bool calculate_normal(Triangle& triangle, const Vec3f& cameraPos)
    {
        // vectors that represents two sides of a given triangle
        sf::Vector3f sideA = {  triangle.v[1].x - triangle.v[0].x ,
                                triangle.v[1].y - triangle.v[0].y ,
                                triangle.v[1].z - triangle.v[0].z };

        sf::Vector3f sideB = {  triangle.v[2].x - triangle.v[0].x ,
                                triangle.v[2].y - triangle.v[0].y ,
                                triangle.v[2].z - triangle.v[0].z };

        // calculating normal vector 
        triangle.normal.x = sideA.y * sideB.z - sideA.z * sideB.y;
        triangle.normal.y = sideA.z * sideB.x - sideA.x * sideB.z;
        triangle.normal.z = sideA.x * sideB.y - sideA.y * sideB.x;
        triangle.normal.normalise();    // and taking it into a unit vector 

        // if Z component of normal vector in compare to the camera position allows triangle to be drawn then i can draw it into the screen
        auto triangleShouldBeVisible = [&]() 
        {
            if (triangle.normal.x * (triangle.v[0].x - cameraPos.x) +
                triangle.normal.y * (triangle.v[0].y - cameraPos.y) +
                triangle.normal.z * (triangle.v[0].z - cameraPos.z) < 0.0f)
            { return true; } else { return false; }
        };

        if (triangleShouldBeVisible())
            return true;
        else return false;
    }

    float distance_from_plane_to_point(const Vec3f& planePoint, Vec3f& planeNormal, const Vec3f& point)
    {
        planeNormal.normalise();
        return planeNormal.x * point.x + planeNormal.y * point.y + planeNormal.z * point.z - dot_product(planeNormal, planePoint);
    }

    Vec3f cross_product(const Vec3f& v1, const Vec3f& v2)
    {
        return Vec3f(   v1.y * v2.z - v1.z * v2.y,
                        v1.z * v2.x - v1.x * v2.z,
                        v1.x * v2.y - v1.y * v2.x );
    }

    Vec3f vector_intersect_plane(const Vec3f& line_start, const Vec3f& line_end, const Vec3f& plane_point, Vec3f& plane_normal)
    {
        plane_normal.normalise();
        float planedP = -dot_product(plane_normal, plane_point);
        float adP = dot_product(line_start, plane_normal);
        float bdP = dot_product(line_end, plane_normal);
        float t = (-planedP - adP) / (bdP - adP);
        Vec3f line = line_end - line_start;
        Vec3f line_to_intersect = line * t;
        return line_start + line_to_intersect;
    }

    Matrix4x4 point_at_matrix(const Vec3f& pos, const Vec3f& forward, const  Vec3f& up)
    {
        // calculate new forward direction
        Vec3f new_forward = forward - pos;
        new_forward.normalise();

        // calculate new up direction
        Vec3f temp = new_forward * dot_product(up, new_forward);
        Vec3f new_up = up - temp;
        new_up.normalise();

        // calculate new right direction
        Vec3f new_right = cross_product(new_up, new_forward);

        // setting up point at matrix
        Matrix4x4 mat;

        mat.m[0][0] = new_right.x;      mat.m[0][1] = new_right.y;      mat.m[0][2] = new_right.z;      mat.m[0][3] = 0;
        mat.m[1][0] = new_up.x;         mat.m[1][1] = new_up.y;         mat.m[1][2] = new_up.z;         mat.m[1][3] = 0;
        mat.m[2][0] = new_forward.x;    mat.m[2][1] = new_forward.y;    mat.m[2][2] = new_forward.z;    mat.m[2][3] = 0;
        mat.m[3][0] = pos.x;            mat.m[3][1] = pos.y;            mat.m[3][2] = pos.z;            mat.m[3][3] = 1;

        return mat;
    }

    Matrix4x4 inverse_point_at_matrix(const Matrix4x4& mat)
    {
        Matrix4x4 result;
        // this code works only for this specific case of matrix(pointAtMatrix)
        result.m[0][0] = mat.m[0][0];    result.m[0][1] = mat.m[1][0];    result.m[0][2] = mat.m[2][0];    result.m[0][3] = 0;
        result.m[1][0] = mat.m[0][1];    result.m[1][1] = mat.m[1][1];    result.m[1][2] = mat.m[2][1];    result.m[1][3] = 0;
        result.m[2][0] = mat.m[0][2];    result.m[2][1] = mat.m[1][2];    result.m[2][2] = mat.m[2][2];    result.m[2][3] = 0;

        result.m[3][0] = -(mat.m[3][0] * result.m[0][0] + mat.m[3][1] * result.m[1][0] + mat.m[3][2] * result.m[2][0]);
        result.m[3][1] = -(mat.m[3][0] * result.m[0][1] + mat.m[3][1] * result.m[1][1] + mat.m[3][2] * result.m[2][1]);
        result.m[3][2] = -(mat.m[3][0] * result.m[0][2] + mat.m[3][1] * result.m[1][2] + mat.m[3][2] * result.m[2][2]);
        result.m[3][3] = 1.0f;
        return result;
    }

    Triangle translate_triangle_by_matrix(const Triangle& tri, const Matrix4x4& matrix)
    {
        Triangle result;
        // multiplying every vertex of the triangle by a translation matrix
        result.v[0] = Matrix4x4::multiply_matrix_by_vec(matrix, tri.v[0]);
        result.v[0].divide_vec_by_W();
        result.v[1] = Matrix4x4::multiply_matrix_by_vec(matrix, tri.v[1]);
        result.v[1].divide_vec_by_W();
        result.v[2] = Matrix4x4::multiply_matrix_by_vec(matrix, tri.v[2]);
        result.v[2].divide_vec_by_W();
        return result;
    }

    void init_projection_matrix(Matrix4x4& matrix, int windowHeight, int windowWidth)
    {
        float fnear = 0.1f;
        float ffar = 1000.0f;
        float fieldOfView = 90.0f;
        float aspectRatio = (float)windowHeight / (float)windowWidth;
        float fovRad = 1.0f / tanf(fieldOfView * 0.5f / 180.0f * 3.14159f);

        matrix.m[0][0] = aspectRatio * fovRad;
        matrix.m[1][1] = fovRad;
        matrix.m[2][2] = ffar / (ffar - fnear);
        matrix.m[3][2] = (-ffar * fnear) / (ffar - fnear);
        matrix.m[2][3] = 1.0f;
        matrix.m[3][3] = 0.0f;
    }

}