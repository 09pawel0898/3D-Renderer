#include "../pch.h"
#include "vector.h"


void Vec3f::show(void)
{
    std::cout << x << " " << y << " " << z << "\n";
}

void Vec3f::normalise(void)
{
    float lenght = get_lenght();
    x /= lenght;
    y /= lenght;
    z /= lenght;
}

void Vec3f::divide_vec_by_W(void)
{
    /* coming to 2d from 3d coordinates, after this dividing i have appropriate X and Y values to draw points to the 2d screen
    (i am not going to use Z component) */
    if (w != 0.0f)
        *this = *this / w;
}

float Vec3f::get_lenght(void)
{
    return std::sqrt(x * x + y * y + z * z);
}

