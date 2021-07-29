#include "../pch.h"
#include "Vector.h"
#include "SFML/Graphics.hpp"

namespace math
{
    void Vec3f::show(void)
    {
        std::cout << x << " " << y << " " << z << "\n";
    }

    void Vec3f::normalise(void)
    {
        float lenght = vec3_lenght(*this);
        if (lenght != 0.0f)
        {
            x /= lenght;    y /= lenght;    z /= lenght;
        }
    }

    void Vec3f::divide_vec_by_W(void)
    {
        // coming to 2d from 3d coordinates, after this dividing i have appropriate X and Y values to draw points to the 2d screen
        // (i am not going to use Z component) 
        if (w != 0.0f)
        {
            x /= w; y /= w; z /= w;
        }
    }

    float Vec3f::get_lenght(void)
    {
        return vec3_lenght(*this);
    }
}