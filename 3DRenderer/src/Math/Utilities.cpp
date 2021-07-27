#include "../pch.h"
#include "Utilities.h"


namespace math
{
    /*
    bool calculateNormal(Triangle& triangle, const Camera& cam)
    {
        sf::Vector3f sidea, sideb;
        static int c = 0;

        // vector that represents first side of a triangle
        sidea.x = triangle.v[1].x - triangle.v[0].x;
        sidea.y = triangle.v[1].y - triangle.v[0].y;
        sidea.z = triangle.v[1].z - triangle.v[0].z;

        // vector that represents second side of a triangle
        sideb.x = triangle.v[2].x - triangle.v[0].x;
        sideb.y = triangle.v[2].y - triangle.v[0].y;
        sideb.z = triangle.v[2].z - triangle.v[0].z;


        // calculating normal vector 
        triangle.normal.x = sidea.y * sideb.z - sidea.z * sideb.y;
        triangle.normal.y = sidea.z * sideb.x - sidea.x * sideb.z;
        triangle.normal.z = sidea.x * sideb.y - sidea.y * sideb.x;


        // and taking it into a unit vector 
        triangle.normal.normaliseVector();

        // if Z component of normal vector in compare to the camera position allows triangle to be drawn then i can draw it into the screen 
        if (triangle.normal.x * (triangle.v[0].x - cam.position.x) + triangle.normal.y * (triangle.v[0].y - cam.position.y) + triangle.normal.z * (triangle.v[0].z - cam.position.z) < 0.0f)
            return true;
        else return false;
    }
    */

    float dot_product(const Vec3f& v1, const Vec3f& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    float distance_from_plane_to_point(const Vec3f& planePoint, Vec3f& planeNormal, const Vec3f& point)
    {
        planeNormal.normalise();
        return (planeNormal.x * point.x + planeNormal.y * point.y + planeNormal.z * point.z - dot_product(planeNormal, planePoint));
    }

    Vec3f cross_product(const Vec3f& v1, const Vec3f& v2)
    {
        return Vec3f(   v1.y * v2.z - v1.z * v2.y,
                        v1.z * v2.x - v1.x * v2.z,
                        v1.x * v2.y - v1.y * v2.x );
    }

    Vec3f vector_intersect_plane(Vec3f& line_start, Vec3f& line_end, Vec3f& plane_point, Vec3f& plane_normal)
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

    int clip_triangle(Vec3f plane_point, Vec3f plane_normal, Triangle& input, Triangle& out1, Triangle& out2, unsigned& clipped_tris_counter, bool clip_hint)
    {
        plane_normal.normalise();

        Vec3f* inside_points[3];
        Vec3f* outside_points[3];
        unsigned inside_points_counter = 0;
        unsigned outside_points_counter = 0;
        float point_distance[3]{ 0 };

        for (unsigned i = 0; i < 3; i++)
            point_distance[i] = distance_from_plane_to_point(plane_point, plane_normal, input.v[i]);

        for (unsigned i = 0; i < 3; i++)
        {
            if (point_distance[i] >= 0)
                inside_points[inside_points_counter++] = &input.v[i];
            else outside_points[outside_points_counter++] = &input.v[i];
        }

        /* if all points are behind the field of wiev, i reject the whole triangle
        (no valid triangles returned)*/
        if (inside_points_counter == 0) return 0;
        else if (inside_points_counter == 1 && outside_points_counter == 2)
        {
            out1.v[0] = *inside_points[0];

            out1.v[1] = vector_intersect_plane(*inside_points[0], *outside_points[0], plane_point, plane_normal);
            out1.v[2] = vector_intersect_plane(*inside_points[0], *outside_points[1], plane_point, plane_normal);
            if (clip_hint) out1.color = sf::Color::Red;
            clipped_tris_counter++;
            return 1;
        }
        else if (inside_points_counter == 2 && outside_points_counter == 1)
        {
            out1.v[0] = *inside_points[0];
            out1.v[1] = *inside_points[1];
            out1.v[2] = vector_intersect_plane(*inside_points[0], *outside_points[0], plane_point, plane_normal);
            if (clip_hint) out1.color = sf::Color::Yellow;

            out2.v[0] = *inside_points[1];
            out2.v[1] = out1.v[2];
            out2.v[2] = vector_intersect_plane(*inside_points[1], *outside_points[0], plane_point, plane_normal);
            if (clip_hint) out2.color = sf::Color::Green;
            clipped_tris_counter += 2;
            return 2;
        }
        else if (inside_points_counter == 3)
        {
            out1 = input;
            return 1;
        }

    }
}